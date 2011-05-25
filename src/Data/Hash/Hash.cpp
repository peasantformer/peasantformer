#include "Hash.h"

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
