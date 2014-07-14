#include "Sqb.h"

using namespace std;

namespace SQB{

Query::Query() :
	nLimit(0),
	queryType(0){
}
Query::~Query(){
}

void Query::setNoDirt(const string &key, const string &value){
	fields[key] = value;
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
		col + " " + op + " \'" +
		escape(value) + "\'" );
}
void Query::addCondition(
	const string &query){

	conditions.push_back( query );
}

int Query::query(const std::string &query){
	int result = mysql_query(
		mysql, query.c_str() );

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
		mysql_fetch_row( result );
	
	return row;
}

void Query::dirtField(const string &fieldName){
	dirtyFields.push_back( fieldName );
}
void Query::cleanDirtyFields(){
	dirtyFields.clear();
}

string Query::findSingleValue(){
	string sql = buildSelect();

	if( query( sql ))
		return "";

	MYSQL_RES *result = storeResult();
	if( result == NULL )
		return "";

	MYSQL_ROW row =
		fetchNextRow( result );
	if( row == NULL )
		return "";
	
	mysql_free_result( result );

	return row[0];
}
Query *Query::findSingleRecord(){
	string sql = buildSelect();

	if( query( sql ))
		return NULL;

	MYSQL_RES *result = storeResult();
	if( result == NULL )
		return NULL;

	MYSQL_ROW row =
		fetchNextRow( result );
	if( row == NULL )
		return NULL;

	SQB::Query *obj = SQB::from( table );
	auto fields =
		fetchFields( result );

	for(int i=0;i<fields.size();i++)
		obj->setNoDirt( fields[i], row[i] );

	/* 나중에 이 레코드를 식별하기 위해 */
	obj->where("id", obj->get("id") );
	obj->setQueryType(
		QueryType::eUPDATE );
	
	mysql_free_result( result );

	return obj;
}
vector<Query*> Query::findRecords(){
	vector<Query*> results;
	string sql = buildSelect();

	if( query( sql ))
		return results;

	MYSQL_RES *result = storeResult();
	if( result == NULL )
		return results;

	auto fields =
		fetchFields( result );
	auto rows =
		fetchRows( result );

	for(int i=0;i<rows.size();i++){
		SQB::Query *obj = SQB::from( table );

		for(int j=0;j<fields.size();j++)
			obj->setNoDirt( fields[j], rows[i][j] );

		/* 나중에 이 레코드를 식별하기 위해 */
		obj->where("id", obj->get("id") );
		obj->setQueryType(
			QueryType::eUPDATE );

		results.push_back( obj );
	}
	
	mysql_free_result( result );

	return results;
}
bool Query::updateRecords(){
	string sql = buildUpdate();

	if( query( sql ))
		return false;
	return true;
}
bool Query::insertRecord(){
	string sql = buildInsert();

	if( query( sql ))
		return false;
	return true;
}
bool Query::removeRecords(){
	string sql = buildDelete();

	if( query( sql ))
		return false;
	return true;
}

}
