#ifndef PEASANTFORMER_Database_Interface
#define PEASANTFORMER_Database_Interface

#include <stdio.h>
#include <map>

#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Data/Hash/Hash.h>
#include <sqlite3.h> 

class DatabaseInterface {
	private:
		sqlite3 *handle;
		sqlite3_stmt *stmt;
		const char *stmt_tail;
		int retcode;
		Hash *results;
	public:
		static const int RET_OK = SQLITE_OK;
		static const int RET_ERROR = SQLITE_ERROR;
		static const int RET_INTERNAL = SQLITE_INTERNAL;
		static const int RET_PERM = SQLITE_PERM;
		static const int RET_ABORT = SQLITE_ABORT;
		static const int RET_BUSY = SQLITE_BUSY;
		static const int RET_LOCKED = SQLITE_LOCKED;
		static const int RET_NOMEM = SQLITE_NOMEM;
		static const int RET_READONLY = SQLITE_READONLY;
		static const int RET_INTERRUPT = SQLITE_INTERRUPT;
		static const int RET_IOERR = SQLITE_IOERR;
		static const int RET_CORRUPT = SQLITE_CORRUPT;
		static const int RET_NOTFOUND = SQLITE_NOTFOUND;
		static const int RET_FULL = SQLITE_FULL;
		static const int RET_CANTOPEN = SQLITE_CANTOPEN;
		static const int RET_PROTOCOL = SQLITE_PROTOCOL;
		static const int RET_EMPTY = SQLITE_EMPTY;
		static const int RET_SCHEMA = SQLITE_SCHEMA;
		static const int RET_TOOBIG = SQLITE_TOOBIG;
		static const int RET_CONSTRAINT = SQLITE_CONSTRAINT;
		static const int RET_MISMATCH = SQLITE_MISMATCH;
		static const int RET_MISUSE = SQLITE_MISUSE;
		static const int RET_NOLFS = SQLITE_NOLFS;
		static const int RET_AUTH = SQLITE_AUTH;
		static const int RET_FORMAT = SQLITE_FORMAT;
		static const int RET_RANGE = SQLITE_RANGE;
		static const int RET_NOTADB = SQLITE_NOTADB;
		static const int RET_ROW = SQLITE_ROW;
		static const int RET_DONE = SQLITE_DONE;
	public:
		bool isDone();
		bool isRow();
	public:
		DatabaseInterface(pio_string &filename);
		~DatabaseInterface();
	public:
		int open(const char *filename);
		int prepare(pio_string &statement);
		int step();

		int column_type(int col);
		double column_double(int col);
		int column_int(int col);
		const unsigned char *column_text(int col);
		int column_bytes(int col);

		pio_string get_text(pio_string col);
		int get_int(pio_string col);
		double get_double(pio_string col);

		int finalize();

		int close();
		
		int bind_double(int pos, double value);
		int bind_int(int pos, int value);
		int bind_null(int pos);
		int bind_text(int pos, pio_string value);
};

#endif
