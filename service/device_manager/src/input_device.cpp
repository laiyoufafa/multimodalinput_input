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

#include "input_device.h"

namespace OHOS {
namespace MMI {
void InputDevice::SetId(int32_t deviceId)
{
    id_ = deviceId;
}

int32_t InputDevice::GetId() const
{
    return id_;
}

void InputDevice::SetName(std::string name)
{
    name_ = name;
}

std::string InputDevice::GetName() const
{
    return name_;
}

void InputDevice::SetType(int32_t deviceType)
{
    deviceType_ = deviceType;
}

int32_t InputDevice::GetType() const
{
    return deviceType_;
}
} // namespace MMI
} // namespace OHOS