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

#include "key_option.h"

namespace OHOS {
namespace MMI {
std::set<int32_t> KeyOption::GetPreKeys() const
{
    return preKeys_;
}

void KeyOption::SetPreKeys(const std::set<int32_t> &preKeys)
{
    preKeys_ = preKeys;
}

int32_t KeyOption::GetFinalKey() const
{
    return finalKey_;
}

void KeyOption::SetFinalKey(int32_t finalKey)
{
    finalKey_ = finalKey;
}

bool KeyOption::IsFinalKeyDown() const
{
    return isFinalKeyDown_;
}
void KeyOption::SetFinalKeyDown(bool pressed)
{
    isFinalKeyDown_ = pressed;
}

int32_t KeyOption::GetFinalKeyDownDuration() const
{
    return finalKeyDownDuration_;
}

void KeyOption::SetFinalKeyDownDuration(int32_t duration)
{
    finalKeyDownDuration_ = duration;
}

bool KeyOption::ReadFromParcel(Parcel &in)
{
    int32_t preKeysSize;
    if (!in.ReadInt32(preKeysSize)) {
        return false;
    }

    if (preKeysSize < 0) {
        return false;
    }

    for (auto i = 0; i < preKeysSize; ++i) {
        int32_t keyValue;
        if (!in.ReadInt32(keyValue)) {
            return false;
        }
        preKeys_.insert(keyValue);
    }

    return (
        in.ReadInt32(finalKey_) &&
        in.ReadBool(isFinalKeyDown_) &&
        in.ReadInt32(finalKeyDownDuration_)
    );
}

bool KeyOption::WriteToParcel(Parcel &out) const
{
    if (preKeys_.size() > std::numeric_limits<int32_t>::max()) {
        return false;
    }

    int32_t preKeysSize = static_cast<int32_t>(preKeys_.size());
    if (!out.WriteInt32(preKeysSize)) {
        return false;
    }

    for (const auto &i : preKeys_) {
        if (!out.WriteInt32(i)) {
            return false;
        }
    }

    return (
        out.WriteInt32(finalKey_) &&
        out.WriteBool(isFinalKeyDown_) &&
        out.WriteInt32(finalKeyDownDuration_)
    );
}
} // namespace MMI
} // namespace OHOS