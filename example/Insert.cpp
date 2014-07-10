#include "stdafx.h"

#include "Sqb.h"

using namespace std;

int main(int argc,char **argv){
	Sqb *query = Sqb::from("account")
		->create();

	(*query2)["id"] = "pjc";
	(*query2)["pw"] = "0247";

	printf("%s\n", query->save().c_str());

	return 0;
}
