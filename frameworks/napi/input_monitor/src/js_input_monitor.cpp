/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "js_input_monitor.h"
#include <cinttypes>
#include <uv.h>
#include "define_multimodal.h"
#include "input_manager.h"
#include "js_register_util.h"

#define INPUTMGR OHOS::MMI::InputManager::GetInstance()

namespace OHOS {
namespace MMI {
namespace {
    static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
        LOG_CORE, MMI_LOG_DOMAIN, "JsInputMonitor"
    };

    constexpr int32_t NAPI_ERR = 3;
    template<class MemberFunType, class ClassType>
    auto CallbackBind2(MemberFunType func, ClassType* obj)
    {
        return std::bind(func, obj, std::placeholders::_1);
    }

    struct CallBackInfo {
        std::shared_ptr<PointerEvent> pointerEvent_ {nullptr};
        std::function<void(std::shared_ptr<PointerEvent>)> handle_ {nullptr};
    };
}
int32_t InputMonitor::Start()
{
    std::lock_guard<std::mutex> guard(lk_);
    if (monitorId_ < 0) {
        monitorId_ = INPUTMGR->AddMonitor2(shared_from_this());
        return monitorId_ >= 0 ? RET_OK : RET_ERR;
    }
    return RET_OK;
}

void InputMonitor::Stop()
{
    std::lock_guard<std::mutex> guard(lk_);
    if (monitorId_ < 0) {
        return;
    }
    monitorId_ = -1;
    return;
}

void InputMonitor::SetCallback(std::function<void(std::shared_ptr<PointerEvent>)> callback)
{
    std::lock_guard<std::mutex> guard(lk_);
    callback_ = callback;
}

void InputMonitor::OnInputEvent(std::shared_ptr<PointerEvent> pointerEvent) const
{
    std::function<void(std::shared_ptr<PointerEvent>)> callback;
    {
        std::lock_guard<std::mutex> guard(lk_);
        if (pointerEvent->GetSourceType() == PointerEvent::SOURCE_TYPE_TOUCHSCREEN) {
            if (pointerEvent->GetPointersIdList().size() == 1) {
                if (pointerEvent->GetPointerAction() == PointerEvent::POINTER_ACTION_DOWN) {
                    consumed_ = false;
                }
            }
        }
        callback = callback_;
    }
    if (!callback) {
        return;
    }
    callback(pointerEvent);
}

void InputMonitor::OnInputEvent(std::shared_ptr<KeyEvent> keyEvent) const {}

void InputMonitor::OnInputEvent(std::shared_ptr<AxisEvent> axisEvent) const {}

void InputMonitor::MarkConsumed(int32_t eventId)
{
    std::lock_guard<std::mutex> guard(lk_);
    if (consumed_) {
        return;
    }
    if (monitorId_ < 0) {
        return;
    }
    INPUTMGR->MarkConsumed(monitorId_, eventId);
    consumed_ = true;
}

JsInputMonitor::JsInputMonitor(napi_env jsEnv, napi_value receiver)
    : monitor_(std::make_shared<InputMonitor>()),
      jsEnv_(jsEnv)
{
    SetReceiver(receiver);
    monitor_->SetCallback([this](std::shared_ptr<PointerEvent> pointerEvent) {
        OnPointerEvent(pointerEvent);
    });
    handle_ = CallbackBind2(&JsInputMonitor::OnPointerEventInJsThread, this);
}

JsInputMonitor::~JsInputMonitor()
{
    monitor_->Stop();
    monitor_.reset();
    uint32_t refCount = 0;
    auto status = napi_reference_unref(jsEnv_, receiver_, &refCount);
    if (status != napi_ok) {
        MMI_LOGE("napi_reference_unref is failed");
        return;
    }
}

void JsInputMonitor::SetReceiver(napi_value receiver)
{
    if (receiver_ == nullptr && jsEnv_ != nullptr) {
        uint32_t refCount = 1;
        auto status = napi_create_reference(jsEnv_, receiver, refCount, &receiver_);
        if (status != napi_ok) {
            MMI_LOGE("napi_create_reference is failed");
            return;
        }
    }
}

void JsInputMonitor::Start()
{
    monitor_->Start();
}

void JsInputMonitor::Stop()
{
    monitor_->Stop();
}

void JsInputMonitor::MarkConsumed(int32_t eventId)
{
    monitor_->MarkConsumed(eventId);
}

int32_t JsInputMonitor::IsMatch(napi_env jsEnv, napi_value receiver)
{
    if (jsEnv == jsEnv_) {
        bool isEquals = false;
        napi_value handlerTemp = nullptr;
        auto status = napi_get_reference_value(jsEnv_, receiver_, &handlerTemp);
        if (status != napi_ok) {
            MMI_LOGE("napi_get_reference_value is failed");
            return NAPI_ERR;
        }
        status = napi_strict_equals(jsEnv_, handlerTemp, receiver, &isEquals);
        if (status != napi_ok) {
            MMI_LOGE("napi_strict_equals is failed");
            return NAPI_ERR;
        }
        if (isEquals) {
            MMI_LOGE("isEquals = %{public}d", isEquals);
            return RET_OK;
        }
        return RET_ERR;
    }
    return RET_ERR;
}

int32_t JsInputMonitor::IsMatch(napi_env jsEnv)
{
    if (jsEnv == jsEnv_) {
        return RET_OK;
    }
    return RET_ERR;
}

void JsInputMonitor::OnPointerEvent(std::shared_ptr<PointerEvent> pointerEvent)
{
    CallBackInfo* cb = new CallBackInfo;
    cb->handle_ = handle_;
    cb->pointerEvent_ = pointerEvent;
    uv_work_t* work = new uv_work_t;
    uv_loop_s* loop {nullptr};
    auto status = napi_get_uv_event_loop(jsEnv_, &loop);
    if (status != napi_ok) {
        MMI_LOGE("napi_get_uv_event_loop is failed");
        return;
    }

    work->data = (void*)cb;

    uv_queue_work(loop,
                  work,
                  [](uv_work_t *work) {},
                  [](uv_work_t *work, int32_t status) {
                      MMI_LOGD("uv_queue_work enter");
                      struct CallBackInfo* cbInfo = (struct CallBackInfo*)work->data;
                      if (cbInfo->handle_ != nullptr && cbInfo->pointerEvent_ != nullptr) {
                          cbInfo->handle_(cbInfo->pointerEvent_);
                          MMI_LOGD("run handle_");
                      }
                      delete cbInfo;
                      delete work;
                      cbInfo = nullptr;
                      work = nullptr;
                      MMI_LOGD("uv_queue_work leave");
                  });
}

void JsInputMonitor::OnPointerEventInJsThread(std::shared_ptr<PointerEvent> pointerEvent)
{
    if (jsEnv_ == nullptr && receiver_ == nullptr && pointerEvent == nullptr) {
        MMI_LOGE("null pointer");
        return;
    }
    auto eventId = pointerEvent->GetId();
    napi_value callBack = nullptr;
    napi_value result = nullptr;
    napi_value touch = nullptr;
    auto status = TransformPointerEvent(pointerEvent, touch);
    if (status != RET_OK) {
        return;
    }
    status = napi_get_reference_value(jsEnv_, receiver_, &callBack);
    if (status != napi_ok) {
        MMI_LOGE("napi_get_reference_value is failed");
        return;
    }
    status = napi_call_function(jsEnv_, nullptr, callBack, 1, &touch, &result);
    if (status != napi_ok) {
        MMI_LOGE("napi_call_function is failed");
        return;
    }
    napi_valuetype valueType = napi_undefined;

    status = napi_typeof(jsEnv_, result, &valueType);
    if (status != napi_ok) {
        MMI_LOGE("napi_typeof is failed");
        return;
    }
    if (valueType != napi_boolean) {
        MMI_LOGE("valueType is not bool");
        return;
    }
    bool retValue = false;
    status = napi_get_value_bool(jsEnv_, result, &retValue);
    if (status != napi_ok) {
        MMI_LOGE("napi_get_value_bool is failed");
        return;
    }
    if (retValue) {
        MarkConsumed(eventId);
    }
}

int32_t JsInputMonitor::TransformPointerEvent(const std::shared_ptr<PointerEvent> pointerEvent, napi_value result)
{
    CHKR(pointerEvent != nullptr, NULL_POINTER, RET_ERR);
    auto status = napi_create_object(jsEnv_, &result);
    if (status != napi_ok) {
        MMI_LOGE("napi_create_object is failed");
        return RET_ERR;
    }
    SetNamedProperty(jsEnv_, result, "type", pointerEvent->GetSourceType());

    napi_value pointers = nullptr;
    status = napi_create_array(jsEnv_, &pointers);
    if (status != napi_ok) {
        MMI_LOGE("napi_create_array is failed");
        return RET_ERR;
    }

    int32_t currentPointerId = pointerEvent->GetPointerId();
    std::vector<PointerEvent::PointerItem> pointerItems;
    for (auto &it : pointerEvent->GetPointersIdList()) {
        PointerEvent::PointerItem pointerItem;
        pointerEvent->GetPointerItem(it, pointerItem);
        pointerItems.push_back(pointerItem);
    }
    uint32_t index = 0;
    int32_t touchArea = 0;
    for (auto &it : pointerItems) {
        napi_value item = nullptr;
        status = napi_create_object(jsEnv_, &item);
        if (status != napi_ok) {
            MMI_LOGE("napi_create_object is failed");
            return RET_ERR;
        }
        if (currentPointerId == it.GetPointerId()) {
            napi_value currentPointer = nullptr;
            status = napi_create_object(jsEnv_, &currentPointer);
            if (status != napi_ok) {
                MMI_LOGE("napi_create_object is failed");
                return RET_ERR;
            }
            SetNamedProperty(jsEnv_, currentPointer, "globalX", it.GetGlobalX());
            SetNamedProperty(jsEnv_, currentPointer, "globalY", it.GetGlobalY());
            SetNamedProperty(jsEnv_, currentPointer, "localX", 0);
            SetNamedProperty(jsEnv_, currentPointer, "localY", 0);
            touchArea = (it.GetWidth() + it.GetHeight()) / 2;
            SetNamedProperty(jsEnv_, currentPointer, "size", touchArea);
            SetNamedProperty(jsEnv_, currentPointer, "force", it.GetPressure());
            SetNamedProperty(jsEnv_, result, "timestamp", it.GetDownTime());
            SetNamedProperty(jsEnv_, result, "deviceId", it.GetDeviceId());
            SetNamedProperty(jsEnv_, result, "changedTouches", currentPointer);
        }
        SetNamedProperty(jsEnv_, item, "globalX", it.GetGlobalX());
        SetNamedProperty(jsEnv_, item, "globalY", it.GetGlobalY());
        SetNamedProperty(jsEnv_, item, "localX", 0);
        SetNamedProperty(jsEnv_, item, "localY", 0);
        touchArea = (it.GetWidth() + it.GetHeight()) / 2;
        SetNamedProperty(jsEnv_, item, "size", touchArea);
        SetNamedProperty(jsEnv_, item, "force", it.GetPressure());
        status = napi_set_element(jsEnv_, pointers, index, item);
        if (status != napi_ok) {
            MMI_LOGE("napi_set_element is failed");
            return RET_ERR;
        }
        index++;
    }
    SetNamedProperty(jsEnv_, result, "touches", pointers);
    return RET_OK;
}
}
}