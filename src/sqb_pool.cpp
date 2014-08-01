#include "Sqb.h"

#include <thread>
#include <concurrent_queue.h>

#include <mysql.h>

using namespace std;
using namespace chrono;
using namespace concurrency;

namespace SQB{

concurrent_queue<MYSQL*> conns;
_declspec(thread) MYSQL *local_db = nullptr;

bool initPool(){
	string host = get_config("host");
	string user = get_config("user");
	string passwd = get_config("password");
	string db = get_config("db");
	int port;
	int pool_size;

	sscanf(
		get_config("pool_size").c_str(), "%d", &pool_size );
	sscanf(
		get_config("port").c_str(), "%d", &port );

	for(int i=0;i<pool_size;i++){
		MYSQL *mysql = mysql_init(NULL);

		if( mysql_real_connect(
			mysql,
			host.c_str(), user.c_str(), passwd.c_str(), 
			db.c_str(), port, NULL, NULL) == 0 ){
			
			mysql_close( mysql );
			printf("failed to connect %s\n", host.c_str());
			return false;
		}

		mysql_autocommit( mysql, false );

		conns.push( mysql );
	}

	return true;
}
void quitPool(){
	while( !conns.empty() ){
		MYSQL *mysql;
	
		if( conns.try_pop( mysql ) )
			mysql_close( mysql );
	}
}

void return_db(MYSQL *mysql){
	local_db = NULL;
	conns.push( mysql );
}

bool try_begin(){
	MYSQL *mysql;

	if( conns.empty() )
		return false;
	if( !conns.try_pop( mysql ) )
		return false;
	else{
		local_db = mysql;
		mysql_query( mysql, "BEGIN" );

		return true;
	}
}
void begin(){
	MYSQL *mysql;

	while( true ){
		if( !conns.empty() ){
			if( conns.try_pop( mysql ) )
				break;
		}

		this_thread::sleep_for(
			milliseconds(10) );
	}

	local_db = mysql;
	mysql_query( mysql, "BEGIN" );
}
void commit(){
	MYSQL *mysql = local_db;

	mysql_query( mysql, "COMMIT" );

	return_db( mysql );
}
void rollback(){
	MYSQL *mysql = local_db;

	mysql_query( mysql, "ROLLBACK" );

	return_db( mysql );
}

MYSQL *get_db(){
	return local_db;
}

}