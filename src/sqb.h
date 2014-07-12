#ifndef _SQB_H
#define _SQB_H

#include <mysql.h>

#include <string>
#include <vector>
#include <map>

/*
	SQB / Simple MYSQL Query Builder
*/
namespace SQB{
	class Query;

	/* size of MYSQL connection pool */
	const int POOL_SIZE = 10;

	bool init();
	void quit();

	/* ERROR API */
	std::string getLastErrorMessage();

	/* CONFIGURATION API */
	void configure(
		const std::string &key, const std::string &value);
	std::string &getConfig(
		const std::string &key);

	/* TRANSACTION API */
	bool tryBegin();
	void begin();
	void commit();
	void rollback();

	/* RAW API */
	MYSQL *getDB();
	int raw_query(const std::string &query);

	/* SELECT DB */
	Query *from(const std::string &table);

	class Query{
		friend Query *from(const std::string &table);

	public:
		Query *where(const std::string &col, const std::string &value);
		Query *where_equal(const std::string &col, const std::string &value);
		Query *where_not_equal(const std::string &col, const std::string &value);
		Query *where_like(const std::string &col, const std::string &value);
		Query *where_not_like(const std::string &col, const std::string &value);
		Query *where_gt(const std::string &col, const std::string &value);
		Query *where_gte(const std::string &col, const std::string &value);
		Query *where_lt(const std::string &col, const std::string &value);
		Query *where_lte(const std::string &col, const std::string &value);
		Query *where_raw(const std::string &query);

		Query *select(const std::string &col);
		Query *select(int count, ...);

		Query *limit(int limit);

		void set(const std::string &key, const std::string &value);
		std::string &get(const std::string &key);

		Query *find_one();
		std::vector<Query*> find_many();

		Query *create();
		bool save();
		bool remove();

		std::string build();

	public:
		std::string &operator[](const std::string &key);

	protected:
		Query();
		virtual ~Query();

		void setConnectionObject(MYSQL *mysql);

		void setNoDirt(const std::string &key, const std::string &value);

		void setQueryType(int queryType);
		void setTable(const std::string &table);
		void setLimit(int limit);

		void addResultColumn(const std::string &col);
		void addCondition(
			const std::string &col, const std::string &op, const std::string &value);
		void addCondition(const std::string &condition);

		std::string buildResultColumns();
		std::string buildConditions();
		std::string buildFieldKeys();
		std::string buildFieldValues();
		std::string buildChanges();

		std::string buildFrom();
		std::string buildInto();

		std::string buildLimit();
	
		std::string buildSelect();
		std::string buildUpdate();
		std::string buildDelete();
		std::string buildInsert();

		void dirtField(const std::string &fieldName);
		void cleanDirtyFields();

		/* SELECT, UPDATE, INSERT, DELETE backends */
		Query *findSingleRecord();
		std::vector<Query*> findRecords();
		bool updateRecords();
		bool insertRecord();
		bool removeRecords();

		/* MYSQL Wrapping */
		int query(const std::string &query);
		MYSQL_RES *storeResult();
		void freeResult(MYSQL_RES *mysql);
		std::vector<std::string> fetchFields(MYSQL_RES *result);
		std::vector<MYSQL_ROW> fetchRows(MYSQL_RES *result);
		MYSQL_ROW fetchNextRow(MYSQL_RES *result);

	protected:
		enum QueryType{
			eSELECT=1,
			eUPDATE,
			eDELETE,
			eINSERT
		};

	protected:
		int queryType;

		std::string table;
		std::vector<std::string> conditions;
		std::vector<std::string> results;
		std::map<std::string,std::string> fields;
		std::vector<std::string> dirtyFields;

		int nLimit;

		MYSQL *mysql;
	};
};




#endif //_SQB_H
