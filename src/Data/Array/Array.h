#ifndef PEASANTFORMER_Data_Array
#define PEASANTFORMER_Data_Array

#include <stddef.h>
#include <map>

template <class T>
class Array {
	private:
		size_t count;
		std::map<size_t,T> data;
	public:
		Array() {
			this->count = 0;
		}
		~Array() {
			this->count = 0;
			this->data.clear();
		}
	public:
		T & operator[](size_t i) {
			return this->data[i];
		}
		Array & operator=(Array const &r) {
			this->data.clear();
			for (size_t i=0; i < r.size(); i++) {
				this->add_item(r.data[i]);
			}
			return *this;
		}
	public:
		size_t add_item(T item) {
			this->count++;
			this->data[this->count-1] = item;
			return this->count-1;
		}
		typename std::map<size_t,T>::iterator find(size_t id) {
			return this->data.find(id);
		}
		T *get_find_ref(size_t id) {
			return &this->data.find(id)->second;
		}
		void del_item (size_t i) {
		}
		size_t size() const {
			return this->count;
		}
		void clear() {
			this->count = 0;
			this->data.clear();
		}
};

#endif
