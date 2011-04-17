#include "World.h"


World::World() {
}
World::World(PeasantSize s_w, PeasantSize s_h) {
}


Section *World::add_section(Section sc) {
}
Particle *World::add_particle(Particle pt) {
}

std::list<Section *>::iterator World::get_sections_begin() {
}
std::list<Section *>::iterator World::get_sections_end() {
}
std::list<Particle *>::iterator World::get_particles_begin() {
}
std::list<Particle *>::iterator World::get_particles_end() {
}


bool World::move_particles(bool init) {
}

Section *World::gen_new_section(Section *current_section, Direction direction, bool gen) {
}


void World::set_u(Section * value) {
}
void World::set_d(Section * value) {
}
void World::set_l(Section * value) {
}
void World::set_r(Section * value) {
}

Section * World::get_u(void) {
}
Section * World::get_d(void) {
}
Section * World::get_l(void) {
}
Section * World::get_r(void) {
}
