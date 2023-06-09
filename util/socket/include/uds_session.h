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
#ifndef UDS_SESSION_H
#define UDS_SESSION_H

#include <memory>

#include <sys/socket.h>
#include <sys/un.h>

#include "nocopyable.h"

#include "net_packet.h"

namespace OHOS {
namespace MMI {
class UDSSession;
using SessionPtr = std::shared_ptr<UDSSession>;
class UDSSession : public std::enable_shared_from_this<UDSSession> {
public:
    UDSSession(const std::string& programName, const int32_t moduleType, const int32_t fd, const int32_t uid,
               const int32_t pid);
    DISALLOW_COPY_AND_MOVE(UDSSession);
    virtual ~UDSSession() = default;

    bool SendMsg(const char *buf, size_t size) const;
    bool SendMsg(NetPacket& pkt) const;
    void Close();

    int32_t GetUid() const
    {
        return uid_;
    }

    int32_t GetPid() const
    {
        return pid_;
    }

    SessionPtr GetSharedPtr()
    {
        return shared_from_this();
    }
    int32_t GetFd() const
    {
        return fd_;
    }

    const std::string& GetDescript() const
    {
        return descript_;
    }

    void UpdateDescript();
    void AddEvent(int32_t id, int64_t time);
    void DelEvents(int32_t id);
    void AddPermission(bool hasPermission);
    bool HasPermission();
    int64_t GetEarlistEventTime() const;
    bool IsEventQueueEmpty();
    bool isANRProcess_ {false};

#ifdef OHOS_BUILD_MMI_DEBUG
    void SetClientFd(const int32_t clientFd)
    {
        clientFd_ = clientFd;
        UpdateDescript();
    }
#endif

protected:
    struct EventTime {
        int32_t id;
        int64_t eventTime;
    };
    std::vector<EventTime> events_;
    std::string descript_;
    const std::string programName_;
    const int32_t moduleType_;
    int32_t fd_;
    const int32_t uid_;
    const int32_t pid_;
    bool hasPermission_ = true;
#ifdef OHOS_BUILD_MMI_DEBUG
    int32_t clientFd_ = -1;
#endif // OHOS_BUILD_MMI_DEBUG
};
} // namespace MMI
} // namespace OHOS
#endif // UDS_SESSION_H