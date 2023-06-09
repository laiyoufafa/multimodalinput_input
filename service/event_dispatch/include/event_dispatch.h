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
#ifndef EVENT_DISPATCH_H
#define EVENT_DISPATCH_H

#include "nocopyable.h"

#include "event_package.h"
#include "i_event_filter.h"
#include "key_event.h"
#include "key_event_value_transformation.h"
#include "pointer_event.h"
#include "uds_server.h"
#include "i_key_command_manager.h"

namespace OHOS {
namespace MMI {
class EventDispatch : public std::enable_shared_from_this<EventDispatch> {
public:
    EventDispatch();
    DISALLOW_COPY_AND_MOVE(EventDispatch);
    virtual ~EventDispatch();
    virtual int32_t AddInputEventFilter(sptr<IEventFilter> filter);
    int32_t DispatchGestureNewEvent(UDSServer& udsServer, struct libinput_event *event,
        std::shared_ptr<PointerEvent> pointer, const int64_t preHandlerTime);
    int32_t DispatchGestureEvent(UDSServer& udsServer, struct libinput_event *event, const EventGesture& gesture,
        const int64_t preHandlerTime);
    int32_t DispatchKeyEvent(UDSServer& udsServer, struct libinput_event *event,
        const KeyEventValueTransformation& trs, EventKeyboard& key, const int64_t preHandlerTime);
    int32_t DispatchKeyEventPid(UDSServer& udsServer, std::shared_ptr<KeyEvent> key);
    int32_t DispatchTouchEvent(UDSServer& udsServer, struct libinput_event *event,
        const EventTouch& touch, const int64_t preHandlerTime);
    int32_t DispatchTabletPadEvent(UDSServer& udsServer, struct libinput_event *event,
        const EventTabletPad& tabletPad, const int64_t preHandlerTime);
    int32_t DispatchJoyStickEvent(UDSServer& udsServer, struct libinput_event *event,
        const EventJoyStickAxis& eventJoyStickAxis, const int64_t preHandlerTime);
    int32_t DispatchCommonPointEvent(UDSServer& udsServer, struct libinput_event *event,
        const EventPointer& point, const int64_t preHandlerTime);
    int32_t DispatchPointerEvent(UDSServer& udsServer, struct libinput_event *event,
        EventPointer& point, const int64_t preHandlerTime);
    int32_t DispatchTabletToolEvent(UDSServer& udsServer, struct libinput_event *event,
        const EventTabletTool& tableTool, const int64_t preHandlerTime);
    int32_t HandlePointerEvent(std::shared_ptr<PointerEvent> point);

protected:
    bool HandlePointerEventFilter(std::shared_ptr<PointerEvent> point);
    void OnEventTouchGetPointEventType(const EventTouch& touch, const int32_t fingerCount,
        POINT_EVENT_TYPE& pointEventType);
    int32_t GestureRegisteredEventDispatch(const MmiMessageId& idMsg, UDSServer& udsServer,
        RegisteredEvent& registeredEvent, int64_t preHandlerTime);
    int32_t KeyBoardRegEveHandler(const EventKeyboard& key, UDSServer& udsServer,
        struct libinput_event *event, int32_t inputDeviceType, int64_t preHandlerTime);
    bool TriggerANR(int64_t time, SessionPtr sess);

private:
    int32_t DispatchTouchEvent(const EventTouch& touch, const int fd,
        const int64_t preHandlerTime, UDSServer& udsServer, NetPacket &pkt) const;

private:
    EventPackage eventPackage_;
    std::shared_ptr<IKeyCommandManager> keyCommand_ = nullptr;
};
} // namespace MMI
} // namespace OHOS
#endif // EVENT_DISPATCH_H