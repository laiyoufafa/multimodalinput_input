/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include <cstdio>

#include "libinput.h"
#include "mouse_event_normalize.h"

namespace OHOS {
namespace MMI {
namespace {
using namespace testing::ext;
}
class MouseEventNormalizeTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void MouseEventNormalizeTest::SetUpTestCase(void)
{
}

void MouseEventNormalizeTest::TearDownTestCase(void)
{
}

void MouseEventNormalizeTest::SetUp()
{
}

void MouseEventNormalizeTest::TearDown()
{
}

/**
 * @tc.name: MouseEventNormalizeTest_GetDisplayId()_001
 * @tc.desc: Test GetDisplayId()
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_GetDisplayId_001, TestSize.Level1)
{
    int32_t idNames = -1;
    ASSERT_EQ(MouseEventHdr->GetDisplayId(), idNames);
}

/**
 * @tc.name: MouseEventNormalizeTest_GetPointerEvent_002
 * @tc.desc: Test GetPointerEvent()
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_GetPointerEvent_002, TestSize.Level1)
{
    ASSERT_EQ(MouseEventHdr->GetPointerEvent(), nullptr);
}

/**
 * @tc.name: MouseEventNormalizeTest_OnEvent_003
 * @tc.desc: Test OnEvent
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_OnEvent_003, TestSize.Level1)
{
    libinput_event *event = nullptr;
    int idNames = -1;
    ASSERT_EQ(MouseEventHdr->OnEvent(event), idNames);
}

/**
 * @tc.name: MouseEventNormalizeTest_NormalizeMoveMouse_004
 * @tc.desc: Test NormalizeMoveMouse
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_NormalizeMoveMouse_004, TestSize.Level1)
{
    bool isNormalize = false;
    int32_t offsetX = 0;
    int32_t offsetY = 0;
    ASSERT_EQ(MouseEventHdr->NormalizeMoveMouse(offsetX, offsetY), isNormalize);
}

/**
 * @tc.name: MouseEventNormalizeTest_Dump_005
 * @tc.desc: Test Dump
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_Dump_005, TestSize.Level1)
{
    std::vector<std::string> args;
    std::vector<std::string> idNames;
    int32_t fd = 0;
    MouseEventHdr->Dump(fd, args);
    ASSERT_EQ(args, idNames);
}

/**
 * @tc.name: MouseEventNormalizeTest_SetPointerSpeed_006
 * @tc.desc: Test SetPointerSpeed
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_SetPointerSpeed_006, TestSize.Level1)
{
    int32_t idNames = 0;
    int32_t speed = 2;
    ASSERT_EQ(MouseEventHdr->SetPointerSpeed(speed), idNames);
}

/**
 * @tc.name: MouseEventNormalizeTest_GetPointerSpeed_007
 * @tc.desc: Test GetPointerSpeed
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_GetPointerSpeed_007, TestSize.Level1)
{
    int32_t speed = 2;
    MouseEventHdr->SetPointerSpeed(speed);
    int32_t idNames = 2;
    ASSERT_EQ(MouseEventHdr->GetPointerSpeed(), idNames);
}

/**
 * @tc.name: MouseEventNormalizeTest_SetPointerLocation_008
 * @tc.desc: Test SetPointerLocation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_SetPointerLocation_008, TestSize.Level1)
{
    int32_t idNames = -1;
    int32_t x = 0;
    int32_t y = 0;
    ASSERT_EQ(MouseEventHdr->SetPointerLocation(x, y), idNames);
}

/**
 * @tc.name: MouseEventNormalizeTest_SetMousePrimaryButton_009
 * @tc.desc: Test SetMousePrimaryButton
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_SetMousePrimaryButton_009, TestSize.Level1)
{
    int32_t primaryButton = 1;
    ASSERT_TRUE(MouseEventHdr->SetMousePrimaryButton(primaryButton) == RET_OK);
    const char *mouseFileName = "/data/service/el1/public/multimodalinput/mouse_settings.xml";
    ASSERT_TRUE(remove(mouseFileName) == RET_OK);
}

/**
 * @tc.name: MouseEventNormalizeTest_GetMousePrimaryButton_010
 * @tc.desc: Test GetMousePrimaryButton
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_GetMousePrimaryButton_010, TestSize.Level1)
{
    int32_t primaryButton = 1;
    MouseEventHdr->SetMousePrimaryButton(primaryButton);
    int32_t primaryButtonRes = 1;
    ASSERT_TRUE(MouseEventHdr->GetMousePrimaryButton() == primaryButtonRes);
    const char *mouseFileName = "/data/service/el1/public/multimodalinput/mouse_settings.xml";
    ASSERT_TRUE(remove(mouseFileName) == RET_OK);
}

/**
 * @tc.name: MouseEventNormalizeTest_SetMouseScrollRows_011
 * @tc.desc: Test SetMouseScrollRows
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_SetMouseScrollRows_011, TestSize.Level1)
{
    int32_t rows = 1;
    ASSERT_TRUE(MouseEventHdr->SetMouseScrollRows(rows) == RET_OK);
    const char *mouseFileName = "/data/service/el1/public/multimodalinput/mouse_settings.xml";
    ASSERT_TRUE(remove(mouseFileName) == RET_OK);
}

/**
 * @tc.name: MouseEventNormalizeTest_GetMouseScrollRows_012
 * @tc.desc: Test GetMouseScrollRows
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MouseEventNormalizeTest, MouseEventNormalizeTest_GetMouseScrollRows_012, TestSize.Level1)
{
    int32_t rows = 50;
    MouseEventHdr->SetMouseScrollRows(rows);
    int32_t newRows = 50;
    ASSERT_TRUE(MouseEventHdr->GetMouseScrollRows() == newRows);
    const char *mouseFileName = "/data/service/el1/public/multimodalinput/mouse_settings.xml";
    ASSERT_TRUE(remove(mouseFileName) == RET_OK);
}
}
}