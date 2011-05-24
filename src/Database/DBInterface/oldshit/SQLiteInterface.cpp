#include "SQLiteInterface.h"

int DatabaseInterface::connect(const char *filename) {
	return sqlite3_open(filename,&this->dbhandle);
}


int DatabaseInterface::prepare(const char *statement, int length) {
	return sqlite3_prepare(this->dbhandle,statement,length,&this->stmt,&this->stmt_tail);
}


int DatabaseInterface::step() {
	return sqlite3_step(this->stmt);
}

int DatabaseInterface::column_type(int col) {
	return sqlite3_column_type(this->stmt, col);
}

int DatabaseInterface::column_bytes(int col) {
	return sqlite3_column_bytes(this->stmt, col);
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


int DatabaseInterface::finalize() {
	return sqlite3_finalize(this->stmt);
	this->stmt = NULL;
	this->stmt_tail = NULL;
}
int DatabaseInterface::disconnect() {
	return sqlite3_close(this->dbhandle);
}

DatabaseInterface::DatabaseInterface() {
	this->dbhandle = NULL;
	this->stmt = NULL;
	this->stmt_tail = NULL;
}
DatabaseInterface::DatabaseInterface(const char *filename) {
	this->dbhandle = NULL;
	this->stmt = NULL;
	this->stmt_tail = NULL;
	this->connect(filename);
}
DatabaseInterface::~DatabaseInterface() {
	this->finalize();
	this->disconnect();
}

int DatabaseInterface::bind_double(int pos, double value) {
	return sqlite3_bind_double(this->stmt, pos, value);
}
int DatabaseInterface::bind_int(int pos, int value) {
	return sqlite3_bind_int(this->stmt, pos, value);
}
int DatabaseInterface::bind_null(int pos) {
	return sqlite3_bind_null(this->stmt, pos);
}
int DatabaseInterface::bind_text(int pos, const char *value, int length) {
	return sqlite3_bind_text(this->stmt, pos, value, length, SQLITE_TRANSIENT);
}
