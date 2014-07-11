#include "Sqb.h"

using namespace std;

namespace SQB{

Query::Query() :
	nLimit(0),
	queryType(0){
}
Query::~Query(){
}

void Query::setQueryType(int _queryType){
	queryType = _queryType;
}
void Query::setTable(const string &_table){
	table = _table;
}
void Query::setLimit(int _limit){
	nLimit = _limit;
}

void Query::addResultColumn(const string &col){
	results.push_back( col );
}
void Query::addCondition(
	const string &col, const string &op, const string &value){

	conditions.push_back(
		col + " " + op + " " + value );
}
void Query::addCondition(
	const string &query){

	conditions.push_back( query );
}

}
