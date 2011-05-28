#ifndef PEASANTFORMER_Proto_Database
#define PEASANTFORMER_Proto_Database

/// @file

#include <stdio.h>
#include <map>

#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Data/Hash/Hash.h>
#include <sqlite3.h> 

/// Database routines class.
class Database {
	private:
		sqlite3 *handle; ///< database handle
		sqlite3_stmt *stmt; ///< sql statement
		const char *stmt_tail; ///< sql statement rejected tail pointer
		int retcode; ///< return code of last method call
		Hash *results; ///< results from query
	public:
		static const int RET_OK = SQLITE_OK; ///< retcode OK
		static const int RET_ERROR = SQLITE_ERROR; ///< retcode ERROR
		static const int RET_INTERNAL = SQLITE_INTERNAL; ///< retcode INTERNAL
		static const int RET_PERM = SQLITE_PERM; ///< retcode PERM
		static const int RET_ABORT = SQLITE_ABORT; ///< retcode ABORT
		static const int RET_BUSY = SQLITE_BUSY; ///< retcode BUSY
		static const int RET_LOCKED = SQLITE_LOCKED; ///< retcode LOCKED
		static const int RET_NOMEM = SQLITE_NOMEM; ///< retcode NOMEM
		static const int RET_READONLY = SQLITE_READONLY; ///< retcode READONLY
		static const int RET_INTERRUPT = SQLITE_INTERRUPT; ///< retcode INTERRUPT
		static const int RET_IOERR = SQLITE_IOERR; ///< retcode IOERR
		static const int RET_CORRUPT = SQLITE_CORRUPT; ///< retcode CORRUPT
		static const int RET_NOTFOUND = SQLITE_NOTFOUND; ///< retcode NOTFOUND
		static const int RET_FULL = SQLITE_FULL; ///< retcode FULL
		static const int RET_CANTOPEN = SQLITE_CANTOPEN; ///< retcode CANTOPEN
		static const int RET_PROTOCOL = SQLITE_PROTOCOL; ///< retcode PROTOCO:
		static const int RET_EMPTY = SQLITE_EMPTY; ///< retcode EMPTY
		static const int RET_SCHEMA = SQLITE_SCHEMA; ///< retcode SCHEMA
		static const int RET_TOOBIG = SQLITE_TOOBIG; ///< retcode TOOBIG
		static const int RET_CONSTRAINT = SQLITE_CONSTRAINT; ///< retcode CONSTRAINT
		static const int RET_MISMATCH = SQLITE_MISMATCH; ///< retcode MISMATCH
		static const int RET_MISUSE = SQLITE_MISUSE; ///< retcode MISUSE
		static const int RET_NOLFS = SQLITE_NOLFS; ///< retcode NOLFS
		static const int RET_AUTH = SQLITE_AUTH; ///< retcode AUTH
		static const int RET_FORMAT = SQLITE_FORMAT; ///< retcode FORMAT
		static const int RET_RANGE = SQLITE_RANGE; ///< retcode RANGE
		static const int RET_NOTADB = SQLITE_NOTADB; ///< retcode NOTAB
		static const int RET_ROW = SQLITE_ROW; ///< retcode ROW
		static const int RET_DONE = SQLITE_DONE; ///< retcode DONE
	public:
		/// Is query done?
		bool isDone();
		/// Is next row pending?
		bool isRow();
	public:
		/// Create database class from filename
		Database(
			pio_string filename ///< [in] file path
		);
		/// Database destructor ^^
		~Database();
	public:
		/// Open database file
		/// @return dunno
		int open(
			const char *filename ///< [in] filepath
		);
		/// Prepare SQL statement
		/// @return dunno
		int prepare(
			pio_string &statement ///< [in] statement string
		);
		/// Iterate prepared statement
		int step();

		/// @return column type
		int column_type(
			int col ///< [in] column index
		);
		/// @return column as double
		double column_double(
			int col ///< [in] column index
		);
		/// @return column as int
		int column_int(
			int col ///< [in] column index
		);
		/// @return column as text
		const unsigned char *column_text(
			int col ///< [in] column index
		);
		/// @return size of column as text
		int column_bytes(
			int col ///< [in] column index
		);

		/// @return column as pio_string text
		pio_string get_text(
			pio_string col ///< [in] column name
		);
		/// @return column as int
		int get_int(
			pio_string col ///< [in] column name
		);
		/// @return column as double
		double get_double(
			pio_string col ///< [in] column name
		);
		
		/// Finalize current transaction.
		int finalize();

		/// Close db handle.
		int close();
		
		/// Bind double to sql statement query
		/// @return bind result
		int bind_double(
			int pos, ///< [in] parameter number
			double value ///< [in] double value
		);
		/// Bind int to sql statement query
		/// @return bind result
		int bind_int(
			int pos, ///< [in] parameter number
			int value ///< [in] int value
		);
		/// Bind null to sql statement query
		/// @return bind result
		int bind_null(
			int pos ///< [in] parameter number
		);
		/// Bind text to sql statement query
		/// @return bind result
		int bind_text(
			int pos,  ///< [in] parameter number
			pio_string value ///< [in] text value
		);
};

#endif
