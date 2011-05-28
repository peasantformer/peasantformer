#include "Hash.h"

/// @file

HashHolder::HashHolder() {
	this->type = HASHHOLDER_NULL;
}
HashHolder::HashHolder(int value) {
	this->type = HASHHOLDER_INT;

	this->double_value = value;
	this->text_value = pio_string("");
	this->int_value = value;
}
HashHolder::HashHolder(double value) {
	this->type = HASHHOLDER_DOUBLE;

	this->int_value =  value;
	this->text_value = pio_string("");
	this->double_value = value;
}
HashHolder::HashHolder(pio_string value) {
	this->type = HASHHOLDER_TEXT;

	this->int_value = 0;
	this->double_value = 0;
	this->text_value = value;
}
HashHolderType HashHolder::get_type() {
	return this->type;
}

int HashHolder::get_int() {
	return int_value;
}
double HashHolder::get_double() {
	return double_value;
}
pio_string HashHolder::get_text() {
	return text_value;
}






void Hash::clear() {
	this->data.clear();
}
void Hash::put(pio_string key, int value) {
	this->data[key] = HashHolder(value);
}
void Hash::put(pio_string key, double value) {
	this->data[key] = HashHolder(value);
}
void Hash::put(pio_string key, pio_string value) {
	this->data[key] = HashHolder(value);
}
int Hash::get_int(pio_string key) {
	return this->data[key].get_int();
}
double Hash::get_double(pio_string key) {
	return this->data[key].get_double();
}
pio_string Hash::get_text(pio_string key) {
	return this->data[key].get_text();
}
