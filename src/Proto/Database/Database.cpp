#include "Database.h"

/// @file

Database::Database(pio_string filename) {
	this->handle = NULL;
	this->stmt = NULL;
	this->stmt_tail = NULL;
	this->retcode = -1;
	this->results = new Hash;
	this->open(filename.c_str());
}

Database::~Database() {
	this->finalize();
	this->close();
	delete this->results;
}

int Database::open(const char *filename) {
	this->retcode = sqlite3_open(filename,&this->handle);
	return this->retcode;
}

int Database::prepare(pio_string &statement) {
	this->retcode = sqlite3_prepare(this->handle,statement.c_str(),statement.length(),&this->stmt,&this->stmt_tail);
	return this->retcode;
}

int Database::step() {
	this->results->clear();
	this->retcode = sqlite3_step(this->stmt);
	if (this->isRow()) {
		int count = sqlite3_column_count(this->stmt);
		for (int i=0; i < count; i++) {
			int type = this->column_type(i);
			const char *name = sqlite3_column_name(this->stmt,i);
			switch (type) {
				case SQLITE_INTEGER:
					this->results->put(name,this->column_int(i));
					break;
				case SQLITE_FLOAT:
					this->results->put(name,this->column_double(i));
					break;
				case SQLITE_TEXT:
					this->results->put(name,this->column_text(i));
					break;
				case SQLITE_BLOB:
					break;
				case SQLITE_NULL:
					break;
			}
		}
	}
	return this->retcode;
}




int Database::column_type(int col) {
	return sqlite3_column_type(this->stmt, col);
}

double Database::column_double(int col) {
	return sqlite3_column_double(this->stmt, col);
}

int Database::column_int(int col) {
	return sqlite3_column_int(this->stmt, col);
}

const unsigned char *Database::column_text(int col) {
	return sqlite3_column_text(this->stmt, col);
}

int Database::column_bytes(int col) {
	return sqlite3_column_bytes(this->stmt, col);
}





int Database::finalize() {
	int ret;
	ret = sqlite3_finalize(this->stmt);
	this->results->clear();
	this->stmt = NULL;
	this->stmt_tail = NULL;
	this->retcode = -1;
	return ret;
}

int Database::close() {
	int ret;
	ret = sqlite3_close(this->handle);
	this->stmt = NULL;
	this->stmt_tail = NULL;
	this->handle = NULL;
	this->retcode = -1;
	return ret;
}


int Database::bind_double(int pos, double value) {
	this->retcode = sqlite3_bind_double(this->stmt, pos, value);
	return this->retcode;
}
int Database::bind_int(int pos, int value) {
	this->retcode = sqlite3_bind_int(this->stmt, pos, value);
	return this->retcode;
}
int Database::bind_null(int pos) {
	this->retcode = sqlite3_bind_null(this->stmt, pos);
	return this->retcode;
}
int Database::bind_text(int pos, pio_string value) {
	this->retcode = sqlite3_bind_text(this->stmt, pos, value.c_str(), value.length(), SQLITE_TRANSIENT);
	return this->retcode;
}




bool Database::isRow() {
	return this->retcode == this->RET_ROW;
}
bool Database::isDone() {
	return this->retcode == this->RET_DONE;
}


pio_string Database::get_text(pio_string col) {
	return this->results->get_text(col);
}
int Database::get_int(pio_string col) {
	return this->results->get_int(col);
}
double Database::get_double(pio_string col) {
	return this->results->get_double(col);
}

