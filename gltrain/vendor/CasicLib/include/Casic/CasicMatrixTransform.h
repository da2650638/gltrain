#pragma once

#include "CasicMath.h"

namespace Casic
{
namespace Math 
{
	/*
	 * Creates an orthographic projection matrix looking towards the -Z axis.
	 *
	 * Args:
	 *   left (float): The left plane of the orthographic projection.
	 *   right (float): The right plane of the orthographic projection.
	 *   bottom (float): The bottom plane of the orthographic projection.
	 *   top (float): The top plane of the orthographic projection.
	 *   znear (float): The near clipping plane (a positive value, closer to the camera).
	 *   zfar (float): The far clipping plane (a positive value, farther from the camera).
	 *
	 * Returns:
	 *   Matrix4: The resulting orthographic projection matrix.
	 */
	CASICLIB_API Matrix4 Ortho(float left, float right, float bottom, float top, float znear, float zfar);

	/*
	 * Creates a perspective projection matrix looking towards the -Z axis.
	 *
	 * Args:
	 *   left (float): The left plane of the perspective frustum.
	 *   right (float): The right plane of the perspective frustum.
	 *   bottom (float): The bottom plane of the perspective frustum.
	 *   top (float): The top plane of the perspective frustum.
	 *   znear (float): The near clipping plane (a positive value, closer to the camera).
	 *   zfar (float): The far clipping plane (a positive value, farther from the camera).
	 *
	 * Returns:
	 *   Matrix4: The resulting perspective projection matrix.
	 *
	 * Note:
	 *   - This function assumes a right-handed coordinate system.
	 *   - The `znear` and `zfar` planes are positive values representing the distance from the camera.
	 *   - The `znear` plane is numerically smaller (closer to the camera) than the `zfar` plane.
	 *   - The resulting matrix maps the perspective frustum to normalized device coordinates (NDC).
	 *   - After the perspective division, values in NDC are in the range [-1, 1] for X, Y, and Z.
	 */
	CASICLIB_API Matrix4 Perspective(float left, float right, float bottom, float top, float znear, float zfar);

	/*
	 * Creates a perspective projection matrix using a vertical field of view (fovy) and aspect ratio.
	 *
	 * Args:
	 *   fovy (float): The vertical field of view in degrees.
	 *   aspect (float): The aspect ratio of the frustum, defined as width / height.
	 *   znear (float): The near clipping plane (a positive value, closer to the camera).
	 *   zfar (float): The far clipping plane (a positive value, farther from the camera).
	 *
	 * Returns:
	 *   Matrix4: The resulting perspective projection matrix.
	 *
	 * Note:
	 *   - The `fovy` defines the vertical field of view angle (in degrees), which determines how "wide" the frustum is.
	 *   - The `aspect` determines the horizontal field of view by scaling the width of the frustum relative to its height.
	 *   - The `znear` and `zfar` planes are positive values representing the distance from the camera.
	 *   - This function internally computes the frustum parameters (left, right, bottom, top) and calls the frustum-based Perspective function.
	 *   - The resulting matrix maps the perspective frustum to normalized device coordinates (NDC), where X, Y, and Z range from [-1, 1].
	 *   - After the perspective division, the visible geometry is constrained to the frustum volume.
	 */
	CASICLIB_API Matrix4 Perspective(float fovy, float aspect, float znear, float zfar);


	/*
	 * Creates a view matrix for a camera looking at a specified target.
	 *
	 * Args:
	 *   eye (Vector3): The position of the camera.
	 *   target (Vector3): The point the camera is looking at.
	 *   up (Vector3): The up direction for the camera.
	 *
	 * Returns:
	 *   Matrix4: The resulting view matrix.
	 *
	 * Note:
	 *   - This function constructs an orthonormal basis using the input parameters.
	 *   - The resulting matrix transforms world coordinates into the camera's coordinate system.
	 */
	CASICLIB_API Matrix4 LookAt(Vector3 eye, Vector3 target, Vector3 up);

	/*
	 * Applies a translation to an existing matrix.
	 *
	 * Args:
	 *   mat (Matrix4): The input matrix to which the translation is applied.
	 *   vec (Vector3): The translation vector specifying movement along the x, y, and z axes.
	 *
	 * Returns:
	 *   Matrix4: The resulting matrix after applying the translation.
	 *
	 * Note:
	 *   - The operation performs `mat * T`, where `T` is the translation matrix.
	 *   - The multiplication order assumes column-major memory layout.
	 */
	CASICLIB_API Matrix4 Translate(Matrix4 mat, Vector3 vec);

	/*
	 * Creates a translation matrix based on the given translation vector.
	 *
	 * Args:
	 *   vec (Vector3): The translation vector specifying movement along the x, y, and z axes.
	 *
	 * Returns:
	 *   Matrix4: The resulting translation matrix.
	 *
	 * Note:
	 *   - The resulting matrix applies only a translation and leaves other transformations as identity.
	 */
	CASICLIB_API Matrix4 Translate(Vector3 vec);

	/*
	 * Applies a scaling transformation to an existing matrix.
	 *
	 * Args:
	 *   mat (Matrix4): The input matrix to which the scaling is applied.
	 *   scale (Vector3): The scaling vector specifying scale factors along the x, y, and z axes.
	 *
	 * Returns:
	 *   Matrix4: The resulting matrix after applying the scaling.
	 *
	 * Note:
	 *   - The operation performs `mat * S`, where `S` is the scaling matrix.
	 *   - The multiplication order assumes column-major memory layout.
	 */
	CASICLIB_API Matrix4 Scale(Matrix4 mat, Vector3 scale);

	/*
	 * Creates a scaling matrix based on the given scaling vector.
	 *
	 * Args:
	 *   scale (Vector3): The scaling vector specifying scale factors along the x, y, and z axes.
	 *
	 * Returns:
	 *   Matrix4: The resulting scaling matrix.
	 *
	 * Note:
	 *   - The resulting matrix applies only a scaling transformation and leaves other transformations as identity.
	 */
	CASICLIB_API Matrix4 Scale(Vector3 scale);

	/*
	 * Applies a rotation transformation to an existing matrix around an arbitrary axis.
	 *
	 * Args:
	 *   mat (Matrix4): The input matrix to which the rotation is applied.
	 *   angle (float): The angle of rotation in degrees.
	 *   axis (Vector3): The axis around which the rotation occurs (must be normalized).
	 *
	 * Returns:
	 *   Matrix4: The resulting matrix after applying the rotation.
	 *
	 * Note:
	 *   - The operation performs `mat * R`, where `R` is the rotation matrix.
	 *   - The multiplication order assumes column-major memory layout.
	 */
	CASICLIB_API Matrix4 Rotate(Matrix4 mat, float angle, Vector3 axis);

	/*
	 * Creates a rotation matrix for an arbitrary axis.
	 *
	 * Args:
	 *   angle (float): The angle of rotation in degrees.
	 *   axis (Vector3): The axis around which the rotation occurs (must be normalized).
	 *
	 * Returns:
	 *   Matrix4: The resulting rotation matrix.
	 *
	 * Note:
	 *   - The resulting matrix applies only a rotation transformation and leaves other transformations as identity.
	 */
	CASICLIB_API Matrix4 Rotate(float angle, Vector3 axis);
}
}
