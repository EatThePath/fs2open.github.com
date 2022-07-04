#pragma once
#include "graphics/color.h"
#include "parse/parselo.h"


// parsehi is intended for higher level, frequently used combinations of parselo functions
// and also for collection of standardized parsing of complex types where appropriate

extern bool parse_optional_float_into(const SCP_string& field_name, float* value_target);

extern bool parse_optional_bool_into(const SCP_string& field_name, bool* value_target);

extern bool parse_optional_color3i_into(const SCP_string& field_name, hdr_color* out_color);

extern bool parse_optional_int_into(const SCP_string& fieldname, int* valuetarget);
extern bool parse_optional_int_list_into(const SCP_string& fieldname, int* valuetarget, int length);

extern bool parse_optional_clamped_int_into(const SCP_string& fieldname, int* valuetarget, int min = 0, int max = 1);

extern bool parse_optional_float_list_into(const SCP_string& fieldname, float* valuetarget, int length);

extern bool parse_optional_clamped_float_into(const SCP_string& fieldname,
	float* valuetarget,
	const float min = 1.0f,
	const float max = 1.0f);
extern bool parse_optional_clamped_float_list_into(const SCP_string& fieldname,
	float* valuetarget,
	int length,
	const float min = 1.0f,
	const float max = 1.0f);

extern bool parse_optional_string_into(const SCP_string& fieldname,
	char* outstr,
	int type,
	int len,
	const char* terminators = nullptr);
extern void parse_required_string_into(const SCP_string& fieldname,
	char* outstr,
	int type,
	int len,
	const char* terminators = nullptr);
