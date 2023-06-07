/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "subscribeswitchevent_fuzzer.h"

#include "securec.h"

#include "input_manager.h"
#include "mmi_log.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "SubscribeSwitchEventFuzzTest" };
} // namespace

void SubscribeSwitchEventFuzzTest(const uint8_t* data, size_t size)
{
    auto fun = [](std::shared_ptr<SwitchEvent> event) {
        MMI_HILOGD("Subscribe SwitchEvent success");
    };
    int32_t subscribeId = InputManager::GetInstance()->SubscribeSwitchEvent(fun);
    InputManager::GetInstance()->UnsubscribeSwitchEvent(subscribeId);
}
} // MMI
} // OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::MMI::SubscribeSwitchEventFuzzTest(data, size);
    return 0;
}
