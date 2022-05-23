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

#ifndef PERMISSION_HELPER_H
#define PERMISSION_HELPER_H

#include "accesstoken_kit.h"
#include "singleton.h"

namespace OHOS {
namespace MMI {
class PermissionHelper : public DelayedSingleton<PermissionHelper> {
public:
    static const int32_t APL_NORMAL = 1 << OHOS::Security::AccessToken::APL_NORMAL;
    static const int32_t APL_SYSTEM_BASIC = 1 << OHOS::Security::AccessToken::APL_SYSTEM_BASIC;
    static const int32_t APL_SYSTEM_CORE = 1 << OHOS::Security::AccessToken::APL_SYSTEM_CORE;
    static const int32_t APL_SYSTEM_BASIC_CORE = APL_SYSTEM_BASIC + APL_SYSTEM_CORE;

public:
    bool CheckPermission(int32_t required);

private:
    bool CheckHapPermission(uint32_t tokenId, int32_t required);
    bool CheckNativePermission(uint32_t tokenId, int32_t required);
};
#define PerHelper PermissionHelper::GetInstance()
} // namespace MMI
} // namespace OHOS
#endif  // PERMISSION_HELPER_H