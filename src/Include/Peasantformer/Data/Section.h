#ifndef PEASANTFORMER_Data_Section
#define PEASANTFORMER_Data_Section

class Section {
	private:
		PeasantID id;

		PeasantPos x;
		PeasantPos y;
		PeasantSize w;
		PeasantSize h;

		Section *u;
		Section *d;
		Section *l;
		Section *r;

		std::list<Particle *> particles;
	public:
		void add_particle_id(Particle *pt) {
			this->particles.push_back(pt);
		}
		void del_particle_id(size_t id) {
			for (std::list<Particle *>::iterator it=this->get_particle_ids_begin(); it != this->get_particle_ids_end(); it++) {
				if ((size_t)(*it)->get_id() == id) {
					this->particles.erase(it);
					break;
				}
			}
		}

		void clear_particle_ids() {
			this->particles.clear();
		}
		std::list<Particle *>::iterator get_particle_ids_begin() {
			return this->particles.begin();
		}
		std::list<Particle *>::iterator get_particle_ids_end() {
			return this->particles.end();
		}

	public:
		Section() {
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
		Section(PeasantPos x
		       ,PeasantPos y
		       ,PeasantSize w = 0
		       ,PeasantSize h = 0) {
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

	public:
		void set_id(PeasantID value) {
			this->id = value;
		}
		void set_x(PeasantPos value) {
			this->x = value;
		}
		void set_y(PeasantPos value) {
			this->y = value;
		}
		void set_w(PeasantSize value) {
			this->w = value;
		}
		void set_h(PeasantSize value) {
			this->h = value;
		}
		void set_u(Section * value) {
			this->u = value;
		}
		void set_d(Section * value) {
			this->d = value;
		}
		void set_l(Section * value) {
			this->l = value;
		}
		void set_r(Section * value) {
			this->r = value;
		}

	public:
		PeasantID get_id(void) {
			return this->id;
		}
		PeasantPos get_x(void) {
			return this->x;
		}
		PeasantPos get_y(void) {
			return this->y;
		}
		PeasantSize get_w(void) {
			return this->w;
		}
		PeasantSize get_h(void) {
			return this->h;
		}
		Section * get_u(void) {
			return this->u;
		}
		Section * get_d(void) {
			return this->d;
		}
		Section * get_l(void) {
			return this->l;
		}
		Section * get_r(void) {
			return this->r;
		}

};

#endif
