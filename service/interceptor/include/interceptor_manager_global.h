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
#ifndef INTERCEPTOR_MANAGER_GLOBAL_H
#define INTERCEPTOR_MANAGER_GLOBAL_H

#include <list>

#include "nocopyable.h"
#include "singleton.h"

#include "key_event.h"
#include "pointer_event.h"
#include "uds_session.h"

namespace OHOS {
namespace MMI {
class InterceptorManagerGlobal : public DelayedSingleton<InterceptorManagerGlobal> {
public:
    InterceptorManagerGlobal();
    DISALLOW_COPY_AND_MOVE(InterceptorManagerGlobal);
    ~InterceptorManagerGlobal() = default;
    void OnAddInterceptor(int32_t sourceType, int32_t id, SessionPtr session);
    void OnRemoveInterceptor(int32_t id);
    bool OnPointerEvent(std::shared_ptr<PointerEvent> pointerEvent);
    bool OnKeyEvent(std::shared_ptr<KeyEvent> keyEvent);
private:
    struct InterceptorItem {
        int32_t sourceType;
        int32_t id;
        SessionPtr session = nullptr;
        bool operator == (const InterceptorItem& item)
        {
            return id == item.id;
        }
    };
private:
    std::list<InterceptorItem> interceptors_;
};
} // namespace MMI
} // namespace OHOS
#endif // INTERCEPTOR_MANAGER_GLOBAL_H