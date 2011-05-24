#ifndef PEASANTFORMER_Database_Interface
#define PEASANTFORMER_Database_Interface

#include <stdio.h>
#include <sqlite3.h> // temprory, should be removed when mroe generic
                     // version of this interface would be built


class DatabaseInterface {
	private:
		sqlite3 *dbhandle; // temprory
		sqlite3_stmt *stmt; //temprory
		const char *stmt_tail;
	public:
		DatabaseInterface();
		DatabaseInterface(const char *filename);
		~DatabaseInterface();
	public:
		int connect(const char *filename);
		int prepare(const char *statement, int length);
		int step();
		
		int column_type(int col);
		
		int column_bytes(int col);
	
		double column_double(int col);
		int column_int(int col);
		const unsigned char *column_text(int col);

		int finalize();

		int disconnect();

		int bind_double(int pos, double value);
		int bind_int(int pos, int value);
		int bind_null(int pos);
		int bind_text(int pos, const char *value, int length);
};

#endif
