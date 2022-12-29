#pragma once

#include <tl/optional.hpp>

using namespace std;
using  tl::optional;
struct gIndex{
	private:
	int index;
	int generation;
	bool is_null = true;
	public:
	gIndex(){ nullify();};
	gIndex(int i,int g,bool b = false){
		if(b){ 
		}
		index = i;
		generation = g;
	};
	void set(int i,int g){
		is_null = false;
		index = i;
		generation = g;
	}
	void nullify()
	{
		index = -1;
		generation = -1;
		is_null=true;
	};
	bool operator==(gIndex &rhs) const {
		if(is_null && rhs.is_null)
			return true;
		if(is_null || rhs.is_null)
			return false;
		return (index == rhs.index && generation == rhs.generation);
	};
	gIndex& operator=(gIndex const&rhs) = default;
	int i() const { return index;};
	int g() const { return generation;};
	bool null() const { return is_null;};
};

template  <typename T>

struct gEntry {
	int generation;
	optional<T> stored;

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
		gIndex i(storage.size()-1,0);
		return i;
	};

	//Internal function to use an existing free slot for a new object
	gIndex use_free(){
		auto i = known_empty.back();
		Assertion(storage[i].value == tl::nullopt ,"Attempted to use_free on a slot that was not free");
		//values are created as nullopt when expanding a list
		//and resest to nullopt here so we need to construct a new object to go in there.
		storage[i].value = T();
		storage[i].generation++;
		gIndex r(i,storage[i].generation);
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
			if (entry.stored != tl::nullopt)
				entry.reset();
		}
		fill_empty_list();
	}

	void reset(size_t cap_in){
		capped = true;
		cap=MAX(cap_in,storage.size());//shrinking a container is illegal.

		for(gEntry<T> entry: storage){
			if (entry.stored != tl::nullopt)
				entry.reset();
		}
		for (size_t i=storage.size(); i<cap;i++){
			gEntry<T> e;
			e.generation = 0;
			e.stored = tl::nullopt;
			storage.push_back(e);
		}
		fill_empty_list();
	}

/*
	//Index into the storage... with optionals
	optional<T&> operator[](gIndex i) {
		T* v = nullptr;
		if (i.index >= storage.size()) {
			return tl::nullopt;
		}
		if (storage[i.index].generation!= i.generation){
			return tl::nullopt;
		}
		if(storage[i.index].stored==tl::nullopt) 
			return tl::nullopt;
		v = &storage[i.index].stored.value();
		return optional<T&>(*v);//(storage[i.index].stored);
	};
*///*
	//Index into the storage... bare reference
	T& operator[](gIndex i) {
		T* v = nullptr;
		if (i.i() >= storage.size()) {
			return *v;
		}
		if (storage[i.i()].generation!= i.g()){
			return *v;
		}
		if(storage[i.i()].stored==tl::nullopt) 
			return *v;
		v = (storage[i.i()].stored).operator->();
		return *v;
	};
//*/
	optional<T*> get_pointer(gIndex i){
		if (i.i() >= storage.size()) {
			return tl::nullopt;
		}
		if (storage[i.i()].generation!= i.g()){
			return tl::nullopt;
		}
		optional<T> s = storage[i.i()].value;
		T* p = s.operator->();
		return p;
	};


	
	optional<gIndex> getNew(){
		T n = T();
		auto i = add(n);
		return i;
	};

	//Store a value in the vector
	//TODO: Handle capped stuff
	//This is kind of in appropriate for the actual usage this is primarily intended to replace
	//to the point where if we want this functionality it might need to be a seperate data structure for sake of strictness
	gIndex add(T input){
	if (known_empty.empty()) {
		gEntry<T> n;
		n.generation = 0;
		n.stored = input;
		storage.push_back(n);
		gIndex i(((int) storage.size())-1,(int) 0);;
		return i;
		}
	else {
		auto i = known_empty.back();
		storage[i].stored = input;
		storage[i].generation++;
		gIndex r(i,storage[i].generation);
		known_empty.pop_back();
		return r;
		}
	};

	void remove(optional<gIndex> i){
		if (i.has_value())
			remove(i.value());
	};
	void remove(gIndex i){
		if (i.i() >= storage.size()) {
			return;
		}
		if (storage[i.i()].generation != i.g() ) {
			return;
		}
		storage[i.i()].stored.reset();
		known_empty.push_back(i.i());
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
			if(inner_position < storage->size()
			   && storage->at(inner_position).value != tl::nullopt){
				full_position.set(inner_position,storage->at(inner_position).generation);
			}
			else{full_position.nullify();}
			return *this;
		};
		iter& operator++(int i){
			return this++;
		}
		T* operator*(){
			vec->get_pointer(full_position);
		};
		T& operator&(){
			return vec[inner_position];
		};
		gIndex position(){return full_position;}
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
	 assert(r.value() != nullptr);
	 T *v = r.value();
	 return v;
   };
   bool check(){
	 return vec->check(ind);
   }
   void destroy(){
	 vec->remove(ind);
   }
};
