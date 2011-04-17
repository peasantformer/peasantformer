#include "Section.h"

Section::Section() {
	this->id = 0;
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;

	this->u = NULL;
	this->d = NULL;
	this->l = NULL;
	this->r = NULL;

}
Section::Section(PeasantPos x
	            ,PeasantPos y
	            ,PeasantSize w
	            ,PeasantSize h) {
	this->id = 0;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	this->u = NULL;
	this->d = NULL;
	this->l = NULL;
	this->r = NULL;
}





void Section::set_id(PeasantID value) {
	this->id = value;
}
void Section::set_x(PeasantPos value) {
	this->x = value;
}
void Section::set_y(PeasantPos value) {
	this->y = value;
}
void Section::set_w(PeasantSize value) {
	this->w = value;
}
void Section::set_h(PeasantSize value) {
	this->h = value;
}
void Section::set_u(Section * value) {
	this->u = value;
}
void Section::set_d(Section * value) {
	this->d = value;
}
void Section::set_l(Section * value) {
	this->l = value;
}
void Section::set_r(Section * value) {
	this->r = value;
}





PeasantID Section::get_id(void) {
	return this->id;
}
PeasantPos Section::get_x(void) {
	return this->x;
}
PeasantPos Section::get_y(void) {
	return this->y;
}
PeasantSize Section::get_w(void) {
	return this->w;
}
PeasantSize Section::get_h(void) {
	return this->h;
}
Section * Section::get_u(void) {
	return this->u;
}
Section * Section::get_d(void) {
	return this->d;
}
Section * Section::get_l(void) {
	return this->l;
}
Section * Section::get_r(void) {
	return this->r;
}
