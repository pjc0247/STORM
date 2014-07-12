SQB
===

c++ simple query builder


idiorm짝퉁

Configuration
----
```C++
SQB::configure("host", "localhost");
SQB::configure("user", "pjc");
SQB::configure("password", "1234");
SQB::configure("db", "dbname");

SQB::init();

SQB::quit();
```

SELECT Query
----
```C++
SQB::Query *query = SQB::from("test");

auto result = query
  ->where("id", "foo")
  ->where("nick", "bar")
  ->select("level")
  ->find_one();
cout<<(*result)["level"];
  
auto = results = query
  ->where_raw("nick=foo")
  ->limit(5)
  ->find_mamy();
for(auto result : results)
  cout<<(*result)["level"]<<endl;
```

INSERT Query
----
```C++
SQB::Query *query =
  SQB::from("test")->create();

query->set("id", "pjc");
query->set("nick", "anz");

(*query)["another"] = "method";

query->save();
```

UPDATE Query
```C++
SQB::Query *query =
  SQB::from("test")
    ->where("id", "pjc0247")
    ->find_one();

(*query)["nickname"] = "anz";

query->save();
```

DELETE Query
```C++
SQB::Query *query =
  SQB::from("test")
    ->where("id", "pjc0247")
    ->find_one();

query->remove();
```

TRANSACTION
----
```C++
SQB::begin();
  // do some work
SQB::commit();

if( SQB::tryBegin() ){
    // do some work
  SQB::rollback();
}
```
