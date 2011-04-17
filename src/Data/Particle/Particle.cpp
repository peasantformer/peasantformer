#include "Particle.h"

Particle::Particle() {
	this->id = 0;
	
	this->ul = NULL;
	this->ur = NULL;
	this->dl = NULL;
	this->dr = NULL;
	
	this->position = Vector2f(0,0);
	this->speed = Vector2f(0,0);
	this->projected_position = Vector2f(0,0);
	
	this->width = 1;
	this->height = 1;
	this->inv_mass = 1;
	this->is_pinned = false;
}
Particle::Particle(Vector2f position
		,Vector2f speed
		,float width
		,float height
		,float inv_mass
		,bool is_pinned) {
	this->id = 0;
	
	this->ul = NULL;
	this->ur= NULL;
	this->dl = NULL;
	this->dr = NULL;
	
	this->position = position;
	this->speed = speed;
	this->projected_position = position;
	
	this->width = width;
	this->height = height;
	this->inv_mass = inv_mass;
	this->is_pinned = is_pinned;
}
void Particle::set_id(PeasantID value) {
	this->id = value;
}
void Particle::set_ul(Section * value) {
	this->ul = value;
}
void Particle::set_ur(Section * value) {
	this->ur = value;
}
void Particle::set_dl(Section * value) {
	this->dl = value;
}
void Particle::set_dr(Section * value) {
	this->dr = value;
}
void Particle::set_position(Vector2f value) {
	this->position = value;
}
void Particle::set_speed(Vector2f value) {
	this->speed = value;
}
void Particle::set_projected_position(Vector2f value) {
	this->projected_position = value;
}
void Particle::set_width(float value) {
	this->width = value;
}
void Particle::set_height(float value) {
	this->height = value;
}
void Particle::set_inv_mass(float value) {
	this->inv_mass = value;
}
void Particle::set_is_pinned(bool value) {
	this->is_pinned = value;
}



PeasantID Particle::get_id(void) {
	return this->id;
}
Section * Particle::get_ul(void) {
	return this->ul;
}
Section * Particle::get_ur(void) {
	return this->ur;
}
Section * Particle::get_dl(void) {
	return this->dl;
}
Section * Particle::get_dr(void) {
	return this->dr;
}
Vector2f Particle::get_position(void) {
	return this->position;
}
Vector2f Particle::get_speed(void) {
	return this->speed;
}
Vector2f Particle::get_projected_position(void) {
	return this->projected_position;
}
float Particle::get_width(void) {
	return this->width;
}
float Particle::get_height(void) {
	return this->height;
}
float Particle::get_inv_mass(void) {
	return this->inv_mass;
}
bool Particle::get_is_pinned(void) {
	return this->is_pinned;
}
