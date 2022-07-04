#include "parsehi.h"
#include "globalincs/vmallocator.h"
#include "parselo.h"
#include "graphics/color.h"

/**
 * @brief Parses an optional table value into a field if the name is found
 *
 * @param field_name The name of the table field
 * @param value_target Pointer to the variable to assign any parsed value to
 *
 * @return True if a value was parsed, false if not
 */
bool parse_optional_bool_into(const SCP_string& field_name, bool* value_target)
{
	if (optional_string(field_name.c_str())) {
		stuff_boolean(value_target);
		return true;
	}
	return false;
}

bool parse_optional_int_into(const SCP_string &fieldname, int* valuetarget)
{
	if(optional_string(fieldname.c_str())){
		stuff_int(valuetarget);
		return true;
	}
	return false;
}

bool parse_optional_clamped_int_into(const SCP_string &fieldname, int* valuetarget,const int min, const int max)
{
	if(parse_optional_int_into(fieldname,valuetarget)){
		CLAMP(*valuetarget,min,max);
		return true;
	}
	return false;
}


/**
 * @brief Parses an optional table value into a field if the name is found
 *
 * @param field_name The name of the table field
 * @param value_target Pointer to the variable to assign any parsed value to
 *
 * @return True if a value was parsed, false if not
 */
bool parse_optional_float_into(const SCP_string& field_name, float* value_target)
{
	if (optional_string(field_name.c_str())) {
		stuff_float(value_target);
		return true;
	}
	return false;
}

bool parse_optional_clamped_float_into(const SCP_string &fieldname, float* valuetarget,const float min, const float max)
{
	if(parse_optional_float_into(fieldname,valuetarget)){
		CLAMP(*valuetarget,min,max);
		return true;
	}
	return false;
}

bool parse_optional_int_list_into(const SCP_string &fieldname, int* valuetarget,int length)
{
	if(optional_string(fieldname.c_str())){
		stuff_int_list(valuetarget, length);
		return true;
	}
	return false;
}


bool parse_optional_float_list_into(const SCP_string &fieldname, float* valuetarget,int length)
{
	if(optional_string(fieldname.c_str())){
		stuff_float_list(valuetarget, length);
		return true;
	}
	return false;
}


bool parse_optional_clamped_float_list_into(const SCP_string &fieldname, float* valuetarget,int length,const float min, const float max)
{
	if(parse_optional_float_list_into(fieldname,valuetarget,length)){
		for(int i=0;i<length;i++){
			CLAMP(valuetarget[i],min,max);
		}
		return true;
	}
	return false;
}

bool parse_optional_string_into(const SCP_string &fieldname,char *outstr, int type, int len, const char *terminators){
	if( optional_string(fieldname.c_str()) ){
		stuff_string(outstr, type, len,terminators);
		return true;
	}
	return false;
}

void parse_required_string_into(const SCP_string &fieldname,char *outstr, int type, int len, const char *terminators)
{
	required_string(fieldname.c_str());
	stuff_string(outstr, type, len,terminators);

}
/**
 * @brief Parses an optional table color into an object if the color is found
 *
 * @param field_name The name of the table field
 * @param value_target Pointer to the variable to assign any parsed value to
 *
 * @return True if a value was parsed, false if not
 */
bool parse_optional_color3i_into(const SCP_string& field_name, hdr_color* out_color)
{
	if (optional_string(field_name.c_str())) {
		int components[3] = {255, 255, 255};
		stuff_int_list(components, 3, RAW_INTEGER_TYPE);
		if (out_color == nullptr) {
			Assertion(out_color,"out_color pointer is null for field %s",field_name.c_str());
			return false;
		}
		out_color->set_rgb(components);
		return true;
	}
	return false;
}