#pragma once

#include <tl/optional.hpp>
#include <vcruntime.h>

using namespace std;
using  tl::optional;
struct gIndex{
	size_t index;
	size_t generation;
};


template  <typename T>
struct gEntry {
	size_t generation;
	optional<T> value;
};

template  <typename T>

class gVector {
	vector<gEntry<T>> storage;
	vector<size_t> known_empty;
	public:
	gVector<T>() = default;
	gVector<T>(size_t capacity){
		//fill the slots backwards, so popping from known empty later gets entry 0.
		for (size_t i=capacity-1; i>=capacity;i--){
			gEntry<T> e;
			e.generation = 0;
			e.value = tl::nullopt;
			storage.push_back(e);
			known_empty.push_back(i);
		}
	};
	optional<T> operator[](gIndex i);
	optional<T*> get_pointer(gIndex i);
	gIndex add(T input);
	void remove(gIndex i);
};


template  <typename T>
optional<T> gVector<T>::operator[](gIndex i){
	if (i.index >= storage.size()) {
		return tl::nullopt;
	}
	if (storage[i.index].generation!= i.generation){
		return tl::nullopt;
	}
	return storage[i.index].value;
};
template  <typename T>

optional<T*> gVector<T>::get_pointer(gIndex i){
	if (i.index >= storage.size()) {
		return tl::nullopt;
	}
	if (storage[i.index].generation!= i.generation){
		return tl::nullopt;
	}
	return *(storage[i.index].value);
}
template  <typename T>
gIndex gVector<T>::add(T input){
	if (known_empty.empty()) {
		gEntry<T> n;
		n.generation = 0;
		n.value = input;
		storage.push_back(n);
		gIndex i;
		i.index = storage.size()-1;
		i.generation=0;
		return i;
		}
	else {
		auto i = known_empty.back();
		storage[i].value = input;
		storage[i].generation++;
		gIndex r;
		r.index = i;
		r.generation = storage[i].generation;
		known_empty.pop_back();
		return r;
	}
}


template  <typename T>
void gVector<T>::remove(gIndex i){
	if (i.index >= storage.size()) {
		return;
	}
	if (storage[i.index].generation != i.generation ) {
		return;
	}
	storage[i.index].value.reset();
	known_empty.push_back(i.index);
}