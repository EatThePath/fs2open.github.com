#pragma once
#include "parse/parselo.h"
#include "graphics/color.h"

//parsehi is intended for higher level, frequently used combinations of parselo functions
//and also for collection of standardized parsing of complex types where appropriate
enum PARSE_MODES{
    FLOAT,
    BOOL
};
enum PARSE_REQUIREMENT{
    OPTIONAL,
    REQUIRED
};
struct parse_map_field{
    SCP_string label;
    size_t target_offset;
    PARSE_MODES mode;
    PARSE_REQUIREMENT required;
}
typedef parse_map SCP_unordered_map<SCP_string,parse_map_field>;

extern bool parse_optional_float_into(const SCP_string& field_name, float* value_target);

extern bool parse_optional_bool_into(const SCP_string& field_name, bool* value_target);

extern bool parse_optional_color3i_into(const SCP_string &field_name, hdr_color *out_color);