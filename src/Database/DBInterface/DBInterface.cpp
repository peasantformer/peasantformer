#include "DBInterface.h"

DatabaseInterface::DatabaseInterface(const pio_string &filename) {
	this->handle = NULL;
	this->stmt = NULL;
	this->stmt_tail = NULL;
	this->retcode = -1;
	this->open(filename.c_str());
}

DatabaseInterface::~DatabaseInterface() {
	this->finalize();
	this->close();
}

int DatabaseInterface::open(const char *filename) {
	this->retcode = sqlite3_open(filename,&this->handle);
	return this->retcode;
}

int DatabaseInterface::prepare(const char *statement, int length) {
	this->retcode = sqlite3_prepare(this->handle,statement,length,&this->stmt,&this->stmt_tail);
	return this->retcode;
}

int DatabaseInterface::step() {
	this->retcode = sqlite3_step(this->stmt);
	return this->retcode;
}




int DatabaseInterface::column_type(int col) {
	return sqlite3_column_type(this->stmt, col);
}

double DatabaseInterface::column_double(int col) {
	return sqlite3_column_double(this->stmt, col);
}

int DatabaseInterface::column_int(int col) {
	return sqlite3_column_int(this->stmt, col);
}

const unsigned char *DatabaseInterface::column_text(int col) {
	return sqlite3_column_text(this->stmt, col);
}

int DatabaseInterface::column_bytes(int col) {
	return sqlite3_column_bytes(this->stmt, col);
}





int DatabaseInterface::finalize() {
	int ret;
	ret = sqlite3_finalize(this->stmt);
	this->stmt = NULL;
	this->stmt_tail = NULL;
	this->retcode = -1;
	return ret;
}

int DatabaseInterface::close() {
	int ret;
	ret = sqlite3_close(this->handle);
	this->stmt = NULL;
	this->stmt_tail = NULL;
	this->handle = NULL;
	this->retcode = -1;
	return ret;
}


int DatabaseInterface::bind_double(int pos, double value) {
	this->retcode = sqlite3_bind_double(this->stmt, pos, value);
	return this->retcode;
}
int DatabaseInterface::bind_int(int pos, int value) {
	this->retcode = sqlite3_bind_int(this->stmt, pos, value);
	return this->retcode;
}
int DatabaseInterface::bind_null(int pos) {
	this->retcode = sqlite3_bind_null(this->stmt, pos);
	return this->retcode;
}
int DatabaseInterface::bind_text(int pos, const char *value, int length) {
	this->retcode = sqlite3_bind_text(this->stmt, pos, value, length, SQLITE_TRANSIENT);
	return this->retcode;
}




bool DatabaseInterface::isRow() {
	return this->retcode == this->RET_ROW;
}
bool DatabaseInterface::isDone() {
	return this->retcode == this->RET_DONE;
}
