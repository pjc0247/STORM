STORM
===

idiorm짝퉁

__테이블마다 id라는 이름의 UNIQUE 필드가 있어야 사용 가능__

Configuration
----
```C++
ORM::configure("host", "localhost");
ORM::configure("user", "pjc");
ORM::configure("password", "1234");
ORM::configure("db", "dbname");

ORM::init();

ORM::quit();
```

SELECT Query
----
```C++
auto query = ORM::from("test");

auto result = query
  ->where("id", "foo")
  ->where("nick", "bar")
  ->select("level")
  ->find_one();
cout<<(*result)["level"];
  
auto = results = query
  ->where_raw("nick=foo")
  ->limit(5)
  ->find_many();
for(auto result : results)
  cout<<(*result)["level"]<<endl;
```

INSERT Query
----

```C++
auto query =
  ORM::from("test")->create();

query->set("id", "pjc");
query->set("nick", "anz");

(*query)["another"] = "method";

query->save();
```

UPDATE Query
----

```C++
auto query =
  ORM::from("test")
    ->where("id", "pjc0247")
    ->find_one();

(*query)["nickname"] = "anz";

query->save();
```

DELETE Query
----

```C++
auto query =
  ORM::from("test")
    ->where("id", "pjc0247")
    ->find_one();
query->remove();

auto query = 
  ORM::from("test")
    ->where("level", "1")
    ->remove();
```

TRANSACTION
----
```C++
ORM::begin();
  // do some work
ORM::commit();

if( ORM::try_begin() ){
    // do some work
  ORM::rollback();
}
```
