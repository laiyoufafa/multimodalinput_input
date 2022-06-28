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

#include "js_key_code.h"

#include "mmi_log.h"
#include "util_napi.h"

#include "key_event.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "JsKeyCode" };
const std::string DEFINE_CLASS = "napi_define_class";
const std::string SET_NAMED_PROPERTY = "napi_set_named_property";
const std::string CREATE_INT32 = "napi_create_int32";
const std::string GET_CB_INFO = "napi_get_cb_info";
} // namespace

napi_value JsKeyCode::GetNapiInt32(napi_env env, int32_t code)
{
    CALL_LOG_ENTER;
    napi_value keyCode = nullptr;
    CHKRP(env, napi_create_int32(env, code, &keyCode), CREATE_INT32);
    return keyCode;
}

napi_value JsKeyCode::EnumClassConstructor(napi_env env, napi_callback_info info)
{
    CALL_LOG_ENTER;
    size_t argc = 0;
    napi_value args[1] = {0};
    napi_value ret = nullptr;
    void *data = nullptr;
    CHKRP(env, napi_get_cb_info(env, info, &argc, args, &ret, &data), GET_CB_INFO);
    return ret;
}

napi_value JsKeyCode::Export(napi_env env, napi_value exports)
{
    CALL_LOG_ENTER;
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FN", GetNapiInt32(env, KeyEvent::KEYCODE_FN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_UNKNOWN", GetNapiInt32(env, KeyEvent::KEYCODE_UNKNOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_HOME", GetNapiInt32(env, KeyEvent::KEYCODE_HOME)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BACK", GetNapiInt32(env, KeyEvent::KEYCODE_BACK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_PLAY_PAUSE", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_PLAY_PAUSE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_STOP", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_STOP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_NEXT", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_NEXT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_PREVIOUS", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_PREVIOUS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_REWIND", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_REWIND)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_FAST_FORWARD",
            GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_FAST_FORWARD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VOLUME_UP", GetNapiInt32(env, KeyEvent::KEYCODE_VOLUME_UP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VOLUME_DOWN", GetNapiInt32(env, KeyEvent::KEYCODE_VOLUME_DOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_POWER", GetNapiInt32(env, KeyEvent::KEYCODE_POWER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CAMERA", GetNapiInt32(env, KeyEvent::KEYCODE_CAMERA)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VOLUME_MUTE", GetNapiInt32(env, KeyEvent::KEYCODE_VOLUME_MUTE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MUTE", GetNapiInt32(env, KeyEvent::KEYCODE_MUTE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BRIGHTNESS_UP", GetNapiInt32(env, KeyEvent::KEYCODE_BRIGHTNESS_UP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BRIGHTNESS_DOWN", GetNapiInt32(env, KeyEvent::KEYCODE_BRIGHTNESS_DOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_0", GetNapiInt32(env, KeyEvent::KEYCODE_0)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_1", GetNapiInt32(env, KeyEvent::KEYCODE_1)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_2", GetNapiInt32(env, KeyEvent::KEYCODE_2)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_3", GetNapiInt32(env, KeyEvent::KEYCODE_3)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_4", GetNapiInt32(env, KeyEvent::KEYCODE_4)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_5", GetNapiInt32(env, KeyEvent::KEYCODE_5)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_6", GetNapiInt32(env, KeyEvent::KEYCODE_6)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_7", GetNapiInt32(env, KeyEvent::KEYCODE_7)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_8", GetNapiInt32(env, KeyEvent::KEYCODE_8)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_9", GetNapiInt32(env, KeyEvent::KEYCODE_9)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_STAR", GetNapiInt32(env, KeyEvent::KEYCODE_STAR)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_POUND", GetNapiInt32(env, KeyEvent::KEYCODE_POUND)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DPAD_UP", GetNapiInt32(env, KeyEvent::KEYCODE_DPAD_UP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DPAD_DOWN", GetNapiInt32(env, KeyEvent::KEYCODE_DPAD_DOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DPAD_LEFT", GetNapiInt32(env, KeyEvent::KEYCODE_DPAD_LEFT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DPAD_RIGHT", GetNapiInt32(env, KeyEvent::KEYCODE_DPAD_RIGHT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DPAD_CENTER", GetNapiInt32(env, KeyEvent::KEYCODE_DPAD_CENTER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_A", GetNapiInt32(env, KeyEvent::KEYCODE_A)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_B", GetNapiInt32(env, KeyEvent::KEYCODE_B)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_C", GetNapiInt32(env, KeyEvent::KEYCODE_C)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_D", GetNapiInt32(env, KeyEvent::KEYCODE_D)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_E", GetNapiInt32(env, KeyEvent::KEYCODE_E)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F", GetNapiInt32(env, KeyEvent::KEYCODE_F)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_G", GetNapiInt32(env, KeyEvent::KEYCODE_G)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_H", GetNapiInt32(env, KeyEvent::KEYCODE_H)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_I", GetNapiInt32(env, KeyEvent::KEYCODE_I)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_J", GetNapiInt32(env, KeyEvent::KEYCODE_J)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_K", GetNapiInt32(env, KeyEvent::KEYCODE_K)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_L", GetNapiInt32(env, KeyEvent::KEYCODE_L)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_M", GetNapiInt32(env, KeyEvent::KEYCODE_M)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_N", GetNapiInt32(env, KeyEvent::KEYCODE_N)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_O", GetNapiInt32(env, KeyEvent::KEYCODE_O)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_P", GetNapiInt32(env, KeyEvent::KEYCODE_P)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_Q", GetNapiInt32(env, KeyEvent::KEYCODE_Q)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_R", GetNapiInt32(env, KeyEvent::KEYCODE_R)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_S", GetNapiInt32(env, KeyEvent::KEYCODE_S)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_T", GetNapiInt32(env, KeyEvent::KEYCODE_T)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_U", GetNapiInt32(env, KeyEvent::KEYCODE_U)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_V", GetNapiInt32(env, KeyEvent::KEYCODE_V)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_W", GetNapiInt32(env, KeyEvent::KEYCODE_W)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_X", GetNapiInt32(env, KeyEvent::KEYCODE_X)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_Y", GetNapiInt32(env, KeyEvent::KEYCODE_Y)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_Z", GetNapiInt32(env, KeyEvent::KEYCODE_Z)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_COMMA", GetNapiInt32(env, KeyEvent::KEYCODE_COMMA)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PERIOD", GetNapiInt32(env, KeyEvent::KEYCODE_PERIOD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ALT_LEFT", GetNapiInt32(env, KeyEvent::KEYCODE_ALT_LEFT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ALT_RIGHT", GetNapiInt32(env, KeyEvent::KEYCODE_ALT_RIGHT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SHIFT_LEFT", GetNapiInt32(env, KeyEvent::KEYCODE_SHIFT_LEFT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SHIFT_RIGHT", GetNapiInt32(env, KeyEvent::KEYCODE_SHIFT_RIGHT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_TAB", GetNapiInt32(env, KeyEvent::KEYCODE_TAB)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SPACE", GetNapiInt32(env, KeyEvent::KEYCODE_SPACE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SYM", GetNapiInt32(env, KeyEvent::KEYCODE_SYM)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_EXPLORER", GetNapiInt32(env, KeyEvent::KEYCODE_EXPLORER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ENVELOPE", GetNapiInt32(env, KeyEvent::KEYCODE_ENVELOPE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ENTER", GetNapiInt32(env, KeyEvent::KEYCODE_ENTER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DEL", GetNapiInt32(env, KeyEvent::KEYCODE_DEL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_GRAVE", GetNapiInt32(env, KeyEvent::KEYCODE_GRAVE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MINUS", GetNapiInt32(env, KeyEvent::KEYCODE_MINUS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_EQUALS", GetNapiInt32(env, KeyEvent::KEYCODE_EQUALS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_LEFT_BRACKET", GetNapiInt32(env, KeyEvent::KEYCODE_LEFT_BRACKET)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_RIGHT_BRACKET", GetNapiInt32(env, KeyEvent::KEYCODE_RIGHT_BRACKET)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BACKSLASH", GetNapiInt32(env, KeyEvent::KEYCODE_BACKSLASH)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SEMICOLON", GetNapiInt32(env, KeyEvent::KEYCODE_SEMICOLON)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_APOSTROPHE", GetNapiInt32(env, KeyEvent::KEYCODE_APOSTROPHE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SLASH", GetNapiInt32(env, KeyEvent::KEYCODE_SLASH)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_AT", GetNapiInt32(env, KeyEvent::KEYCODE_AT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PLUS", GetNapiInt32(env, KeyEvent::KEYCODE_PLUS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MENU", GetNapiInt32(env, KeyEvent::KEYCODE_MENU)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PAGE_UP", GetNapiInt32(env, KeyEvent::KEYCODE_PAGE_UP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PAGE_DOWN", GetNapiInt32(env, KeyEvent::KEYCODE_PAGE_DOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ESCAPE", GetNapiInt32(env, KeyEvent::KEYCODE_ESCAPE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FORWARD_DEL", GetNapiInt32(env, KeyEvent::KEYCODE_FORWARD_DEL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CTRL_LEFT", GetNapiInt32(env, KeyEvent::KEYCODE_CTRL_LEFT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CTRL_RIGHT", GetNapiInt32(env, KeyEvent::KEYCODE_CTRL_RIGHT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CAPS_LOCK", GetNapiInt32(env, KeyEvent::KEYCODE_CAPS_LOCK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SCROLL_LOCK", GetNapiInt32(env, KeyEvent::KEYCODE_SCROLL_LOCK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_META_LEFT", GetNapiInt32(env, KeyEvent::KEYCODE_META_LEFT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_META_RIGHT", GetNapiInt32(env, KeyEvent::KEYCODE_META_RIGHT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FUNCTION", GetNapiInt32(env, KeyEvent::KEYCODE_FUNCTION)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SYSRQ", GetNapiInt32(env, KeyEvent::KEYCODE_SYSRQ)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BREAK", GetNapiInt32(env, KeyEvent::KEYCODE_BREAK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MOVE_HOME", GetNapiInt32(env, KeyEvent::KEYCODE_MOVE_HOME)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MOVE_END", GetNapiInt32(env, KeyEvent::KEYCODE_MOVE_END)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_INSERT", GetNapiInt32(env, KeyEvent::KEYCODE_INSERT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FORWARD", GetNapiInt32(env, KeyEvent::KEYCODE_FORWARD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_PLAY", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_PLAY)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_PAUSE", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_PAUSE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_CLOSE", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_CLOSE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_EJECT", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_EJECT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_RECORD", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_RECORD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F1", GetNapiInt32(env, KeyEvent::KEYCODE_F1)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F2", GetNapiInt32(env, KeyEvent::KEYCODE_F2)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F3", GetNapiInt32(env, KeyEvent::KEYCODE_F3)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F4", GetNapiInt32(env, KeyEvent::KEYCODE_F4)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F5", GetNapiInt32(env, KeyEvent::KEYCODE_F5)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F6", GetNapiInt32(env, KeyEvent::KEYCODE_F6)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F7", GetNapiInt32(env, KeyEvent::KEYCODE_F7)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F8", GetNapiInt32(env, KeyEvent::KEYCODE_F8)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F9", GetNapiInt32(env, KeyEvent::KEYCODE_F9)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F10", GetNapiInt32(env, KeyEvent::KEYCODE_F10)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F11", GetNapiInt32(env, KeyEvent::KEYCODE_F11)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F12", GetNapiInt32(env, KeyEvent::KEYCODE_F12)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUM_LOCK", GetNapiInt32(env, KeyEvent::KEYCODE_NUM_LOCK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_0", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_0)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_1", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_1)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_2", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_2)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_3", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_3)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_4", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_4)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_5", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_5)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_6", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_6)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_7", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_7)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_8", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_8)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_9", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_9)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_DIVIDE", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_DIVIDE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_MULTIPLY", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_MULTIPLY)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_SUBTRACT", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_SUBTRACT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_ADD", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_ADD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_DOT", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_DOT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_COMMA", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_COMMA)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_ENTER", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_ENTER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_EQUALS", GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_EQUALS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_LEFT_PAREN",
            GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_LEFT_PAREN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_RIGHT_PAREN",
            GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_RIGHT_PAREN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VIRTUAL_MULTITASK",
            GetNapiInt32(env, KeyEvent::KEYCODE_VIRTUAL_MULTITASK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SLEEP", GetNapiInt32(env, KeyEvent::KEYCODE_SLEEP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ZENKAKU_HANKAKU", GetNapiInt32(env, KeyEvent::KEYCODE_ZENKAKU_HANKAKU)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_102ND", GetNapiInt32(env, KeyEvent::KEYCODE_102ND)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_RO", GetNapiInt32(env, KeyEvent::KEYCODE_RO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KATAKANA", GetNapiInt32(env, KeyEvent::KEYCODE_KATAKANA)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_HIRAGANA", GetNapiInt32(env, KeyEvent::KEYCODE_HIRAGANA)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_HENKAN", GetNapiInt32(env, KeyEvent::KEYCODE_HENKAN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KATAKANA_HIRAGANA",
            GetNapiInt32(env, KeyEvent::KEYCODE_KATAKANA_HIRAGANA)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MUHENKAN", GetNapiInt32(env, KeyEvent::KEYCODE_MUHENKAN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_LINEFEED", GetNapiInt32(env, KeyEvent::KEYCODE_LINEFEED)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MACRO", GetNapiInt32(env, KeyEvent::KEYCODE_MACRO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NUMPAD_PLUSMINUS",
            GetNapiInt32(env, KeyEvent::KEYCODE_NUMPAD_PLUSMINUS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SCALE", GetNapiInt32(env, KeyEvent::KEYCODE_SCALE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_HANGUEL", GetNapiInt32(env, KeyEvent::KEYCODE_HANGUEL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_HANJA", GetNapiInt32(env, KeyEvent::KEYCODE_HANJA)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_YEN", GetNapiInt32(env, KeyEvent::KEYCODE_YEN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_STOP", GetNapiInt32(env, KeyEvent::KEYCODE_STOP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_AGAIN", GetNapiInt32(env, KeyEvent::KEYCODE_AGAIN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PROPS", GetNapiInt32(env, KeyEvent::KEYCODE_PROPS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_UNDO", GetNapiInt32(env, KeyEvent::KEYCODE_UNDO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_COPY", GetNapiInt32(env, KeyEvent::KEYCODE_COPY)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_OPEN", GetNapiInt32(env, KeyEvent::KEYCODE_OPEN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PASTE", GetNapiInt32(env, KeyEvent::KEYCODE_PASTE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FIND", GetNapiInt32(env, KeyEvent::KEYCODE_FIND)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CUT", GetNapiInt32(env, KeyEvent::KEYCODE_CUT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_HELP", GetNapiInt32(env, KeyEvent::KEYCODE_HELP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CALC", GetNapiInt32(env, KeyEvent::KEYCODE_CALC)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FILE", GetNapiInt32(env, KeyEvent::KEYCODE_FILE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BOOKMARKS", GetNapiInt32(env, KeyEvent::KEYCODE_BOOKMARKS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NEXT", GetNapiInt32(env, KeyEvent::KEYCODE_NEXT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PLAYPAUSE", GetNapiInt32(env, KeyEvent::KEYCODE_PLAYPAUSE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PREVIOUS", GetNapiInt32(env, KeyEvent::KEYCODE_PREVIOUS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_STOPCD", GetNapiInt32(env, KeyEvent::KEYCODE_STOPCD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CONFIG", GetNapiInt32(env, KeyEvent::KEYCODE_CONFIG)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_REFRESH", GetNapiInt32(env, KeyEvent::KEYCODE_REFRESH)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_EXIT", GetNapiInt32(env, KeyEvent::KEYCODE_EXIT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_EDIT", GetNapiInt32(env, KeyEvent::KEYCODE_EDIT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SCROLLUP", GetNapiInt32(env, KeyEvent::KEYCODE_SCROLLUP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SCROLLDOWN", GetNapiInt32(env, KeyEvent::KEYCODE_SCROLLDOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NEW", GetNapiInt32(env, KeyEvent::KEYCODE_NEW)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_REDO", GetNapiInt32(env, KeyEvent::KEYCODE_REDO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CLOSE", GetNapiInt32(env, KeyEvent::KEYCODE_CLOSE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PLAY", GetNapiInt32(env, KeyEvent::KEYCODE_PLAY)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BASSBOOST", GetNapiInt32(env, KeyEvent::KEYCODE_BASSBOOST)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PRINT", GetNapiInt32(env, KeyEvent::KEYCODE_PRINT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CHAT", GetNapiInt32(env, KeyEvent::KEYCODE_CHAT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FINANCE", GetNapiInt32(env, KeyEvent::KEYCODE_FINANCE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CANCEL", GetNapiInt32(env, KeyEvent::KEYCODE_CANCEL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDILLUM_TOGGLE", GetNapiInt32(env, KeyEvent::KEYCODE_KBDILLUM_TOGGLE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDILLUM_DOWN", GetNapiInt32(env, KeyEvent::KEYCODE_KBDILLUM_DOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDILLUM_UP", GetNapiInt32(env, KeyEvent::KEYCODE_KBDILLUM_UP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SEND", GetNapiInt32(env, KeyEvent::KEYCODE_SEND)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_REPLY", GetNapiInt32(env, KeyEvent::KEYCODE_REPLY)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FORWARDMAIL", GetNapiInt32(env, KeyEvent::KEYCODE_FORWARDMAIL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SAVE", GetNapiInt32(env, KeyEvent::KEYCODE_SAVE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DOCUMENTS", GetNapiInt32(env, KeyEvent::KEYCODE_DOCUMENTS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VIDEO_NEXT", GetNapiInt32(env, KeyEvent::KEYCODE_VIDEO_NEXT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VIDEO_PREV", GetNapiInt32(env, KeyEvent::KEYCODE_VIDEO_PREV)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BRIGHTNESS_CYCLE",
            GetNapiInt32(env, KeyEvent::KEYCODE_BRIGHTNESS_CYCLE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BRIGHTNESS_ZERO", GetNapiInt32(env, KeyEvent::KEYCODE_BRIGHTNESS_ZERO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DISPLAY_OFF", GetNapiInt32(env, KeyEvent::KEYCODE_DISPLAY_OFF)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_MISC", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_MISC)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_GOTO", GetNapiInt32(env, KeyEvent::KEYCODE_GOTO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_INFO", GetNapiInt32(env, KeyEvent::KEYCODE_INFO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PROGRAM", GetNapiInt32(env, KeyEvent::KEYCODE_PROGRAM)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PVR", GetNapiInt32(env, KeyEvent::KEYCODE_PVR)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SUBTITLE", GetNapiInt32(env, KeyEvent::KEYCODE_SUBTITLE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FULL_SCREEN", GetNapiInt32(env, KeyEvent::KEYCODE_FULL_SCREEN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KEYBOARD", GetNapiInt32(env, KeyEvent::KEYCODE_KEYBOARD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ASPECT_RATIO", GetNapiInt32(env, KeyEvent::KEYCODE_ASPECT_RATIO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PC", GetNapiInt32(env, KeyEvent::KEYCODE_PC)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_TV", GetNapiInt32(env, KeyEvent::KEYCODE_TV)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_TV2", GetNapiInt32(env, KeyEvent::KEYCODE_TV2)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VCR", GetNapiInt32(env, KeyEvent::KEYCODE_VCR)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VCR2", GetNapiInt32(env, KeyEvent::KEYCODE_VCR2)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SAT", GetNapiInt32(env, KeyEvent::KEYCODE_SAT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CD", GetNapiInt32(env, KeyEvent::KEYCODE_CD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_TAPE", GetNapiInt32(env, KeyEvent::KEYCODE_TAPE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_TUNER", GetNapiInt32(env, KeyEvent::KEYCODE_TUNER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PLAYER", GetNapiInt32(env, KeyEvent::KEYCODE_PLAYER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DVD", GetNapiInt32(env, KeyEvent::KEYCODE_DVD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_AUDIO", GetNapiInt32(env, KeyEvent::KEYCODE_AUDIO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VIDEO", GetNapiInt32(env, KeyEvent::KEYCODE_VIDEO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEMO", GetNapiInt32(env, KeyEvent::KEYCODE_MEMO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CALENDAR", GetNapiInt32(env, KeyEvent::KEYCODE_CALENDAR)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_RED", GetNapiInt32(env, KeyEvent::KEYCODE_RED)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_GREEN", GetNapiInt32(env, KeyEvent::KEYCODE_GREEN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_YELLOW", GetNapiInt32(env, KeyEvent::KEYCODE_YELLOW)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BLUE", GetNapiInt32(env, KeyEvent::KEYCODE_BLUE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CHANNELUP", GetNapiInt32(env, KeyEvent::KEYCODE_CHANNELUP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CHANNELDOWN", GetNapiInt32(env, KeyEvent::KEYCODE_CHANNELDOWN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_LAST", GetNapiInt32(env, KeyEvent::KEYCODE_LAST)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_RESTART", GetNapiInt32(env, KeyEvent::KEYCODE_RESTART)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SLOW", GetNapiInt32(env, KeyEvent::KEYCODE_SLOW)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SHUFFLE", GetNapiInt32(env, KeyEvent::KEYCODE_SHUFFLE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VIDEOPHONE", GetNapiInt32(env, KeyEvent::KEYCODE_VIDEOPHONE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_GAMES", GetNapiInt32(env, KeyEvent::KEYCODE_GAMES)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ZOOMIN", GetNapiInt32(env, KeyEvent::KEYCODE_ZOOMIN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ZOOMOUT", GetNapiInt32(env, KeyEvent::KEYCODE_ZOOMOUT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ZOOMRESET", GetNapiInt32(env, KeyEvent::KEYCODE_ZOOMRESET)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_WORDPROCESSOR", GetNapiInt32(env, KeyEvent::KEYCODE_WORDPROCESSOR)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_EDITOR", GetNapiInt32(env, KeyEvent::KEYCODE_EDITOR)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SPREADSHEET", GetNapiInt32(env, KeyEvent::KEYCODE_SPREADSHEET)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_GRAPHICSEDITOR", GetNapiInt32(env, KeyEvent::KEYCODE_GRAPHICSEDITOR)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PRESENTATION", GetNapiInt32(env, KeyEvent::KEYCODE_PRESENTATION)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DATABASE", GetNapiInt32(env, KeyEvent::KEYCODE_DATABASE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_NEWS", GetNapiInt32(env, KeyEvent::KEYCODE_NEWS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_VOICEMAIL", GetNapiInt32(env, KeyEvent::KEYCODE_VOICEMAIL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ADDRESSBOOK", GetNapiInt32(env, KeyEvent::KEYCODE_ADDRESSBOOK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MESSENGER", GetNapiInt32(env, KeyEvent::KEYCODE_MESSENGER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BRIGHTNESS_TOGGLE",
            GetNapiInt32(env, KeyEvent::KEYCODE_BRIGHTNESS_TOGGLE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SPELLCHECK", GetNapiInt32(env, KeyEvent::KEYCODE_SPELLCHECK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_COFFEE", GetNapiInt32(env, KeyEvent::KEYCODE_COFFEE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MEDIA_REPEAT", GetNapiInt32(env, KeyEvent::KEYCODE_MEDIA_REPEAT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_IMAGES", GetNapiInt32(env, KeyEvent::KEYCODE_IMAGES)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BUTTONCONFIG", GetNapiInt32(env, KeyEvent::KEYCODE_BUTTONCONFIG)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_TASKMANAGER", GetNapiInt32(env, KeyEvent::KEYCODE_TASKMANAGER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_JOURNAL", GetNapiInt32(env, KeyEvent::KEYCODE_JOURNAL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CONTROLPANEL", GetNapiInt32(env, KeyEvent::KEYCODE_CONTROLPANEL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_APPSELECT", GetNapiInt32(env, KeyEvent::KEYCODE_APPSELECT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SCREENSAVER", GetNapiInt32(env, KeyEvent::KEYCODE_SCREENSAVER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ASSISTANT", GetNapiInt32(env, KeyEvent::KEYCODE_ASSISTANT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBD_LAYOUT_NEXT", GetNapiInt32(env, KeyEvent::KEYCODE_KBD_LAYOUT_NEXT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BRIGHTNESS_MIN", GetNapiInt32(env, KeyEvent::KEYCODE_BRIGHTNESS_MIN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BRIGHTNESS_MAX", GetNapiInt32(env, KeyEvent::KEYCODE_BRIGHTNESS_MAX)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDINPUTASSIST_PREV",
            GetNapiInt32(env, KeyEvent::KEYCODE_KBDINPUTASSIST_PREV)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDINPUTASSIST_NEXT",
            GetNapiInt32(env, KeyEvent::KEYCODE_KBDINPUTASSIST_NEXT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDINPUTASSIST_PREVGROUP",
            GetNapiInt32(env, KeyEvent::KEYCODE_KBDINPUTASSIST_PREVGROUP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDINPUTASSIST_NEXTGROUP",
            GetNapiInt32(env, KeyEvent::KEYCODE_KBDINPUTASSIST_NEXTGROUP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDINPUTASSIST_ACCEPT",
            GetNapiInt32(env, KeyEvent::KEYCODE_KBDINPUTASSIST_ACCEPT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_KBDINPUTASSIST_CANCEL",
            GetNapiInt32(env, KeyEvent::KEYCODE_KBDINPUTASSIST_CANCEL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_FRONT", GetNapiInt32(env, KeyEvent::KEYCODE_FRONT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SETUP", GetNapiInt32(env, KeyEvent::KEYCODE_SETUP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_WAKEUP", GetNapiInt32(env, KeyEvent::KEYCODE_WAKEUP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SENDFILE", GetNapiInt32(env, KeyEvent::KEYCODE_SENDFILE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DELETEFILE", GetNapiInt32(env, KeyEvent::KEYCODE_DELETEFILE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_XFER", GetNapiInt32(env, KeyEvent::KEYCODE_XFER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PROG1", GetNapiInt32(env, KeyEvent::KEYCODE_PROG1)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PROG2", GetNapiInt32(env, KeyEvent::KEYCODE_PROG2)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MSDOS", GetNapiInt32(env, KeyEvent::KEYCODE_MSDOS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SCREENLOCK", GetNapiInt32(env, KeyEvent::KEYCODE_SCREENLOCK)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DIRECTION_ROTATE_DISPLAY",
            GetNapiInt32(env, KeyEvent::KEYCODE_DIRECTION_ROTATE_DISPLAY)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CYCLEWINDOWS", GetNapiInt32(env, KeyEvent::KEYCODE_CYCLEWINDOWS)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_COMPUTER", GetNapiInt32(env, KeyEvent::KEYCODE_COMPUTER)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_EJECTCLOSECD", GetNapiInt32(env, KeyEvent::KEYCODE_EJECTCLOSECD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ISO", GetNapiInt32(env, KeyEvent::KEYCODE_ISO)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_MOVE", GetNapiInt32(env, KeyEvent::KEYCODE_MOVE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F13", GetNapiInt32(env, KeyEvent::KEYCODE_F13)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F14", GetNapiInt32(env, KeyEvent::KEYCODE_F14)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F15", GetNapiInt32(env, KeyEvent::KEYCODE_F15)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F16", GetNapiInt32(env, KeyEvent::KEYCODE_F16)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F17", GetNapiInt32(env, KeyEvent::KEYCODE_F17)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F18", GetNapiInt32(env, KeyEvent::KEYCODE_F18)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F19", GetNapiInt32(env, KeyEvent::KEYCODE_F19)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F20", GetNapiInt32(env, KeyEvent::KEYCODE_F20)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F21", GetNapiInt32(env, KeyEvent::KEYCODE_F21)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F22", GetNapiInt32(env, KeyEvent::KEYCODE_F22)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F23", GetNapiInt32(env, KeyEvent::KEYCODE_F23)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_F24", GetNapiInt32(env, KeyEvent::KEYCODE_F24)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PROG3", GetNapiInt32(env, KeyEvent::KEYCODE_PROG3)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_PROG4", GetNapiInt32(env, KeyEvent::KEYCODE_PROG4)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_DASHBOARD", GetNapiInt32(env, KeyEvent::KEYCODE_DASHBOARD)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SUSPEND", GetNapiInt32(env, KeyEvent::KEYCODE_SUSPEND)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_HP", GetNapiInt32(env, KeyEvent::KEYCODE_HP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SOUND", GetNapiInt32(env, KeyEvent::KEYCODE_SOUND)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_QUESTION", GetNapiInt32(env, KeyEvent::KEYCODE_QUESTION)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CONNECT", GetNapiInt32(env, KeyEvent::KEYCODE_CONNECT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SPORT", GetNapiInt32(env, KeyEvent::KEYCODE_SPORT)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SHOP", GetNapiInt32(env, KeyEvent::KEYCODE_SHOP)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_ALTERASE", GetNapiInt32(env, KeyEvent::KEYCODE_ALTERASE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_SWITCHVIDEOMODE", GetNapiInt32(env, KeyEvent::KEYCODE_SWITCHVIDEOMODE)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BATTERY", GetNapiInt32(env, KeyEvent::KEYCODE_BATTERY)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BLUETOOTH", GetNapiInt32(env, KeyEvent::KEYCODE_BLUETOOTH)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_WLAN", GetNapiInt32(env, KeyEvent::KEYCODE_WLAN)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_UWB", GetNapiInt32(env, KeyEvent::KEYCODE_UWB)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_WWAN_WIMAX", GetNapiInt32(env, KeyEvent::KEYCODE_WWAN_WIMAX)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_RFKILL", GetNapiInt32(env, KeyEvent::KEYCODE_RFKILL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_CHANNEL", GetNapiInt32(env, KeyEvent::KEYCODE_CHANNEL)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_0", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_0)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_1", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_1)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_2", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_2)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_3", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_3)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_4", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_4)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_5", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_5)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_6", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_6)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_7", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_7)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_8", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_8)),
        DECLARE_NAPI_STATIC_PROPERTY("KEYCODE_BTN_9", GetNapiInt32(env, KeyEvent::KEYCODE_BTN_9)),
    };

    napi_value result = nullptr;
    CHKRP(env, napi_define_class(env, "KeyCode", NAPI_AUTO_LENGTH, EnumClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result), DEFINE_CLASS);
    CHKRP(env, napi_set_named_property(env, exports, "KeyCode", result), SET_NAMED_PROPERTY);
    return exports;
}
} // namespace MMI
} // namespace OHOS