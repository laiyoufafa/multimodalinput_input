/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef MULTIMODALINPUT_IPC_INTERFACE_CODE_H
#define MULTIMODALINPUT_IPC_INTERFACE_CODE_H

/* SAID: 3101 */
namespace OHOS {
namespace MMI {
enum class MultimodalinputConnectInterfaceCode {
    ALLOC_SOCKET_FD = 0,
    ADD_INPUT_EVENT_FILTER = 1,
    SET_POINTER_VISIBLE = 2,
    IS_POINTER_VISIBLE = 3,
    MARK_PROCESSED = 4,
    SUBSCRIBE_KEY_EVENT = 6,
    UNSUBSCRIBE_KEY_EVENT = 7,
    ADD_INPUT_HANDLER = 8,
    REMOVE_INPUT_HANDLER = 9,
    MARK_EVENT_CONSUMED = 10,
    MOVE_MOUSE = 11,
    INJECT_KEY_EVENT = 12,
    INJECT_POINTER_EVENT = 13,
    SET_ANR_OBSERVER = 14,
    SUPPORT_KEYS = 15,
    GET_DEVICE_IDS = 16,
    GET_DEVICE = 17,
    REGISTER_DEV_MONITOR = 18,
    UNREGISTER_DEV_MONITOR = 19,
    GET_KEYBOARD_TYPE = 20,
    SET_POINTER_SPEED = 21,
    GET_POINTER_SPEED = 22,
    SET_POINTER_STYLE = 23,
    GET_POINTER_STYLE = 24,
    SET_FUNCTION_KEY_STATE = 25,
    GET_FUNCTION_KEY_STATE = 26,
    RMV_INPUT_EVENT_FILTER = 27,
    SET_CAPTURE_MODE = 28,
    GET_DISPLAY_BIND_INFO = 29,
    SET_DISPLAY_BIND = 30,
    SET_INPUT_DEVICE_TO_SCREEN = 31,
    SET_POINTER_LOCATION = 32,
    GET_WINDOW_PID = 33,
    APPEND_EXTRA_DATA = 34,
    SUBSCRIBE_SWITCH_EVENT = 35,
    UNSUBSCRIBE_SWITCH_EVENT = 36,
    ENABLE_INPUT_DEVICE = 37,
    SET_MOUSE_PRIMARY_BUTTON = 38,
    GET_MOUSE_PRIMARY_BUTTON = 39,
    SET_KEY_DOWN_DURATION = 40,
    SET_HOVER_SCROLL_STATE = 41,
    GET_HOVER_SCROLL_STATE = 42,
    SET_MOUSE_SCROLL_ROWS = 43,
    GET_MOUSE_SCROLL_ROWS = 44,
    SET_TP_SCROLL_SWITCH = 45,
    GET_TP_SCROLL_SWITCH = 46,
    SET_TP_SCROLL_DIRECT_SWITCH = 47,
    GET_TP_SCROLL_DIRECT_SWITCH = 48,
    SET_TP_TAP_SWITCH = 49,
    GET_TP_TAP_SWITCH = 50,
    SET_TP_POINTER_SPEED = 51,
    GET_TP_POINTER_SPEED = 52,
    SET_MOUSE_ICON = 53,
    SET_KEYBOARD_REPEAT_DELAY = 54,
    SET_KEYBOARD_REPEAT_RATE = 55,
};

enum class MultimodalinputEventInterfaceCode {
    HANDLE_KEY_EVENT = 0,
    HANDLE_POINTER_EVENT = 1,
};

} // namespace MMI
} // namespace OHOS
#endif // MULTIMODALINPUT_IPC_INTERFACE_CODE_H