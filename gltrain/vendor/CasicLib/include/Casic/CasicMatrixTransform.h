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
	 *   near (float): The near plane of the orthographic projection (greater than `far`).
	 *   far (float): The far plane of the orthographic projection (smaller than `near`).
	 *
	 * Returns:
	 *   Matrix4: The resulting orthographic projection matrix.
	 *
	 * Note:
	 *   - The custom math library is used for clarity and simplicity.
	 *   - In this implementation, `near` is greater than `far`.
	 */
	CASICLIB_API Matrix4 Ortho(float left, float right, float bottom, float top, float near, float far);

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
