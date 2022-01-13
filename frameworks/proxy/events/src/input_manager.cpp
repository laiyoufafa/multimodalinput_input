/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "error_multimodal.h"
#include "input_manager.h"
#include "input_event_monitor_manager.h"
#include "input_manager_impl.h"
#include "key_event_input_subscribe_manager.h"
#include "libmmi_util.h"
#include "multimodal_event_handler.h"

namespace OHOS {
namespace MMI {
namespace {
    static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
        LOG_CORE, MMI_LOG_DOMAIN, "InputManager"
    };
}

InputManager *InputManager::mInstance_ = nullptr;

InputManager *InputManager::GetInstance()
{
    if (mInstance_ == nullptr) {
        mInstance_ = new InputManager();
    }
    return mInstance_;
}

void InputManager::UpdateDisplayInfo(const std::vector<PhysicalDisplayInfo> &physicalDisplays,
    const std::vector<LogicalDisplayInfo> &logicalDisplays)
{
    InputManagerImpl::GetInstance()->UpdateDisplayInfo(physicalDisplays, logicalDisplays);
}

void InputManager::SetInputEventFilter(std::function<bool(std::shared_ptr<PointerEvent> filter)>) {}

void InputManager::SetWindowInputEventConsumer(std::shared_ptr<OHOS::MMI::IInputEventConsumer> inputEventConsumer)
{
    InputManagerImpl::GetInstance()->SetWindowInputEventConsumer(inputEventConsumer);
}

int32_t InputManager::SubscribeKeyEvent(std::shared_ptr<KeyOption> keyOption,
    std::function<void(std::shared_ptr<KeyEvent>)> callback)
{
    return KeyEventInputSubscribeMgr.SubscribeKeyEvent(keyOption, callback);
}

void InputManager::UnsubscribeKeyEvent(int32_t subscriberId)
{
    KeyEventInputSubscribeMgr.UnSubscribeKeyEvent(subscriberId);
}

int32_t InputManager::AddMonitor(std::function<void(std::shared_ptr<KeyEvent>)> monitor)
{
    if (monitor == nullptr) {
        MMI_LOGE("InputManager::%{public}s param should not be null!", __func__);
        return OHOS::MMI_STANDARD_EVENT_INVALID_PARAMETER;
    }
    InputManagerImpl::GetInstance()->AddMonitor(monitor);
    return MMI_STANDARD_EVENT_SUCCESS;
}
int32_t InputManager::AddMonitor(std::function<void(std::shared_ptr<PointerEvent>)> monitor)
{
    return 0;
}
int32_t InputManager::AddMonitor(std::function<bool(std::shared_ptr<KeyEvent>)> monitor)
{
    return 0;
}
void InputManager::RemoveMonitor(int32_t monitorId)
{
    InputManagerImpl::GetInstance()->RemoveMonitor(monitorId);
}

int32_t InputManager::AddInputEventTouchpadMontior(std::function<void(std::shared_ptr<PointerEvent>)> monitor)
{
    if (monitor == nullptr) {
        MMI_LOGE("InputManager::%{public}s param should not be null!", __func__);
        return InputEventMonitorManager::INVALID_MONITOR_ID;
    }
    return InputManagerImpl::GetInstance()->AddInputEventTouchpadMontior(monitor);
}

int32_t InputManager::AddInputEventTouchpadMontior(std::function<bool(std::shared_ptr<PointerEvent>)> monitor)
{
    return 0;
}

void InputManager::RemoveInputEventTouchpadMontior(int32_t monitorId)
{
    InputManagerImpl::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
}

int32_t InputManager::AddMonitor2(std::shared_ptr<IInputEventConsumer> consumer)
{
    return InputManagerImpl::GetInstance()->AddMonitor2(consumer);
}

void InputManager::RemoveMonitor2(int32_t monitorId)
{
    InputManagerImpl::GetInstance()->RemoveMonitor2(monitorId);
}

void InputManager::MarkConsumed(int32_t monitorId, int32_t eventId)
{
    InputManagerImpl::GetInstance()->MarkConsumed(monitorId, eventId);
}

int32_t InputManager::AddInterceptor(std::shared_ptr<IInputEventConsumer> interceptorId)
{
    return 0;
}

int32_t InputManager::AddInterceptor(int32_t sourceType, std::function<void(std::shared_ptr<PointerEvent>)> interceptor)
{
    if (interceptor == nullptr) {
        MMI_LOGE("InputManager::%{public}s param should not be null!", __func__);
        return OHOS::MMI_STANDARD_EVENT_INVALID_PARAMETER;
    }
    return InputManagerImpl::GetInstance()->AddInterceptor(sourceType, interceptor);
}

void InputManager::RemoveInterceptor(int32_t interceptorId)
{
    InputManagerImpl::GetInstance()->RemoveInterceptor(interceptorId);
}

void InputManager::SimulateInputEvent(std::shared_ptr<KeyEvent> keyEvent) {}
void InputManager::SimulateInputEvent(std::list<std::shared_ptr<KeyEvent>> keyEvents) {}
void InputManager::SimulateInputEvent(std::shared_ptr<PointerEvent> pointerEvent)
{
    if (MultimodalEventHandler::GetInstance().InjectPointerEvent(pointerEvent) != RET_OK)
        MMI_LOGE("Failed to inject pointer event!");
}
void InputManager::SimulateInputEvent(std::list<std::shared_ptr<PointerEvent>> pointerEvents) {}
}
}