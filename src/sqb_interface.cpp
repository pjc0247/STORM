#include "Sqb.h"

#include <cstdarg>

using namespace std;

Sqb *Sqb::from(const string &table){
	Sqb *sqb = new Sqb();
		
		sqb->setTable( table );
	return sqb;
}

Sqb *Sqb::where(const string &col, const string &value){
	addCondition( col, "=", value );
	return this;
}
Sqb *Sqb::where_equal(const string &col, const string &value){
	addCondition( col, "=", value );
	return this;
}
Sqb *Sqb::where_not_equal(const string &col, const string &value){
	addCondition( col, "!=", value );
	return this;
}
Sqb *Sqb::where_like(const string &col, const string &value){
	addCondition( col, "LIKE", value );
	return this;
}
Sqb *Sqb::where_not_like(const string &col, const string &value){
	addCondition( col, "NOT LIKE", value );
	return this;
}
Sqb *Sqb::where_gt(const string &col, const string &value){
	addCondition( col, ">", value );
	return this;
}
Sqb *Sqb::where_gte(const string &col, const string &value){
	addCondition( col, ">=", value );
	return this;
}
Sqb *Sqb::where_lt(const string &col, const string &value){
	addCondition( col, "<", value );
	return this;
}
Sqb *Sqb::where_lte(const string &col, const string &value){
	addCondition( col, "<=", value );
	return this;
}

Sqb *Sqb::where_raw(const string &query){
	addCondition( query );
	return this;
}

Sqb *Sqb::select(const string &col){
	addResultColumn( col );
	return this;
}
Sqb *Sqb::select(int count, ...){
	va_list va;

	va_start( va, count );
		for(int i=0;i<count;i++){
			addResultColumn(
				va_arg( va, const string ));
		}
	va_end( va );
	return this;
}

Sqb *Sqb::limit(int limit){
	setLimit( limit );
	return this;
}

Sqb *Sqb::create(){
	Sqb *sqb = Sqb::from( table );
	sqb->setQueryType(
		QueryType::INSERT );

	return sqb;
}

string Sqb::find_one(){
	setLimit( 1 );
	return buildSelect();
}
string Sqb::find_many(){
	return buildSelect();
}
string Sqb::save(){
	return buildInsert();
}

void Sqb::set(const string &key, const string &value){
	fields[key] = value;
}
string &Sqb::get(const string &key){
	return fields[key];
}
string &Sqb::operator[](const std::string &key){
	return fields[key];
}
