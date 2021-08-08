

#pragma once


enum tonemapper :int {
	tnm_Linear = 0,
	tnm_Uncharted = 1,
	tnm_Aces = 2,
	tnm_Aces_Approx = 3,
	tnm_Cineon = 4,
	tnm_Reinhard_Jodie = 5,
	tnm_Reinhard_Extended = 6,
	tnm_PPC = 7,
	tnm_PPC_RGB= 8
};
enum LIGHT_PROFILE_VALUE{
	LPV_NULL,
	LPV_NAME,
	LPV_MAPPER
};
struct light_profile_table_line{
	int i;
	float f;
	bool b;
	SCP_string s;
};

//load tables into intermedite data structure that holds only the table values, not their final core  representations
//then compact them down into a 'final table' and THEN prase THAT
//intermediate table might be able to use a data wrapper that contains metadata
class light_profile{
public:
	SCP_string name;

    int tonemapper;
    float exposure;


    float static_light_factor ;
    float static_tube_factor ;
    float static_point_factor;

    float PPC_toeS;
    float PPC_toeL;
    float PPC_shoulderS;
    float PPC_shoulderL;
    float PPC_shoulderA;

    float glow_brightness_factor;
    float vfx_brightness_factor;

    float ambient_brightness_factor;
    float ambient_brigthness_floor;
    float sun_brightness_factor;
    float point_brightness_factor;
    float tube_brightness_factor;

    float missile_radius_factor;
	float laser_radius_factor;
    float other_point_radius_factor;
    float tube_radius_factor;

    void reset();
	static light_profile* find_by_name(SCP_string* name);
	static void add_default_default();
	static void load_profiles();
	static void read_tables(const char *filename);
	static void create_profiles();
	static int current_tonemapper();
};
