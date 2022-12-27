#include <gtest/gtest.h>

#include "globalincs/generational.h"
#include "tl/optional.hpp"
struct testobj{
	int id;
	//testobj() {id=0;};
};
TEST(GenerationalIndex, get){
	gVector<testobj> store;
	// getNewRef returns an optional<gRef>, .value() gets the gRef
	// I don't like the ergenomics of this but 
	gRef<testobj> a = store.getNewRef().value();
	gRef<testobj> b = store.getNewRef().value();
	gRef<testobj> c = store.getNewRef().value();
	//grefs are index+gVector pointer, and inspecting in debug
	// shows a proper spread of indexes.
	auto * a_ptr = a.get_pointer();
	auto * b_ptr = b.get_pointer();
	auto * c_ptr = c.get_pointer();
	//but inspecting also shows these three pointers are identical, lol.

	a_ptr->id = 1;
	b_ptr->id = 2;
	c_ptr->id = 3;

	b.destroy();
	a_ptr = a.get_pointer();
	int a_val = a_ptr->id;
	ASSERT_EQ(1,a_val);
	ASSERT_EQ(nullptr,b.get_pointer());
	int c_val =c.get_pointer()->id;
	ASSERT_EQ(3,c_val);
}
/*
TEST(GenerationalIndex, access) {
	{
		gVector<float> data;
		gIndex first = data.add(1.0f);
		gIndex second = data.add(2.0f);
		gIndex third = data.add(3.0f);
		data.remove(second);
		gIndex fourth = data.add(4.0f); 
		auto bad_access = data[second];
		auto good_access = data[fourth];
		ASSERT_EQ(tl::nullopt, bad_access);
		ASSERT_EQ(4.0f, good_access);
	}
}
TEST(GenerationalIndex, indexes) {
	{
		gVector<float> data;
		gIndex first = data.add(1.0f);
		gIndex second = data.add(2.0f);
		gIndex third = data.add(3.0f);
		data.remove(second);
		gIndex fourth = data.add(4.0f);
		gIndex fifth = data.add(5.0f);
		ASSERT_EQ(0, first.index);
		ASSERT_EQ(0, first.generation);
		ASSERT_EQ(1, second.index);
		ASSERT_EQ(0, second.generation);
		ASSERT_EQ(2, third.index);
		ASSERT_EQ(0, third.generation);
		ASSERT_EQ(1, fourth.index);
		ASSERT_EQ(1, fourth.generation);
		ASSERT_EQ(3, fifth.index);
		ASSERT_EQ(0, fifth.generation);
	}
	*/
