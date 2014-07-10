#include "Sqb.h"

using namespace std;

Sqb::Sqb() :
	nLimit(0),
	queryType(0){
}
Sqb::~Sqb(){
}

void Sqb::setTable(const string &_table){
	table = _table;
}
void Sqb::setLimit(int _limit){
	nLimit = _limit;
}

void Sqb::addResultColumn(const string &col){
	results.push_back( col );
}
void Sqb::addCondition(
	const string &col, const string &op, const string &value){

	conditions.push_back(
		col + " " + op + " " + value );
}
void Sqb::addCondition(
	const string &query){

	conditions.push_back( query );
}
