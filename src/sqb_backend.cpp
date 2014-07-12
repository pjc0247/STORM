#include "Sqb.h"

using namespace std;

namespace SQB{

Query::Query() :
	nLimit(0),
	queryType(0){
}
Query::~Query(){
}

void Query::setConnectionObject(MYSQL *_mysql){
	mysql = _mysql;
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
		col + " " + op + " \'" + value + "\'" );
}
void Query::addCondition(
	const string &query){

	conditions.push_back( query );
}

int Query::query(const std::string &query){
	int result = 
		mysql_query( mysql, query.c_str() );

	return result;
}
MYSQL_RES *Query::storeResult(){
	MYSQL_RES *result = 
		mysql_store_result( mysql );

	return result;
}
void Query::freeResult(MYSQL_RES *mysql){
	mysql_free_result( mysql );
}
vector<string> Query::fetchFields(MYSQL_RES *result){
	vector<string> fields;
	int nField =
		mysql_num_fields( result );

	for(int i=0;i<nField;i++) {
		MYSQL_FIELD *field =
			mysql_fetch_field( result );
		
		fields.push_back(
			field->name );
	}

	return fields;
}
vector<MYSQL_ROW> Query::fetchRows(MYSQL_RES *result){
	vector<MYSQL_ROW> rows;
	int nRow =
		mysql_num_rows( result );
	
	for(int i=0;i<nRow;i++) {
		MYSQL_ROW row = 
			fetchNextRow( result );

		rows.push_back( row );
	}

	return rows;
}
MYSQL_ROW Query::fetchNextRow(MYSQL_RES *result){
	MYSQL_ROW row =
		mysql_fetch_row (result );
	
	return row;
}

}
