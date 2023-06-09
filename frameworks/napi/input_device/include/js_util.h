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

#ifndef JS_UTIL_H
#define JS_UTIL_H

#include <uv.h>
#include <sstream>

#include "input_device_impl.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "refbase.h"

namespace OHOS {
namespace MMI {
class JsUtil {
public:
    struct CallbackData {
        std::vector<int32_t> ids;
        std::shared_ptr<InputDeviceImpl::InputDeviceInfo> device = nullptr;
        std::map<int32_t, bool> keystrokeAbility;
        int32_t deviceId = 0;
        int32_t keyboardType = 0;
    };

    struct CallbackInfo {
        CallbackInfo();
        ~CallbackInfo();
        napi_env env = nullptr;
        napi_ref ref = nullptr;
        napi_deferred deferred = nullptr;
        CallbackData data;
    };

    int32_t GetUserData(uv_work_t *work);
    bool IsHandleEquals(napi_env env, napi_value handle, napi_ref ref);
};

class AsyncContext : public RefBase {
public:
    napi_env env = nullptr;
    napi_async_work work = nullptr;
    napi_deferred deferred = nullptr;
    napi_ref callback = nullptr;
    int32_t errorCode {-1};
    std::stringstream reserve;
    AsyncContext(napi_env env) : env(env) {}
    ~AsyncContext();
};
} // namespace MMI
} // namespace OHOS

#endif // JS_UTIL_H