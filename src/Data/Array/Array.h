#ifndef PEASANTFORMER_Data_Array
#define PEASANTFORMER_Data_Array

/// @file

#include <stddef.h>
#include <map>

/// Array container.

template <class T>
class Array {
	private:
		size_t count; ///< count of elements stored in Array
		std::map<size_t,T> data; ///< actual data of the Array
	public:
		
		/// Dummy constuctor.
		Array() {
			this->count = 0;
		}
		/// Dummy destructor.
		~Array() {
			this->count = 0;
			this->data.clear();
		}
	public:
		/// Subscripting operator.
		/// @return subscripted value
		T & operator[](
			size_t i ///< index of element
		) {
			return this->data[i];
		}
		/// Asignment operator.
		/// @return resulting expression
		Array & operator=(
			Array const &r ///< [in] Right hand expression
		) {
			this->data.clear();
			for (size_t i=0; i < r.size(); i++) {
				this->add_item(r.data[i]);
			}
			return *this;
		}
	public:
		/// Adds item to container.
		/// @return index of added item
		size_t add_item(
			T item ///< [in] item to store 
		) {
			this->count++;
			this->data[this->count-1] = item;
			return this->count-1;
		}
		/// matches element by key.
		/// @return iterator to matched element
		typename std::map<size_t,T>::iterator find(
			size_t id ///< [in] key of the element
		) {
			return this->data.find(id);
		}
		/// returns refference to to item with specified key.
		/// @return reference to matched element
		T *get_find_ref(
			size_t id ///< [in] key of the element
		) {
			return &this->data.find(id)->second;
		}
		/// deletes item by given key.
		void del_item (
			size_t i ///< [in] item key
		) {
		}
		/// returns number of contained elements.
		/// @return number of contained elements
		size_t size() const {
			return this->count;
		}
		///  wipes out array
		///
		void clear() {
			this->count = 0;
			this->data.clear();
		}
};

#endif
