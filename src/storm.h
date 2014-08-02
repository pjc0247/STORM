#ifndef _STORM_H
#define _STORM_H

#include <mysql.h>

#include <string>
#include <vector>
#include <map>

namespace ORM{
	class Query;

	bool init();
	void quit();

	/* ERROR API */
	std::string get_last_error();

	/* CONFIGURATION API */
	void configure(
		const std::string &key, const std::string &value);
	std::string &get_config(
		const std::string &key);

	/* TRANSACTION API */
	bool try_begin();
	void begin();
	void commit();
	void rollback();

	/* RAW API */
	MYSQL *get_db();
	int raw_query(const std::string &query);

	/* SELECT TABLE */
	Query *from(const std::string &table);

	/* ESCAPE */
	std::string escape(const std::string &str);

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

		std::string find_min(const std::string &col);
		std::string find_max(const std::string &col);
		std::string find_avg(const std::string &col);
		std::string find_sum(const std::string &col);

		Query *create();
		bool save();
		bool remove();

		std::string build();

	public:
		std::string &operator[](const std::string &key);

	protected:
		Query();
		virtual ~Query();

		void set_connection_object(MYSQL *mysql);

		void set_with_no_dirt(const std::string &key, const std::string &value);

		void set_query_type(int query_type);
		void set_table(const std::string &table);
		void set_limit(int limit);

		void add_result_column(const std::string &col);
		void add_condition(
			const std::string &col, const std::string &op, const std::string &value);
		void add_condition(const std::string &condition);

		std::string build_result_columns();
		std::string build_conditions();
		std::string build_field_keys();
		std::string build_field_values();
		std::string build_changes();

		std::string build_from();
		std::string build_into();

		std::string build_limit();
	
		std::string build_select();
		std::string build_update();
		std::string build_delete();
		std::string build_insert();

		void dirt_field(const std::string &field_name);
		void clean_dirty_fields();

		/* SELECT, UPDATE, INSERT, DELETE backends */
		std::string find_single_value();
		Query *find_single_record();
		std::vector<Query*> find_records();
		bool update_records();
		bool insert_record();
		bool remove_records();

		/* MYSQL Wrapping */
		int query(const std::string &query);
		MYSQL_RES *store_result();
		void free_result(MYSQL_RES *mysql);
		std::vector<std::string> fetch_fields(MYSQL_RES *result);
		std::vector<MYSQL_ROW> fetch_rows(MYSQL_RES *result);
		MYSQL_ROW fetch_next_row(MYSQL_RES *result);

	protected:
		enum QueryType{
			eSELECT=1,
			eUPDATE,
			eDELETE,
			eINSERT
		};

	protected:
		int query_type;

		std::string table;
		std::vector<std::string> conditions;
		std::vector<std::string> results;
		std::map<std::string,std::string> fields;
		std::vector<std::string> dirty_fields;

		int n_limit;
		bool currupted;

		MYSQL *mysql;
	};
};

#endif //_STORM_H
