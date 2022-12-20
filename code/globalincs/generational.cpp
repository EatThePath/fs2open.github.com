#include "globalincs/generational.h"

using namespace std;
using tl::optional;

template  <typename T>
optional<T> gVector<T>::operator[](gIndex i){
	if (i.index >= storage.size()) {
		return tl::nullopt;
	}
	if (storage[i.index].generation!= i.generation){
		return tl::nullopt;
	}
	return storage[i].value;
};

template  <typename T>
gIndex gVector<T>::add(T input){
	if (known_empty.empty()) {
		gEntry n;
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
		auto *n = *storage[i];
		n->value = input;
		n->generation++;
		gIndex r;
		r.index = i;
		r.generation = n->generation;
		known_empty.pop_back();
		return r;
	}
}


template  <typename T>
void gVector<T>::remove(gIndex i){
	if (i.index >= storage.size()) {
		return;
	}
	auto *s = *storage[i.index];
	if (i.generation != s->generation ) {
		return;
	}
	s->value.reset();
	known_empty.push_back(i.index);
}