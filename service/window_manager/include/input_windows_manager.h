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
#ifndef INPUT_WINDOWS_MANAGER_H
#define INPUT_WINDOWS_MANAGER_H

#include <vector>
#include "singleton.h"
#include "uds_server.h"
#include "display_info.h"
#include "input_event.h"
#include "pointer_event.h"
#include "libinput.h"

namespace OHOS {
namespace MMI {
struct SurfaceInfo {
    int32_t surfaceId;
    int32_t dstX;
    int32_t dstY;
    int32_t dstW;
    int32_t dstH;
    int32_t srcX;
    int32_t srcY;
    int32_t srcW;
    int32_t srcH;
    double opacity;
    int32_t visibility; // 0 or 1
    int32_t onLayerId;
};
struct LayerInfo {
    int32_t layerId;
    int32_t dstX;
    int32_t dstY;
    int32_t dstW;
    int32_t dstH;
    int32_t srcX;
    int32_t srcY;
    int32_t srcW;
    int32_t srcH;
    double opacity;
    int32_t visibility; // 0 or 1
    int32_t onScreenId;
    int32_t nSurfaces;
    SurfaceInfo** surfaces;
};
struct ScreenInfo {
    int32_t screenId;
    char* connectorName;
    int32_t width;
    int32_t height;
    int32_t nLayers;
    LayerInfo** layers;
};
struct SeatInfo {
    char* seatName;
    int32_t deviceFlags;
    int32_t focusWindowId;
};

struct MouseLocation {
    int32_t globalX;
    int32_t globalY;
};

class InputWindowsManager : public DelayedSingleton<InputWindowsManager> {
public:
    InputWindowsManager();
    virtual ~InputWindowsManager();

    bool Init(UDSServer& udsServer);
    void UpdateSeatsInfo();
    void UpdateScreensInfo();


    void PrintAllNormalSurface();

    void Clear();
    void Dump(int32_t fd);

    /*********************************新框架接口添加*************************** */
    int32_t GetPidAndUpdateTarget(std::shared_ptr<InputEvent> inputEvent);
    int32_t UpdateTarget(std::shared_ptr<InputEvent> inputEvent);
    void UpdateDisplayInfo(const std::vector<PhysicalDisplayInfo> &physicalDisplays,
        const std::vector<LogicalDisplayInfo> &logicalDisplays);
    bool TouchPadPointToDisplayPoint_2(struct libinput_event_touch* touch,
        int32_t& logicalX, int32_t& logicalY, int32_t& logicalDisplayId);
    const std::vector<LogicalDisplayInfo>& GetLogicalDisplayInfo() const;
    const std::map<int32_t, WindowInfo>& GetWindowInfo() const;
    MouseLocation GetMouseInfo();
    void UpdateAndAdjustMouseLoction(double& x, double& y);
    void AdjustGlobalCoordinate(int32_t& globalX, int32_t& globalY, int32_t width, int32_t height);
    bool UpdataDisplayId(int32_t& displayId);
    LogicalDisplayInfo* GetLogicalDisplayId(int32_t displayId);
    int32_t UpdateTargetPointer(std::shared_ptr<PointerEvent> pointerEvent);
    bool TouchDownPointToDisplayPoint(struct libinput_event_touch* touch, Direction& direction,
        int32_t& logicalX, int32_t& logicalY, int32_t& logicalDisplayId);
    bool TouchMotionPointToDisplayPoint(struct libinput_event_touch* touch, Direction& direction,
        int32_t targetDisplayId, int32_t& displayX, int32_t& displayY);
    bool TransformDisplayPoint(struct libinput_event_touch* touch, Direction& direction, int32_t &globalLogicalX,
        int32_t &globalLogicalY);
    void RotateTouchScreen(PhysicalDisplayInfo* info, Direction direction,
        int32_t& logicalX, int32_t& logicalY);
    void AdjustCoordinate(double &coordinateX, double &coordinateY);

private:
    void SetFocusId(int32_t id);
    void PrintDebugInfo();
    int32_t UpdateMouseTargetOld(std::shared_ptr<PointerEvent> pointerEvent);
    int32_t UpdateTouchScreenTargetOld(std::shared_ptr<PointerEvent> pointerEvent);
    int32_t UpdateTouchPadTargetOld(std::shared_ptr<PointerEvent> pointerEvent);

    /*********************************新框架接口添加*************************** */
    bool IsInsideWindow(int32_t x, int32_t y, const WindowInfo &info) const;
    void PrintDisplayDebugInfo();
    int32_t UpdateMouseTarget(std::shared_ptr<PointerEvent> pointerEvent);
    int32_t UpdateTouchScreenTarget(std::shared_ptr<PointerEvent> pointerEvent);
    int32_t UpdateTouchPadTarget(std::shared_ptr<PointerEvent> pointerEvent);
    PhysicalDisplayInfo* GetPhysicalDisplay(int32_t id);
    PhysicalDisplayInfo* FindPhysicalDisplayInfo(const std::string seatId, const std::string seatName);
    int32_t GetDisplayId(std::shared_ptr<InputEvent> inputEvent);

private:
    std::mutex mu_;
    SeatInfo** seatsInfo_ = nullptr;
    ScreenInfo **screensInfo_ = nullptr;
    int32_t focusInfoID_ = 0;
    int32_t touchFocusId_ = 0;
    UDSServer* udsServer_ = nullptr;
    int32_t firstBtnDownWindowId_ = -1;
    /* *********************************新框架接口添加*************************** */
    std::vector<PhysicalDisplayInfo> physicalDisplays_ = {};
    std::vector<LogicalDisplayInfo> logicalDisplays_ = {};
    std::map<int32_t, WindowInfo> windowInfos_ = {};
    MouseLocation mouseLoction_ = {};
};

#define WinMgr InputWindowsManager::GetInstance()
} // namespace MMI
} // namespace OHOS
#endif // INPUT_WINDOWS_MANAGER_H
