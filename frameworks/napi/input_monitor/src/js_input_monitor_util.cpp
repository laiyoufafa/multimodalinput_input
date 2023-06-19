/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "js_input_monitor_util.h"

#include <cinttypes>

#include "define_multimodal.h"
#include "napi_constants.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "JsInputMonitorUtil" };
} // namespace

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, bool value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_int32(env, value, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}d failed", name.c_str(), value);
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, uint16_t value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_uint32(env, value, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}u failed", name.c_str(), value);
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, uint32_t value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_uint32(env, value, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}u failed", name.c_str(), value);
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, int32_t value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_int32(env, value, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}d failed", name.c_str(), value);
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, float value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_double(env, value, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}f failed", name.c_str(), value);
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, double value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_double(env, value, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}lf failed", name.c_str(), value);
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, int64_t value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_int64(env, value, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}" PRId64 " failed", name.c_str(), value);
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, std::string value)
{
    napi_status status;
    napi_value napiValue = nullptr;
    status = napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &napiValue);
    if (status != napi_ok) {
        MMI_HILOGE("%{public}s=%{public}s failed", name.c_str(), value.c_str());
        return status;
    }
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    return status;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, KeyEvent::KeyItem& value)
{
    napi_value napiValue{};
    auto status = value.WriteToJsValue(env, napiValue);
    CHECK_RETURN(status != napi_ok, "create key property failed", status);
    status = napi_set_named_property(env, object, name.c_str(), napiValue);
    CHECK_RETURN(status != napi_ok, "set key property failed", status);
    return napi_ok;
}

napi_status SetNameProperty(
    const napi_env &env, napi_value object, const std::string &name, std::vector<KeyEvent::KeyItem> &value)
{
    napi_value keys{};
    auto status = napi_create_array(env, &keys);
    CHECK_RETURN(status != napi_ok, "create array failed", status);
    uint32_t idx = 0;
    for (const auto &keyItem : value) {
        napi_value item{};
        status = keyItem.WriteToJsValue(env, item);
        CHECK_RETURN(status != napi_ok, "create key property failed", status);
        status = napi_set_element(env, keys, idx, item);
        CHECK_RETURN(status != napi_ok, "set element failed", status);
        ++idx;
    }
    status = napi_set_named_property(env, object, "keys", keys);
    CHECK_RETURN(status != napi_ok, "set keys property failed", status);
    return napi_ok;
}

napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, napi_value value)
{
    auto status = napi_set_named_property(env, object, name.c_str(), value);
    return status;
}

bool GetNamePropertyBool(const napi_env& env, const napi_value& object, const std::string& name)
{
    napi_value napiValue = {};
    napi_get_named_property(env, object, name.c_str(), &napiValue);
    napi_valuetype tmpType = napi_undefined;
    if (napi_typeof(env, napiValue, &tmpType) != napi_ok) {
        MMI_HILOGE("Call napi_typeof failed");
        return false;
    }
    bool value = false;
    if (tmpType != napi_boolean) {
        MMI_HILOGI("The value is not bool");
        return value;
    }

    napi_get_value_bool(env, napiValue, &value);
    return value;
}

std::string GetNamePropertyString(const napi_env& env, const napi_value& object, const std::string& name)
{
    std::string value = "";
    napi_value napiValue = {};
    napi_get_named_property(env, object, name.c_str(), &napiValue);
    napi_valuetype tmpType = napi_undefined;
    if (napi_typeof(env, napiValue, &tmpType) != napi_ok) {
        MMI_HILOGE("Call napi_typeof failed");
        return value;
    }
    if (tmpType != napi_string) {
        MMI_HILOGI("The value is not string");
        return value;
    }

    char tmpValue[MAX_STRING_LEN] = { 0 };
    size_t typeLen = 0;
    napi_get_value_string_utf8(env, napiValue, tmpValue, MAX_STRING_LEN - 1, &typeLen);
    value = tmpValue;
    return value;
}

int32_t GetNamePropertyInt32(const napi_env& env, const napi_value& object, const std::string& name)
{
    int32_t value = 0;
    napi_value napiValue = {};
    napi_get_named_property(env, object, name.c_str(), &napiValue);
    napi_valuetype tmpType = napi_undefined;
    if (napi_typeof(env, napiValue, &tmpType) != napi_ok) {
        MMI_HILOGE("Call napi_typeof failed");
        return value;
    }
    if (tmpType != napi_number) {
        MMI_HILOGI("The value is not number");
        return value;
    }
    napi_get_value_int32(env, napiValue, &value);
    return value;
}

int64_t GetNamePropertyInt64(const napi_env& env, const napi_value& object, const std::string& name)
{
    int64_t value = 0;
    napi_value napiValue = {};
    napi_get_named_property(env, object, name.c_str(), &napiValue);
    napi_valuetype tmpType = napi_undefined;
    if (napi_typeof(env, napiValue, &tmpType) != napi_ok) {
        MMI_HILOGE("Call napi_typeof failed");
        return value;
    }
    if (tmpType != napi_number) {
        MMI_HILOGI("The value is not number");
        return value;
    }
    napi_get_value_int64(env, napiValue, &value);
    return value;
}

uint32_t GetNamePropertyUint32(const napi_env& env, const napi_value& object, const std::string& name)
{
    uint32_t value = 0;
    napi_value napiValue = {};
    napi_get_named_property(env, object, name.c_str(), &napiValue);
    napi_valuetype tmpType = napi_undefined;
    if (napi_typeof(env, napiValue, &tmpType) != napi_ok) {
        MMI_HILOGE("Call napi_typeof failed");
        return value;
    }
    if (tmpType != napi_number) {
        MMI_HILOGI("The value is not number");
        return value;
    }
    napi_get_value_uint32(env, napiValue, &value);
    return value;
}

KeyEvent::KeyItem GetNamePropertyKeyItem(const napi_env& env, const napi_value& object, const std::string& name)
{
    napi_value napiValue{};
    auto status = napi_get_named_property(env, object, name.c_str(), &napiValue);
    CHECK_RETURN(status != napi_ok, "get KeyItem property failed", status);
    KeyEvent::KeyItem keyItem;
    int32_t keyCode = GetNamePropertyInt32(env, napiValue, "code");
    keyItem.SetKeyCode(keyCode);
    int64_t pressedTime = GetNamePropertyInt64(env, napiValue, "pressedTime");
    keyItem.SetDownTime(pressedTime);
    int32_t deviceId = GetNamePropertyInt32(env, napiValue, "deviceId");
    keyItem.SetDeviceId(deviceId);
    return keyItem;
}

std::vector<KeyEvent::KeyItem> GetNamePropertyKeyItems(
    const napi_env &env, const napi_value &object, const std::string &name)
{
    napi_value napiValue = {};
    auto status = napi_get_named_property(env, object, name.c_str(), &napiValue);
    CHECK_RETURN(status != napi_ok, "get property failed", status);

    uint32_t length;
    status = napi_get_array_length(env, napiValue, &length);
    CHECK_RETURN(status != napi_ok, "get array length failed", status);

    std::vector<KeyEvent::KeyItem> keyItems;
    for (uint32_t i = 0; i < length; ++i) {
        napi_value element = {};
        status = napi_get_element(env, napiValue, i, &element);
        CHECK_RETURN((status != napi_ok) && (element != nullptr), "get element failed", status);
        KeyEvent::KeyItem keyItem;
        status = keyItem.ReadFromJsValue(env, element);
        CHECK_RETURN(status != napi_ok, "read keyItem property failed", status);
        keyItems.push_back(keyItem);
    }
    return keyItems;
}
} // namespace MMI
} // namespace OHOS
