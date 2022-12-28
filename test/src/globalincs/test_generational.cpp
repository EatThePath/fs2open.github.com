#include <gtest/gtest.h>
#include <exception>
#include "globalincs/generational.h"
#include "tl/optional.hpp"
struct testobj{
	int id;
};

TEST(GenerationalIndex, safe_access){
	gVector<testobj> store;
	auto a = store.getNew();
	auto b = store.getNew();
	store[a].id=1;
	store[b].id=2;
	store.remove(b);
	auto *bptr = &store[b];
	ASSERT_EQ(store[a].id, 1);
	ASSERT_EQ(bptr,nullptr);
}



TEST(GenerationalIndex, bad_access){
	gVector<testobj> store;
	auto a = store.getNew();
	auto b = store.getNew();
	store[a].id=1;
	store[b].id=2;
	store.remove(b);
	ASSERT_EQ(&store[b],nullptr);
}

TEST(GenerationalIndex, safe_reuse){
	gVector<testobj> store;
	auto a = store.getNew();
	auto b = store.getNew();
	store[a].id=1;
	store[b].id=2;
	store.remove(a);
	auto c = store.getNew();
	store[c].id=3;
	ASSERT_EQ(a.index,c.index);
	ASSERT_NE(a.generation,c.generation);
	ASSERT_EQ(&store[a],nullptr);
	ASSERT_EQ(store[c].id, 3);
}

