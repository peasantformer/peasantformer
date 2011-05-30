#ifndef PEASANTFORMER_Data_Hash
#define PEASANTFORMER_Data_Hash

/// @file

#include <map>
#include <Data/PString/PString.h>

/// Hash holder value type
enum HashHolderType {
	HASHHOLDER_NULL,   ///< NULL value
	HASHHOLDER_INT,    ///< int value
	HASHHOLDER_DOUBLE, ///< double value
	HASHHOLDER_STRING  ///< string value
};


/// Union-like class.
class HashHolder {
	private:
		HashHolderType type;  ///< type of data
		int value_int;        ///< int data
		double value_double;  ///< double data
		PString value_string; ///< string data
		
	public:
		/// Dummy constructor
		HashHolder();
		
		/// Int sonctructor
		///
		/// \param [in] value is int value
		HashHolder(int value);
		
		/// Double sonctructor
		///
		/// \param [in] value is double value
		HashHolder(double value);
		
		/// String constructor
		///
		/// \param [in] value is a PString string
		HashHolder(PString value);
	public:
		
		/// Get hash holder type
		///
		/// @return type of the hash cell
		HashHolderType get_type();
		
		/// Get int data from hash
		///
		/// @return int value
		int get_int();
		
		/// Get double data from hash
		///
		/// @return double value
		double get_double();
		
		/// Get string data from hash
		///
		/// @return PString string value
		PString get_string();
};

/// Hash class
template <class T>
class Hash {
	private:
		std::map<T,HashHolder> data; ///< Hash cell data
	public:
		/// Dummy constructor
		Hash();
		/// Dummy destructor
		~Hash();
	public:
		/// Clear function, wipes data
		void clear();
		
		/// Puts NULL in hash
		///
		/// \param [in] key is key value
		void put(T key);
		
		/// Puts int in hash
		///
		/// \param [in] key is key value
		/// \param [in] value is int value
		void put(T key, int value);
		
		/// Puts double in hash
		///
		/// \param [in] key is key value
		/// \param [in] value is double value
		void put(T key, double value);
		
		/// Puts string in hash
		///
		/// \param [in] key is key value
		/// \param [in] value is PString value
		void put(T key, PString value);
		
		/// Gets type of element from hash
		///
		/// \param [in] key is key value
		/// @return type of hash cell
		HashHolderType get_type(T key);
		
		/// Gets int from hash
		///
		/// \param [in] key is key value
		/// @return int
		int get_int(T key);
		
		/// Gets double from hash
		///
		/// \param [in] key is key value
		/// @return double
		double get_double(T key);
		
		/// Gets string from hash
		///
		/// \param [in] key is key value
		/// @return PString
		PString get_string(T key);
};

template <class T>
Hash<T>::Hash() {
}

template <class T>
Hash<T>::~Hash() {
}

template <class T>
void Hash<T>::clear() {
	data.clear();
}

template <class T>
void Hash<T>::put(T key) {
	data[key] = HashHolder();
}

template <class T>
void Hash<T>::put(T key, int value) {
	data[key] = HashHolder(value);
}

template <class T>
void Hash<T>::put(T key, double value) {
	data[key] = HashHolder(value);
}

template <class T>
void Hash<T>::put(T key, PString value) {
	data[key] = HashHolder(value);
}

template <class T>
HashHolderType Hash<T>::get_type(T key) {
	return data[key].get_type();
}

template <class T>
int Hash<T>::get_int(T key) {
	return data[key].get_int();
}

template <class T>
double Hash<T>::get_double(T key) {
	return data[key].get_double();
}

template <class T>
PString Hash<T>::get_string(T key) {
	return data[key].get_string();
}
#endif
