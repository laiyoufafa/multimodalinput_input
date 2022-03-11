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
#ifndef KEY_OPTION_H
#define KEY_OPTION_H

#include <set>

#include "nocopyable.h"

namespace OHOS {
namespace MMI {
class KeyOption {
public:
    KeyOption() = default;
    DISALLOW_COPY_AND_MOVE(KeyOption);

    std::set<int32_t> GetPreKeys() const;
    void SetPreKeys(const std::set<int32_t>& preKeys);
    int32_t GetFinalKey() const;
    void SetFinalKey(int32_t finalKey);

    bool IsFinalKeyDown() const;
    void SetFinalKeyDown(bool pressed);

    int32_t GetFinalKeyDownDuration() const;
    void SetFinalKeyDownDuration(int32_t duration);

private:
    std::set<int32_t> preKeys_ {};
    int32_t finalKey_ { -1 };
    bool isFinalKeyDown_;
    int32_t finalKeyDownDuration_ { 0 };
};
} // namespace MMI
} // namespace OHOS

#endif // KEY_OPTION_H