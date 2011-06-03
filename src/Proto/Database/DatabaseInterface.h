#ifndef PEASANTFORMER_Proto_DatabaseInterface
#define PEASANTFORMER_Proto_DatabaseInterface

/// @file

#include <vector>
#include <map>
#include <Data/PString/PString.h>
#include <Data/Hash/Hash.h>

/// Database column types
enum DatabaseColumnType {
	DATABASECOLUMN_NULL,   ///< column of NULL type
	DATABASECOLUMN_INT,    ///< column of int type
	DATABASECOLUMN_DOUBLE, ///< column of double type
	DATABASECOLUMN_STRING  ///< column of string type
};

/// Database interface
class DatabaseInterface {
	protected:
		std::vector<PString> columns; ///< coluns of results query
		std::map<PString,DatabaseColumnType> types; ///< types of columns in results query
		Hash<PString> results;        ///< query results are stored here
	public:
		/// Dummy constructor
		DatabaseInterface() {}
		/// Virtual destructor
		virtual ~DatabaseInterface() {}
	public:
		/// Connect to database.
		///
		/// \param [in] address to connect
		virtual void connect(PString address) = 0;
		
		/// Prepare statement.
		///
		/// \param [in] statement is statement to prepare
		virtual void prepare(PString statement) = 0;
		
		/// Iterate statement.
		virtual void step() = 0;
		
		/// Finalize statement.
		virtual void finalize() = 0;
		
		/// Disconnect from database.
		virtual void disconnect() = 0;
		
		/// Gets all results from current row
		///
		/// @return results hash
		virtual Hash<PString> get_results() = 0;
		
		/// Get column names
		///
		/// @return vector of column names
		virtual std::vector<PString> get_columns() = 0;
		
		/// Get type of column with given name.
		///
		/// \param [in] column is a column name
		/// @return DatabaseColumnType value
		virtual DatabaseColumnType get_type(PString column) = 0;
		
		/// Get int value of column with given name
		///
		/// \param [in] column is a column name
		/// @return int value
		virtual int get_int(PString column) = 0;
		
		/// Get double value of column with given name
		///
		/// \param [in] column is a column name
		/// @return double value
		virtual double get_double(PString column) = 0;
		
		/// Get string value of column with given name
		///
		/// \param [in] column is a column name
		/// @return string value
		virtual PString get_string(PString column) = 0;
		
		/// Bind a int value to the Nth statement parameter
		///
		/// \param [in] param is a number of parameter
		/// \param [in] value is an int value
		virtual void bind_int(int param, int value) = 0;
		
		/// Bind a double value to the Nth statement parameter
		///
		/// \param [in] param is a number of parameter
		/// \param [in] value is a dobule value
		virtual void bind_double(int param, double value) = 0;
		
		/// Bind a string value to the Nth statement parameter
		///
		/// \param [in] param is a number of parameter
		/// \param [in] value is a string value
		virtual void bind_string(int param, PString value) = 0;
		
		/// Returns true if there is no more rows pending
		///
		/// @return boolean
		virtual bool isDone() = 0;
		
		/// Returns true if there is any rows pending
		///
		/// @return boolean
		virtual bool isRow() = 0;
};


#endif
