/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#ifndef I_INTERCEPTOR_MANAGER_H
#define I_INTERCEPTOR_MANAGER_H

#include "nocopyable.h"
#include "singleton.h"

#include "i_input_event_consumer.h"
#include "pointer_event.h"

namespace OHOS {
namespace MMI {
class IInterceptorManager : public DelayedSingleton<IInterceptorManager> {
public:
    IInterceptorManager() = default;
    ~IInterceptorManager() = default;
    DISALLOW_COPY_AND_MOVE(IInterceptorManager);
    int32_t AddInterceptor(int32_t sourceType,
        std::function<void(std::shared_ptr<PointerEvent>)> interceptor);
    int32_t AddInterceptor(std::function<void(std::shared_ptr<KeyEvent>)> interceptor);
    void RemoveInterceptor(int32_t interceptorId);
    int32_t OnPointerEvent(std::shared_ptr<PointerEvent> pointerEvent, int32_t id);
    int32_t OnKeyEvent(std::shared_ptr<KeyEvent> keyEvent);
};
} // namespace MMI
} // namespace OHOS
#endif // I_INTERCEPTOR_MANAGER_H