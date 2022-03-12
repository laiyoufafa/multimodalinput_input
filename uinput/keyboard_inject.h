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

#ifndef KEYBOARD_INJECT_H
#define KEYBOARD_INJECT_H

#include <iostream>
#include <map>
#include <mutex>

#include "inject_thread.h"
#include "nocopyable.h"
#include "singleton.h"
#include "virtual_keyboard.h"

namespace OHOS {
namespace MMI {
class KeyboardInject : public Singleton<KeyboardInject> {
public:
    KeyboardInject();
    virtual ~KeyboardInject() = default;
    DISALLOW_COPY_AND_MOVE(KeyboardInject);
    void InjectKeyEvent(uint16_t code, uint32_t value) const;

private:
    static std::unique_ptr<InjectThread> injectThread_;
    std::map<int32_t, int32_t> keyCodeMap_ = {};
    static std::mutex mutex_;
    static constexpr int32_t INPUT_KEY_BACK = 2;
    static constexpr int32_t LINUX_KEY_BACK = 158;
};
} // namespace MMI
} // namespace OHOS

#endif // KEYBOARD_INJECT_H