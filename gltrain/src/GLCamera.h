#pragma once

#include "Casic/CasicMath.h"
#include "Casic/CasicGraphics.h"
#include "Casic/CasicMatrix.h"
#include "Casic/CasicMatrixTransform.h"
#include "Casic/CasicQuaternion.h"

namespace Casic
{
namespace GL
{
    enum class CameraMode {
        CAMERA_CUSTOM = 0,       // 自定义摄像机模式，由用户完全控制，系统不会自动更新摄像机状态
        CAMERA_FREE,             // 自由摄像机模式，允许摄像机在3D空间中自由移动和旋转
        CAMERA_ORBITAL,          // 轨道摄像机模式，摄像机围绕一个目标点旋转，支持缩放
        CAMERA_FIRST_PERSON,     // 第一人称摄像机模式，模拟玩家第一人称视角，适用于沉浸式体验
        CAMERA_THIRD_PERSON,     // 第三人称摄像机模式，摄像机跟随目标并保持一定距离，可旋转和缩放视角

        CAMERA_MODE_COUNT        // 摄像机模式计数，用于统计或校验模式的总数量
    };

    enum class CameraProjection {
        CAMERA_PERSPECTIVE = 0,   // Perspective projection mode
        CAMERA_ORTHOGRAPHIC       // Orthographic projection mode
    };

    struct Camera3D {
        Math::Vector3 position;   // Camera position in 3D space
        Math::Vector3 target;     // Target point the camera is looking at
        Math::Vector3 up;         // Up direction vector (defines the "up" for the camera)
        float fovy;               // Field of view in the Y-axis, in degrees
        int projection;           // Projection type (see CameraProjection enum)
    };

    using Camera = typename Camera3D;
}
}

