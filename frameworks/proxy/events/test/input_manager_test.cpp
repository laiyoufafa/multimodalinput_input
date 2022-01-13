/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "input_manager.h"
#include "error_multimodal.h"
#include <sstream>
#include <gtest/gtest.h>
#include "define_multimodal.h"
#include "input_handler_manager.h"
#include "input_event_monitor_manager.h"
#include "input_manager.h"
#include "key_event_pre.h"
#include "multimodal_event_handler.h"
#include "pointer_event.h"
#include "proto.h"
#include "run_shell_util.h"

namespace {
using namespace testing::ext;
using namespace OHOS;
using namespace MMI;
namespace {
constexpr int32_t HOS_KEY_BACK = 2;
constexpr bool ACTION_DOWN = true;
constexpr bool ACTION_UP = false;
constexpr int32_t DEFAULT_DEVICE_ID = 1;
constexpr int32_t DEFAULT_POINTER_ID = 0;
constexpr int32_t NANOSECOND_TO_MILLISECOND = 1000000;
constexpr int32_t SEC_TO_NANOSEC = 1000000000;
constexpr int32_t SLEEP_FOR_A_SEC = 1000;
constexpr bool ISINTERCEPTED_TRUE = true;
constexpr int32_t SLEEP = 3000;
constexpr int32_t INDEX_FIRST = 1;
constexpr int32_t INDEX_SECOND = 2;
constexpr int32_t INDEX_THIRD = 3;
constexpr int32_t INDEX_INVALID = -1;
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "InputManagerTest" };
}

class InputManagerTest : public testing::Test {
public:
    static void SetUpTestCase(void) {}
    static void TearDownTestCase(void) {}
    static int64_t GetNanoTime();
    static std::string DumpPointerItem(const PointerEvent::PointerItem &item);
    static std::string DumpPointerEvent(const std::shared_ptr<PointerEvent> &pointE);
    static std::shared_ptr<PointerEvent> SetupPointerEvent001();
    static std::shared_ptr<PointerEvent> SetupPointerEvent002();
    static std::shared_ptr<PointerEvent> SetupPointerEvent003();
    static std::shared_ptr<PointerEvent> SetupPointerEvent006();
    static std::shared_ptr<PointerEvent> SetupPointerEvent007();
    static std::shared_ptr<PointerEvent> SetupPointerEvent008();
    static std::shared_ptr<PointerEvent> SetupPointerEvent009();
    static std::shared_ptr<PointerEvent> SetupPointerEvent012();
    static void TestSimulateInputEvent(std::shared_ptr<PointerEvent> pointerEvent);
    static void TestSimulateInputEvent_2(std::shared_ptr<PointerEvent> pointerEvent);
    std::shared_ptr<PointerEvent> TestMarkConsumedStep1();
    std::shared_ptr<PointerEvent> TestMarkConsumedStep2();
    void TestMarkConsumedStep3(RunShellUtil &runCommand, int32_t monitorId, int32_t eventId);
    void TestMarkConsumedStep4(RunShellUtil &runCommand);
    void TestMarkConsumedStep5(RunShellUtil &runCommand);
    void TestMarkConsumedStep6(RunShellUtil &runCommand);
    static void InterceptorManagerCallBack(std::shared_ptr<PointerEvent> interceptor);
    static void TouchPadMonitorCallBack(std::shared_ptr<OHOS::MMI::PointerEvent> pointerEvent);
};

int64_t InputManagerTest::GetNanoTime()
{
    struct timespec time = { 0 };
    clock_gettime(CLOCK_MONOTONIC, &time);
    return static_cast<uint64_t>(time.tv_sec) * SEC_TO_NANOSEC + time.tv_nsec;
}

class InputEventCallback : public OHOS::MMI::IInputEventConsumer {
public:
    virtual void OnInputEvent(std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent) const override
    {
        MMI_LOGT("OnInputEvent keyCode = %{public}d", keyEvent->GetKeyCode());
    }
    virtual void OnInputEvent(std::shared_ptr<PointerEvent> pointerEvent) const override
    {
        MMI_LOGT("PointerEvent received.");
    }
    virtual void OnInputEvent(std::shared_ptr<AxisEvent> axisEvent) const override {}
    static std::shared_ptr<InputEventCallback> GetPtr();
};

std::shared_ptr<InputEventCallback> InputEventCallback::GetPtr()
{
    return std::make_shared<InputEventCallback>();
}

std::shared_ptr<PointerEvent> InputManagerTest::TestMarkConsumedStep1()
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(823);   // test code，set the GlobalX = 823
    item.SetGlobalY(723);   // test code，set the GlobalY = 723
    item.SetPressure(5);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetId(1);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetPointerId(0);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);

    MMI_LOGD("Call InputManager::SimulatePointerEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    return pointerEvent;
}

std::shared_ptr<PointerEvent> InputManagerTest::TestMarkConsumedStep2()
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(1023);  // test code，set the GlobalX = 823
    item.SetGlobalY(723);   // test code，set the GlobalY = 723
    item.SetPressure(5);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetId(2);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetPointerId(0);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);

    MMI_LOGD("Call InputManager::SimulatePointerEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    return pointerEvent;
}

void InputManagerTest::TestMarkConsumedStep3(RunShellUtil &runCommand, int32_t monitorId, int32_t eventId)
{
    std::string command { "ClientMsgHandler: in OnPointerEvent, #[[:digit:]]\\{1,\\}, Operation canceled." };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::MarkConsumed ...");
    InputManager::GetInstance()->MarkConsumed(monitorId, eventId);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }
}

void InputManagerTest::TestMarkConsumedStep4(RunShellUtil &runCommand)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(1123);  // test code，set the GlobalX = 823
    item.SetGlobalY(723);   // test code，set the GlobalY = 723
    item.SetPressure(5);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetId(3);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetPointerId(0);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);

    std::string command {
        "EventDispatch: in handlePointerEvent, #[[:digit:]]\\{1,\\}, "
        "PointerEvent consumed,will not send to client."
    };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulatePointerEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }
}

void InputManagerTest::TestMarkConsumedStep5(RunShellUtil &runCommand)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(0);  // test code，set the GlobalX = 823
    item.SetGlobalY(0);   // test code，set the GlobalY = 723
    item.SetPressure(0);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetId(4);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_UP);
    pointerEvent->SetPointerId(0);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);

    std::string command {
        "EventDispatch: in handlePointerEvent, #[[:digit:]]\\{1,\\}, "
        "PointerEvent consumed,will not send to client."
    };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    pointerEvent->SetId(3);
    MMI_LOGD("Call InputManager::SimulatePointerEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }
}

void InputManagerTest::TestMarkConsumedStep6(RunShellUtil &runCommand)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(823);   // test code，set the GlobalX = 823
    item.SetGlobalY(723);   // test code，set the GlobalY = 723
    item.SetPressure(5);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetId(4);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetPointerId(0);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);

    std::string command {
        "ClientMsgHandler: in OnPointerEvent, #[[:digit:]]\\{1,\\}, "
        "pointer event dispatcher of client:"
    };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulatePointerEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }
}

HWTEST_F(InputManagerTest, InputManagerTest_SetWindowInputEventConsumer_001, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "ServerStartTime =";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);
    auto callBackPtr = InputEventCallback::GetPtr();
    EXPECT_TRUE(callBackPtr != nullptr);
    InputManager::GetInstance()->SetWindowInputEventConsumer(callBackPtr);

    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));
    OHOS::KeyEvent injectDownEvent;
    int64_t downTime = static_cast<int64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, HOS_KEY_BACK, downTime, 0, "", 0, 0, "", 0, false, 0,
        ISINTERCEPTED_TRUE);
    int32_t response = MMIEventHdl.InjectEvent(injectDownEvent);
    EXPECT_TRUE(response);

    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<int64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, HOS_KEY_BACK, downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    response = MMIEventHdl.InjectEvent(injectUpEvent);
    EXPECT_TRUE(response);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));

    std::vector<std::string> vLog;
    ASSERT_TRUE(runCommand.RunShellCommand(command, vLog) == RET_OK);
    MMI_LOGT("vlogsize = %{public}d", vLog.size());
    ASSERT_TRUE(vLog.size() > 0);
    if (log.size() == 0) {
        EXPECT_TRUE(vLog.size() > log.size());
        EXPECT_TRUE(vLog.back().find(command) != vLog.back().npos);
    } else {
        EXPECT_TRUE(std::strcmp(vLog.back().c_str(), log.back().c_str()) != 0);
    }
}

std::string InputManagerTest::DumpPointerItem(const PointerEvent::PointerItem &item)
{
    std::ostringstream strm;
    strm << "ClientMsgHandler: in OnPointerEvent, #[[:digit:]]\\{1,\\}, downTime=" << item.GetDownTime()
         << ",isPressed=" << std::boolalpha << item.IsPressed() << ",globalX=" << item.GetGlobalX()
         << ",globalY=" << item.GetGlobalY() << ",localX=" << item.GetLocalX()
         << ",localY=" << item.GetLocalY() << ",width=" << item.GetWidth() << ",height=" << item.GetHeight()
         << ",pressure=" << item.GetPressure();
    return strm.str();
}

std::string InputManagerTest::DumpPointerEvent(const std::shared_ptr<PointerEvent> &pointerEvent)
{
    const int precision = 2;
    std::ostringstream strm;
    strm << "ClientMsgHandler: in OnPointerEvent, #[[:digit:]]\\{1,\\}, eventType="
         << pointerEvent->GetEventType()
         << ",actionTime=" << pointerEvent->GetActionTime()
         << ",action=" << pointerEvent->GetAction()
         << ",actionStartTime=" << pointerEvent->GetActionStartTime()
         << ",flag=" << pointerEvent->GetFlag()
         << ",pointerAction=" << pointerEvent->GetPointerAction()
         << ",sourceType=" << pointerEvent->GetSourceType()
         << ",VerticalAxisValue=" << std::fixed << std::setprecision(precision)
         << pointerEvent->GetAxisValue(PointerEvent::AXIS_TYPE_SCROLL_VERTICAL)
         << ",HorizontalAxisValue=" << std::fixed << std::setprecision(precision)
         << pointerEvent->GetAxisValue(PointerEvent::AXIS_TYPE_SCROLL_HORIZONTAL);
    return strm.str();
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent001()
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(823);   // test code，set the GlobalX = 823
    item.SetGlobalY(723);   // test code，set the GlobalY = 723
    item.SetPressure(5);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    item.SetPointerId(1);   // test code，set the PointerId = 1
    item.SetGlobalX(1010);   // test code，set the GlobalX = 1010
    item.SetGlobalY(910);   // test code，set the GlobalY = 910
    item.SetPressure(7);    // test code，set the Pressure = 7
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetPointerId(1);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    return pointerEvent;
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent002()
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(823);   // test code，set the GlobalX = 823
    item.SetGlobalY(723);   // test code，set the GlobalY = 723
    item.SetPressure(5);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    item.SetPointerId(1);   // test code，set the PointerId = 1
    item.SetGlobalX(1000);   // test code，set the GlobalX = 1000
    item.SetGlobalY(610);   // test code，set the GlobalY = 610
    item.SetPressure(7);    // test code，set the Pressure = 7
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetPointerId(1);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    return pointerEvent;
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent003()
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);   // test code，set the PointerId = 0
    item.SetGlobalX(823);   // test code，set the GlobalX = 823
    item.SetGlobalY(723);   // test code，set the GlobalY = 723
    item.SetPressure(5);    // test code，set the Pressure = 5
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    item.SetPointerId(1);   // test code，set the PointerId = 1
    item.SetGlobalX(0);   // test code，set the GlobalX = 0
    item.SetGlobalY(0);   // test code，set the GlobalY = 0
    item.SetPressure(0);    // test code，set the Pressure = 0
    item.SetDeviceId(1);    // test code，set the DeviceId = 1
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_UP);
    pointerEvent->SetPointerId(1);  // test code，set the PointerId = 1
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    return pointerEvent;
}

void InputManagerTest::TestSimulateInputEvent(std::shared_ptr<PointerEvent> pointerEvent)
{
    RunShellUtil runCommand;

    PointerEvent::PointerItem item;
    pointerEvent->GetPointerItem(0, item);
    std::string sItem1 { DumpPointerItem(item) };
    std::vector<std::string> sLogItem1s;
    ASSERT_TRUE(runCommand.RunShellCommand(sItem1, sLogItem1s) == RET_OK);
    MMI_LOGD("sItem1 = %{public}s", sItem1.c_str());

    pointerEvent->GetPointerItem(1, item);
    std::string sItem2 { DumpPointerItem(item) };
    std::vector<std::string> sLogItem2s;
    ASSERT_TRUE(runCommand.RunShellCommand(sItem2, sLogItem2s) == RET_OK);
    MMI_LOGD("sItem2 = %{public}s", sItem2.c_str());

    std::string sPointeE { DumpPointerEvent(pointerEvent) };
    std::vector<std::string> sLogPointerEs;
    ASSERT_TRUE(runCommand.RunShellCommand(sPointeE, sLogPointerEs) == RET_OK);
    MMI_LOGD("sPointerE = %{public}s", sPointeE.c_str());

    std::string sCmd {
        "InputManagerImpl: in OnPointerEvent, #[[:digit:]]\\{1,\\}, "
        "Pointer event received, processing ..."
    };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(sCmd, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulateInputEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));

    std::vector<std::string> tLogItem1s;
    ASSERT_TRUE(runCommand.RunShellCommand(sItem1, tLogItem1s) == RET_OK);
    EXPECT_TRUE(tLogItem1s.size() > sLogItem1s.size());
    if (!tLogItem1s.empty() && !sLogItem1s.empty())
        EXPECT_TRUE(tLogItem1s.back() != sLogItem1s.back());

    std::vector<std::string> tLogItem2s;
    ASSERT_TRUE(runCommand.RunShellCommand(sItem2, tLogItem2s) == RET_OK);
    EXPECT_TRUE(tLogItem2s.size() > sLogItem2s.size());
    if (!tLogItem2s.empty() && !sLogItem2s.empty())
        EXPECT_TRUE(tLogItem2s.back() != sLogItem2s.back());

    std::vector<std::string> tLogPointerEs;
    ASSERT_TRUE(runCommand.RunShellCommand(sPointeE, tLogPointerEs) == RET_OK);
    EXPECT_TRUE(tLogPointerEs.size() > sLogPointerEs.size());
    if (!tLogPointerEs.empty() && !sLogPointerEs.empty())
        EXPECT_TRUE(tLogPointerEs.back() != sLogPointerEs.back());

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(sCmd, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_001, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent001() };
    TestSimulateInputEvent(pointerEvent);
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_002, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent002() };
    TestSimulateInputEvent(pointerEvent);
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_003, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent002() };
    TestSimulateInputEvent(pointerEvent);
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_004, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command { "CHKR(pointerEvent->GetPointerItem(pointerId, pointerItem)) errCode:65077251" };

    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    auto pointerEvent = PointerEvent::Create();
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_UP);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    pointerEvent->SetPointerId(-1);
    MMI_LOGD("Call InputManager::SimulateInputEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_005, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command { "ServerMsgHandler: in OnInjectPointerEvent, #[[:digit:]]\\{1,\\}, "
        "Unknown source type!" };

    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetPressure(5);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetSourceType(-1);
    pointerEvent->SetPointerId(0);
    MMI_LOGD("Call InputManager::SimulateInputEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

void InputManagerTest::TestSimulateInputEvent_2(std::shared_ptr<PointerEvent> pointerEvent)
{
    RunShellUtil runCommand;

    PointerEvent::PointerItem item;
    pointerEvent->GetPointerItem(1, item);
    std::string sItem1 { DumpPointerItem(item) };
    std::vector<std::string> sLogItem1s;
    ASSERT_TRUE(runCommand.RunShellCommand(sItem1, sLogItem1s) == RET_OK);
    MMI_LOGD("sItem1 = %{public}s", sItem1.c_str());

    std::string sPointeE { DumpPointerEvent(pointerEvent) };
    std::vector<std::string> sLogPointerEs;
    ASSERT_TRUE(runCommand.RunShellCommand(sPointeE, sLogPointerEs) == RET_OK);
    MMI_LOGD("sPointerE = %{public}s", sPointeE.c_str());

    std::string sCmd { "InputManagerImpl: in OnPointerEvent, #[[:digit:]]\\{1,\\}, "
        "Pointer event received, processing ..." };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(sCmd, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulateInputEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));

    std::vector<std::string> tLogItem1s;
    ASSERT_TRUE(runCommand.RunShellCommand(sItem1, tLogItem1s) == RET_OK);
    EXPECT_TRUE(tLogItem1s.size() > sLogItem1s.size());
    if (!tLogItem1s.empty() && !sLogItem1s.empty())
        EXPECT_TRUE(tLogItem1s.back() != sLogItem1s.back());

    std::vector<std::string> tLogPointerEs;
    ASSERT_TRUE(runCommand.RunShellCommand(sPointeE, tLogPointerEs) == RET_OK);
    EXPECT_TRUE(tLogPointerEs.size() > sLogPointerEs.size());
    if (!tLogPointerEs.empty() && !sLogPointerEs.empty())
        EXPECT_TRUE(tLogPointerEs.back() != sLogPointerEs.back());

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(sCmd, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent006()
{
    auto pointerEvent = PointerEvent::Create();
    int64_t downTime = static_cast<int64_t>(GetNanoTime()/NANOSECOND_TO_MILLISECOND);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_MOUSE);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_BUTTON_DOWN);
    pointerEvent->SetButtonId(PointerEvent::MOUSE_BUTTON_LEFT);
    pointerEvent->SetPointerId(1);
    pointerEvent->SetButtonPressed(PointerEvent::MOUSE_BUTTON_LEFT);
    PointerEvent::PointerItem item;
    item.SetPointerId(1);
    item.SetDownTime(downTime);
    item.SetPressed(true);

    item.SetGlobalX(200);
    item.SetGlobalY(200);
    item.SetLocalX(300);
    item.SetLocalY(300);

    item.SetWidth(0);
    item.SetHeight(0);
    item.SetPressure(0);
    item.SetDeviceId(0);
    pointerEvent->AddPointerItem(item);
    return pointerEvent;
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent007()
{
    auto pointerEvent = PointerEvent::Create();
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_MOUSE);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetPointerId(1);
    PointerEvent::PointerItem item;
    item.SetPointerId(1);
    item.SetDownTime(0);
    item.SetPressed(false);

    item.SetGlobalX(200);
    item.SetGlobalY(200);
    item.SetLocalX(300);
    item.SetLocalY(300);

    item.SetWidth(0);
    item.SetHeight(0);
    item.SetPressure(0);
    item.SetDeviceId(0);
    pointerEvent->AddPointerItem(item);
    return pointerEvent;
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent008()
{
    auto pointerEvent = PointerEvent::Create();
    int64_t downTime = static_cast<int64_t>(GetNanoTime()/NANOSECOND_TO_MILLISECOND);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_MOUSE);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_BUTTON_UP);
    pointerEvent->SetButtonId(PointerEvent::MOUSE_BUTTON_LEFT);
    pointerEvent->SetPointerId(1);
    pointerEvent->SetButtonPressed(PointerEvent::MOUSE_BUTTON_LEFT);
    PointerEvent::PointerItem item;
    item.SetPointerId(1);
    item.SetDownTime(downTime);
    item.SetPressed(false);

    item.SetGlobalX(200);
    item.SetGlobalY(200);
    item.SetLocalX(300);
    item.SetLocalY(300);

    item.SetWidth(0);
    item.SetHeight(0);
    item.SetPressure(0);
    item.SetDeviceId(0);
    pointerEvent->AddPointerItem(item);
    return pointerEvent;
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent009()
{
    auto pointerEvent = PointerEvent::Create();
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_MOUSE);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_AXIS_UPDATE);
    pointerEvent->SetPointerId(1);
    pointerEvent->SetAxisValue(PointerEvent::AXIS_TYPE_SCROLL_VERTICAL, -1.0000);
    PointerEvent::PointerItem item;
    item.SetPointerId(1);
    item.SetDownTime(0);
    item.SetPressed(false);

    item.SetGlobalX(200);
    item.SetGlobalY(200);
    item.SetLocalX(300);
    item.SetLocalY(300);

    item.SetWidth(0);
    item.SetHeight(0);
    item.SetPressure(0);
    item.SetDeviceId(0);
    pointerEvent->AddPointerItem(item);
    return pointerEvent;
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_006, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent006() };
    TestSimulateInputEvent_2(pointerEvent);
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_007, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent007() };
    TestSimulateInputEvent_2(pointerEvent);
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_008, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent008() };
    TestSimulateInputEvent_2(pointerEvent);
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_009, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent009() };
    TestSimulateInputEvent_2(pointerEvent);
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_010, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command { "ServerMsgHandler: in OnInjectPointerEvent, #[[:digit:]]\\{1,\\}, "
        "Unknown source type!" };

    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    auto pointerEvent = PointerEvent::Create();
    int64_t downTime = static_cast<int64_t>(GetNanoTime()/NANOSECOND_TO_MILLISECOND);
    pointerEvent->SetSourceType(-1);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_BUTTON_DOWN);
    pointerEvent->SetButtonId(PointerEvent::MOUSE_BUTTON_LEFT);
    pointerEvent->SetPointerId(1);
    pointerEvent->SetButtonPressed(PointerEvent::MOUSE_BUTTON_LEFT);
    PointerEvent::PointerItem item;
    item.SetDownTime(downTime);
    item.SetPressed(true);

    item.SetGlobalX(200);
    item.SetGlobalY(200);
    item.SetLocalX(300);
    item.SetLocalY(300);

    item.SetWidth(0);
    item.SetHeight(0);
    item.SetPressure(0);
    item.SetDeviceId(0);
    pointerEvent->AddPointerItem(item);
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_011, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command { "ServerMsgHandler: in OnInjectPointerEvent, #[[:digit:]]\\{1,\\}, "
        "Unknown source type!" };

    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    auto pointerEvent = PointerEvent::Create();
    int64_t downTime = static_cast<int64_t>(GetNanoTime()/NANOSECOND_TO_MILLISECOND);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_BUTTON_DOWN);
    pointerEvent->SetButtonId(PointerEvent::MOUSE_BUTTON_LEFT);
    pointerEvent->SetPointerId(1);
    pointerEvent->SetButtonPressed(PointerEvent::MOUSE_BUTTON_LEFT);
    PointerEvent::PointerItem item;
    item.SetDownTime(downTime);
    item.SetPressed(true);

    item.SetGlobalX(200);
    item.SetGlobalY(200);
    item.SetLocalX(300);
    item.SetLocalY(300);

    item.SetWidth(0);
    item.SetHeight(0);
    item.SetPressure(0);
    item.SetDeviceId(0);
    pointerEvent->AddPointerItem(item);
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

std::shared_ptr<PointerEvent> InputManagerTest::SetupPointerEvent012()
{
    auto pointerEvent = PointerEvent::Create();
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_MOUSE);
    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_AXIS_UPDATE);
    pointerEvent->SetPointerId(1);
    pointerEvent->SetAxisValue(PointerEvent::AXIS_TYPE_SCROLL_VERTICAL, 30.0);
    pointerEvent->SetAxisValue(PointerEvent::AXIS_TYPE_SCROLL_HORIZONTAL, 40.0);
    PointerEvent::PointerItem item;
    item.SetPointerId(1);
    item.SetDownTime(0);
    item.SetPressed(false);

    item.SetGlobalX(200);
    item.SetGlobalY(200);
    item.SetLocalX(300);
    item.SetLocalY(300);

    item.SetWidth(0);
    item.SetHeight(0);
    item.SetPressure(0);
    item.SetDeviceId(0);
    pointerEvent->AddPointerItem(item);
    return pointerEvent;
}

HWTEST_F(InputManagerTest, InputManager_SimulateInputEvent_012, TestSize.Level1)
{
    std::shared_ptr<PointerEvent> pointerEvent { SetupPointerEvent012() };
    TestSimulateInputEvent_2(pointerEvent);
}

struct PhysicalDisplayInfo g_defaultPhysicalInfo = {
    .id = 0,
    .leftDisplayId = 1,
    .upDisplayId = 2,
    .topLeftX = 3,
    .topLeftY = 8,
    .width = 10,
    .height = 12,
    .name = "physicalDisplay0",
    .seatId = "seat0",
    .seatName = "default0",
    .logicWidth = 4,
    .logicHeight = 5,
    .direction = Direction0,
};

struct WindowInfo g_defaultWindowInfo = {
    .id = 0,
    .pid = 10,
    .uid = 11,
    .topLeftX = 3,
    .topLeftY = 4,
    .width = 2,
    .height = 3,
    .displayId = 0,
    .agentWindowId = 1,
};
HWTEST_F(InputManagerTest, InputManagerTest_UpdateDisplayInfo_001, TestSize.Level1)
{
    std::vector<PhysicalDisplayInfo> physicalDisplays;
    physicalDisplays.push_back(g_defaultPhysicalInfo);

    std::vector<LogicalDisplayInfo> logicalDisplays;
    std::vector<WindowInfo> windowInfos;
    windowInfos.push_back(g_defaultWindowInfo);
    windowInfos.push_back(g_defaultWindowInfo);
    LogicalDisplayInfo logicalInfo1 = {
        .id = 0,
        .topLeftX = 1,
        .topLeftY = 2,
        .width = 4,
        .height = 8,
        .name = "logicalDisplay0",
        .seatId = "seat0",
        .seatName = "default0",
        .focusWindowId = 1,
        .windowsInfo_ = windowInfos,
    };
    logicalDisplays.push_back(logicalInfo1);
    InputManager::GetInstance()->UpdateDisplayInfo(physicalDisplays, logicalDisplays);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));
    InputManager::GetInstance()->UpdateDisplayInfo(physicalDisplays, logicalDisplays);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP));
}

void KeyMonitorCallBack(std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent)
{
    MMI_LOGD("KeyMonitorCallBack: keyCode = %{public}d, keyAction = %{public}d , action = %{public}d,"
             "deviceId=%{private}d, actionTime = %{public}d", keyEvent->GetKeyCode(), keyEvent->GetKeyAction(),
             keyEvent->GetAction(), keyEvent->GetDeviceId(), keyEvent->GetActionTime());
    EXPECT_EQ(keyEvent->GetKeyCode(), OHOS::MMI::KeyEvent::KEYCODE_BACK);
    EXPECT_EQ(keyEvent->GetKeyAction(), OHOS::MMI::KeyEvent::KEY_ACTION_UP);
    EXPECT_EQ(keyEvent->GetAction(), OHOS::MMI::KeyEvent::KEY_ACTION_UP);
    EXPECT_EQ(keyEvent->GetDeviceId(), 0);
}

HWTEST_F(InputManagerTest, InputManagerTest_AddMonitor_001, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = MMI_STANDARD_EVENT_SUCCESS;
    response = InputManager::GetInstance()->AddMonitor(KeyMonitorCallBack);
    EXPECT_EQ(MMI_STANDARD_EVENT_SUCCESS, response);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    OHOS::KeyEvent injectUpEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime()/NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, HOS_KEY_BACK, downTime, 0, "", 0, 0, "", 0, false, 0,
        ISINTERCEPTED_TRUE);
    response = MMIEventHdl.InjectEvent(injectUpEvent);
    EXPECT_TRUE(response);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    MMIEventHdl.RemoveInputEventMontior(INDEX_FIRST);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

HWTEST_F(InputManagerTest, InputManagerTest_AddMonitor_002, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = MMI_STANDARD_EVENT_SUCCESS;
    response = InputManager::GetInstance()->AddMonitor(KeyMonitorCallBack);
    EXPECT_EQ(MMI_STANDARD_EVENT_SUCCESS, response);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    response = InputManager::GetInstance()->AddMonitor(KeyMonitorCallBack);
    EXPECT_EQ(MMI_STANDARD_EVENT_SUCCESS, response);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    response = InputManager::GetInstance()->AddMonitor(KeyMonitorCallBack);
    EXPECT_EQ(MMI_STANDARD_EVENT_SUCCESS, response);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    OHOS::KeyEvent injectUpEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime()/NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, HOS_KEY_BACK, downTime, 0, "", 0, 0, "", 0, false, 0,
        ISINTERCEPTED_TRUE);
    response = MMIEventHdl.InjectEvent(injectUpEvent);
    EXPECT_TRUE(response);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    InputManager::GetInstance()->RemoveMonitor(INDEX_FIRST);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    InputManager::GetInstance()->RemoveMonitor(INDEX_SECOND);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    InputManager::GetInstance()->RemoveMonitor(INDEX_THIRD);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    InputManager::GetInstance()->RemoveMonitor(INDEX_INVALID);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

HWTEST_F(InputManagerTest, InputManagerTest_AddHandler_001, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string addCmd {
        "InputHandlerManagerGlobal: in AddMonitor, #[[:digit:]]\\{1,\\}, "
        "Service AddMonitor Success."
    };
    std::vector<std::string> addLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(addCmd, addLogs) == RET_OK);

    auto callBackPtr = InputEventCallback::GetPtr();
    EXPECT_TRUE(callBackPtr != nullptr);
    int32_t id1 = InputManager::GetInstance()->AddMonitor2(callBackPtr);
    EXPECT_TRUE(id1 >= 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> addItem;
    ASSERT_TRUE(runCommand.RunShellCommand(addCmd, addItem) == RET_OK);
    EXPECT_TRUE(addItem.size() > addLogs.size());
    if (!addItem.empty() && !addLogs.empty()) {
        EXPECT_TRUE(addItem.back() != addLogs.back());
    }
    InputManager::GetInstance()->RemoveMonitor2(id1);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
}

HWTEST_F(InputManagerTest, InputManagerTest_AddHandler_002, TestSize.Level1)
{
    RunShellUtil runCommand;
    auto callBackPtr = InputEventCallback::GetPtr();
    EXPECT_TRUE(callBackPtr != nullptr);

    int32_t id1 = InputManager::GetInstance()->AddMonitor2(callBackPtr);
    EXPECT_TRUE(id1 >= 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::string removeCmd {
        "InputHandlerManagerGlobal: in RemoveMonitor, #[[:digit:]]\\{1,\\}, "
        "Service RemoveMonitor Success."
    };
    std::vector<std::string> removeLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, removeLogs) == RET_OK);

    InputManager::GetInstance()->RemoveMonitor2(id1);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> removeItem;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, removeItem) == RET_OK);
    EXPECT_TRUE(removeItem.size() > removeLogs.size());
    if (!removeItem.empty() && !removeLogs.empty()) {
        EXPECT_TRUE(removeItem.back() != removeLogs.back());
    }
}

HWTEST_F(InputManagerTest, InputManagerTest_AddHandler_003, TestSize.Level1)
{
    const std::vector<int32_t>::size_type N_TEST_CASES { 3 };
    std::vector<int32_t> ids(N_TEST_CASES);
    std::vector<std::shared_ptr<InputEventCallback>> cbs(N_TEST_CASES);

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        cbs[i] = InputEventCallback::GetPtr();
        EXPECT_TRUE(cbs[i] != nullptr);
        ids[i] = InputManager::GetInstance()->AddMonitor2(cbs[i]);
        EXPECT_TRUE(ids[i] >= 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }

    RunShellUtil runCommand;
    std::string command {
        "InputManagerTest: in OnInputEvent, #[[:digit:]]\\{1,\\}, "
        "PointerEvent received."
    };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    auto pointerEvent = SetupPointerEvent001();
    MMI_LOGD("Call InputManager::SimulatePointerEvent ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() >= (sLogs.size() + N_TEST_CASES));

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        InputManager::GetInstance()->RemoveMonitor2(ids[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }
}

HWTEST_F(InputManagerTest, InputManagerTest_AddHandler_004, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command {
        "InputHandlerManager: in AddHandler, #[[:digit:]]\\{1,\\}, "
        "The number of handlers exceeds the maximum."
    };
    std::vector<std::string> addItem;
    ASSERT_TRUE(runCommand.RunShellCommand(command, addItem) == RET_OK);

    const std::vector<int32_t>::size_type N_TEST_CASES { MAX_N_INPUT_HANDLERS };
    std::vector<int32_t> ids(N_TEST_CASES);
    std::shared_ptr<InputEventCallback> cb = InputEventCallback::GetPtr();
    EXPECT_TRUE(cb != nullptr);

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        ids[i] = InputManager::GetInstance()->AddMonitor2(cb);
        EXPECT_TRUE(ids[i] >= 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }

    int32_t monitorId = InputManager::GetInstance()->AddMonitor2(cb);
    EXPECT_TRUE(monitorId < 0);

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        if (ids[i] > 0) {
            InputManager::GetInstance()->RemoveMonitor2(ids[i]);
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
        }
    }
    std::vector<std::string> addAfter;
    ASSERT_TRUE(runCommand.RunShellCommand(command, addAfter) == RET_OK);
    EXPECT_TRUE(addAfter.size() > addItem.size());
    if (!addAfter.empty() && !addItem.empty()) {
        EXPECT_TRUE(addAfter.back() != addItem.back());
    }
}

HWTEST_F(InputManagerTest, InputManagerTest_AddHandler_005, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::shared_ptr<InputEventCallback> cb = InputEventCallback::GetPtr();
    EXPECT_TRUE(cb != nullptr);
    int32_t monitorId = InputManager::GetInstance()->AddMonitor2(cb);
    EXPECT_TRUE(monitorId > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    TestMarkConsumedStep1();
    auto pointerEvent = TestMarkConsumedStep2();
    TestMarkConsumedStep3(runCommand, monitorId, pointerEvent->GetId());
    TestMarkConsumedStep4(runCommand);
    TestMarkConsumedStep5(runCommand);

    if (monitorId > 0) {
        InputManager::GetInstance()->RemoveMonitor2(monitorId);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }
}

HWTEST_F(InputManagerTest, InputManagerTest_AddHandler_006, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::shared_ptr<InputEventCallback> cb = InputEventCallback::GetPtr();
    EXPECT_TRUE(cb != nullptr);
    int32_t monitorId = InputManager::GetInstance()->AddMonitor2(cb);
    EXPECT_TRUE(monitorId > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    auto pointerEvent = TestMarkConsumedStep1();
    TestMarkConsumedStep3(runCommand, monitorId, pointerEvent->GetId());
    TestMarkConsumedStep4(runCommand);
    TestMarkConsumedStep6(runCommand);

    if (monitorId > 0) {
        InputManager::GetInstance()->RemoveMonitor2(monitorId);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_001
 * @tc.desc:Verify the subscribe key event.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_001, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_MENU);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(0);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [=](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent)
    {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("subscribe key event trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    OHOS::KeyEvent injectDownEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_MENU,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key
    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_MENU,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_002
 * @tc.desc:Verify the repeat subscribe key event.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_002, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_POWER);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(0);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent)
    {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("subscribe key event trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent)
    {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("subscribe key event trigger callback");
    });
    EXPECT_TRUE(response < 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    OHOS::KeyEvent injectDownEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_POWER,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key
    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_POWER,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(2);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_003
 * @tc.desc:Verify the unsubscribe key event.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_003, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_HOME);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(0);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent)
    {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("subscribe key event trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    OHOS::KeyEvent injectDownEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_HOME,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key
    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_HOME,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(3);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_004
 * @tc.desc:Verify down trigger subscribe key event.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_004, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_BACK);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(0);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent)
    {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("subscribe key event down trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    OHOS::KeyEvent injectDownEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_BACK,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key
    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_BACK,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(4);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_005
 * @tc.desc:Verify down trigger subscribe key event, need to hold down for a while.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_005, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_CALL);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(2000);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent) {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("hold down for a while. subscribe key event down trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    OHOS::KeyEvent injectDownEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_CALL,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // release pressed key
    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_CALL,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_006
 * @tc.desc:Verify down trigger subscribe key event, other keys are pressed during the hold time.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_006, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_ENDCALL);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(2000);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent) {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("hold down for a while. subscribe key event down trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    OHOS::KeyEvent injectDownEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_ENDCALL,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    // other keys are pressed during the hold time
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    OHOS::KeyEvent injectDownEvent2;
    injectDownEvent2.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_VOLUME_UP,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent2);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key1
    OHOS::KeyEvent injectUpEvent1;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent1.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_ENDCALL,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent1);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key2
    OHOS::KeyEvent injectUpEvent2;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent2.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_VOLUME_UP,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent2);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(6);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_007
 * @tc.desc:Verify up trigger subscribe key event.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_007, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_VOLUME_DOWN);
    keyOption->SetFinalKeyDown(false);
    keyOption->SetFinalKeyDownDuration(0);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent) {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("subscribe key event up trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime()/NANOSECOND_TO_MILLISECOND);
    OHOS::KeyEvent injectDownEvent;
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_VOLUME_DOWN,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key , up trigger
    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_VOLUME_DOWN,
                             downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(7);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_008
 * @tc.desc:Verify invalid parameter.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_008, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_VOLUME_MUTE);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(0);
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption, nullptr);
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

/**
 * @tc.name:InputManagerTest_SubscribeKeyEvent_009
 * @tc.desc:Verify subscribe different key event.
 * @tc.type: FUNC
 * @tc.require: SR000GGQL4  AR000GJNGN
 * @tc.author: yangguang
 */
HWTEST_F(InputManagerTest, InputManagerTest_SubscribeKeyEvent_009, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command = "consumer is null";
    std::vector<std::string> log;
    ASSERT_TRUE(runCommand.RunShellCommand(command, log) == RET_OK);

    int32_t response = -1;
    std::vector<int32_t> preKeys;
    std::shared_ptr<OHOS::MMI::KeyOption> keyOption = std::make_shared<OHOS::MMI::KeyOption>();
    keyOption->SetPreKeys(preKeys);
    keyOption->SetFinalKey(OHOS::MMI::KeyEvent::KEYCODE_MUTE);
    keyOption->SetFinalKeyDown(true);
    keyOption->SetFinalKeyDownDuration(0);
    // KEYCODE_MUTE, KEYCODE_HEADSETHOOK, MEDIA_PLAY, MEDIA_PAUSE, MEDIA_PLAY_PAUSE
    response = InputManager::GetInstance()->SubscribeKeyEvent(keyOption,
        [](std::shared_ptr<OHOS::MMI::KeyEvent> keyEvent) {
        MMI_LOGD("KeyEventId=%{public}d,KeyCode=%{public}d,ActionTime=%{public}d,"
                 "ActionStartTime=%{public}d,Action=%{public}d,KeyAction=%{public}d,"
                 "EventType=%{public}d,Flag=%{public}d",
                 keyEvent->GetId(), keyEvent->GetKeyCode(), keyEvent->GetActionTime(),
                 keyEvent->GetActionStartTime(), keyEvent->GetAction(), keyEvent->GetKeyAction(),
                 keyEvent->GetEventType(), keyEvent->GetFlag());
        MMI_LOGD("subscribe key event KEYCODE_MUTE trigger callback");
    });
    EXPECT_TRUE(response > 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    OHOS::KeyEvent injectDownEvent;
    uint64_t downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectDownEvent.Initialize(0, ACTION_DOWN, OHOS::MMI::KeyEvent::KEYCODE_MUTE,
                               downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectDownEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // release pressed key
    OHOS::KeyEvent injectUpEvent;
    downTime = static_cast<uint64_t>(GetNanoTime() / NANOSECOND_TO_MILLISECOND);
    injectUpEvent.Initialize(0, ACTION_UP, OHOS::MMI::KeyEvent::KEYCODE_MUTE,
                             downTime, 0, "", 0, 0, "", 0, false, 0, ISINTERCEPTED_TRUE);
    MMIEventHdl.InjectEvent(injectUpEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    InputManager::GetInstance()->UnsubscribeKeyEvent(8);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void InputManagerTest::InterceptorManagerCallBack(std::shared_ptr<PointerEvent> pointerEvent)
{
    int32_t pointerId = pointerEvent->GetPointerId();
    OHOS::MMI::PointerEvent::PointerItem pointerItem;
    pointerEvent->GetPointerItem(pointerId, pointerItem);
    MMI_LOGD("InterceptorManagerCallBack: pointerAction=%{public}d, pointerId=%{public}d,x=%{public}d,y=%{public}d",
             pointerEvent->GetPointerAction(), pointerEvent->GetPointerId(),
             pointerItem.GetGlobalX(), pointerItem.GetGlobalY());
}

HWTEST_F(InputManagerTest, InterceptorManagerTest_OnAddInterceptor_001, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call InterceptorManager ...");

     std::string removeCmd {
    "InputManagerTest: in InterceptorManagerCallBack, #[[:digit:]]\\{1,\\}, "
    "InterceptorManagerCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    InputManager::GetInstance()->AddInterceptor(PointerEvent::SOURCE_TYPE_TOUCHPAD,
                                                InterceptorManagerCallBack);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    int32_t response = 0;
    response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    InputManager::GetInstance()->RemoveInterceptor(INDEX_FIRST);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InterceptorManagerTest_OnAddInterceptor_002, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call InterceptorManager ...");

     std::string removeCmd {
    "InputManagerTest: in InterceptorManagerCallBack, #[[:digit:]]\\{1,\\}, "
    "InterceptorManagerCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    InputManager::GetInstance()->AddInterceptor(PointerEvent::SOURCE_TYPE_TOUCHPAD,
                                                InterceptorManagerCallBack);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    int32_t response = 0;
    response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    InputManager::GetInstance()->RemoveInterceptor(INDEX_FIRST);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InterceptorManagerTest_OnAddInterceptor_003, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_UP);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call InterceptorManager ...");

     std::string removeCmd {
    "InputManagerTest: in InterceptorManagerCallBack, #[[:digit:]]\\{1,\\}, "
    "InterceptorManagerCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    InputManager::GetInstance()->AddInterceptor(PointerEvent::SOURCE_TYPE_TOUCHPAD,
                                                InterceptorManagerCallBack);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    int32_t response = 0;
    response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    InputManager::GetInstance()->RemoveInterceptor(INDEX_FIRST);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InterceptorManagerTest_OnAddInterceptor_004, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call InterceptorManager ......");

    const std::vector<int32_t>::size_type N_TEST_CASES { 3 };
    std::vector<int32_t> ids(N_TEST_CASES);

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        ids[i] = InputManager::GetInstance()->AddInterceptor(PointerEvent::SOURCE_TYPE_TOUCHPAD,
                                                InterceptorManagerCallBack);
        EXPECT_TRUE(ids[i] >= 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }

    RunShellUtil runCommand;
    std::string removeCmd {
    "InputManagerTest: in InterceptorManagerCallBack, #[[:digit:]]\\{1,\\}, "
    "InterceptorManagerCallBack: pointerAction"
    };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() >= (sLogs.size() + N_TEST_CASES));

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        InputManager::GetInstance()->RemoveInterceptor(ids[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }
}

HWTEST_F(InputManagerTest, InterceptorManagerTest_OnAddInterceptor_005, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(-1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call InterceptorManager ...");

     std::string removeCmd {
    "InputManagerTest: in InterceptorManagerCallBack, #[[:digit:]]\\{1,\\}, "
    "InterceptorManagerCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    InputManager::GetInstance()->AddInterceptor(PointerEvent::SOURCE_TYPE_TOUCHPAD,
                                                InterceptorManagerCallBack);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    int32_t response = 0;
    response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    InputManager::GetInstance()->RemoveInterceptor(INDEX_FIRST);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() == sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() == sLogs.back());
}

void InputManagerTest::TouchPadMonitorCallBack(std::shared_ptr<OHOS::MMI::PointerEvent> pointerEvent)
{
    int32_t pointerId = pointerEvent->GetPointerId();
    OHOS::MMI::PointerEvent::PointerItem pointerItem;
    pointerEvent->GetPointerItem(pointerId, pointerItem);
    MMI_LOGD("TouchPadMonitorCallBack: pointerAction = %{public}d, pointerId = %{public}d,"
        "x = %{public}d, y = %{public}d", pointerEvent->GetPointerAction(),
        pointerEvent->GetPointerId(), pointerItem.GetGlobalX(), pointerItem.GetGlobalY());
}

HWTEST_F(InputManagerTest, InputManagerTest_OnAddTouchPadMonitor_001, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call MontiorManager ......");

    std::string removeCmd {
    "InputManagerTest: in TouchPadMonitorCallBack, #[[:digit:]]\\{1,\\}, "
    "TouchPadMonitorCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    int32_t monitorId { };
    monitorId = InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack);
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    int32_t response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InputManagerTest_OnAddTouchPadMonitor_002, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call MontiorManager ......");

    std::string removeCmd {
    "InputManagerTest: in TouchPadMonitorCallBack, #[[:digit:]]\\{1,\\}, "
    "TouchPadMonitorCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    int32_t monitorId { };
    monitorId = InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack);
    MMI_LOGD("InjectSucceeded: pointerAction = %{public}d", pointerEvent->GetPointerAction());
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    int32_t response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InputManagerTest_OnAddTouchPadMonitor_003, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_UP);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call MontiorManager ......");

    std::string removeCmd {
    "InputManagerTest: in TouchPadMonitorCallBack, #[[:digit:]]\\{1,\\}, "
    "TouchPadMonitorCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    int32_t monitorId { };
    monitorId = InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack);
    MMI_LOGD("InjectSucceeded: pointerAction = %{public}d", pointerEvent->GetPointerAction());
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    int32_t response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}

HWTEST_F(InputManagerTest, InputManagerTest_OnAddTouchPadMonitor_004, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_UP);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);
    MMI_LOGD("Call MontiorManager ......");

    const std::vector<int32_t>::size_type N_TEST_CASES { 3 };
    std::vector<int32_t> ids(N_TEST_CASES);

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        ids[i] = InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack);
        EXPECT_TRUE(ids[i] >= 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }

    RunShellUtil runCommand;
    std::string removeCmd {
    "InputManagerTest: in TouchPadMonitorCallBack, #[[:digit:]]\\{1,\\}, "
    "TouchPadMonitorCallBack: pointerAction"
    };
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);

    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);
    EXPECT_TRUE(tLogs.size() >= (sLogs.size() + N_TEST_CASES));

    for (std::vector<int32_t>::size_type i = 0; i < N_TEST_CASES; ++i) {
        InputManager::GetInstance()->RemoveInputEventTouchpadMontior(ids[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
    }
}

HWTEST_F(InputManagerTest, InputManagerTest_OnAddTouchPadMonitor_005, TestSize.Level1)
{
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetDownTime(10010);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(1);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetPointerId(0);
    pointerEvent->SetSourceType(-1);
    MMI_LOGD("Call MontiorManager ......");

    std::string removeCmd {
    "InputManagerTest: in TouchPadMonitorCallBack, #[[:digit:]]\\{1,\\}, "
    "TouchPadMonitorCallBack: pointerAction"
    };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, sLogs) == RET_OK);
    int32_t monitorId { };
    monitorId = InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack);
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    int32_t response = MMIEventHdl.InjectPointerEvent(pointerEvent);
    EXPECT_EQ(RET_OK, response);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(removeCmd, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() == sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() == sLogs.back());
}

HWTEST_F(InputManagerTest, InputManager_TouchPadSimulateInputEvent_001, TestSize.Level1)
{
    int32_t monitorId { InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack) };
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    int32_t actionTime { static_cast<int32_t>(GetSysClockTime()) };
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item { };
    item.SetPointerId(DEFAULT_POINTER_ID);
    item.SetDownTime(actionTime);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(DEFAULT_DEVICE_ID);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetActionTime(actionTime);
    pointerEvent->SetPointerId(DEFAULT_POINTER_ID);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);

    std::ostringstream sCmd;
    sCmd << "TouchPadMonitorCallBack: pointerAction=" << pointerEvent->GetPointerAction();
    std::string command { sCmd.str() };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulateInputEvent 1 ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
}

HWTEST_F(InputManagerTest, InputManager_TouchPadSimulateInputEvent_002, TestSize.Level1)
{
    int32_t monitorId { InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack) };
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    int32_t actionTime { static_cast<int32_t>(GetSysClockTime()) };
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item { };
    item.SetPointerId(DEFAULT_POINTER_ID);
    item.SetDownTime(actionTime);
    item.SetPressed(true);
    item.SetGlobalX(1000);
    item.SetGlobalY(610);
    item.SetDeviceId(DEFAULT_DEVICE_ID);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    pointerEvent->SetActionTime(actionTime);
    pointerEvent->SetPointerId(DEFAULT_POINTER_ID);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);

    std::ostringstream sCmd;
    sCmd << "TouchPadMonitorCallBack: pointerAction=" << pointerEvent->GetPointerAction();
    std::string command { sCmd.str() };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulateInputEvent 2 ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
}

HWTEST_F(InputManagerTest, InputManager_TouchPadSimulateInputEvent_003, TestSize.Level1)
{
    int32_t monitorId { InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack) };
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    int32_t actionTime { static_cast<int32_t>(GetSysClockTime()) };
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item { };
    item.SetPointerId(DEFAULT_POINTER_ID);
    item.SetDownTime(actionTime);
    item.SetPressed(false);
    item.SetGlobalX(0);
    item.SetGlobalY(0);
    item.SetDeviceId(DEFAULT_DEVICE_ID);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_UP);
    pointerEvent->SetActionTime(actionTime);
    pointerEvent->SetPointerId(DEFAULT_POINTER_ID);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);

    std::ostringstream sCmd;
    sCmd << "TouchPadMonitorCallBack: pointerAction=" << pointerEvent->GetPointerAction();
    std::string command { sCmd.str() };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulateInputEvent 3 ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
}

HWTEST_F(InputManagerTest, InputManager_TouchPadSimulateInputEvent_004, TestSize.Level1)
{
    int32_t monitorId { InputManager::GetInstance()->AddInputEventTouchpadMontior(TouchPadMonitorCallBack) };
    EXPECT_TRUE(monitorId != InputEventMonitorManager::INVALID_MONITOR_ID);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    int32_t actionTime { static_cast<int32_t>(GetSysClockTime()) };
    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item { };
    item.SetPointerId(DEFAULT_POINTER_ID);
    item.SetDownTime(actionTime);
    item.SetPressed(true);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetDeviceId(DEFAULT_DEVICE_ID);
    pointerEvent->AddPointerItem(item);

    item.SetPointerId(1);
    item.SetDownTime(actionTime);
    item.SetPressed(true);
    item.SetGlobalX(840);
    item.SetGlobalY(740);
    item.SetDeviceId(DEFAULT_DEVICE_ID);
    pointerEvent->AddPointerItem(item);

    item.SetPointerId(2);
    item.SetDownTime(actionTime);
    item.SetPressed(true);
    item.SetGlobalX(860);
    item.SetGlobalY(760);
    item.SetDeviceId(DEFAULT_DEVICE_ID);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetActionTime(actionTime);
    pointerEvent->SetPointerId(DEFAULT_POINTER_ID);
    pointerEvent->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHPAD);

    std::ostringstream sCmd;
    sCmd << "TouchPadMonitorCallBack: pointerAction=" << pointerEvent->GetPointerAction();
    std::string command { sCmd.str() };
    RunShellUtil runCommand;
    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    MMI_LOGD("Call InputManager::SimulateInputEvent 4 ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty()) {
        EXPECT_TRUE(tLogs.back() != sLogs.back());
    }

    InputManager::GetInstance()->RemoveInputEventTouchpadMontior(monitorId);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));
}

HWTEST_F(InputManagerTest, InputManager_TouchPadSimulateInputEvent_005, TestSize.Level1)
{
    RunShellUtil runCommand;
    std::string command { "EventDispatch: in handlePointerEvent, #[[:digit:]]\\{1,\\}, "
        "Unknown source type!" };

    std::vector<std::string> sLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, sLogs) == RET_OK);

    auto pointerEvent = PointerEvent::Create();
    PointerEvent::PointerItem item;
    item.SetPointerId(0);
    item.SetGlobalX(823);
    item.SetGlobalY(723);
    item.SetPressure(5);
    pointerEvent->AddPointerItem(item);

    pointerEvent->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);
    pointerEvent->SetSourceType(-1);
    pointerEvent->SetPointerId(0);
    MMI_LOGD("Call InputManager::SimulateInputEvent 6 ...");
    InputManager::GetInstance()->SimulateInputEvent(pointerEvent);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR_A_SEC));

    std::vector<std::string> tLogs;
    ASSERT_TRUE(runCommand.RunShellCommand(command, tLogs) == RET_OK);

    MMI_LOGD("tLogs.size() = %{public}d", tLogs.size());
    EXPECT_TRUE(tLogs.size() > sLogs.size());
    if (!tLogs.empty() && !sLogs.empty())
        EXPECT_TRUE(tLogs.back() != sLogs.back());
}
}