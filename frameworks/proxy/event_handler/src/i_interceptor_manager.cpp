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

#include "i_interceptor_manager.h"
#include "define_multimodal.h"
#include "mmi_log.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "IInterceptorManager" };
} // namespace

int32_t IInterceptorManager::AddInterceptor(int32_t sourceType,
    std::function<void(std::shared_ptr<PointerEvent>)> interceptor)
{
    MMI_HILOGD("Add mgr module dose not support");
    return RET_ERR;
}

int32_t IInterceptorManager::AddInterceptor(std::function<void(std::shared_ptr<KeyEvent>)> interceptor)
{
    MMI_HILOGD("Add mgr module dose not support");
    return RET_ERR;
}

void IInterceptorManager::RemoveInterceptor(int32_t interceptorId)
{
    MMI_HILOGD("Remove mgr module dose not support");
    return;
}

int32_t IInterceptorManager::OnPointerEvent(std::shared_ptr<PointerEvent> pointerEvent, int32_t id)
{
    MMI_HILOGD("Pointer mgr module dose not support");
    return RET_ERR;
}

int32_t IInterceptorManager::OnKeyEvent(std::shared_ptr<KeyEvent> keyEvent)
{
    MMI_HILOGD("Key mgr module dose not support");
    return RET_ERR;
}
} // namespace MMI
} // namespace OHOS