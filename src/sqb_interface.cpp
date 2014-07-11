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
		QueryType::INSERT );

	return Query;
}

string Query::find_one(){
	setLimit( 1 );
	return buildSelect();
}
string Query::find_many(){
	return buildSelect();
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
