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

#include "js_input_device_manager.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "JsInputDeviceManager" };
std::mutex mutex_;
const std::string CREATE_PROMISE = "napi_create_promise";
const std::string CREATE_STRING_UTF8 = "napi_create_string_utf8";
const std::string GET_UNDEFINED = "napi_get_undefined";
const std::string RESOLVE_DEFERRED = "napi_resolve_deferred";
const std::string REJECT_DEFERRED = "napi_reject_deferred";
const std::string CREATE_REFERENCE = "napi_create_reference";
const std::string GET_REFERENCE = "napi_get_reference_value";
const std::string CALL_FUNCTION = "napi_call_function";
} // namespace

JsInputDeviceManager::JsInputDeviceManager()
{
    CALL_LOG_ENTER;
    InputDevImp.RegisterInputDeviceMonitor(TargetOn);
}

JsInputDeviceManager::~JsInputDeviceManager() {}

void JsInputDeviceManager::RegisterInputDeviceMonitor(napi_env env, std::string type, napi_value handle)
{
    CALL_LOG_ENTER;
    AddMonitor(env, type, handle);
}

void JsInputDeviceManager::UnRegisterInputDeviceMonitor(napi_env env, std::string type, napi_value handle)
{
    CALL_LOG_ENTER;
    RemoveMonitor(env, type, handle);
}

napi_value JsInputDeviceManager::GetDeviceIds(napi_env env, napi_value handle)
{
    CALL_LOG_ENTER;
    std::lock_guard<std::mutex> guard(mutex_);
    int32_t userData = InputDevImp.GetUserData();
    napi_value ret = CreateCallbackInfo(env, handle, userData);
    InputDevImp.GetInputDeviceIdsAsync(EmitJsIds);
    return ret;
}

napi_value JsInputDeviceManager::GetDevice(napi_env env, int32_t id, napi_value handle)
{
    CALL_LOG_ENTER;
    std::lock_guard<std::mutex> guard(mutex_);
    int32_t userData = InputDevImp.GetUserData();
    napi_value ret = CreateCallbackInfo(env, handle, userData);
    InputDevImp.GetInputDeviceAsync(id, EmitJsDev);
    return ret;
}

void AsyncCallbackWork(sptr<AsyncContext> asyncContext)
{
    CALL_LOG_ENTER;
    CHKPV(asyncContext);
    CHKPV(asyncContext->env);
    napi_env env = asyncContext->env;
    napi_value resource = nullptr;
    CHKRV(env, napi_create_string_utf8(env, "asyncWork", NAPI_AUTO_LENGTH, &resource), CREATE_STRING_UTF8);
    asyncContext->IncStrongRef(nullptr);
    napi_status status = napi_create_async_work(env, nullptr, resource,
        [](napi_env env, void* data) {
            AsyncContext* asyncContext = static_cast<AsyncContext*>(data);
            bool visible;
            asyncContext->reserve >> visible;
            asyncContext->errorCode = InputManager::GetInstance()->SetPointerVisible(visible);
        },
        [](napi_env env, napi_status status, void* data) {
            sptr<AsyncContext> asyncContext(static_cast<AsyncContext *>(data));
            /**
             * After the asynchronous task is created, the asyncCallbackInfo reference count is reduced
             * to 0 destructions, so you need to add 1 to the asyncCallbackInfo reference count when the
             * asynchronous task is created, and subtract 1 from the reference count after the naked
             * pointer is converted to a pointer when the asynchronous task is executed, the reference
             * count of the smart pointer is guaranteed to be 1.
             */
            asyncContext->DecStrongRef(nullptr);
            napi_value result = nullptr;
            CHKRV(env, napi_get_undefined(env, &result), GET_UNDEFINED);
            if (asyncContext->deferred) {
                if (asyncContext->errorCode == RET_OK) {
                    CHKRV(env, napi_resolve_deferred(env, asyncContext->deferred, result), RESOLVE_DEFERRED);
                } else {
                    CHKRV(env, napi_reject_deferred(env, asyncContext->deferred, result), REJECT_DEFERRED);
                }
            } else {
                napi_value callback = nullptr;
                CHKRV(env, napi_get_reference_value(env, asyncContext->callback, &callback), GET_REFERENCE);
                napi_value callResult = nullptr;
                CHKRV(env, napi_call_function(env, nullptr, callback, 1, &result, &callResult), CALL_FUNCTION);
            }
        },
        asyncContext.GetRefPtr(), &asyncContext->work);
    if (status != napi_ok || napi_queue_async_work(env, asyncContext->work) != napi_ok) {
        MMI_HILOGE("create async work fail");
        asyncContext->DecStrongRef(nullptr);
    }
}

napi_value JsInputDeviceManager::SetPointerVisible(napi_env env, bool visible, napi_value handle)
{
    CALL_LOG_ENTER;
    sptr<AsyncContext> asyncContext = new (std::nothrow) AsyncContext(env);
    if (asyncContext == nullptr) {
        THROWERR(env, "create AsyncContext failed");
        return nullptr;
    }
    asyncContext->reserve << visible;

    napi_value promise = nullptr;
    if (handle != nullptr) {
        CHKRP(env, napi_create_reference(env, handle, 1, &asyncContext->callback), CREATE_REFERENCE);
        CHKRP(env, napi_get_undefined(env, &promise), GET_UNDEFINED);
    } else {
        CHKRP(env, napi_create_promise(env, &asyncContext->deferred, &promise), CREATE_PROMISE);
    }
    AsyncCallbackWork(asyncContext);
    return promise;
}

napi_value JsInputDeviceManager::GetKeystrokeAbility(napi_env env, int32_t id, std::vector<int32_t> keyCodes,
                                                     napi_value handle)
{
    CALL_LOG_ENTER;
    std::lock_guard<std::mutex> guard(mutex_);
    int32_t userData = InputDevImp.GetUserData();
    napi_value ret = CreateCallbackInfo(env, handle, userData);
    InputDevImp.GetKeystrokeAbility(id, keyCodes, EmitJsKeystrokeAbility);
    return ret;
}

napi_value JsInputDeviceManager::GetKeyboardType(napi_env env, int32_t id, napi_value handle)
{
    CALL_LOG_ENTER;
    int32_t userData = InputDevImp.GetUserData();
    napi_value ret = CreateCallbackInfo(env, handle, userData);
    InputDevImp.GetKeyboardTypeAsync(id, EmitJsKeyboardType);
    return ret;
}

void JsInputDeviceManager::ResetEnv()
{
    CALL_LOG_ENTER;
    InputDevImp.UnRegisterInputDeviceMonitor();
    JsEventTarget::ResetEnv();
}
} // namespace MMI
} // namespace OHOS