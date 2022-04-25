#pragma once
#include "graphics/2d.h"
#include "graphics/color.h"
#include "globalincs/pstypes.h"
#include "globalincs/vmallocator.h"

class hdr_color{
	float r,g,b,a,i;
public:
	hdr_color();
	hdr_color(float new_r, float new_g, float new_b, float new_a = 1.0f, float new_i =1.0f);
	hdr_color(hdr_color* source_color);

	void reset();

	void get_v5f(SCP_vector<float>* outvec) const;
	void get_v3f(SCP_vector<float>* outvec) const;
	void set_vecf(const SCP_vector<float>* input);
	void set_i(float new_intensity);
	void set_rgb(int new_r, int new_g, int new_b);
	void set_rgb(const color * new_rgb);

	float g_r() const;
	float g_b() const;
	float g_g() const;
	float g_a() const;
	float g_i() const;
};