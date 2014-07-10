#ifndef _SQB_H
#define _SQB_H

#include <string>
#include <vector>

/*
	SQB / Simple Query Builder
*/
class Sqb{
public:
	static Sqb *from(const std::string &table);

	Sqb *where(const std::string &col, const std::string &value);
	Sqb *where_equal(const std::string &col, const std::string &value);
	Sqb *where_not_equal(const std::string &col, const std::string &value);
	Sqb *where_like(const std::string &col, const std::string &value);
	Sqb *where_not_like(const std::string &col, const std::string &value);
	Sqb *where_gt(const std::string &col, const std::string &value);
	Sqb *where_gte(const std::string &col, const std::string &value);
	Sqb *where_lt(const std::string &col, const std::string &value);
	Sqb *where_lte(const std::string &col, const std::string &value);
	Sqb *where_raw(const std::string &query);

	Sqb *select(const std::string &col);
	Sqb *select(int count, ...);

	Sqb *limit(int limit);

	std::string find_one();
	std::string find_many();

	std::string build();
protected:
	Sqb();
	virtual ~Sqb();

	void setTable(const std::string &table);
	void setLimit(int limit);

	void addResultColumn(const std::string &col);
	void addCondition(
		const std::string &col, const std::string &op, const std::string &value);
	void addCondition(const std::string &condition);

	std::string buildResultColumns();
	std::string buildConditions();
	std::string buildFrom();
	std::string buildLimit();
	
	std::string buildSelect();
	std::string buildUpdate();
	std::string buildDelete();
	std::string buildInsert();

protected:
	enum QueryType{
		SELECT=1,
		UPDATE,
		DELETE,
		INSERT
	};

protected:
	int queryType;

	std::string table;
	std::vector<std::string> conditions;
	std::vector<std::string> results;

	int nLimit;
};


#endif //_SQB_H
