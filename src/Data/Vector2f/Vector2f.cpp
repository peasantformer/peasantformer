#include "Vector2f.h"
#include <stdio.h>



Vector2f::Vector2f() {
	this->x = 0.0f;
	this->y = 0.0f;
	Vector2f *next;
}
Vector2f::Vector2f(float x, float y) {
	this->x = x;
	this->y = y;
}
const Vector2f Vector2f::operator+=(Vector2f r) {
	this->x += r.x;
	this->y += r.y;
	return (*this);

}
const Vector2f Vector2f::operator-=(Vector2f r) {
	this->x -= r.x;
	this->y -= r.y;
	return (*this);

}
const Vector2f Vector2f::operator*=(Vector2f r) {
	this->x *= r.x;
	this->y *= r.y;
	return (*this);

}
const float Vector2f::square_length() {
	return (x*x + y*y);
}
const float Vector2f::length() {
	return sqrt(this->square_length());
}
const Vector2f Vector2f::normalize() {
	float len = this->length();
	if (len == 0)
		return Vector2f(0,0);
	
	float inv_length = 1.0f / len;
	return ((*this)	 * inv_length);	
}


const Vector2f Vector2f::normalize_ext() {
	float len = this->length();
	Vector2f out = *this;

	if (fabs(len) < 0.0001) {
		out = out / 1;
	}
	return out;
}

Vector2f Vector2f::abs_normalize() {
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

PointClasses Vector2f::classify (Vector2f p0, Vector2f p1) {
	Vector2f p2 = *this;
	Vector2f a = p1 - p0;
	Vector2f b = p2 - p0;
	float sa = a.x * b.y - b.x * a.y;
	if (sa > 0.0)
		return V_LEFT;
	if (sa < 0.0)
		return V_RIGHT;
	if ((a.x * b.x < 0.0) || (a.y * b.y < 0.0))
		return V_BEHIND;
	if (a.length() < b.length())
		return V_BEYOND;
	if (p0 == p2)
		return V_ORIGIN;
	if (p1 == p2)
		return V_DESTINATION;
	return V_BETWEEN;
}

Vector2f operator*(Vector2f l, float r) {
	return Vector2f(l.x*r, l.y*r);
}

Vector2f operator/(Vector2f l, float r) {
	return Vector2f(l.x/r, l.y/r);
}

Vector2f operator*(Vector2f l, Vector2f r) {
	return Vector2f(l.x*r.x, l.y*r.y);
}

Vector2f operator/(Vector2f l, Vector2f r) {
	return Vector2f(l.x/r.x, l.y/r.y);
}

Vector2f operator+(Vector2f l, Vector2f r) {
	return Vector2f(l.x+r.x, l.y+r.y);
}

Vector2f operator-(Vector2f l, Vector2f r) {
	return Vector2f(l.x-r.x, l.y-r.y);
}

bool operator!=(Vector2f l, Vector2f r) {
	return ((l.x != r.x) || (l.y != r.y));
}

bool operator==(Vector2f l, Vector2f r) {
	return ((l.x == r.x) && (l.y == r.y));
}
/*
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
*/

float orient(const Vector2f *a, const Vector2f *b, const Vector2f *c) {
	return (a->x - c->x) * (b->y - c->y) - (a->y - c->y) * (b->x - c->x);
}

Vector2f rotate(Vector2f l, float sine, float cosine) {
	return Vector2f(l.x * cosine - l.y * sine, l.x * sine + l.y * cosine);
}

Vector2f rotate(Vector2f tgt,float tenshi) {
	return rotate(tgt,sin(tenshi * (PI/180)),cos(tenshi * (PI/180)));
}

Vector2f angelVector(float tenshi) {
	return Vector2f(cos(tenshi),sin(tenshi));
}

float angelOfVector(Vector2f V) {
	float a = acos(V.x);
	if (V.y < 0) a = PI + PI - a;
	return a;
}

float distance(Vector2f *a, Vector2f *b, Vector2f *c) {
	float dx = a->x - b->x;
	float dy = a->y - b->y;
	float D = dx * (c->y - a->y) - dy * (c->x - a->x);
	return fabs(D / sqrt(dx*dx + dy*dy));
}

float circleIntersects(Vector2f C, float radius, Vector2f A, Vector2f B, Vector2f *R1, Vector2f *R2) {
	A -= C;
	B -= C;

	float Dx = B.x - A.x;
	float Dy = B.y - A.y;
	float Dr = sqrt(Dx*Dx + Dy*Dy);
		
	float D = A.x*B.y - B.x*B.y;
		
	float isinter = radius*radius * Dr*Dr - D*D;
		
	R1->x = (D * Dy + ((Dy < 0) ? -1 : 1) * Dx * sqrt(radius*radius * Dr*Dr - D*D)) / (Dr*Dr);
	R2->x = (D * Dy - ((Dy < 0) ? -1 : 1) * Dx * sqrt(radius*radius * Dr*Dr - D*D)) / (Dr*Dr);
		
	R1->y = (-D * Dx + fabs(Dy) * sqrt(radius*radius * Dr*Dr - D*D)) / (Dr*Dr);
	R2->y = (-D * Dx - fabs(Dy) * sqrt(radius*radius * Dr*Dr - D*D)) / (Dr*Dr);
	
	*R1 += C;
	*R2 += C;
	return isinter;
}

float D2(float a11, float a12, float a21, float a22) {
	return a11 * a22 - a12 * a21;
}

bool lines_intersect(Vector2f A, Vector2f B, Vector2f C, Vector2f D, Vector2f * P) {
	
	float x1 = A.x;
	float x2 = B.x;
	float x3 = C.x;
	float x4 = D.x;
	
	float y1 = A.y;
	float y2 = B.y;
	float y3 = C.y;
	float y4 = D.y;

	
	P->x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
	P->y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

	bool xpred = ((MIN(A.x,B.x) <= P->x + 0.0001) && (MAX(A.x,B.x) >= P->x -0.0001));
	bool ypred = ((MIN(A.y,B.y) <= P->y + 0.0001) && (MAX(A.y,B.y) >= P->y -0.0001));
	
	bool wpred = ((MIN(C.x,D.x) <= P->x + 0.0001) && (MAX(C.x,D.x) >= P->x -0.0001));
	bool zpred = ((MIN(C.y,D.y) <= P->y + 0.0001) && (MAX(C.y,D.y) >= P->y -0.0001));

	return (xpred && ypred && wpred && zpred);
	PointClasses c1 = P->classify(C,D);
	PointClasses c2 = P->classify(A,B);
	return (
		   (c1 == V_BETWEEN || c1 == V_ORIGIN || c1 == V_DESTINATION)
		&& (c2 == V_BETWEEN || c2 == V_ORIGIN || c2 == V_DESTINATION)
	);
	return false;
	
}


