#pragma once

#include <tl/optional.hpp>

using namespace std;
using  tl::optional;
struct gIndex{
	size_t index;
	size_t generation;
};


template  <typename T>

class gVector {
	struct gEntry{
		size_t generation;
		optional<T> value;
	};
	vector<gEntry> storage;
	vector<size_t> known_empty;
	public:
	optional<T> operator[](gIndex i);
	gIndex add(T input);
	void remove(gIndex i);
};