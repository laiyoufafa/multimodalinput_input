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
#include "log.h"
#include "mouse_device_state.h"
#include "gesture_transform_point_processor.h"

namespace OHOS::MMI {
    namespace {
        static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, MMI_LOG_DOMAIN,
            "GestureTransformPointProcessor"};
    }

GestureTransformPointProcessor::GestureTransformPointProcessor(int32_t deviceId) : deviceId_(deviceId)
{
    pointerEvent_ = PointerEvent::Create();
}

GestureTransformPointProcessor::~GestureTransformPointProcessor()
{
}

void GestureTransformPointProcessor::SetPointEventSource(int32_t sourceType)
{
    pointerEvent_->SetSourceType(sourceType);
}

void GestureTransformPointProcessor::OnEventTouchPadPinchBegin(libinput_event_gesture *data)
{
    MMI_LOGT("Touchpad begin event");
    CHKP(data, PARAM_INPUT_INVALID);
    auto time = libinput_event_gesture_get_time(data);
    auto scale = libinput_event_gesture_get_scale(data);
    pointerEvent_->SetActionTime(static_cast<int64_t>(GetSysClockTime()));
    pointerEvent_->SetActionStartTime(time);

    PointerEvent::PointerItem pointerItem;
    pointerItem.SetDownTime(time);
    pointerItem.SetGlobalX(MouseState->GetMouseCoordsX());
    pointerItem.SetGlobalY(MouseState->GetMouseCoordsY());
    pointerItem.SetDeviceId(deviceId_);
    pointerItem.SetPointerId(defaultPointerId);
    pointerItem.SetWidth(0);
    pointerItem.SetHeight(0);
    pointerItem.SetPressed(MouseState->IsLeftBtnPressed());
    pointerEvent_->AddPointerItem(pointerItem);

    pointerEvent_->ClearButtonPressed();
    std::vector<int32_t> pressedButtons;
    MouseState->GetPressedButtons(pressedButtons);
    for (const auto &item : pressedButtons) {
        pointerEvent_->SetButtonPressed(item);
    }

    pointerEvent_->SetDeviceId(deviceId_);
    pointerEvent_->SetTargetDisplayId(0);
    pointerEvent_->SetPointerId(defaultPointerId);
    pointerEvent_->SetAxisValue(PointerEvent::AXIS_TYPE_PINCH, scale);
}

void GestureTransformPointProcessor::OnEventTouchPadPinchUpdate(libinput_event_gesture *data)
{
    MMI_LOGT("Touchpad update event");
    CHKP(data, PARAM_INPUT_INVALID);
    auto time = libinput_event_gesture_get_time(data);
    auto scale = libinput_event_gesture_get_scale(data);
    pointerEvent_->SetActionTime(static_cast<int64_t>(GetSysClockTime()));
    pointerEvent_->SetActionStartTime(time);

    PointerEvent::PointerItem pointerItem;
    pointerEvent_->GetPointerItem(defaultPointerId, pointerItem);
    pointerItem.SetGlobalX(MouseState->GetMouseCoordsX());
    pointerItem.SetGlobalY(MouseState->GetMouseCoordsY());
    pointerItem.SetPressed(MouseState->IsLeftBtnPressed());
    pointerEvent_->UpdatePointerItem(defaultPointerId, pointerItem);

    pointerEvent_->ClearButtonPressed();
    std::vector<int32_t> pressedButtons;
    MouseState->GetPressedButtons(pressedButtons);
    for (const auto &item : pressedButtons) {
        pointerEvent_->SetButtonPressed(item);
    }
    pointerEvent_->SetAxisValue(PointerEvent::AXIS_TYPE_PINCH, scale);
}

void GestureTransformPointProcessor::OnEventTouchPadPinchEnd(libinput_event_gesture *data)
{
    MMI_LOGT("Touchpad end event");
    CHKP(data, PARAM_INPUT_INVALID);
    auto time = libinput_event_gesture_get_time(data);
    auto scale = libinput_event_gesture_get_scale(data);
    pointerEvent_->SetActionTime(static_cast<int64_t>(GetSysClockTime()));
    pointerEvent_->SetActionStartTime(time);

    PointerEvent::PointerItem pointerItem;
    pointerEvent_->GetPointerItem(defaultPointerId, pointerItem);
    pointerItem.SetGlobalX(MouseState->GetMouseCoordsX());
    pointerItem.SetGlobalY(MouseState->GetMouseCoordsY());
    pointerItem.SetPressed(MouseState->IsLeftBtnPressed());
    pointerEvent_->UpdatePointerItem(defaultPointerId, pointerItem);

    pointerEvent_->ClearButtonPressed();
    std::vector<int32_t> pressedButtons;
    MouseState->GetPressedButtons(pressedButtons);
    for (const auto &item : pressedButtons) {
        pointerEvent_->SetButtonPressed(item);
    }
    pointerEvent_->SetAxisValue(PointerEvent::AXIS_TYPE_PINCH, scale);
}

std::shared_ptr<PointerEvent> GestureTransformPointProcessor::OnTouchPadGestrueEvent(
    libinput_event *event)
{
    MMI_LOGT("call OnTouchPadGestrueEvent begin");
    CHKPR(event, PARAM_INPUT_INVALID, nullptr);
    auto data = libinput_event_get_gesture_event(event);
    CHKPR(data, ERROR_NULL_POINTER, nullptr);
    pointerEvent_->UpdateId();
    auto type = libinput_event_get_type(event);
    switch (type) {
        case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN: {
            pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_AXIS_BEGIN);
            OnEventTouchPadPinchBegin(data);
            break;
        }
        case LIBINPUT_EVENT_GESTURE_PINCH_UPDATE: {
            pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_AXIS_UPDATE);
            OnEventTouchPadPinchUpdate(data);
            break;
        }
        case LIBINPUT_EVENT_GESTURE_PINCH_END: {
            pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_AXIS_END);
            OnEventTouchPadPinchEnd(data);
            break;
        }
        case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
        case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
        case LIBINPUT_EVENT_GESTURE_SWIPE_END: {
            MMI_LOGW("Three refers to the need to use, preserve the code");
            return nullptr;
        }
        default: {
            MMI_LOGE("Unknown event_type of pointer class has been reported!\n");
            return nullptr;
        }
    }
    MMI_LOGT("call OnTouchPadGestrueEvent end");
    return pointerEvent_;
}
}