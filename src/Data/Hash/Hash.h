#ifndef PEASANTFORMER_Data_Hash
#define PEASANTFORMER_Data_Hash

/// @file

#include <map>
#include <Proto/PlainIO/PlainIOHighLevel.h>

/// Hash holder type enum.
enum HashHolderType {
	HASHHOLDER_NULL, ///< Holder is NULL
	HASHHOLDER_INT, ///< Holder is int
	HASHHOLDER_DOUBLE, ///< Holder is double
	HASHHOLDER_TEXT ///< holder is text
};

/// HashHolder class.
class HashHolder {
	private:
		int int_value; ///< int holder value 
		double double_value; ///< double holder value
		pio_string text_value; ///< text holder value

		HashHolderType type; ///< holder type
	public:
		/// Dummy constructor.
		HashHolder();
		/// Int constructor.
		HashHolder(
			int value ///< [in] int value.
		);
		/// Double constructor.
		HashHolder(
			double value ///< [in] double value.
		);
		/// Text constructor
		HashHolder(
			pio_string value ///< [in] text value.
		);
	public:
		/// @return holder type
		HashHolderType get_type();

		/// @return int value
		int get_int();
		
		/// @return double value
		double get_double();
		
		/// @return text value
		pio_string get_text();
};

/// Hash class.
class Hash {
	private:
		std::map<pio_string,HashHolder> data; ///< data stored in hash
	public:
		/// Clears hash from contents
		void clear();
	public:
		/// Stores int value in hash.
		void put(
			pio_string key, ///< [in] key
			int value ///< [in] int value
		);
		/// Stores double value in hash.
		void put(
			pio_string key,  ///< [in] key
			double value ///< [in] double value
		);
		/// Stores text value in hash.
		void put(
			pio_string key, ///< [in] key
			pio_string value ///< [in] text value
		);
	public:
		/// @return int value
		int get_int(
			pio_string key ///< [in] key
		);
		/// @return double value
		double get_double(
			pio_string key ///< [in] key
		);
		/// @return text value
		pio_string get_text(
			pio_string key ///< [in] key
		);
};

#endif
