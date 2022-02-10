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

#ifndef OHOS_MULTIMDOALINPUT_INPUT_INTERCEPTOR_MANAGER_H
#define OHOS_MULTIMDOALINPUT_INPUT_INTERCEPTOR_MANAGER_H
#include <memory>
#include "input_handler_type.h"
#include "i_input_event_consumer.h"
#include "singleton.h"

namespace OHOS::MMI {
class InputInterceptorManager : public Singleton<InputInterceptorManager> {
public:
    int32_t AddInterceptor(std::shared_ptr<IInputEventConsumer> interceptor);
    void RemoveInterceptor(int32_t interceptorId);

public:
    static bool IsValidInterceptorId(int32_t interceptorId);
};

inline bool InputInterceptorManager::IsValidInterceptorId(int32_t interceptorId)
{
    return IsValidHandlerId(interceptorId);
}
} // namespace OHOS::MMI
#endif // OHOS_MULTIMDOALINPUT_INPUT_INTERCEPTOR_MANAGER_H