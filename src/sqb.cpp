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
int raw_query(const std::string &query){
	return mysql_query(
		getDB(), query.c_str() );
}

}
