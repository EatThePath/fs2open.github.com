#pragma once
#include "parselo.h"
#include "graphics/color.h"

enum Parse_Require{
    PARSE_REQ_OPTIONAL,
    PARSE_REQ_REQUIRED
};

//parsehi is intended for higher level, frequently used combinations of parselo functions
//and also for collection of standardized parsing of complex types where appropriate

extern bool parse_float_into(const SCP_string& field_name, float* value_target, Parse_Require need_level);

extern bool parse_bool_into(const SCP_string& field_name, bool* value_target, Parse_Require need_level);

extern bool parse_color3i_into(const SCP_string &field_name, hdr_color *out_color, Parse_Require need_level);