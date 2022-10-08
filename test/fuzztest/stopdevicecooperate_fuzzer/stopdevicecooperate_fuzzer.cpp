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

#include "stopdevicecooperate_fuzzer.h"

#include "input_manager.h"
#include "mmi_log.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "StopDeviceCooperateFuzzTest" };
} // namespace

void StopDeviceCooperateFuzzTest(const uint8_t* data, size_t  size)
{
    if (data == nullptr) {
        return;
    }
    int32_t userData = *(reinterpret_cast<const int32_t*>(data));
    auto fun = [](std::string listener, CooperationMessage cooperateMessages) {
        MMI_HILOGD("StopDeviceCooperateFuzzTest");
    };

    InputManager::GetInstance()->StopDeviceCooperate(userData,fun);
}
} // MMI
} // OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::MMI::StopDeviceCooperateFuzzTest(data, size);
    return 0;
}

