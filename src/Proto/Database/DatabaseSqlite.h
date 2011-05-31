#ifndef PEASANTFORMER_Proto_DatabaseSqlite3
#define PEASANTFORMER_Proto_DatabaseSqlite3

/// @file

#include <sqlite3.h>

#include <Proto/Database/DatabaseInterface.h>


/// Sqlite3 database interface. See DatabaseInterface for refference
class DatabaseSqlite3 : public DatabaseInterface {
	private:
		sqlite3 *dbhandle;     ///< database handle
		sqlite3_stmt *stmt;    ///< prepared sql statement
		const char *stmt_tail; ///< statement rejected tail
		int retcode;           ///< last return code of internal functions
	private:
		/// Helper function for constructors
		void init();
	public:
	
		/// Dummy constructor
		DatabaseSqlite3();
		
		/// Constructor from db address
		///
		/// \param [in] address is a database address
		DatabaseSqlite3(PString address);
		
		/// Dummy destructor
		~DatabaseSqlite3();
	public:
		void connect(PString address);
		void prepare(PString statement);
		void step();
		void finalize();
		void disconnect();
		std::vector<PString> get_columns();
		DatabaseColumnType get_type(PString column);
		int get_int(PString column);
		double get_double(PString column);
		PString get_string(PString column);
		void bind_int(int param, int value);
		void bind_double(int param, double value);
		void bind_string(int param, PString value);
		bool isDone();
		bool isRow();
		
};

#endif 
