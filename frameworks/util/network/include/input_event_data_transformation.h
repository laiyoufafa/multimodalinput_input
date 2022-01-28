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
#ifndef OHOS_INPUT_EVENT_DATA_TRANSFORMATION_H
#define OHOS_INPUT_EVENT_DATA_TRANSFORMATION_H

#include "key_event.h"
#include "net_packet.h"
#include "proto.h"
#include "pointer_event.h"

namespace OHOS {
namespace MMI {
class InputEventDataTransformation {
    static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "KeyEventDataTransformation" };
public:
    static int32_t KeyEventToNetPacket(const std::shared_ptr<KeyEvent> key, NetPacket &pck);
    static int32_t NetPacketToKeyEvent(bool skipId, std::shared_ptr<KeyEvent> key, NetPacket &pck);
    static int32_t SerializeInputEvent(std::shared_ptr<InputEvent> pointerE, NetPacket &pck);
    static int32_t DeserializeInputEvent(bool skipId, std::shared_ptr<InputEvent> pointerE, NetPacket &pck);
    static int32_t SerializePointerEvent(std::shared_ptr<PointerEvent> pointerE, NetPacket &pck);
    static int32_t DeserializePointerEvent(bool skipId, std::shared_ptr<PointerEvent> pointerE, NetPacket &pck);
    static int32_t DeserializePointerItem(PointerEvent::PointerItem &item, NetPacket &pck);
};
}
}
#endif
