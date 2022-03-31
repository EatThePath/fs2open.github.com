#include "parselo.h"
#include "parsehi.h"

bool parse_optional_float_into(const SCP_string &fieldname, float* valuetarget)
{
	if(optional_string(fieldname.c_str())){
		stuff_float(valuetarget);
		return true;
	}
	return false;
}
