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
auto a = store.getNew();
auto b = store.getNew();
auto c = store.getNew();
//grefs are index+gVector pointer, and inspecting in debug
// shows a proper spread of indexes.
auto a_ptr = store[a];
auto b_ptr = store[b];
auto c_ptr = store[c];
//but inspecting also shows these three pointers are identical, lol.

	a_ptr.value().id = 1;
	b_ptr.value().id = 2;
	c_ptr.value().id = 3;
	store.remove(b);
//	b.destroy();
	a_ptr = store[a];
	int a_val = store[a].value().id;
	auto b_ref = store[b];
	int c_val = store[c]->id;
	ASSERT_EQ(1,a_val);
	ASSERT_EQ(tl::nullopt,b_ref);
	//ASSERT_NE(2,b_ref.value().id);
	ASSERT_EQ(3,c_val);
}


TEST(GenerationalIndex, get2){
gVector<testobj> store;
auto a = store.getNew();
auto a2 = store[a];
a2->id =1;
ASSERT_EQ(1,a2->id);
ASSERT_EQ(1,store[a]->id);	
ASSERT_EQ(1,(store[a])->id);
}

TEST(GenerationalIndex, get3){
gVector<testobj> store;
	auto a = store.getNew();
	auto a2 = store[a];
	a2->id =1;
	ASSERT_EQ(1,store[a]->id);
}
TEST(GenerationalIndex, get4){
gVector<testobj> store;
// getNewRef returns an optional<gRef>, .value() gets the gRef
// I don't like the ergenomics of this but 
	auto a = store.getNew();
	auto a2 = store[a];
	a2->id =1;
	ASSERT_EQ(1,(store[a])->id);

}
TEST(GenerationalIndex, get5){
gVector<testobj> store;
// getNewRef returns an optional<gRef>, .value() gets the gRef
// I don't like the ergenomics of this but 
	auto a = store.getNew();
	store[a]->id =1;
	ASSERT_EQ(1,(store[a])->id);

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
