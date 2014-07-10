#include <stdio.h>
#include "Sqb.h"


int main(int argc,char **argv){
	Sqb *query = Sqb::from("account");
	
	auto result = query
		->where("id", "pjc0247")
		->where_not_equal("nick", "pmnxis")
		->select(3, string("level"), string("A"), string("B"))
		->find_one();

	printf("%s\n", result.c_str());
	return 0;
}
 
 
