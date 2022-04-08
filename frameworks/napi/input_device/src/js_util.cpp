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

#include "js_util.h"

#include "mmi_log.h"
#include "util_napi.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "JsUtil" };
} // namespace
int32_t JsUtil::GetUserData(uv_work_t *work)
{
    int32_t *uData = static_cast<int32_t*>(work->data);
    int32_t userData = *uData;
    delete uData;
    uData = nullptr;
    delete work;
    work = nullptr;
    return userData;
}

JsUtil::CallbackInfo::CallbackInfo() {}

JsUtil::CallbackInfo::~CallbackInfo()
{
    CALL_LOG_ENTER;
    if (ref != nullptr) {
        CHKRV(env, napi_delete_reference(env, ref), "napi_delete_reference");
    }
}
} // namespace MMI
} // namespace OHOS