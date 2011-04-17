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
		const Vector2f operator+=(Vector2f r);
		const Vector2f operator-=(Vector2f r);
		const Vector2f operator*=(Vector2f r);
	public:
		const float square_length();
		const float length();
		const Vector2f normalize();
		Vector2f abs_normalize();
};


Vector2f operator*(Vector2f l, float r);
Vector2f operator/(Vector2f l, float r);
Vector2f operator*(Vector2f l, Vector2f r);
Vector2f operator/(Vector2f l, Vector2f r);
Vector2f operator+(Vector2f l, Vector2f r);
Vector2f operator-(Vector2f l, Vector2f r);
bool operator!=(Vector2f l, Vector2f r);
bool operator==(Vector2f l, Vector2f r);

#endif
