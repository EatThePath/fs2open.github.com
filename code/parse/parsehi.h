#pragma once


//The immediate purpose of this file is to replace frequently repeated code patterns in table parsing functons


#include "parse/parselo.h"

extern bool parse_optional_int_into(const SCP_string &fieldname, int* valuetarget);
extern bool parse_optional_clamped_int_into(const SCP_string &fieldname, int* valuetarget,int min = 0, int max = 1);

extern bool parse_optional_float_into(const SCP_string &fieldname, float* valuetarget);
extern bool parse_optional_clamped_float_into(const SCP_string &fieldname, float* valuetarget,const float min = 1.0f, const float max = 1.0f);
extern bool parse_optional_clamped_float_list_into(const SCP_string &fieldname, float* valuetarget,int length,const float min = 1.0f, const float max = 1.0f);
extern bool parse_optional_string_into(const SCP_string &fieldname, char *outstr, int type, int len, const char *terminators = nullptr);
extern void parse_required_string_into(const SCP_string &fieldname, char *outstr, int type, int len, const char *terminators = nullptr);
