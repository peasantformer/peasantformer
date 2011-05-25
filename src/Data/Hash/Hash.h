#ifndef PEASANTFORMER_Data_Hash
#define PEASANTFORMER_Data_Hash

#include <map>
#include <Proto/PlainIO/PlainIOHighLevel.h>

enum HashHolderType {
	HASHHOLDER_NULL,
	HASHHOLDER_INT,
	HASHHOLDER_DOUBLE,
	HASHHOLDER_TEXT
};

class HashHolder {
	private:
		int int_value;
		double double_value;
		pio_string text_value;

		HashHolderType type;
	public:
		HashHolder();
		HashHolder(int value);
		HashHolder(double value);
		HashHolder(pio_string value);
	public:
		HashHolderType get_type();

		int get_int();
		double get_double();
		pio_string get_text();
};

class Hash {
	private:
		std::map<pio_string,HashHolder> data;
	public:
		void clear() {
			this->data.clear();
		}
	public:
		void put(pio_string key, int value) {
			this->data[key] = HashHolder(value);
		}
		void put(pio_string key, double value) {
			this->data[key] = HashHolder(value);
		}
		void put(pio_string key, pio_string value) {
			this->data[key] = HashHolder(value);
		}
	public:
		int get_int(pio_string key) {
			return this->data[key].get_int();
		}
		double get_double(pio_string key) {
			return this->data[key].get_double();
		}
		pio_string get_text(pio_string key) {
			return this->data[key].get_text();
		}
};

#endif
