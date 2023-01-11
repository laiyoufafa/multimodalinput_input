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
#include "input_display_bind_helper.h"

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "mmi_log.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "InputDisplayBindHelper" };
}
class BindInfo
{
public:
	int32_t GetInputDeviceId() const { return inputDeviceId_; }
	std::string GetInputDeviceName() const { return inputDeviceName_; }
	int32_t GetDisplayId() const { return displayId_; }
	std::string GetDisplayName() const { return displayName_; }
	bool IsUnbind() const {	return ((inputDeviceId_ == -1) || (displayId_ == -1)); }
	bool InputDeviceNotBind() const { return (inputDeviceId_ == -1);	}
	bool DisplayNotBind() const { return (displayId_ == -1); }
	bool AddInputDevice(int32_t deviceId, const std::string &deviceName);
	void RemoveInputDevice();
	bool AddDisplay(int32_t id, const std::string &name);
	void RemoveDisplay();
	std::string GetDesc() const;
	friend bool operator<(const BindInfo &l, const BindInfo &r);
	friend std::ostream& operator<<(std::ostream &os, const BindInfo &r);
	friend std::istream& operator>>(std::istream &is, BindInfo &r);
private:
	int32_t inputDeviceId_ { -1 };
	std::string inputDeviceName_;
	int32_t displayId_ { -1 };
	std::string displayName_;
};

class BindInfos {
public:
	bool Add(const BindInfo &info);
	void UnbindInputDevice(int32_t deviceId);
	void UnbindDisplay(int32_t displayId);
	BindInfo GetUnbindInputDevice(const std::string &displayName);
	BindInfo GetUnbindDisplay(const std::string &inputDeviceName);    
	std::string GetDisplayNameByInputDevice(const std::string &name);
	int32_t GetBindDisplayIdByInputDevice(int32_t inputDeviceId) const;
	std::string GetBindDisplayNameByInputDevice(int32_t inputDeviceId) const;
	std::string GetInputDeviceByDisplayName(const std::string &name);
    std::string GetDesc() const;
	const std::list<BindInfo> &GetInfos() const { return infos_; }
	friend std::ostream& operator<<(std::ostream &os, const BindInfos &r);
	friend std::istream& operator>>(std::istream &is, BindInfos &r);
private:
    BindInfo GetUnbindInputDevice();
    BindInfo GetUnbindInfo();
    BindInfo GetUnbindDisplay();
    std::list<BindInfo> infos_;
};

bool BindInfo::AddInputDevice(int32_t deviceId, const std::string &deviceName)
{
    if ((inputDeviceId_ != -1) || !inputDeviceName_.empty()) {
        return false;
    }
    inputDeviceId_ = deviceId;
    inputDeviceName_ = deviceName;
    return true;
}

void BindInfo::RemoveInputDevice() {
    inputDeviceId_ = -1;
    inputDeviceName_.clear();
}
bool BindInfo::AddDisplay(int32_t id, const std::string &name) {
    if ((displayId_ != -1) || !displayName_.empty()) {
        return false;
    }
    displayId_ = id;
    displayName_ = name;
    return true;
}
void BindInfo::RemoveDisplay() {
    displayId_ = -1;
    displayName_.clear();
}
std::string BindInfo::GetDesc() const {
    std::ostringstream oss;
    oss << "InputDevice(id:" << inputDeviceId_ << ",name:" << inputDeviceName_ << "),Display(id:" << displayId_ << ",name:" << displayName_ << ")";
    return oss.str();
}

bool operator<(const BindInfo &l, const BindInfo &r) {
	if (l.inputDeviceId_ != r.inputDeviceId_) {
		return (l.inputDeviceId_ < r.inputDeviceId_);
	}
	return (l.displayId_ < r.displayId_);
}

std::ostream& operator<<(std::ostream &os, const BindInfo &r) {
	os << r.inputDeviceName_ << "<=>" << r.displayName_ << std::endl;
	return os;
}

std::istream& operator>>(std::istream &is, BindInfo &r) {
	std::string line;
	std::getline(is, line);
	const std::string delim = "<=>";
	std::string::size_type pos = line.find(delim);
	if (pos == std::string::npos) {
		return is;
	}
	r.inputDeviceName_ = line.substr(0, pos);
	r.displayName_ = line.substr(pos + delim.length());
	r.inputDeviceId_ = 0;
	r.displayId_ = 0;
	return is;
}

std::string BindInfos::GetDesc() const {
	int32_t index = 0;
	std::ostringstream oss;
	for (auto &info : infos_) {
		oss << "index:" << index << "," << info.GetDesc() << std::endl;
	}
	return oss.str();
}

int32_t BindInfos::GetBindDisplayIdByInputDevice(int32_t inputDeviceId) const {
	for (const auto &item : infos_) {
		if (item.GetInputDeviceId() == inputDeviceId) {
			if (!item.IsUnbind()) {
				return item.GetDisplayId();
			}
		}
	}
	return -1;
}

std::string BindInfos::GetBindDisplayNameByInputDevice(int32_t inputDeviceId) const {
	for (const auto &item : infos_) {
		if (item.GetInputDeviceId() == inputDeviceId) {
			if (!item.IsUnbind()) {
				return item.GetDisplayName();
			}
		}
	}
	return "";
}

std::string BindInfos::GetDisplayNameByInputDevice(const std::string &name) {
	for (const auto &item : infos_) {
		if (item.GetInputDeviceName() == name) {
			return item.GetDisplayName();
		}
	}
	return "";
}

std::string BindInfos::GetInputDeviceByDisplayName(const std::string &name) {
	for (const auto &item : infos_) {
		if (item.GetDisplayName() == name) {
			return item.GetInputDeviceName();
		}
	}
	return "";
}

bool BindInfos::Add(const BindInfo &info) {
	auto it = infos_.begin();
	for (; it != infos_.end(); ++it) {
		if (info < *it) {
			break;
		}
	}
	auto it2 = infos_.emplace(it, std::move(info));
	if (it2 == infos_.end()) {
		MMI_HILOGE("Duplicate %{public}s", info.GetDesc().c_str());
	}
	return true;
}

void BindInfos::UnbindInputDevice(int32_t deviceId) {
	auto it = infos_.begin();
	for (; it != infos_.end(); ++it) {
		if (it->GetInputDeviceId() == deviceId) {
			it->RemoveInputDevice();
			if (it->IsUnbind()) {
				infos_.erase(it);
			}
			return;
		}
	} 
}

void BindInfos::UnbindDisplay(int32_t displayId) {
	auto it = infos_.begin();
	for (; it != infos_.end(); ++it) {
		if (it->GetDisplayId() == displayId) {
			it->RemoveDisplay();
			if (it->IsUnbind()) {
				infos_.erase(it);
			}
			return;
		}
	} 
}

BindInfo BindInfos::GetUnbindInfo() {
	auto it = infos_.begin();
	while (it != infos_.end()) {
		if (it->IsUnbind()) {
			auto info = std::move(*it);
			infos_.erase(it);
			return info;
		}
		++it;
	}
	return BindInfo();
}

BindInfo BindInfos::GetUnbindInputDevice(const std::string &displayName) {
	auto it = infos_.begin();
	while (it != infos_.end()) {
		if (it->InputDeviceNotBind()) {
			if (it->GetDisplayName() == displayName) {
				auto info = std::move(*it);
				infos_.erase(it);
				return info;
			}
		}
		++it;
	}
	return BindInfo();
}

BindInfo BindInfos::GetUnbindDisplay() {
	auto it = infos_.begin();
	while (it != infos_.end()) {
		if (it->DisplayNotBind()) {
			auto info = std::move(*it);
			infos_.erase(it);
			return info;
		}
		++it;
	}
	return BindInfo();
}

BindInfo BindInfos::GetUnbindDisplay(const std::string &inputDeviceName) {
	auto it = infos_.begin();
	while (it != infos_.end()) {
		if (it->DisplayNotBind()) {
			if (it->GetInputDeviceName() == inputDeviceName) {
				auto info = std::move(*it);
				infos_.erase(it);
				return info;
			}
		}
		++it;
	}
	return GetUnbindDisplay();
}

std::ostream& operator<<(std::ostream &os, const BindInfos &r) {
	const auto &infos = r.GetInfos();
	for (const auto &info : infos) {
		if (!info.IsUnbind()) {
			os << info;
		}
	}
	return os;
}

std::istream& operator>>(std::istream &is, BindInfos &r) {
	while (!is.eof()) {
		BindInfo info;
		is >> info;
		if (info.IsUnbind()) {
			break;
		}
		r.Add(info);
	}
	return is;
}

InputDisplayBindHelper::InputDisplayBindHelper(const std::string bindCfgFile)
	: fileName_(bindCfgFile),
	infos_(std::make_shared<BindInfos>()),
	configFileInfos_(std::make_shared<BindInfos>())
{}

std::string InputDisplayBindHelper::GetBindDisplayNameByInputDevice(int32_t inputDeviceId) const 
{
	CALL_DEBUG_ENTER;
	if (infos_ == nullptr) {
		return {};
	}
	return infos_->GetBindDisplayNameByInputDevice(inputDeviceId);
}

void InputDisplayBindHelper::AddInputDevice(int32_t id, const std::string &name)
{
	CALL_DEBUG_ENTER;
	MMI_HILOGD("Param: id:%{public}d, name:%{public}s", id, name.c_str());
	auto displayName = configFileInfos_->GetDisplayNameByInputDevice(name);
	BindInfo info = infos_->GetUnbindInputDevice(displayName);
	info.AddInputDevice(id, name);
	infos_->Add(info);
	Store();
}

void InputDisplayBindHelper::RemoveInputDevice(int32_t id)
{
	CALL_DEBUG_ENTER;
	MMI_HILOGD("Param: id:%{public}d", id);
	infos_->UnbindInputDevice(id);
}

bool InputDisplayBindHelper::IsDisplayAdd(int32_t id, const std::string &name)
{
	const auto &infos = infos_->GetInfos();
	for (const auto &info : infos)	 {
		if ((info.GetDisplayName() == name) && (info.GetDisplayId() == id)) {
			return true;
		}
	}
	return false;
}

std::set<std::pair<int32_t, std::string>> InputDisplayBindHelper::GetDisplayIdNames() const
{
	CALL_DEBUG_ENTER;
	std::set<std::pair<int32_t, std::string>> idNames;
	const auto &infos = infos_->GetInfos();
	for (const auto &info : infos)	 {
		if (info.GetDisplayId() != -1) {
			idNames.insert(std::make_pair(info.GetDisplayId(), info.GetDisplayName()));
		}
	}
	return idNames;
}

void InputDisplayBindHelper::AddDisplay(int32_t id, const std::string &name)
{
	CALL_DEBUG_ENTER;
	MMI_HILOGD("Param: id:%{public}d, name:%{public}s", id, name.c_str());
	auto inputDeviceName = configFileInfos_->GetInputDeviceByDisplayName(name);
	BindInfo info = infos_->GetUnbindDisplay(inputDeviceName);
	info.AddDisplay(id, name);
	infos_->Add(info);
	Store();
}

void InputDisplayBindHelper::RemoveDisplay(int32_t id)
{
	CALL_DEBUG_ENTER;
	MMI_HILOGD("Param: id:%{public}d", id);
	infos_->UnbindDisplay(id);
}

void InputDisplayBindHelper::Store()
{
	CALL_DEBUG_ENTER;
	if (infos_ == nullptr) {
		return;
	}
	std::ofstream ofs(fileName_.c_str(), std::ios::trunc | std::ios::out | std::ios_base::binary);
	if (!ofs) {
		MMI_HILOGE("Open file fail.%{public}s, errno:%{public}d", fileName_.c_str(), errno);
		return;
	}
	ofs << *infos_;
	ofs.close();
}

int32_t InputDisplayBindHelper::GetDisplayBindInfo(DisplayBindInfos &infos)
{
	CALL_DEBUG_ENTER;
	if (infos_ == nullptr) {
		MMI_HILOGE("Infos_ is nullptr");
		return RET_ERR;
	}
	for (const auto &item : infos_->GetInfos()) {
		infos.push_back({
			.inputDeviceId = item.GetInputDeviceId(), 
			.inputDeviceName = item.GetInputDeviceName(), 
			.displayId = item.GetDisplayId(), 
			.displayName = item.GetDisplayName(), 
		});
	}
	return RET_OK;
}

int32_t InputDisplayBindHelper::SetDisplayBind(int32_t deviceId, int32_t displayId, std::string &msg)
{
	CALL_DEBUG_ENTER;
	MMI_HILOGD("Param: deviceId:%{public}d, displayId:%{public}d", deviceId, displayId);
	if ((deviceId == -1) || (displayId == -1)) {
		msg = "The deviceId or displayId is invalid";
		MMI_HILOGE("%s", msg.c_str());
		return RET_ERR;
	}
	if (infos_ == nullptr) {
		msg = "Infos_ is nullptr";
		MMI_HILOGE("%s", msg.c_str());
		return RET_ERR;
	}

	BindInfo bindByDevice;
	BindInfo bindByDisplay;
	for (const auto &item : infos_->GetInfos()) {
		if (item.GetInputDeviceId() == deviceId) {
			bindByDevice = item;
		}
		if (item.GetDisplayId() == displayId) {
			bindByDisplay = item;
		}
	}
	if (bindByDevice.GetInputDeviceId() == -1) {
		msg = "The deviceId is invalid";
		MMI_HILOGE("%s", msg.c_str());
		return RET_ERR;
	}
	if (bindByDisplay.GetDisplayId() == -1) {
		msg = "The displayId is invalid";
		MMI_HILOGE("%s", msg.c_str());
		return RET_ERR;
	}

	if (infos_->GetBindDisplayIdByInputDevice(deviceId) == displayId) {
		msg = "The input device and display has alread bind";
		MMI_HILOGE("%s", msg.c_str());
		return RET_ERR;
	}

	infos_->UnbindInputDevice(bindByDevice.GetInputDeviceId());
	infos_->UnbindInputDevice(bindByDisplay.GetInputDeviceId());
	infos_->UnbindDisplay(bindByDevice.GetDisplayId());
	infos_->UnbindDisplay(bindByDisplay.GetDisplayId());

	BindInfo info1;
	info1.AddInputDevice(bindByDevice.GetInputDeviceId(), bindByDevice.GetInputDeviceName()) ;
	info1.AddDisplay(bindByDisplay.GetDisplayId(), bindByDisplay.GetDisplayName());
	infos_->Add(info1);

	if ((bindByDevice.GetDisplayId() != -1) && (bindByDisplay.GetInputDeviceId() != -1)) {
		MMI_HILOGD("Both display id and input device id are invalid");
		BindInfo info2;
		info2.AddInputDevice(bindByDisplay.GetInputDeviceId(), bindByDisplay.GetInputDeviceName());
		info2.AddDisplay(bindByDevice.GetDisplayId(), bindByDevice.GetDisplayName());
		infos_->Add(info2);
		return RET_OK;
	}

	if (bindByDevice.GetDisplayId() != -1) {
		MMI_HILOGD("The display id is invalid");
		AddDisplay(bindByDevice.GetDisplayId(), bindByDevice.GetDisplayName());
		return RET_OK;
	}

	if (bindByDisplay.GetInputDeviceId() != -1) {
		MMI_HILOGD("The input device id is invalid");
		AddInputDevice(bindByDisplay.GetInputDeviceId(), bindByDisplay.GetInputDeviceName());
		return RET_OK;
	}
	
	msg = "Can not reach here";
	return RET_ERR;
}

void InputDisplayBindHelper::Load()
{
	CALL_DEBUG_ENTER;
	std::ifstream ifs(fileName_.c_str());
	MMI_HILOGEK("Open file end:%{public}s", fileName_.c_str());
	if (!ifs) {
		MMI_HILOGE("Open file fail.%{public}s, errno:%{public}d", fileName_.c_str(), errno);
		return;
	}
	ifs >> *configFileInfos_;
	ifs.close();
}

std::string InputDisplayBindHelper::Dumps() const
{
	CALL_DEBUG_ENTER;
	if (infos_ == nullptr) {
		return {};
	}
	std::ostringstream oss;
	oss << *infos_;
	return oss.str();
}
} //namespace MMI
} // namespace OHOS