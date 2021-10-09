

#pragma once


#include "globalincs/vmallocator.h"
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
struct table_line{
	SCP_string entity_name;
	SCP_string value_name;
	SCP_string line_data;
	SCP_string blame;
	bool set = false;
};

struct om_table_line{
	SCP_string line_data;
	SCP_string blame;
	bool set = false;
};
//load tables into intermedite data structure that holds only the table values, not their final core  representations
//then compact them down into a 'final table' and THEN prase THAT
//intermediate table might be able to use a data wrapper that contains metadata
class light_profile{
public:
	static void newparse_init();
	static void newparse_all();
	static void newparse_file(const char *filename);
	static void newparse_profile(const char *blame);
	static void newparse_nextline(const char *blame, SCP_string *const profile_name);
	static void newparse_set_virtual_table_value(table_line *line, SCP_vector<table_line> *set);
	static void newparse_set_virtual_table_value(table_line *line, std::map<SCP_string, std::map<SCP_string,om_table_line>> *set);
	static bool newparse_is_set(SCP_string *const keyname, std::map<SCP_string,om_table_line> *vtable);
	static void build_real_tables();
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
