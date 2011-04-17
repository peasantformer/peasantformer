#ifndef PEASANTFORMER_Data_Particle
#define PEASANTFORMER_Data_Particle

#include <list>

#include <Data/Generic/GenericTypes.h>
#include <Data/Section/Section.h>
#include <Data/Vector2f/Vector2f.h>

class Section;

class Particle {
	private:
		PeasantID id;
		
		Section *ul;
		Section *ur;
		Section *dl;
		Section *dr;
		
		Vector2f position;
		Vector2f speed;
		Vector2f projected_position;
		
		float width, height;
		float inv_mass;
		bool is_pinned;
		
		std::list<Section *> sections;
	public:
		Particle();
		Particle(Vector2f position
		        ,Vector2f speed
		        ,float width
		        ,float height
		        ,float inv_mass
		        ,bool is_pinned);
	public:
		void set_id(PeasantID value);
		void set_ul(Section * value);
		void set_ur(Section * value);
		void set_dl(Section * value);
		void set_dr(Section * value);
		void set_position(Vector2f value);
		void set_speed(Vector2f value);
		void set_projected_position(Vector2f value);
		void set_width(float value);
		void set_height(float value);
		void set_inv_mass(float value);
		void set_is_pinned(bool value);
	public:
		PeasantID get_id(void);
		Section * get_ul(void);
		Section * get_ur(void);
		Section * get_dl(void);
		Section * get_dr(void);
		Vector2f get_position(void);
		Vector2f get_speed(void);
		Vector2f get_projected_position(void);
		float get_width(void);
		float get_height(void);
		float get_inv_mass(void);
		bool get_is_pinned(void);
};


#endif
