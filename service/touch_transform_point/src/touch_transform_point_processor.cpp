/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "touch_transform_point_processor.h"

#include "mmi_log.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, MMI_LOG_DOMAIN, "TouchTransformPointProcessor"};
} // namespace

TouchTransformPointProcessor::TouchTransformPointProcessor(int32_t deviceId) : deviceId_(deviceId)
{
    pointerEvent_ = PointerEvent::Create();
    CHKPL(pointerEvent_);
}

TouchTransformPointProcessor::~TouchTransformPointProcessor() {}

bool TouchTransformPointProcessor::OnEventTouchDown(struct libinput_event *event)
{
    CALL_LOG_ENTER;
    CHKPF(event);
    auto data = libinput_event_get_touch_event(event);
    CHKPF(data);
    int32_t logicalY = -1;
    int32_t logicalX = -1;
    int32_t logicalDisplayId = -1;
    if (!WinMgr->TouchDownPointToDisplayPoint(data, logicalX, logicalY, logicalDisplayId)) {
        MMI_HILOGE("TouchDownPointToDisplayPoint failed");
        return false;
    }
    auto pointIds = pointerEvent_->GetPointersIdList();
    int64_t time = GetSysClockTime();
    if (pointIds.empty()) {
        pointerEvent_->SetActionStartTime(time);
        pointerEvent_->SetTargetDisplayId(logicalDisplayId);
    }
    pointerEvent_->SetActionTime(time);
    pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);

    PointerEvent::PointerItem item;
    auto pressure = libinput_event_touch_get_pressure(data);
    auto seatSlot = libinput_event_touch_get_seat_slot(data);
    item.SetPressure(pressure);
    item.SetPointerId(seatSlot);
    item.SetDownTime(time);
    item.SetPressed(true);
    item.SetGlobalX(logicalX);
    item.SetGlobalY(logicalY);
    item.SetDeviceId(deviceId_);
    pointerEvent_->SetDeviceId(deviceId_);
    pointerEvent_->AddPointerItem(item);
    pointerEvent_->SetPointerId(seatSlot);
    MMI_HILOGD("LogicalX:%{public}d, logicalY:%{public}d, logicalDisplay:%{public}d, pressure:%{public}f",
               logicalX, logicalY, logicalDisplayId, pressure);
    return true;
}

bool TouchTransformPointProcessor::OnEventTouchMotion(struct libinput_event *event)
{
    CALL_LOG_ENTER;
    CHKPF(event);
    auto data = libinput_event_get_touch_event(event);
    CHKPF(data);
    int64_t time = GetSysClockTime();
    pointerEvent_->SetActionTime(time);
    pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    int32_t logicalY = -1;
    int32_t logicalX = -1;
    int32_t logicalDisplayId = pointerEvent_->GetTargetDisplayId();
    if (!WinMgr->TouchMotionPointToDisplayPoint(data, logicalDisplayId, logicalX, logicalY)) {
        MMI_HILOGE("Get TouchMotionPointToDisplayPoint failed");
        return false;
    }
    PointerEvent::PointerItem item;
    auto seatSlot = libinput_event_touch_get_seat_slot(data);
    if (!(pointerEvent_->GetPointerItem(seatSlot, item))) {
        MMI_HILOGE("Get pointer parameter failed");
        return false;
    }
    auto pressure = libinput_event_touch_get_pressure(data);
    item.SetPressure(pressure);
    item.SetGlobalX(logicalX);
    item.SetGlobalY(logicalY);
    pointerEvent_->UpdatePointerItem(seatSlot, item);
    pointerEvent_->SetPointerId(seatSlot);
    MMI_HILOGD("LogicalX:%{public}d, logicalY:%{public}d, pressure:%{public}f",
               logicalX, logicalY, pressure);
    return true;
}

bool TouchTransformPointProcessor::OnEventTouchUp(struct libinput_event *event)
{
    CALL_LOG_ENTER;
    CHKPF(event);
    auto data = libinput_event_get_touch_event(event);
    CHKPF(data);
    int64_t time = GetSysClockTime();
    pointerEvent_->SetActionTime(time);
    pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_UP);

    PointerEvent::PointerItem item;
    auto seatSlot = libinput_event_touch_get_seat_slot(data);
    if (!(pointerEvent_->GetPointerItem(seatSlot, item))) {
        MMI_HILOGE("Get pointer parameter failed");
        return false;
    }
    item.SetPressed(false);
    pointerEvent_->UpdatePointerItem(seatSlot, item);
    pointerEvent_->SetPointerId(seatSlot);
    return true;
}

std::shared_ptr<PointerEvent> TouchTransformPointProcessor::OnLibinputTouchEvent(struct libinput_event *event)
{
    CALL_LOG_ENTER;
    CHKPP(event);
    CHKPP(pointerEvent_);
    auto type = libinput_event_get_type(event);
    switch (type) {
        case LIBINPUT_EVENT_TOUCH_DOWN: {
            if (!OnEventTouchDown(event)) {
                MMI_HILOGE("Get OnEventTouchDown failed");
                return nullptr;
            }
            break;
        }
        case LIBINPUT_EVENT_TOUCH_UP: {
            if (!OnEventTouchUp(event)) {
                MMI_HILOGE("Get OnEventTouchUp failed");
                return nullptr;
            }
            break;
        }
        case LIBINPUT_EVENT_TOUCH_MOTION: {
            if (!OnEventTouchMotion(event)) {
                MMI_HILOGE("Get OnEventTouchMotion failed");
                return nullptr;
            }
            break;
        }
        default: {
            MMI_HILOGE("Unknown event type, touchType:%{public}d", type);
            return nullptr;
        }
    }
    pointerEvent_->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    pointerEvent_->UpdateId();
    return pointerEvent_;
}
} // namespace MMI
} // namespace OHOS
