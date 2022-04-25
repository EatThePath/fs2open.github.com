#include "graphics/color.h"

#include "globalincs/pstypes.h"
#include "globalincs/vmallocator.h"

#include "graphics/2d.h"

void hdr_color::set_vecf(const SCP_vector<float>* input)
{
	size_t l = input->size();
	Assertion(l >= 3, "attempted to set color values invalid component count");
	this->r = input->at(0);
	this->g = input->at(1);
	this->b = input->at(2);
	if (l > 3)
		this->a = input->at(3);
	if (l > 4)
		this->i = input->at(4);
}

void hdr_color::get_v5f(SCP_vector<float>* to_fill) const
{
	if(to_fill ==nullptr)
		to_fill = new SCP_vector<float>;
	to_fill->clear();
	to_fill->push_back(this->r);
	to_fill->push_back(this->g);
	to_fill->push_back(this->b);
	to_fill->push_back(this->a);
	to_fill->push_back(this->i);
}

void hdr_color::get_v3f(SCP_vector<float>* to_fill) const
{
	if(to_fill ==nullptr)
		to_fill = new SCP_vector<float>;
	to_fill->clear();
	to_fill->push_back(this->r*this->i);
	to_fill->push_back(this->g*this->i);
	to_fill->push_back(this->b*this->i);
}

hdr_color::hdr_color()
{
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 1.0f;
	this->a = 1.0f;
	this->i = 1.0f;
}

hdr_color::hdr_color(float new_r, float new_g, float new_b, float new_a, float new_i)
{
	this->r = new_r;
	this->g = new_g;
	this->b = new_b;
	this->a = new_a;
	this->i = new_i;
}
hdr_color::hdr_color(hdr_color *source_color)
{
	this->r = source_color->r;
	this->g = source_color->g;
	this->b = source_color->b;
	this->a = source_color->a;
	this->i = source_color->i;
}
void hdr_color::set_i(float new_intensity)
{
	this->i = new_intensity;
}

void hdr_color::set_rgb(int new_r, int new_g, int new_b)
{
	this->r = i2fl(new_r)/255.0f;
	this->g = i2fl(new_g)/255.0f;
	this->b = i2fl(new_b)/255.0f;
}

void hdr_color::set_rgb(const color * new_color)
{
	this->set_rgb(new_color->red, new_color->green, new_color->blue);
	this->a=i2fl(new_color->alpha);
}
float hdr_color::g_r() const {
	return this->r;
}
float hdr_color::g_b() const {
	return this->b;
}
float hdr_color::g_g() const {
	return this->g;
}
float hdr_color::g_a() const {
	return this->a;
}
float hdr_color::g_i() const {
	return this->i;
}

void hdr_color::reset(){
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 1.0f;
	this->a = 1.0f;
	this->i = 1.0f;
}