#pragma once

#include "deflate.h"
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
//forward declaration of gref, fun.
class gRef;
template  <typename T>

//rules:
//Caps can only ever be set at the current size or smaller
//shrinking the vector would lead to forgetting the generation counts and then potential slot reuse if allowed to expand later.
class gVector {
	vector<gEntry<T>> storage;
	vector<size_t> known_empty;
	bool capped = false;
	size_t cap = 0;

	//Internal function to expand the storage list with a new object
	//for use when there are no empty slots and the storage is not capped
	gIndex add_new(){
		gEntry<T> n;
		n.generation = 0;
		//Assuming trivial constructor behavior here...
		storage.push_back(n);
		gIndex i;
		i.index = storage.size()-1;
		i.generation = 0;
		return i;
	};

	//Internal function to use an existing free slot for a new object
	gIndex use_free(){
		auto i = known_empty.back();
		Assertion(storage[i].value == nullopt ,"Attempted to use_free on a slot that was not free");
		//values are created as nullopt when expanding a list
		//and resest to nullopt here so we need to construct a new object to go in there.
		storage[i].value = T();
		storage[i].generation++;
		gIndex r;
		r.index = i;
		r.generation = storage[i].generation;
		known_empty.pop_back();
		return r;
	}
	//internal function to find or create a new slot
	//if size is capped and there's no slot, return nullopt.
	optional<gIndex> get_new(){
		if (known_empty.empty()) {
			if (capped && storage.size()>=cap) {
				return tl::nullopt;
			}
			return add_new();
		}
		return use_free();
	};
	//Internal function to 
	void fill_empty_list(){
		known_empty.clear();
		for (size_t i=storage.size()-1; i>=0;i--){
			known_empty.push_back(i);
		}
	}

	public:
	gVector<T>() = default;
	gVector<T>(size_t cap){
	};

	void reset(){
		capped = false;
		cap = 0;
		for(gEntry<T> entry: storage){
			if (entry.value != tl::nullopt)
				entry.reset;
		}
		fill_empty_list();
	}

	void reset(size_t cap_in){
		capped = true;
		cap=MAX(cap_in,storage.size());//shrinking a container is illegal.

		for(gEntry<T> entry: storage){
			if (entry.value != tl::nullopt)
				entry.reset;
		}
		for (size_t i=storage.size(); i<cap;i++){
			gEntry<T> e;
			e.generation = 0;
			e.value = tl::nullopt;
			storage.push_back(e);
		}
		fill_empty_list();
	}

	//Index into the storage...
	optional<T> operator[](gIndex i) {
		if (i.index >= storage.size()) {
			return tl::nullopt;
		}
		if (storage[i.index].generation!= i.generation){
			return tl::nullopt;
		}
		return storage[i.index].value;
	};

	optional<T*> get_pointer(gIndex i){
		if (i.index >= storage.size()) {
			return tl::nullopt;
		}
		if (storage[i.index].generation!= i.generation){
			return tl::nullopt;
		}
		return *(storage[i.index].value);
	};

	//gets a gRef object
	//this sidesteps much of the boilerplate stuff, a ref can just be asked for a pointer
	optional<gRef<T>> getNewRef(){
		T n;
		auto i = add(n);
		gRef<T> r = gRef<T>(i,this);
		return i;

	};
	optional<gRef<T>> getRef(gIndex i){
		if (i.index >= storage.size()) {
			return tl::nullopt;
		}
		if (storage[i.index].generation!= i.generation){
			return tl::nullopt;
		}
		gRef<T> r = gRef<T>(i,this);

		return r;
	};

	//Store a value in the vector
	//TODO: Handle capped stuff
	//This is kind of in appropriate for the actual usage this is primarily intended to replace
	//to the point where if we want this functionality it might need to be a seperate data structure for sake of strictness
	gIndex add(T input){
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
	};

	void remove(gIndex i){
		if (i.index >= storage.size()) {
			return;
		}
		if (storage[i.index].generation != i.generation ) {
			return;
		}
		storage[i.index].value.reset();
		known_empty.push_back(i.index);
	};
	bool check(gIndex i){
		return (this[i]!=tl::nullopt);
	}



	//iterator shit.
	struct iter{
		using iterator_category = std::input_iterator_tag;
		using index = size_t;

		iter(gVector<T> *vec_ptr, vector<gEntry<T>> *storage_ptr){
			storage = storage_ptr;
			vec = vec_ptr;
		};
		iter(gVector<T> *vec_ptr, vector<gEntry<T>> *storage_ptr, size_t start_pos){
			storage = storage_ptr;
			vec = vec_ptr;
			inner_position = start_pos;
			if(storage->at(inner_position).value == nullptr){
				(*this)++;
			}
		};
		bool operator==(const iter& rhs){
			return inner_position == rhs.inner_position;
		};
		bool operator<(const iter& rhs){
			return inner_position < rhs.inner_position;
		};
		iter& operator++(){
			inner_position++;
			//we want to skip empty slots
			while(storage->at(inner_position).value == tl::nullopt 
				  && inner_position < storage->size()){
				inner_position++;
			}
			full_position.index=inner_position;
			if(inner_position < storage->size()
			   && storage->at(inner_position).value != tl::nullopt){
				full_position.generation = storage->at(inner_position).generation;
			}
			return *this;
		};
		iter& operator++(int i){
			return this++;
		}
		T* operator*(){
			vec->get_pointer(full_position);
		};
		gRef<T> operator&(){
			vec->getRef(inner_position);
		};
	private:
		index inner_position;
		gIndex full_position;
		vector<gEntry<T>> *storage;
		gVector<T> *vec;
	};
	iter begin(){
		return iter(this,&storage,0);
		//if (known_empty_) {
		//statements
		//}
	};
	iter end(){
		return iter(this,&storage,storage.size());
	}
};

template  <typename T>
class gRef{
   gVector<T> *vec;
   gIndex ind;
   public:
   gRef(){ vec = nullptr; ind = gIndex();};
   gRef(gVector<T> *v,gIndex i){
	vec = v;
	ind = i;

   };
   T* get_pointer(){
	 if(vec==nullptr){return nullptr;}
	 auto r = vec->get_pointer(ind);
	 if(r == tl::nullopt)
	 {
		return nullptr;
	 }
	 assert(r!= nullptr);
	 return r;
   };
   bool check(){
	 return vec->check(ind);
   }
   void destroy(){
	vec->remove(ind);
   }
};
