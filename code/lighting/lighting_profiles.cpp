
#include "lighting/lighting_profiles.h"

void parse_lighting_profiles_tbl(const char *filename)
{
	int i;
	char profile_name[NAME_LENGTH];
	ai_profile_t dummy_profile;
	char *saved_Mp = NULL;
	char buf[NAME_LENGTH];

	try
	{
		if (filename == NULL)
			read_file_text_from_default(defaults_get_file("ai_profiles.tbl"));
		else
			read_file_text(filename, CF_TYPE_TABLES);

		reset_parse();

void light_profile::reset(){
    int tonemapper_select = tnm_Uncharted;
    float exposure = 4.0f;
    
    
    float static_light_factor = 1.0f;
    float static_tube_factor = 1.0f;
    float static_point_factor = 1.0f;
    int tonemapper_selection = 1;
    
    float PPC_toeS = 0.5f;
    float PPC_toeL = 0.5f;
    float PPC_shoulderS = 0.0f;
    float PPC_shoulderL = 0.5f;
    float PPC_shoulderA = 0.1f;
    
    float glow_brightness_factor = 1.0f;
    float vfx_brightness_factor = 1.0f;    

    float ambient_brightness_factor = 1.0f;
    float ambient_brigthness_floor = 1.0f;
    float sun_brightness_factor = 1.0f;
    float point_brightness_factor = 1.0f;
    float tube_brightness_factor = 1.0f;
        
    float point_radius_factor = 1.0f;
    float tube_radius_factor = 1.0f;

    void reset();
}