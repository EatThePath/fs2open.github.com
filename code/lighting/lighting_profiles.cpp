#include "ai/ai_profiles.h"
#include "def_files/def_files.h"
#include "globalincs/globals.h"
#include "globalincs/pstypes.h"
#include "globalincs/vmallocator.h"
#include "lighting/lighting.h"
#include "lighting/lighting_profiles.h"
#include "osapi/dialogs.h"
#include "parse/parselo.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <map>
#include <string>


SCP_vector<light_profile> light_profiles;

//A copy of the active light profile is made and used as our working data, so that we can make changes with SEXP and such
//and then revert easily.
light_profile current_light_profile;

//TODO: What 'default light profile' means is a bit in flux, document once it's figured out
int default_light_profile_index = 0;
SCP_string default_light_profile_name = "default";

/*
;So what's a lighting profile look like?
;IMO there's no need for ordering save for names being the delimiter.
;There's no need for a prefix character either, newlines to : is the identifier -
;		Actually I always assumed newlines were singificant to tables and that's
;			ACTUALLY JOKES IT'S TRUE AFTER ALL disregrard everything in this block I need to rethink.
;		So I need to either make it true for this case or use a distinct marker character.
*/

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


//*************PARSE LOGIC***************
//Every 'actionable' line of a table is read into a map of said lines
//No actual profile objects are created until all tables are read into this map
//If duplicate lines are encountered, then the value is overwritten at during the tableread
//This creates a flattened 'virtual table' representing all the table files laid over each other
//Thats then is parsed by asking the map for the values needed to set up each property of the table

SCP_vector<SCP_string> light_profile_value_names;
std::map<SCP_string, std::map<SCP_string,om_table_line>> virtual_light_profile_tables;

void light_profile::load_profiles(){
	mprintf(("TBM  =>  Starting parse of lighting profiles ...\n"));
	parse_init();
	parse_all();
	build_real_tables();
	light_profile_manager::activate_default_profile();
}
//This builds the dictionary of line names that the table will want to parse
//This is stand-in for eventually using the parse-items infrastructure
void light_profile:: parse_init(){
	light_profile_value_names.clear();
	light_profile_value_names.push_back("+Tonemapper:");
	light_profile_value_names.push_back("+exposure:");
	light_profile_value_names.push_back("+static light factor:");
	light_profile_value_names.push_back("+static tube factor:");
	light_profile_value_names.push_back("+static point factor:");
	light_profile_value_names.push_back("+PPC toeS:");
	light_profile_value_names.push_back("+PPC toeL:");
	light_profile_value_names.push_back("+PPC shoulderS:");
	light_profile_value_names.push_back("+PPC shoulderL:");
	light_profile_value_names.push_back("+PPC shoulderA:");
	light_profile_value_names.push_back("+glow brightness factor:");
	light_profile_value_names.push_back("+vfx brightness factor:");
	light_profile_value_names.push_back("+ambient brightness factor:");
	light_profile_value_names.push_back("+ambient brigthness floor:");
	light_profile_value_names.push_back("+sun brightness factor:");
	light_profile_value_names.push_back("+point brightness factor:");
	light_profile_value_names.push_back("+tube brightness factor:");
	light_profile_value_names.push_back("+missile radius factor:");
	light_profile_value_names.push_back("+laser radius factor:");
	light_profile_value_names.push_back("+other point radius factor:");
	light_profile_value_names.push_back("+tube radius factor:");
}

//The implicit table that sets the 'retail' values, sort of redundant with reset but
//  if filled out it'll allow a table dump possibility later.
void light_profile::add_default_default(){
	default_light_profile_name = "Uncharted 2";
	auto *dtl =new table_line;
	dtl->entity_name=default_light_profile_name;
	dtl->blame="BUILT IN-FSO SOURCE";
	dtl->value_name="+Tonemapper:";
	dtl->line_data="Uncharted 2";
	dtl->set = true;
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="linear";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="Aces";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="aces approximate";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="cineon";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="reinhard jodie";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="reinhard extended";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="piecewise";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);

	dtl->entity_name=dtl->line_data="piecewise rgb";
	set_virtual_table_value(dtl,&virtual_light_profile_tables);
}

//The logic for grabbing all the parseable files
void light_profile:: parse_all()
{
	if (cf_exists_full("lighting_profiles.tbl", CF_TYPE_TABLES)){
		light_profile::parse_file("lighting_profiles.tbl");
	}
	else{
		add_default_default();
	}
	parse_modular_table("*-ltp.tbm", light_profile::parse_file);
	if(virtual_light_profile_tables.empty()){
		add_default_default();
	}
}

//Handle an individual file.
void light_profile::parse_file(const char *filename)
{
	SCP_string buffer;
	try
	{
		if (filename == nullptr){
			//Normal parsing uses this:
			//read_file_text_from_default(defaults_get_file("lighting_profiles.tbl"));
			//To set the defaults, I intend to set them as virtual table lines before here
			//so, if no file, gtfo.
			return;
		}
		read_file_text(filename, CF_TYPE_TABLES);
		reset_parse();

		//There should be a meta-data section before getting to the profiles
		//Graceful handling of different sections tbd
		while ( !check_for_string("#End") && required_string("+Profile:")){
			//maybe should validate the name of the thing here, may even
			//  be better to consume and pass it here, not sure.
			parse_profile(filename);
		}
    }	catch (const parse::ParseException& e)
	{
		mprintf(("TABLES: Unable to parse '%s'!  Error message = %s.\n", (filename) ? filename : "<default ai_profiles.tbl>", e.what()));
		return;
	}
}

//Each discrete entity is handled in this function
//  it may be good later to have this take a pointer to the table of the
//  thing that it is parsing, a parse items dictionary etc, and an optional
//  identifier if it is a 'subtable' section(beam sections, etc)
void light_profile::parse_profile(const char *blame)
{
	SCP_string profile_name;
	profile_name.clear();
	//The calling function is currently expected to have consumed the token before the
	//  name but left the name there.
	//  needs some validation and an early exit, I guess...
	stuff_string(profile_name,F_RAW);
	SCP_tolower(profile_name);

	if(profile_name.length()<1)
	{
		//TODO make some error noise
		return;
	}
	while(parse_nextline(blame,&profile_name)){}
	//Nextline returns true whenever it finds a next line, if it returns false we're /
	//  done and will yeild back to the file for section checking and stuff

}

//Returns true if it processed at least one line
//  can return potentially loop through all lines of a profile in one go if they're in the right order
bool light_profile::parse_nextline(const char *blame, SCP_string *const profile_name){

	SCP_string buffer;
	bool found = false;
	for(auto& key : light_profile_value_names){
		if(optional_string(key.c_str())){
			buffer.clear();
			stuff_string(buffer,F_RAW);
			auto *line = new table_line();
			line->entity_name=*profile_name;
			line->value_name=key;
			line->set=true;
			line->blame = blame;
			line->line_data = buffer;
			set_virtual_table_value(line,&virtual_light_profile_tables);
			found = true;
		}
	}
	return found;
}

//Using maps because otherwise I'm terrified of how long it'll take for my weapon tbms to load
void light_profile::set_virtual_table_value(table_line* line, std::map<SCP_string, std::map<SCP_string,om_table_line>>* set){
	auto n = line->entity_name;
	SCP_tolower(n);
	//these bools are figured here because the conditionlines were getting ugly to my eyes
	 bool entity_exists = (set->find(n)==set->end());

	 bool line_exists = (entity_exists &&
	 	(*set)[n].find(line->value_name) == (*set)[n].end());

	 if(!entity_exists){
		std::map<SCP_string,om_table_line> new_entity;
		 (*set)[n] = new_entity;
	 }
	 if(!line_exists){
		 om_table_line new_vt_line;
		 new_vt_line.line_data = line->line_data;
		 new_vt_line.blame = line->blame;
		 new_vt_line.set = true;
		 (*set)[n][line->value_name] = new_vt_line;
	 }
	 else{
		 auto& vtline =	 (*set)[n][line->value_name];
		 vtline.line_data = line->line_data;
		 vtline.blame = line->blame;
		 vtline.set = true;
	 }
}

//Now that all the 'virtual tables' are built, make the real profiles from them.
void light_profile::build_real_tables(){
	char inbuffer[512]; //TODO: make this better

	//The parser shouldn't be doing anything important right now anyway, but in case it is
	pause_parse();
	snprintf(Current_filename, sizeof(Current_filename), "virtual light profiles");

	//get the unique profile names in the virtual tables
	//build a profile for each one.
	SCP_string buffer;
	for(auto t : virtual_light_profile_tables){
		//Reach up inside the parser and make it do our bidding.
		auto vlp =t.second;
		strcpy(inbuffer,"");
		strcpy(inbuffer,(t.first+"\0").c_str());
		Mp = inbuffer;

		stuff_string(buffer,F_RAW);
		SCP_tolower(buffer);

		auto *lp = new light_profile();
		lp->reset();
		lp->name = buffer;

		SCP_string k ="+Tonemapper:";
		if(virtual_table_value_is_set(&k,&vlp)){

			//Why two copies? Paranoia.
			strcpy(inbuffer,"");
			strcpy(inbuffer,(vlp[k].line_data+"\0").c_str());
			Mp = inbuffer;


			stuff_string(buffer,F_RAW);
			SCP_tolower(buffer);

			auto tn = tnm_Uncharted;
			if(buffer == "linear")
				tn = tnm_Linear;
			else if(buffer=="aces")
				tn = tnm_Aces;
			else if(buffer=="aces approximate")
				tn = tnm_Aces_Approx;
			else if((buffer=="uncharted") ||( buffer=="uncharted 2"))
				tn = tnm_Uncharted;
			else if(buffer=="cineon")
				tn =tnm_Cineon;
			else if(buffer=="reinhard jodie")
				tn =tnm_Reinhard_Jodie;
			else if(buffer=="reinhard extended")
				tn =tnm_Reinhard_Extended;
			else if(buffer=="piecewise")
				tn = tnm_PPC;
			else if(buffer=="piecewise rgb")
				tn = tnm_PPC_RGB;
			else{
				//error goes here
			}
			lp->tonemapper = tn;
		}
		light_profiles.push_back(*lp);
	}
	//Put the parser back
	unpause_parse();

}
bool light_profile::virtual_table_value_is_set(SCP_string *const keyname, std::map<SCP_string,om_table_line> *vtable){
	if(vtable->find(*keyname)==vtable->end()){
		return false;
	}
	return vtable->at(*keyname).set;
}

light_profile*  light_profile_manager::find_by_name(SCP_string *name){
	SCP_string target = *name;
	SCP_tolower(target);
	for(light_profile& lp : light_profiles)
	{
		if(lp.name == target){
			return &lp;
		}
	}
	return nullptr;
}
light_profile*  light_profile_manager::at(int i){
	if( i<0 || i>=light_profiles.size()){
		Error(LOCATION,"Invalid light profile index requested.");
		return nullptr;
	}
	return &light_profiles[i];
}
void light_profile_manager::activate(int i){
	if( i<0 || i>=light_profiles.size()){
		Error(LOCATION,"Invalid light profile index activated.");
	}
	activate(&light_profiles[i]);
}
void light_profile_manager::activate(SCP_string* name){
	auto *p = find_by_name(name);
	if(p == nullptr){
		Error(LOCATION,"Invalid light profile name activated");
	}
	else
		activate(p);
}

void light_profile_manager::activate(light_profile* base){
	current_light_profile.name = base->name;
	current_light_profile.tonemapper = base->tonemapper;
}
void light_profile_manager::activate_default_profile(){
	if(light_profiles.empty()){
		Error(LOCATION,"No light profiles available, what happened to the defaults?");
	}
	auto *base = find_by_name(&default_light_profile_name);

	if (base==nullptr) {
		Warning(LOCATION,"Couldn't find default lighting profile, falling back to first profile.");
		base = at(0);
	}
	activate(base);
}

int light_profile_manager::current_tonemapper(){
	return current_light_profile.tonemapper;
}
SCP_string light_profile_manager::current_name(){
	return current_light_profile.name;
}
light_profile saved_light_profile;
void light_profile_manager::save_current(){
	saved_light_profile = current_light_profile;
}
void light_profile_manager::restore_saved(){
	current_light_profile = saved_light_profile;
}
int light_profile_manager::count(){
	return light_profiles.size();
}