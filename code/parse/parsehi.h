#pragma once

//The immediate purpose of this file is to replace frequently repeated code constructs in table parsing code

#include "parse/parselo.h"
extern bool parse_optional_float_into(const SCP_string &fieldname, float* valuetarget);
//extern bool stuff_optional(const SCP_string &fieldname, SCP_string* valuetarget,int type);