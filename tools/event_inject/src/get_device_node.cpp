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

#include "get_device_node.h"

using namespace OHOS::MMI;

namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "GetDeviceNode" };
} // namespace

GetDeviceNode::GetDeviceNode()
{
    InitDeviceInfo();
}

int32_t GetDeviceNode::GetDeviceNodeName(const std::string &targetName, uint16_t devIndex, std::string &deviceNode)
{
    std::string cmd = "cat /proc/bus/input/devices";
    std::vector<std::string> cmdResult;
    ExecuteCmd(cmd, cmdResult);
    DeviceList deviceList;
    GetDeviceCmd(cmdResult, deviceList);
    if (deviceList.empty()) {
        MMI_LOGE("device list is null");
        return RET_ERR;
    }
    std::string deviceName = deviceList_[targetName];
    auto iter = deviceList.find(deviceName);
    if (iter == deviceList.end()) {
        MMI_LOGE("faild for find deviceName:%{public}s", deviceName.c_str());
        return RET_ERR;
    }
    size_t targetSize = iter->second.size();
    if (devIndex > targetSize) {
        MMI_LOGE("faild for devIndex:%{public}d > targetSize:%{public}zu", devIndex, targetSize);
        return RET_ERR;
    }
    std::string nodeRootPath = "/dev/input/";
    deviceNode = nodeRootPath + iter->second[devIndex];
    MMI_LOGI("%{public}s[%{public}d] --> %{public}s", targetName.c_str(), devIndex,
             deviceNode.c_str());

    return RET_OK;
}

void GetDeviceNode::InitDeviceInfo()
{
    deviceList_["mouse"] = "Virtual Mouse";
    deviceList_["touch"] = "Virtual TouchScreen";
    deviceList_["finger"] = "Virtual Finger";
    deviceList_["pad"] = "Virtual Touchpad";
    deviceList_["pen"] = "Virtual Stylus";
    deviceList_["gamePad"] = "Virtual GamePad";
    deviceList_["joystick"] = "Virtual Joystick";
    deviceList_["remoteControl"] = "Virtual RemoteControl";
    deviceList_["knob model1"] = "Virtual KnobConsumerCtrl";
    deviceList_["knob model2"] = "Virtual Knob";
    deviceList_["knob model3"] = "Virtual KnobMouse";
    deviceList_["keyboard model1"] = "Virtual keyboard";
    deviceList_["keyboard model2"] = "Virtual KeyboardConsumerCtrl";
    deviceList_["keyboard model3"] = "Virtual KeyboardSysCtrl";
    deviceList_["trackball"] = "Virtual Trackball";
    deviceList_["trackpad model1"] = "Virtual TrackPadMouse";
    deviceList_["trackpad model2"] = "Virtual Trackpad";
}

int32_t GetDeviceNode::ExecuteCmd(const std::string cmd, std::vector<std::string> &cmdResult)
{
    if (cmd.empty()) {
        return RET_ERR;
    }
    FILE* pin = popen(cmd.c_str(), "r");
    CHKPR(pin, RET_ERR);
    cmdResult.clear();
    char buffer[READ_CMD_BUFF_SIZE] = {};
    while (!feof(pin)) {
        if (fgets(buffer, sizeof(buffer), pin) != nullptr) {
            cmdResult.push_back(buffer);
        }
    }
    return pclose(pin);
}

void GetDeviceNode::GetDeviceCmd(const std::vector<std::string>& cmdResult, DeviceList& deviceList)
{
    std::string name;
    for (const auto &item : cmdResult) {
        std::string temp = item.substr(0, 1);
        uint64_t endPos = 0;
        uint64_t startPos = 0;
        if (temp == "N") {
            startPos = item.find("=") + strlen("N:");
            endPos = item.size() - 1;
            name = item.substr(startPos, endPos - startPos - 1);
        }
        if (temp == "H") {
            startPos = item.find("event");
            std::string endString = item.substr(startPos + strlen("event") + 1, 1);
            uint64_t eventLength = CMD_EVENT_LENGTH;
            if (endString != " ") {
                eventLength = CMD_EVENT_LENGTH + 1;
            }
            std::string target = item.substr(startPos, eventLength);
            if (!(name.empty())) {
                deviceList[name].push_back(target);
                name.clear();
                target.clear();
            }
        }
    }
}