#include <peasantformer/Vector2f.h>

Vector2f vector2f_lines_intersect(Vector2f p11, Vector2f p12, Vector2f p21, Vector2f p22, bool deep_check) {
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
