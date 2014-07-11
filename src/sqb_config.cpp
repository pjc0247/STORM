#include "Sqb.h"

using namespace std;

namespace SQB{

map<string,string> configs;

void configure(
	const string &key, const string &value){

	configs[key] = value;
}
void configureNoOverride(
	const string &key, const string &value){

	auto pair = configs.find( key );
	if( pair == configs.end() )
		configs[key] = value;
}
string &getConfig(
	const string &key){

	return configs[key];
}

bool initConfig(){
	configureNoOverride("host", "127.0.0.1");
	configureNoOverride("port", "3306");

	return true;
}

}
