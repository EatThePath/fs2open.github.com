#include "ai/ai_profiles.h"
#include "def_files/def_files.h"
#include "globalincs/globals.h"
#include "globalincs/pstypes.h"
#include "globalincs/vmallocator.h"
#include "lighting/lighting_profiles.h"
#include "parse/parselo.h"
//A copy of the active light profile is made and used as our working data, so that we can make changes with SEXP and such
//and then revert easily.

SCP_vector<light_profile> light_profiles;

SCP_vector<SCP_unordered_map<LIGHT_PROFILE_VALUE,light_profile_table_line>> raw_light_profiles;

light_profile current_light_profile;

int default_light_profile_index = 0;
/*
;So what's a lighting profile look like?
;IMO there's no need for ordering save for names being the delimiter.
;There's no need for a prefix character either, newlines to : is the identifier -
;		Actually I always assumed newlines were singificant to tables and that's not true!
;		So I need to either make it true for this case or use a distinct marker character.
*/
light_profile* light_profile::find_by_name(SCP_string *name){
	for(auto& lp : light_profiles)
	{
		if(lp.name == *name){
			return &lp;
		}
	}
	return nullptr;
}

SCP_string default_light_profile_name = "default";

void activate_profile(light_profile* base){
	current_light_profile.name = base->name;
	current_light_profile.tonemapper = base->tonemapper;
}
void activate_default_profile(){
	assert(!light_profiles.empty());
	light_profile *base = &light_profiles[0];
	for(auto & lp : light_profiles){
		if(lp.name == default_light_profile_name){
			base = &lp;
		}
	}
	activate_profile(base);
}
void light_profile::load_profiles(){
	if (cf_exists_full("lighting_profiles.tbl", CF_TYPE_TABLES))
		light_profile::read_tables("lighting_profiles.tbl");
	parse_modular_table("*-ltp.tbm", light_profile::read_tables);
	light_profile::create_profiles();
	activate_default_profile();
}
void light_profile::read_tables(const char *filename)
{
	SCP_string default_name;
	SCP_string buffer;
	SCP_unordered_map<LIGHT_PROFILE_VALUE,light_profile_table_line> *next_profile;
	light_profile_table_line line;

	try
	{
		if (filename == nullptr)
			read_file_text_from_default(defaults_get_file("lighting_profiles.tbl"));
		else
			read_file_text(filename, CF_TYPE_TABLES);

		reset_parse();
		if(optional_string("Default profile:")){
			stuff_string(default_light_profile_name,F_NAME);
		}
		while ( !check_for_string("#End") && required_string("Name:"))
		{
			buffer.clear();
			next_profile = new SCP_unordered_map<LIGHT_PROFILE_VALUE,light_profile_table_line>;
			stuff_string(buffer,F_NAME);
			SCP_tolower(buffer);
			line.s = buffer;
			next_profile->insert(
				std::pair<LIGHT_PROFILE_VALUE,light_profile_table_line>
				(LPV_NAME, line) );
			while (!check_for_string("Name:") && !check_for_string("#End")){
				LIGHT_PROFILE_VALUE value_key = LPV_NULL;
				buffer.clear();
				if(optional_string("Tonemapper:")){
					stuff_string(buffer,F_NAME);
					line.i = 0;
					value_key =  LPV_MAPPER;
					SCP_tolower(buffer);
					//I wanted to do a switch statement
					//I don't know how to make that work with scpstrings
					if(buffer == "linear")
						line.i = tnm_Linear;
					else if(buffer=="aces")
						line.i = tnm_Aces;
					else if(buffer=="aces approximate")
						line.i = tnm_Aces_Approx;
					else if((buffer=="uncharted") ||( buffer=="uncharted 2"))
						line.i = tnm_Uncharted;
					else{
						value_key = LPV_NULL;
						//put debug error here
					}

				}
				//After we've checked every possible value, if it's one of them, stick in the wip table
				if(value_key != LPV_NULL){
					next_profile->insert(
					std::pair<LIGHT_PROFILE_VALUE,light_profile_table_line>
					(value_key, line) );
				}
			}
			raw_light_profiles.push_back(*next_profile);
		}
    }	catch (const parse::ParseException& e)
	{
		mprintf(("TABLES: Unable to parse '%s'!  Error message = %s.\n", (filename) ? filename : "<default ai_profiles.tbl>", e.what()));
		return;
	}
}


bool map_exists(SCP_unordered_map<LIGHT_PROFILE_VALUE,light_profile_table_line>* map, LIGHT_PROFILE_VALUE k){
	return( map->find(k)!=map->end());
}
int light_profile::current_tonemapper(){
	return current_light_profile.tonemapper;
}
void light_profile::create_profiles(){
	SCP_vector<SCP_string> profile_names;
	for(auto& r : raw_light_profiles){
		if(map_exists(&r ,LPV_NAME) &&
		!SCP_vector_contains(profile_names, r[LPV_NAME].s)) {
			profile_names.push_back((r[LPV_NAME].s));

		}
	}
	SCP_vector<SCP_unordered_map<LIGHT_PROFILE_VALUE,light_profile_table_line>> subtotals;
	for(auto& s : profile_names){
		SCP_unordered_map<LIGHT_PROFILE_VALUE,light_profile_table_line>* next_subtotal = nullptr;
		//itterate through all the things
		// copy the first one that matches the name to the subtotal
		// then overwrite any lines with subsequent matches
		for(auto & raw_light_profile : raw_light_profiles){
			if(next_subtotal == nullptr && raw_light_profile[LPV_NAME].s ==s) {
				next_subtotal = &raw_light_profile;
			}
			else if(raw_light_profile[LPV_NAME].s ==s){
				//we need to do this shit for every single one fuck argh.
				if(map_exists(&raw_light_profile , LPV_MAPPER)){
					next_subtotal->at(LPV_MAPPER).i=raw_light_profile[LPV_MAPPER].i;
				}
			}
		}
		subtotals.push_back(*next_subtotal);
	}
	//the semitotals now contains only what we need right?
	for(auto& st : subtotals){
		auto *lp = new light_profile();
		lp->reset();
		lp->name=st.at(LPV_NAME).s;
		if(map_exists(&st, LPV_MAPPER)){
			lp->tonemapper = st.at(LPV_MAPPER).i;
		}
		light_profiles.push_back(*lp);
	}
	if(light_profiles.empty()){
		auto* lp = new light_profile();
		lp->name = "default";
		default_light_profile_name = "default";
		light_profiles.push_back(*lp);
	}
}
void light_profile::reset(){
    exposure = 4.0f;
	name = "";

    static_light_factor = 1.0f;
    static_tube_factor = 1.0f;
    static_point_factor = 1.0f;
    tonemapper = tnm_Uncharted;

	PPC_toeS = 0.5f;
    PPC_toeL = 0.5f;
    PPC_shoulderS = 0.0f;
    PPC_shoulderL = 0.5f;
    PPC_shoulderA = 0.1f;

	glow_brightness_factor = 1.0f;
    vfx_brightness_factor = 1.0f;
    ambient_brightness_factor = 1.0f;
    ambient_brigthness_floor = 1.0f;

    sun_brightness_factor = 1.0f;
    point_brightness_factor = 1.0f;
    tube_brightness_factor = 1.0f;

	missile_radius_factor = 1.0f;
	laser_radius_factor = 1.0f;
    other_point_radius_factor = 1.0f;
    tube_radius_factor = 1.0f;
}
