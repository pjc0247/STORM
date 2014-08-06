#include "storm.h"

#include <cstdarg>

using namespace std;

namespace ORM{

Query *from(const string &table){
	Query *Query = new ORM::Query();
		
		Query->set_table( table );

	return Query;
}

Query *Query::where(const string &col, const string &value){
	add_condition( col, "=", value );

	return this;
}
Query *Query::where_equal(const string &col, const string &value){
	add_condition( col, "=", value );

	return this;
}
Query *Query::where_not_equal(const string &col, const string &value){
	add_condition( col, "!=", value );

	return this;
}
Query *Query::where_like(const string &col, const string &value){
	add_condition( col, "LIKE", value );

	return this;
}
Query *Query::where_not_like(const string &col, const string &value){
	add_condition( col, "NOT LIKE", value );

	return this;
}
Query *Query::where_gt(const string &col, const string &value){
	add_condition( col, ">", value );

	return this;
}
Query *Query::where_gte(const string &col, const string &value){
	add_condition( col, ">=", value );

	return this;
}
Query *Query::where_lt(const string &col, const string &value){
	add_condition( col, "<", value );

	return this;
}
Query *Query::where_lte(const string &col, const string &value){
	add_condition( col, "<=", value );

	return this;
}

Query *Query::where_raw(const string &query){
	add_condition( query );

	return this;
}

Query *Query::select(const string &col){
	add_result_column( col );

	return this;
}
Query *Query::select(int count, ...){
	va_list va;

	va_start( va, count );
		for(int i=0;i<count;i++){
			add_result_column(
				va_arg( va, const string ));
		}
	va_end( va );

	return this;
}

Query *Query::limit(int limit){
	set_limit( limit );

	return this;
}

Query *Query::order_by_expr(const string &expr){
	add_order_by( expr, "" );

	return this;
}
Query *Query::order_by_asc(const string &col){
	add_order_by( col, "ASC" );

	return this;
}
Query *Query::order_by_desc(const string &col){
	add_order_by( col, "DESC" );

	return this;
}

Query *Query::create(){
	Query *Query = ORM::from( table );
	Query->set_query_type(
		QueryType::eINSERT );

	return Query;
}

Query *Query::find_one(){
	set_limit( 1 );
	set_connection_object( get_db() );

	return find_single_record();
}
vector<Query*> Query::find_many(){
	set_connection_object( get_db() );

	return find_records();
}

string Query::find_min(const string &col){
	set_connection_object( get_db() );

	results.clear();
	add_result_column(
		"MIN(" + col + ")" );

	return find_single_value();
}
string Query::find_max(const string &col){
	set_connection_object( get_db() );

	results.clear();
	add_result_column(
		"MAX(" + col + ")" );

	return find_single_value();
}
string Query::find_avg(const string &col){
	set_connection_object( get_db() );

	results.clear();
	add_result_column(
		"AVG(" + col + ")" );

	return find_single_value();
}
string Query::find_sum(const string &col){
	set_connection_object( get_db() );

	results.clear();
	add_result_column(
		"SUM(" + col + ")" );

	return find_single_value();
}

bool Query::remove(){
	set_connection_object( get_db() );

	return remove_records();
}
bool Query::save(){
	set_connection_object( get_db() );

	bool ret;

	/* UPDATE Query */
	if( query_type == QueryType::eUPDATE )
		ret = update_records();
	/* INSERT Query */
	else
		ret = insert_record();

	clean_dirty_fields();

	return ret;
}

void Query::set(const string &key, const string &value){
	dirt_field( key );
	fields[key] = value;
}
string &Query::get(const string &key){
	return fields[key];
}
string &Query::operator[](const std::string &key){
	/* operator[]로 접근된 값들은 수정 여부를 알 수 없으므로
	   무조건 dirt시킨다, 나중에 수정 */
	dirt_field( key );

	return fields[key];
}

}
