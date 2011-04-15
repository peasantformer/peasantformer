#include <Peasantformer/Data/Vector2f.h>



Vector2f::Vector2f() {
	this->x = 0.0f;
	this->y = 0.0f;
}
Vector2f::Vector2f(float x, float y) {
	this->x = x;
	this->y = y;
}
inline const Vector2f Vector2f::operator+=(Vector2f r) {
	this->x += r.x;
	this->y += r.y;
	return (*this);

}
inline const Vector2f Vector2f::operator-=(Vector2f r) {
	this->x -= r.x;
	this->y -= r.y;
	return (*this);

}
inline const Vector2f Vector2f::operator*=(Vector2f r) {
	this->x *= r.x;
	this->y *= r.y;
	return (*this);

}
inline const float Vector2f::square_length() {
	return (x*x + y*y);
}
inline const float Vector2f::length() {
	return sqrt(this->square_length());
}
inline const Vector2f Vector2f::normalize() {
	float len = this->length();
	if (len == 0)
		return Vector2f(0,0);
	float inv_length = 1.0f / len;
	return ((*this)	 * inv_length);	
}
inline Vector2f Vector2f::abs_normalize() {
	float len = this->length();
	if (len == 0)
		return Vector2f(0,0);
	float inv_length = 1.0f / len;
	Vector2f norm = ((*this) * inv_length);
	float bound = 0.7;
//			printf("I: %f %f\n",norm.x,norm.y);
	if (norm.y > 0) {
		if (norm.y > bound) norm.y = 1;
		if (norm.y < bound) norm.y = 0;

	} else {
		if (norm.y < -bound) norm.y = -1;
		if (norm.y > -bound) norm.y = 0;
	}
	if (norm.x > 0) {
		if (norm.x > bound) norm.x = 1;
		if (norm.x < bound) norm.x = 0;
	} else {
		if (norm.x < -bound) norm.x = -1;
		if (norm.x > -bound) norm.x = 0;
	}
	return norm;
}

inline Vector2f operator*(Vector2f l, float r) {
	return Vector2f(l.x*r, l.y*r);
}

inline Vector2f operator/(Vector2f l, float r) {
	return Vector2f(l.x/r, l.y/r);
}

inline Vector2f operator*(Vector2f l, Vector2f r) {
	return Vector2f(l.x*r.x, l.y*r.y);
}

inline Vector2f operator/(Vector2f l, Vector2f r) {
	return Vector2f(l.x/r.x, l.y/r.y);
}

inline Vector2f operator+(Vector2f l, Vector2f r) {
	return Vector2f(l.x+r.x, l.y+r.y);
}

inline Vector2f operator-(Vector2f l, Vector2f r) {
	return Vector2f(l.x-r.x, l.y-r.y);
}

inline bool operator!=(Vector2f l, Vector2f r) {
	return ((l.x != r.x) || (l.y != r.y));
}

inline bool operator==(Vector2f l, Vector2f r) {
	return ((l.x == r.x) && (l.y == r.y));
}

Vector2f lines_intersect(Vector2f p11, Vector2f p12, Vector2f p21, Vector2f p22, bool deep_check = false) {
	float Z  = (p12.y-p11.y)*(p21.x-p22.x)-(p21.y-p22.y)*(p12.x-p11.x);
	float Ca = (p12.y-p11.y)*(p21.x-p11.x)-(p21.y-p11.y)*(p12.x-p11.x);
	float Cb = (p21.y-p11.y)*(p21.x-p22.x)-(p21.y-p22.y)*(p21.x-p11.x);

	if ((Z ==0) && (Ca == 0) && (Cb == 0)) {
		return Vector2f(-1,-1);
	}
	if (Z == 0) {
		return Vector2f(-1,-1);
	}

	float Ua = Ca/Z;
	float Ub = Cb/Z;
	if ((0 <= Ua) && (Ua <= 1) && (0 <= Ub) && (Ub <= 1)) {
		return Vector2f(p11.x + (p12.x - p11.x) * Ub, p11.y + (p12.y - p11.y) * Ub);
	} else {
		if (deep_check) {
			return Vector2f(p11.x + (p12.x - p11.x) * Ub, p11.y + (p12.y - p11.y) * Ub);
		}

	}
	return Vector2f(-1,-1);
}
