#include "storm.h"

using namespace std;

namespace ORM{

string join(vector<string> vec, string delim){
	string result;

	if( vec.empty() )
		return "";

	for(auto it=vec.begin();it!=vec.end()-1;++it){
		result +=
			(*it) + delim;
	}

	return result +
		(*vec.rbegin());
}

string Query::build_result_columns(){
	if( results.empty() )
		return "*";
	else
		return join( results, "," );
}
string Query::build_conditions(){
	string query = "WHERE ";

	return query +
		join( conditions, " AND " );
}
string Query::build_field_keys(){
	string keys;
	auto it = fields.begin();

	while( it != fields.end() ){
		keys += (*it).first;

		if( ++it != fields.end() )
			keys += ",";
	}

	return keys;
}
string Query::build_field_values(){
	string values;
	auto it = fields.begin();

	while( it != fields.end() ){
		values += "\'" + 
			escape((*it).second) + "\'";

		if( ++it != fields.end() )
			values += ",";
	}

	return values;
}
string Query::build_changes(){
	string changes;

	if( dirty_fields.empty() )
		return "";

	for(auto field_name : dirty_fields){
		changes +=
			field_name + "=\'" +
			escape(get(field_name)) + "\',";
	}
	changes.pop_back();

	return changes;
}

string Query::build_from(){
	string query = "FROM ";

	query += table;
	
	return query;
}
string Query::build_into(){
	string query = "INTO ";

	query += table;
	
	return query;
}
string Query::build_limit(){
	string query = "LIMIT ";
	char buf[12];

	if( n_limit == 0 )
		return "";

	sprintf( buf, "%d", n_limit );
	query += string(buf);
	
	return query;
}

string Query::build_select(){
	string query = "SELECT ";
	query +=
		build_result_columns() + " " +
		build_from() + " " +
		build_conditions() + " " +
		build_limit();

	return query;
}
string Query::build_update(){
	string query = "UPDATE ";

	query +=
		table + " SET " +
		build_changes() + " " +
		build_conditions();
	return query;
}
string Query::build_delete(){
	string query = "DELETE ";

	query +=
		build_from() + " " +
		build_conditions();
	return query;
}
string Query::build_insert(){
	string query = "INSERT ";
	query +=
		build_into() + " (" +
		build_field_keys() + ") VALUES (" +
		build_field_values() +")";

	return query;
}

string Query::build(){
	switch( query_type ){
	case eSELECT:
		return build_select();
	case eUPDATE:
		return build_update();
	case eDELETE:
		return build_delete();
	case eINSERT:
		return build_insert();
	}
}

}
