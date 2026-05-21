#pragma once

namespace KREngine::Math
{
	struct Vector3
	{
		union
		{
			struct { float x, y, z; };
			std::array<float, 3> v;// same as float[3]
		};

		constexpr Vector3() noexcept : Vector3(0.0f) {}
		explicit constexpr Vector3(float f) noexcept : Vector3(f, f, f) {}
		constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}

		// commonly used vectors
		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;
		const static Vector3 Forward;
		const static Vector3 Backward;
		const static Vector3 Left;
		const static Vector3 Right;

		constexpr Vector3 operator-() const { return { -x, -y, -z }; }
		constexpr Vector3 operator+(Vector3 b) const { return { x + b.x, y + b.y, z + b.z }; }
		constexpr Vector3 operator-(Vector3 b) const { return { x - b.x, y - b.y, z - b.z }; }
		constexpr Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }
		constexpr Vector3 operator/(float s) const { return { x / s, y / s, z / s }; }

		constexpr Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		constexpr Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		constexpr Vector3& operator*=(float s) { x *= s; y *= s, z *= s; return *this; }
		constexpr Vector3& operator/=(float s) { x /= s; y /= s, z /= s; return *this; }

		constexpr static Vector3 Cross(const Vector3& v1, const Vector3& v2)
		{
			return Vector3(
				(v1.y * v2.z) - (v1.z * v2.y),
				(v1.z * v2.x) - (v1.x * v2.z),
				(v1.x * v2.y) - (v1.y * v2.x)
			);
		}
		constexpr static Vector3 Normalize(const Vector3& v)
		{
			float lengthSquared = v.x * v.x + v.y * v.y + v.z * v.z;
			if (lengthSquared == 0.0f)
				return Vector3::Zero;
			float invLength = 1.0f / std::sqrt(lengthSquared);
			return v * invLength;
		}
	};
}