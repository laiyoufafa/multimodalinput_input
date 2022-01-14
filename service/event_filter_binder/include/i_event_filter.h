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

#ifndef OHOS_I_POINTER_EVENT_FILTER_H
#define OHOS_I_POINTER_EVENT_FILTER_H

#include "iremote_broker.h"
#include "pointer_event.h"

namespace OHOS {
namespace MMI {
class IEventFilter : public IRemoteBroker {
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.multimodalinput.IEventFilter");
    virtual bool HandlePointerEvent(const std::shared_ptr<PointerEvent> event) = 0;

    enum class OPERATOR_TYPE {
        HANDLE_POINTER_EVENT = 0,
    };
};
} // namespace MMI
} // namespace OHOS

#endif // OHOS_I_POINTER_EVENT_FILTER_H