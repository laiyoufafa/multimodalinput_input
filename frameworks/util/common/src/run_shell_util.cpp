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

#include "run_shell_util.h"
#include <cstdio>
#include "define_multimodal.h"

namespace OHOS::MMI {
namespace {
    const std::string HILOG_GREP = "hilog -x | grep ";
    constexpr int32_t MAXSIZE = 1000;
    const std::regex REGEX_LOG("\n");
    static inline constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "RUNSHELLUTILL" };
}

RunShellUtil::RunShellUtil(): logMaxSize(100) {}

RunShellUtil::~RunShellUtil() {}

int32_t RunShellUtil::RunShellCommand(const std::string &command, std::vector<std::string> &vLog)
{
    MMI_LOGD("enter");
    vLog.clear();
    std::string retLog = "";
    const std::string command_ = HILOG_GREP + "'" + command + "'";

    if ((fp = popen(command_.c_str(), "r")) == nullptr) {
        MMI_LOGE("open fail");
        pclose(fp);
        fp = nullptr;
        return RET_ERR;
    }
    int i = 0;
    while (logMaxSize > i) {
        char buf[MAXSIZE] = {0};
        if (fgets(buf, sizeof(buf), fp) == nullptr) {
            MMI_LOGE("read fp end");
            retLog.append(std::string(buf));
            pclose(fp);
            fp = nullptr;
            break;
        }
        retLog.append(std::string(buf));
        i++;
    }
    if (retLog.length() == 0) {
        MMI_LOGD("retLog is empty");
        return RET_OK;
    }
    StringToVectorByRegex(retLog, vLog, REGEX_LOG);
    return RET_OK;
}

int32_t RunShellUtil::SetLogMaxSize(int32_t logSize)
{
    if (logSize <= 0) {
        return RET_ERR;
    }
    logMaxSize = logSize;
    return RET_OK;
}

int32_t RunShellUtil::StringToVectorByRegex(const std::string &log, std::vector<std::string> &vLog, const std::regex &r)
{
    std::vector<std::string> v(std::sregex_token_iterator(log.begin(), log.end(), r, -1),
        std::sregex_token_iterator());
    vLog = v;
    return RET_OK;
}
}