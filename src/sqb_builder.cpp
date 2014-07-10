#include "Sqb.h"

using namespace std;

string join(vector<string> vec, string delim){
	string result;

	for(auto it=vec.begin();it!=vec.end()-1;++it){
		result +=
			(*it) + delim;
	}

	return result +
		(*vec.rbegin());
}

string Sqb::buildResultColumns(){
	string query = "WHERE ";
	
	return query +
		join( results, "," );
}
string Sqb::buildConditions(){
	string query = "WHERE ";

	return query +
		join( conditions, " AND " );
}
string Sqb::buildFrom(){
	string query = "FROM ";

	query += table;
	
	return query;
}
string Sqb::buildLimit(){
	string query = "LIMIT ";
	char buf[12];

	sprintf( buf, "%d", nLimit );
	query += string(buf);
	
	return query;
}

string Sqb::buildSelect(){
	string query = "SELECT ";

	query +=
		buildResultColumns() + " " +
		buildFrom() + " " +
		buildConditions() + " " +
		buildLimit();
	return query;
}
string Sqb::buildUpdate(){
	return "";
}
string Sqb::buildDelete(){
	return "";
}
string Sqb::buildInsert(){
	return "";
}

string Sqb::build(){
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
