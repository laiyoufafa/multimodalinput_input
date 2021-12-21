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

#ifndef OHOS_VIRTUAL_DEVICE_H
#define OHOS_VIRTUAL_DEVICE_H

#include "virtual_device_config.h"

namespace OHOS {
namespace MMI {
class VirtualDevice {
public:
    VirtualDevice(const std::string &device_name, uint16_t busType, uint16_t vendorId, uint16_t product_id);
    virtual ~VirtualDevice();
    static bool CatFload(StringList& fileList);
    static bool SyncSymbolFile();
    bool DoIoctl(int32_t fd, int32_t request, const uint32_t value);
    bool CreateKey();
    bool SetAbsResolution(const std::string deviceName);
    bool SetPhys(const std::string deviceName);
    bool SetUp();
    static void CloseAllDevice(const StringList& fileList);
    static void StartAllDevices();
    static void MakeFolder(const std::string &filePath);
    static bool SelectDevice(StringList& fileList);
    static bool CreateHandle(const std::string deviceArgv);
    static bool AddDevice(const StringList& fileList);
    static bool CloseDevice(const StringList& fileList);
    static bool FunctionalShunt(const std::string firstArgv, StringList argvList);

protected:
    virtual const std::vector<uint32_t>& GetEventTypes() const;
    virtual const std::vector<uint32_t>& GetKeys() const;
    virtual const std::vector<uint32_t>& GetProperties() const;
    virtual const std::vector<uint32_t>& GetAbs() const;
    virtual const std::vector<uint32_t>& GetRelBits() const;
    virtual const std::vector<uint32_t>& GetLeds() const;
    virtual const std::vector<uint32_t>& GetReps() const;
    virtual const std::vector<uint32_t>& GetMscs() const;
    virtual const std::vector<uint32_t>& GetSws() const;
    int32_t fd_ = -1;
    std::string deviceName_;
    const uint16_t busTtype_;
    const uint16_t vendorId_;
    const uint16_t productId_;
    const uint16_t version_;
    struct uinput_user_dev dev_ {};
    struct uinput_abs_setup absTemp_ = {};
    std::vector<uinput_abs_setup> absInit_;
};
}
}
#endif  // OHOS_VIRTUAL_DEVICE_H
