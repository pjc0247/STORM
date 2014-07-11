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

}
