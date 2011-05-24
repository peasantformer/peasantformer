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
		HashHolder() {
			this->type = HASHHOLDER_NULL;
		}
		HashHolder(int value) {
			this->type = HASHHOLDER_INT;

			this->double_value = value;
			this->text_value = pio_string("");
			this->int_value = value;
		}
		HashHolder(double value) {
			this->type = HASHHOLDER_DOUBLE;

			this->int_value =  value;
			this->text_value = pio_string("");
			this->double_value = value;
		}
		HashHolder(pio_string value) {
			this->type = HASHHOLDER_TEXT;

			this->int_value = 0;
			this->double_value = 0;
			this->text_value = value;
		}
	public:
		HashHolderType get_type() {
			return this->type;
		}

		int get_int() {
			return int_value;
		}
		double get_double() {
			return double_value;
		}
		pio_string get_text() {
			return text_value;
		}
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
