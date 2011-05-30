#include "Hash.h"

/// @file

HashHolder::HashHolder() {
	this->type = HASHHOLDER_NULL;
}

HashHolder::HashHolder(int value) {
	this->type = HASHHOLDER_INT;
	this->value_int = value;
}

HashHolder::HashHolder(double value) {
	this->type = HASHHOLDER_DOUBLE;
	this->value_double = value;
}

HashHolder::HashHolder(PString value) {
	this->type = HASHHOLDER_STRING;
	this->value_string = value;
}

HashHolderType HashHolder::get_type() {
	return this->type;
}

int HashHolder::get_int() {
	assert(this->type == HASHHOLDER_INT);
	return this->value_int;
	
}

double HashHolder::get_double() {
	assert(this->type == HASHHOLDER_DOUBLE);
	return this->value_double;
}

PString HashHolder::get_string() {
	assert(this->type == HASHHOLDER_STRING);
	return this->value_string;
}
