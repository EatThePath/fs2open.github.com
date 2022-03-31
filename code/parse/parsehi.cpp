#include "parselo.h"
#include "parsehi.h"




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


bool parse_optional_float_into(const SCP_string &fieldname, float* valuetarget)
{
	if(optional_string(fieldname.c_str())){
		stuff_float(valuetarget);
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