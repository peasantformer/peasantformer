#ifndef PEASANTFORMER_OBJECT_UTILITY_VECTOR2F
#define PEASANTFORMER_OBJECT_UTILITY_VECTOR2F


class Vector2f operator*(Vector2f, float);

class Vector2f {
	public:
		float x,y;
	public:
		Vector2f() {
			this->x = 0.0f;
			this->y = 0.0f;
		}
		Vector2f(float x, float y) {
			this->x = x;
			this->y = y;
		}
	public:
		inline const Vector2f operator+=(Vector2f r) {
			this->x += r.x;
			this->y += r.y;
			return (*this);

		}
		inline const Vector2f operator-=(Vector2f r) {
			this->x -= r.x;
			this->y -= r.y;
			return (*this);

		}
		inline const Vector2f operator*=(Vector2f r) {
			this->x *= r.x;
			this->y *= r.y;
			return (*this);

		}
	public:
		inline const float square_length() {
			return (x*x + y*y);
		}
		inline const float length() {
			return sqrt(this->square_length());
		}
		inline const Vector2f normalize() {
			float len = this->length();
			if (len == 0)
				return Vector2f(0,0);
			float inv_length = 1.0f / len;
			return ((*this)	 * inv_length);	
		}
		inline Vector2f abs_normalize() {
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
};

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

Vector2f vector2f_lines_intersect();
#endif
