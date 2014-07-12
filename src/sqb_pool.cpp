#include "Sqb.h"

#include <thread>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>

#include <mysql.h>

using namespace std;
using namespace chrono;
using namespace concurrency;

namespace SQB{

concurrent_queue<MYSQL*> conns;
concurrent_unordered_map<thread::id, MYSQL*> tls;

bool initPool(){
	string host = getConfig("host");
	string user = getConfig("user");
	string passwd = getConfig("password");
	string db = getConfig("db");
	int port;

	sscanf(
		getConfig("port").c_str(), "%d", &port );

	for(int i=0;i<SQB::POOL_SIZE;i++){
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

MYSQL *localGetDB(){
	return tls[ this_thread::get_id() ];
}
void localSetDB(MYSQL *mysql){
	tls[ this_thread::get_id() ] = mysql;
}

bool tryBegin(){
	MYSQL *mysql;

	if( conns.empty() )
		return false;
	if( !conns.try_pop( mysql ) )
		return false;
	else{
		localSetDB( mysql );
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

	localSetDB( mysql );
	mysql_query( mysql, "BEGIN" );
}
void commit(){
	MYSQL *mysql = localGetDB();

	mysql_query( mysql, "COMMIT" );

	conns.push( mysql );
}
void rollback(){
	MYSQL *mysql = localGetDB();

	mysql_query( mysql, "ROLLBACK" );

	conns.push( mysql );
}

MYSQL *getDB(){
	auto pair = tls.find( this_thread::get_id() );

	if( pair == tls.end() )
		return NULL;
	else
		return pair->second;
}

}
