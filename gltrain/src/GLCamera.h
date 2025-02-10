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

    typedef struct Camera3D {
        Math::Vector3 position;   // Camera position in 3D space
        Math::Vector3 target;     // Target point the camera is looking at
        Math::Vector3 up;         // Up direction vector (defines the "up" for the camera)
        float fovy;               // Field of view in the Y-axis, in degrees
        int projection;           // Projection type (see CameraProjection enum)
    }Camera3D;

    /*
        成员变量的意义：
        offset (屏幕坐标系中的偏移量)

            表示相机焦点（target）在屏幕上的显示位置。
            它定义了屏幕坐标系中的一个点，该点将显示为相机在世界中关注的目标点（target）。
            通常用于调整屏幕中心的位置。例如，将 offset 设为屏幕中心 (屏幕宽度 / 2, 屏幕高度 / 2)，则目标点 target 会显示在屏幕正中央。
        target (世界坐标系中的目标点)

            表示相机关注的焦点，定义了相机在世界空间中需要跟踪或显示的目标点。
            这个点会根据相机的其他属性（rotation、zoom 等）进行变换，并最终映射到屏幕上的 offset 位置。
        rotation (相机的旋转角度)

            定义了相机视图的旋转角度，单位是度（逆时针为正方向）。
            旋转的中心点是 target，整个视图会围绕 target 旋转。
            默认值为 0.0f，表示没有旋转。
        zoom (相机的缩放比例)

            定义了视图的缩放比例，表示相机的“远近”程度。
            默认值为 1.0f（无缩放），大于 1.0f 表示放大，小于 1.0f 表示缩小。
            缩放也是以 target 为中心进行的。
        工作机制：
            Camera2D 的工作原理是将世界坐标系中的点映射到屏幕坐标系中：

            以 target 为中心平移： 使相机关注的焦点成为世界变换的参考点。
            根据 rotation 旋转： 围绕 target 旋转整个视图。
            根据 zoom 缩放： 以 target 为基准放大或缩小视图。
            调整为 offset： 将结果映射到屏幕的偏移位置。
            这种机制可以用来实现动态的相机效果，例如跟踪目标、平滑缩放以及视图旋转。
    */
    typedef struct Camera2D {
        Math::Vector2 offset;     // Camera offset (displacement from target)
        Math::Vector2 target;     // Camera target (rotation and zoom origin)
        float         rotation = 0.0f;   // Camera rotation in degrees
        float         zoom = 1.0f;       // Camera zoom (scaling), should be 1.0f by default
    };

    using Camera = typename Camera3D;
}
}

