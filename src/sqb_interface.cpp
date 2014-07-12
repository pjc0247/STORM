#include "Sqb.h"

#include <cstdarg>

using namespace std;

namespace SQB{

Query *from(const string &table){
	Query *Query = new SQB::Query();
		
		Query->setTable( table );
	return Query;
}

Query *Query::where(const string &col, const string &value){
	addCondition( col, "=", value );
	return this;
}
Query *Query::where_equal(const string &col, const string &value){
	addCondition( col, "=", value );
	return this;
}
Query *Query::where_not_equal(const string &col, const string &value){
	addCondition( col, "!=", value );
	return this;
}
Query *Query::where_like(const string &col, const string &value){
	addCondition( col, "LIKE", value );
	return this;
}
Query *Query::where_not_like(const string &col, const string &value){
	addCondition( col, "NOT LIKE", value );
	return this;
}
Query *Query::where_gt(const string &col, const string &value){
	addCondition( col, ">", value );
	return this;
}
Query *Query::where_gte(const string &col, const string &value){
	addCondition( col, ">=", value );
	return this;
}
Query *Query::where_lt(const string &col, const string &value){
	addCondition( col, "<", value );
	return this;
}
Query *Query::where_lte(const string &col, const string &value){
	addCondition( col, "<=", value );
	return this;
}

Query *Query::where_raw(const string &query){
	addCondition( query );
	return this;
}

Query *Query::select(const string &col){
	addResultColumn( col );
	return this;
}
Query *Query::select(int count, ...){
	va_list va;

	va_start( va, count );
		for(int i=0;i<count;i++){
			addResultColumn(
				va_arg( va, const string ));
		}
	va_end( va );
	return this;
}

Query *Query::limit(int limit){
	setLimit( limit );
	return this;
}

Query *Query::create(){
	Query *Query = SQB::from( table );
	Query->setQueryType(
		QueryType::eINSERT );

	return Query;
}

Query *Query::find_one(){
	setLimit( 1 );
	setConnectionObject( getDB() );

	string sql = buildSelect();

	if( query( sql.c_str() ))
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
		(*obj)[ fields[i] ] = row[i];
	
	mysql_free_result( result );

	return obj;
}
vector<Query*> Query::find_many(){
	setConnectionObject( getDB() );

	vector<Query*> results;
	string sql = buildSelect();

	if( query( sql.c_str() ))
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
			(*obj)[ fields[j] ] = rows[i][j];

		results.push_back( obj );
	}
	
	mysql_free_result( result );

	return results;
}
string Query::save(){
	return buildInsert();
}

void Query::set(const string &key, const string &value){
	fields[key] = value;
}
string &Query::get(const string &key){
	return fields[key];
}
string &Query::operator[](const std::string &key){
	return fields[key];
}

}
