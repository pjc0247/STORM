#include "Sqb.h"

using namespace std;

namespace SQB{

string join(vector<string> vec, string delim){
	string result;

	for(auto it=vec.begin();it!=vec.end()-1;++it){
		result +=
			(*it) + delim;
	}

	return result +
		(*vec.rbegin());
}

string Query::buildResultColumns(){
	string query = "WHERE ";
	
	return query +
		join( results, "," );
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
	return "";
}
string Query::buildDelete(){
	return "";
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
	case SELECT:
		return buildSelect();
	case UPDATE:
		return buildUpdate();
	case DELETE:
		return buildDelete();
	case INSERT:
		return buildInsert();
	}
}

}
