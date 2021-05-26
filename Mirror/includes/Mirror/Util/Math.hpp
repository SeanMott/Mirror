//defines math stuff for the engine

#ifndef Math_h
#define Math_h

namespace Mirror::Util
{
	//allows type punning || returns a pointer
#define Mirror_Math_TypePunP(data, type) ((type*)&data)
		//allows type punning || returns a def refed pointer
#define Mirror_Math_TypePun(data, type) *((type*)&data)

	//vectors
	template<typename T>
	struct Vector2
	{
		T x;
		T y;

		//Constructor
		Vector2<T>() {}

		//Constructor
		Vector2<T>(T _x, T _y)
			:x(_x), y(_y) {}

		//gets the magnitude (length)
		inline float Magnitude()
		{
			return ((x * x) + (y * y)) * 0.5f;
		}

		//normalizes the vector
		inline Vector2<T> Normalize()
		{
			const float r = 1 / Magnitude();
			return { x * r, y * r };
		}

		//gets the perpendicular of the Vector
		inline Vector2<T> Perpendicular()
		{
			return { -y, x };
		}

		//gets the dot product
		inline float Dot(const Vector2<T>& vector)
		{
			return x * vector.x + y * vector.y;
		}

		//returns a zero vector
		static inline Vector2<T> Zero()
		{
			return Vector2<T>(0.0f, 0.0f);
		}

		//returns a one vector
		static inline Vector2<T> One()
		{
			return Vector2<T>(1.0f, 1.0f);
		}

		//returns a up vector
		static inline Vector2<T> Up()
		{
			return Vector2<T>(0.0f, 1.0f);
		}

		//returns a up down vector
		static inline Vector2<T> Down()
		{
			return Vector2<T>(0.0f, -1.0f);
		}

		//returns a up left vector
		static inline Vector2<T> Left()
		{
			return Vector2<T>(-1.0f, 0.0f);
		}

		//returns a up right vector
		static inline Vector2<T> Right()
		{
			return Vector2<T>(1.0f, 0.0f);
		}

		//add Vec2
		Vector2<T> operator+(const Vector2<T>& vector)
		{
			return { x + vector.x, y + vector.y };
		}
		//sub Vec2
		Vector2<T> operator-(const Vector2<T>& vector)
		{
			return { x - vector.x, y - vector.y };
		}
		//div Vec2
		Vector2<T> operator/(const float& vector)
		{
			return { x / vector, y / vector };
		}
		//mull Vec2
		Vector2<T> operator*(const float& vector)
		{
			return { x * vector, y * vector };
		}
		//div Vec2
		Vector2<T> operator/(const Vector2<T>& vector)
		{
			return { x / vector.x, y / vector.y };
		}
		//mull Vec2
		Vector2<T> operator*(const Vector2<T>& vector)
		{
			return { x * vector.x, y * vector.y };
		}

		//add Vec2
		Vector2<T>& operator+=(const Vector2<T>& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}
		//sub Vec2
		Vector2<T>& operator-=(const Vector2<T>& vector)
		{
			x -= vector.x;
			y -= vector.y;
			return *this;
		}
		//div Vec2
		Vector2<T>& operator/=(const float& vector)
		{
			x /= vector;
			y /= vector;
			return *this;
		}
		//mull Vec2
		Vector2<T>& operator*=(const float& vector)
		{
			x *= vector;
			y *= vector;
			return *this;
		}
		//div Vec2
		Vector2<T>& operator/=(const Vector2<T>& vector)
		{
			x /= vector.x;
			y /= vector.y;
			return *this;
		}
		//mull Vec2
		Vector2<T>& operator*=(const Vector2<T>& vector)
		{
			x *= vector.x;
			y *= vector.y;
			return *this;
		}
	};

	template<typename T>
	struct Vector3
	{
		T x;
		T y;
		T z;

		//Constructor
		Vector3<T>() {}

		//Constructor
		Vector3<T>(T _x, T _y, T _z)
			: x(_x), y(_y), z(_z) {}

		//gets the magnitude (length)
		inline float Magnitude()
		{
			return ((x * x) + (y * y) + (z * z)) * 0.5f;
		}

		//normalizes the vector
		inline Vector3<T> Normalize()
		{
			const float r = 1 / Magnitude();
			return { x * r, y * r, z * r };
		}

		//gets the perpendicular of the Vector
		inline Vector3<T> Perpendicular()
		{
			return { -z, y, x };
		}

		//gets the dot product
		inline float Dot(const Vector3<T>& vector)
		{
			return x * vector.x + y * vector.y + z * vector.z;
		}

		//turns it into a Vector 2
		inline Vector2<T> ToVec2() { return { x, y }; }

		//returns a zero vector
		static inline Vector3<T> Zero()
		{
			return Vector3<T>(0.0f, 0.0f, 0.0f);
		}

		//returns a one vector
		static inline Vector3<T> One()
		{
			return Vector3<T>(1.0f, 1.0f, 1.0f);
		}

		//returns a up vector
		static inline Vector3<T> Up()
		{
			return Vector3<T>(0.0f, 1.0f, 0.0f);
		}

		//returns a up down vector
		static inline Vector3<T> Down()
		{
			return Vector3<T>(0.0f, -1.0f, 0.0f);
		}

		//returns a up left vector
		static inline Vector3<T> Left()
		{
			return Vector3<T>(-1.0f, 0.0f, 0.0f);
		}

		//returns a up right vector
		static inline Vector3<T> Right()
		{
			return Vector3<T>(1.0f, 0.0f, 0.0f);
		}

		//add Vec3
		Vector3<T> operator+(const Vector3<T>& vector)
		{
			return { x + vector.x, y + vector.y, z + vector.z };
		}
		//sub Vec3
		Vector3<T> operator-(const Vector3<T>& vector)
		{
			return { x - vector.x, y - vector.y, z - vector.z };
		}
		//div Vec3
		Vector3<T> operator/(const float& vector)
		{
			return { x / vector, y / vector, z / vector };
		}
		//mull Vec3
		Vector3<T> operator*(const float& vector)
		{
			return { x * vector, y * vector, z * vector };
		}
		//div Vec3
		Vector3<T> operator/(const Vector3<T>& vector)
		{
			return { x / vector.x, y / vector.y, z / vector.z };
		}
		//mull Vec3
		Vector3<T> operator*(const Vector3<T>& vector)
		{
			return { x * vector.x, y * vector.y, z * vector.z };
		}

		//add Vec3
		Vector3<T>& operator+=(const Vector3<T>& vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			return *this;
		}
		//sub Vec3
		Vector3<T>& operator-=(const Vector3<T>& vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			return *this;
		}
		//div Vec3
		Vector3<T>& operator/=(const float& vector)
		{
			x /= vector;
			y /= vector;
			z /= vector;
			return *this;
		}
		//mull Vec3
		Vector3<T>& operator*=(const float& vector)
		{
			x *= vector;
			y *= vector;
			z *= vector;
			return *this;
		}
		//div Vec3
		Vector3<T>& operator/=(const Vector3<T>& vector)
		{
			x /= vector.x;
			y /= vector.y;
			z /= vector.z;
			return *this;
		}
		//mull Vec3
		Vector3<T>& operator*=(const Vector3<T>& vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
			return *this;
		}
	};

	template<typename T>
	struct Vector4
	{
		T x;
		T y;
		T z;
		T w;

		//Constructor
		Vector4<T>() {}

		//Constructor
		Vector4<T>(T _x, T _y, T _z, T _w)
			:x(_x), y(_y), z(_z), w(_w) {}

		//turns it into a Vector 3
		inline Vector3<T> ToVec3() { return { x, y, z }; }
		//turns it into a Vector 2
		inline Vector2<T> ToVec2() { return { x, y}; }

		//returns a zero vector
		static inline Vector4<T> Zero()
		{
			return { 0.0f, 0.0f, 0.0f, 0.0f };
		}

		//returns a one vector
		static inline Vector4<T> One()
		{
			return { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	};
}

namespace Mirror::Util::Physics
{
	template<typename P, typename R>
	inline bool PointVRect(const Vector2<P>& pointPos, const Vector2<R>& rectPos, const Vector2<R>& rectSize)
	{
		return (pointPos.x >= rectPos.x && pointPos.y >= rectPos.y && pointPos.x < rectPos.x + rectSize.x && pointPos.y < rectPos.y + rectSize.y);
	}

	template<typename R1, typename R2>
	inline bool RectVRect(const Vector2<R1>& rect1Pos, const Vector2<R1>& rect1Size,
		const Vector2<R2>& rect2Pos, const Vector2<R2>& rect2Size)
	{
		return (rect1Pos.x < rect2Pos.x + rect2Size.x && rect1Pos.x + rect1Size.x > rect2Pos.x && rect1Pos.y < rect2Pos.y + rect2Size.y && rect1Pos.y + rect1Size.y > rect2Pos.y);
	}
}

#endif