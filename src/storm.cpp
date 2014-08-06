#include "storm.h"

#include <memory>
#include <stdarg.h>

using namespace std;

namespace ORM{

/* from storm_config.cpp */
bool initConfig();

/* from storm_pool.cpp */
bool initPool();
void quitPool();

bool init(){
	if( !initConfig() ) return false;
	if( !initPool() ) return false;

	return true;
}
void quit(){
	quitPool();
}

string get_last_error(){
	MYSQL *mysql = get_db();

	return mysql_error( mysql );
}
int raw_query(const std::string &query){
	return mysql_query(
		get_db(), query.c_str() );
}
int raw_query(const std::string format, ...){
	va_list va;
	std::unique_ptr<char[]> query;
	int size = format.size() * 2; /* guessed size */

	while( true ){
		query.reset( new char [size] );

		va_start( va, format );
		int real = vsnprintf(
			&query[0],size,
			format.c_str(), va );
		va_end( va );

		if( real >= size )
			size += abs( real - size + 1);
		else
			break;
	}

	return mysql_query(
		get_db(), query.get() );
}

string escape(const string &str){
	static MYSQL *mysql = mysql_init( NULL );
	char escaped[1024];

	mysql_real_escape_string(
		mysql, escaped, str.c_str(), str.length() );

	return escaped;
}

}
