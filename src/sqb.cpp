#include "Sqb.h"

using namespace std;

namespace SQB{

/* from sqb_config.cpp */
bool initConfig();

/* from sqb_pool.cpp */
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

string getLastErrorMessage(){
	MYSQL *mysql = getDB();

	return mysql_error( mysql );
}
int rawQuery(const std::string &query){
	return mysql_query(
		getDB(), query.c_str() );
}

string escape(const string &str){
	static MYSQL *mysql = mysql_init( NULL );
	char escaped[1024];

	mysql_real_escape_string(
		mysql, escaped, str.c_str(), str.length() );

	return escaped;
}

}
