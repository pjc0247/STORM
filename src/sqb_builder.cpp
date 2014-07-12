#include "Sqb.h"

using namespace std;

namespace SQB{

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

string Query::buildResultColumns(){
	if( results.empty() )
		return "*";
	else
		return join( results, "," );
}
string Query::buildConditions(){
	string query = "WHERE ";

	return query +
		join( conditions, " AND " );
}
string Query::buildFieldKeys(){
	string keys;
	auto it = fields.begin();

	while( it != fields.end() ){
		keys += (*it).first;

		if( ++it != fields.end() )
			keys += ",";
	}

	return keys;
}
string Query::buildFieldValues(){
	string values;
	auto it = fields.begin();

	while( it != fields.end() ){
		values += "\'" + (*it).second + "\'";

		if( ++it != fields.end() )
			values += ",";
	}

	return values;
}
string Query::buildChanges(){
	string changes;

	if( dirtyFields.empty() )
		return "";

	for(auto fieldName : dirtyFields){
		changes +=
			fieldName + "=\'" +
			get( fieldName ) + "\',";
	}
	changes.pop_back();

	return changes;
}

string Query::buildFrom(){
	string query = "FROM ";

	query += table;
	
	return query;
}
string Query::buildInto(){
	string query = "INTO ";

	query += table;
	
	return query;
}
string Query::buildLimit(){
	string query = "LIMIT ";
	char buf[12];

	if( nLimit == 0 )
		return "";

	sprintf( buf, "%d", nLimit );
	query += string(buf);
	
	return query;
}

string Query::buildSelect(){
	string query = "SELECT ";
	query +=
		buildResultColumns() + " " +
		buildFrom() + " " +
		buildConditions() + " " +
		buildLimit();

	return query;
}
string Query::buildUpdate(){
	string query = "UPDATE ";

	query +=
		table + " SET " +
		buildChanges() + " " +
		buildConditions();
	return query;
}
string Query::buildDelete(){
	string query = "DELETE ";

	query +=
		buildFrom() + " " +
		buildConditions();
	return query;
}
string Query::buildInsert(){
	string query = "INSERT ";
	query +=
		buildInto() + " (" +
		buildFieldKeys() + ") VALUES (" +
		buildFieldValues() +")";

	return query;
}

string Query::build(){
	switch( queryType ){
	case eSELECT:
		return buildSelect();
	case eUPDATE:
		return buildUpdate();
	case eDELETE:
		return buildDelete();
	case eINSERT:
		return buildInsert();
	}
}

}
