/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef KEY_EVENT_INPUT_SUBSCRIBE_MANAGER_H_
#define KEY_EVENT_INPUT_SUBSCRIBE_MANAGER_H_

#include <functional>
#include <list>
#include <memory>
#include "key_event.h"
#include "key_option.h"
#include "singleton.h"

namespace OHOS {
namespace MMI {
class KeyEventInputSubscribeManager : public Singleton<KeyEventInputSubscribeManager> {
public:
    class SubscribeKeyEventInfo {
    public:
        explicit SubscribeKeyEventInfo(std::shared_ptr<OHOS::MMI::KeyOption> keyOption,
            std::function<void(std::shared_ptr<OHOS::MMI::KeyEvent>)> callback)
            : keyOption_(keyOption), callback_(callback)
        {
            ++KeyEventInputSubscribeManager::subscribeIdManager_;
            subscribeId_ = KeyEventInputSubscribeManager::subscribeIdManager_;
        }
        ~SubscribeKeyEventInfo() = default;

        int32_t GetSubscribeId() const
        {
            return subscribeId_;
        }

        std::shared_ptr<OHOS::MMI::KeyOption> GetKeyOption() const
        {
            return keyOption_;
        }

        std::function<void(std::shared_ptr<OHOS::MMI::KeyEvent>)> GetCallback() const
        {
            return callback_;
        }

    private:
        int32_t subscribeId_;
        std::shared_ptr<OHOS::MMI::KeyOption> keyOption_;
        std::function<void(std::shared_ptr<OHOS::MMI::KeyEvent>)> callback_;
    };

public:
    KeyEventInputSubscribeManager() = default;
    ~KeyEventInputSubscribeManager() = default;

    bool CheckRepeatSubscribeKeyEevent(std::shared_ptr<OHOS::MMI::KeyOption> keyOption);
    int32_t SubscribeKeyEvent(std::shared_ptr<OHOS::MMI::KeyOption> keyOption,
        std::function<void(std::shared_ptr<OHOS::MMI::KeyEvent>)> callback);
    int32_t UnSubscribeKeyEvent(int32_t subscribeId);

    int32_t OnSubscribeKeyEventCallback(std::shared_ptr<OHOS::MMI::KeyEvent> event, int32_t subscribeId);

private:
    std::list<SubscribeKeyEventInfo> subscribeKeyEventInfoList_;
    static int32_t subscribeIdManager_;
};
}  // namespace MMI
}  // namespace OHOS

#define KeyEventInputSubscribeMgr OHOS::MMI::KeyEventInputSubscribeManager::GetInstance()

#endif  // KEY_EVENT_INPUT_SUBSCRIBE_MANAGER_H_