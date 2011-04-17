#ifndef PEASANTFORMER_Data_Vector2f
#define PEASANTFORMER_Data_Vector2f

#include <cmath>

class Vector2f {
	public:
		float x,y;
	public:
		Vector2f();
		Vector2f(float x, float y);
	public:
		inline const Vector2f operator+=(Vector2f r);
		inline const Vector2f operator-=(Vector2f r);
		inline const Vector2f operator*=(Vector2f r);
	public:
		inline const float square_length();
		inline const float length();
		inline const Vector2f normalize();
		inline Vector2f abs_normalize();
};


inline Vector2f operator*(Vector2f l, float r);
inline Vector2f operator/(Vector2f l, float r);
inline Vector2f operator*(Vector2f l, Vector2f r);
inline Vector2f operator/(Vector2f l, Vector2f r);
inline Vector2f operator+(Vector2f l, Vector2f r);
inline Vector2f operator-(Vector2f l, Vector2f r);
inline bool operator!=(Vector2f l, Vector2f r);
inline bool operator==(Vector2f l, Vector2f r);
#endif
