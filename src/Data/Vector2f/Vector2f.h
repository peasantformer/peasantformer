#ifndef PEASANTFORMER_Data_Vector2f
#define PEASANTFORMER_Data_Vector2f

#include <cmath>
#define PI 3.14159265
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

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
		const Vector2f normalize_ext();
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


float circleIntersects(Vector2f C, float radius, Vector2f A, Vector2f B, Vector2f *R1, Vector2f *R2);
bool lines_intersect(Vector2f A, Vector2f B, Vector2f C, Vector2f D, Vector2f * P);

Vector2f angelVector(float tenshi);
float angelOfVector(Vector2f V);
#endif
