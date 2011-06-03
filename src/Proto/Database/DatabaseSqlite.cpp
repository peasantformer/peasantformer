#include "DatabaseSqlite.h"

/// @file

void DatabaseSqlite3::init() {
	stmt = NULL;
	stmt_tail = NULL;
	retcode = 0;
}

DatabaseSqlite3::DatabaseSqlite3() {
	dbhandle = NULL;
	init();
}

DatabaseSqlite3::DatabaseSqlite3(PString address) {
	dbhandle = NULL;
	init();
	connect(address);
}

DatabaseSqlite3::~DatabaseSqlite3() {
	disconnect();
}

void DatabaseSqlite3::connect(PString address) {
	retcode = sqlite3_open(address.c_str(),&dbhandle);
}
void DatabaseSqlite3::prepare(PString statement) {
	retcode = sqlite3_prepare(dbhandle,statement.c_str(),statement.c_str_length(),&stmt,&stmt_tail);
}
void DatabaseSqlite3::step() {
	results.clear();
	columns.clear();
	types.clear();
	retcode = sqlite3_step(stmt);
	if (isRow()) {
		int count = sqlite3_column_count(stmt);
		for (int i=0; i < count; i++) {
			int type = sqlite3_column_type(stmt,i);
			PString name(sqlite3_column_name(stmt,i));
			columns.push_back(name);
			switch (type) {
				case SQLITE_INTEGER:
					results.put(name,sqlite3_column_int(stmt,i));
					types[name] = DATABASECOLUMN_INT;
					break;
				case SQLITE_FLOAT:
					results.put(name,sqlite3_column_double(stmt,i));
					types[name] = DATABASECOLUMN_DOUBLE;
					break;
				case SQLITE_TEXT:
					results.put(name,PString((const char*)sqlite3_column_text(stmt,i)));
					types[name] = DATABASECOLUMN_STRING;
					break;
				case SQLITE_BLOB:
					break;
				case SQLITE_NULL:
					break;
					
			}
		}
	}
}
void DatabaseSqlite3::finalize() {
	results.clear();
	columns.clear();
	types.clear();
	init();
	retcode = sqlite3_finalize(stmt);
	
}
void DatabaseSqlite3::disconnect() {
	finalize();
	retcode = sqlite3_close(dbhandle);
	dbhandle = NULL;
}

Hash<PString> DatabaseSqlite3::get_results() {
	return results;
}

std::vector<PString> DatabaseSqlite3::get_columns() {
	return columns;
}
DatabaseColumnType DatabaseSqlite3::get_type(PString column) {
	return types[column];
}
int DatabaseSqlite3::get_int(PString column) {
	return results.get_int(column);
}
double DatabaseSqlite3::get_double(PString column) {
	return results.get_double(column);
}
PString DatabaseSqlite3::get_string(PString column) {
	return results.get_string(column);
}
void DatabaseSqlite3::bind_int(int param, int value) {
	retcode = sqlite3_bind_int(stmt, param, value);
}
void DatabaseSqlite3::bind_double(int param, double value) {
	retcode = sqlite3_bind_double(stmt, param, value);
}
void DatabaseSqlite3::bind_string(int param, PString value) {
	retcode = sqlite3_bind_text(stmt, param, value.c_str(),value.c_str_length(),SQLITE_TRANSIENT);
}
bool DatabaseSqlite3::isDone() {
	return retcode == SQLITE_DONE;
}
bool DatabaseSqlite3::isRow() {
	return retcode == SQLITE_ROW;
}
