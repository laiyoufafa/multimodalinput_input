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

#include "js_input_monitor_manager.h"

#include <uv.h>

#include "define_multimodal.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, MMI_LOG_DOMAIN, "JsInputMonitorManager" };

const std::string GET_CB_INFO = "napi_get_cb_info";
const std::string REFERENCE_UNREF = "napi_reference_unref";
} // namespace

JsInputMonitorManager& JsInputMonitorManager::GetInstance()
{
    static JsInputMonitorManager instance;
    return instance;
}

void JsInputMonitorManager::AddMonitor(napi_env jsEnv, const std::string &typeName, napi_value callback)
{
    CALL_LOG_ENTER;
    std::lock_guard<std::mutex> guard(mutex_);
    for (const auto& item : monitors_) {
        if ((item != nullptr) && (item->IsMatch(jsEnv, callback) != RET_ERR)) {
            MMI_HILOGD("add js monitor failed");
            return;
        }
    }
    auto monitor = std::make_unique<JsInputMonitor>(jsEnv, typeName, callback, nextId_++);
    CHKPV(monitor);
    if (!monitor->Start()) {
        THROWERR(jsEnv, "js monitor startup failedn");
        return;
    }
    monitors_.push_back(std::move(monitor));
}

void JsInputMonitorManager::RemoveMonitor(napi_env jsEnv, const std::string &typeName, napi_value callback)
{
    CALL_LOG_ENTER;
    std::unique_ptr<JsInputMonitor> monitor = nullptr;
    do {
        std::lock_guard<std::mutex> guard(mutex_);
        for (auto it = monitors_.begin(); it != monitors_.end();) {
            if ((*it) == nullptr) {
                monitors_.erase(it++);
                continue;
            }
            if ((*it)->GetTypeName() == typeName) {
                if ((*it)->IsMatch(jsEnv, callback) == RET_OK) {
                    monitor = std::move(*it);
                    monitors_.erase(it++);
                    MMI_HILOGD("Found monitor");
                    break;
                }
            }
            ++it;
        }
    } while (0);
    if (monitor != nullptr) {
        monitor->Stop();
    }
}

void JsInputMonitorManager::RemoveMonitor(napi_env jsEnv, const std::string &typeName)
{
    CALL_LOG_ENTER;
    std::list<std::unique_ptr<JsInputMonitor>> monitors;
    do {
        std::lock_guard<std::mutex> guard(mutex_);
        for (auto it = monitors_.begin(); it != monitors_.end();) {
            if ((*it) == nullptr) {
                monitors_.erase(it++);
                continue;
            }
            if ((*it)->GetTypeName() == typeName) {
                if ((*it)->IsMatch(jsEnv) == RET_OK) {
                    monitors.push_back(std::move(*it));
                    monitors_.erase(it++);
                    continue;
                }
            }
            ++it;
        }
    } while (0);

    for (const auto &item : monitors) {
        if (item != nullptr) {
            item->Stop();
        }
    }
}

void JsInputMonitorManager::RemoveMonitor(napi_env jsEnv)
{
    CALL_LOG_ENTER;
    std::list<std::unique_ptr<JsInputMonitor>> monitors;
    do {
        std::lock_guard<std::mutex> guard(mutex_);
        for (auto it = monitors_.begin(); it != monitors_.end();) {
            if ((*it) == nullptr) {
                monitors_.erase(it++);
                continue;
            }
            if ((*it)->IsMatch(jsEnv) == RET_OK) {
                monitors.push_back(std::move(*it));
                monitors_.erase(it++);
                continue;
            }
            ++it;
        }
    } while (0);

    for (const auto &item : monitors) {
        if (item != nullptr) {
            item->Stop();
        }
    }
}

const std::unique_ptr<JsInputMonitor>& JsInputMonitorManager::GetMonitor(int32_t id)
{
    CALL_LOG_ENTER;
    std::lock_guard<std::mutex> guard(mutex_);
    for (const auto &item : monitors_) {
        if ((item != nullptr) && (item->GetId() == id)) {
            return item;
        }
    }
    MMI_HILOGD("No monitor found");
    return nullptr;
}

bool JsInputMonitorManager::AddEnv(napi_env env, napi_callback_info cbInfo)
{
    CALL_LOG_ENTER;
    if (IsExisting(env)) {
        MMI_HILOGD("Env is already exists");
        return true;
    }
    napi_value thisVar = nullptr;
    void *data = nullptr;
    int32_t *id = new (std::nothrow) int32_t;
    CHKPF(id);
    *id = 0;
    CHKRB(env, napi_get_cb_info(env, cbInfo, nullptr, nullptr, &thisVar, &data), GET_CB_INFO);
    auto status = napi_wrap(env, thisVar, static_cast<void*>(id),
                            [](napi_env env, void *data, void *hint) {
                                MMI_HILOGD("napi_wrap enter");
                                int32_t *id = static_cast<int32_t *>(data);
                                delete id;
                                id = nullptr;
                                JsInputMonMgr.RemoveMonitor(env);
                                JsInputMonMgr.RemoveEnv(env);
                                MMI_HILOGD("napi_wrap leave");
                                }, nullptr, nullptr);
    if (status != napi_ok) {
        MMI_HILOGE("napi_wrap failed");
        delete id;
        return false;
    }
    napi_ref ref = nullptr;
    status = napi_create_reference(env, thisVar, 1, &ref);
    if (status != napi_ok) {
        MMI_HILOGE("napi_create_reference failed");
        delete id;
        return false;
    }
    auto iter = envManager_.insert(std::pair<napi_env, napi_ref>(env, ref));
    if (!iter.second) {
        MMI_HILOGE("Insert value failed");
        return false;
    }
    return true;
}

void JsInputMonitorManager::RemoveEnv(napi_env env)
{
    CALL_LOG_ENTER;
    auto it = envManager_.find(env);
    if (it == envManager_.end()) {
        MMI_HILOGD("No env found");
        return;
    }
    RemoveEnv(it);
}

void JsInputMonitorManager::RemoveEnv(std::map<napi_env, napi_ref>::iterator it)
{
    CALL_LOG_ENTER;
    uint32_t refCount;
    CHKRV(it->first, napi_reference_unref(it->first, it->second, &refCount), REFERENCE_UNREF);
    envManager_.erase(it);
}

void JsInputMonitorManager::RemoveAllEnv()
{
    CALL_LOG_ENTER;
    for (auto it = envManager_.begin(); it != envManager_.end();) {
        RemoveEnv(it++);
    }
}

bool JsInputMonitorManager::IsExisting(napi_env env)
{
    CALL_LOG_ENTER;
    auto it = envManager_.find(env);
    if (it == envManager_.end()) {
        MMI_HILOGD("No env found");
        return false;
    }

    return true;
}
} // namespace MMI
} // namespace OHOS
