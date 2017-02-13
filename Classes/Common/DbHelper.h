#pragma once
#include "cocos2d.h"
#include "sqlite3.h"
//#include "external/sqlite3/include/sqlite3.h"

USING_NS_CC;

typedef std::map < std::string, std::string > DbKV;
typedef std::map < std::string, std::string >* DbKVPtr;
typedef std::map < std::string, std::string >::iterator DbKVIter;

typedef std::vector<DbKVPtr> DbRecordSet;
typedef std::vector<DbKVPtr>* DbRecordSetPtr;

enum EDbSort
{
	ASC  = 0,
	DESC = 1
};

class DbHelper
{
public:
	static DbHelper* getInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new DbHelper();
		}

		return m_pInstance;
	}


	bool open();
	bool close();

	//根据某一个字段的值获取一条记录中的某个字段的值
	const std::string getValueByKey(const char* pTableName, const char* pResultColumn,
		const char* pWhereColumn = NULL, const char* pWhereValue = NULL, const char* defaultValue = NULL);
	const std::string getValueByKey(const char* pTableName, const char* pResultColumn,
		const char* pWhereColumn = NULL, int iWhereValue = -1, const char* defaultValue = NULL);

	int getValueByKeyToInt(const char* pTableName, const char* pResultColumn,
		const char* pWhereColumn = NULL, const char* pWhereValue = NULL, int defaultValue = 0);
	int getValueByKeyToInt(const char* pTableName, const char* pResultColumn,
		const char* pWhereColumn = NULL, int iWhereValue = -1, int defaultValue = 0);

	float getValueByKeyToFloat(const char* pTableName, const char* pResultColumn,
		const char* pWhereColumn = NULL, const char* pWhereValue = NULL, float defaultValue = 0.0f);
	float getValueByKeyToFloat(const char* pTableName, const char* pResultColumn,
		const char* pWhereColumn = NULL, int iWhereValue = -1, float defaultValue = 0.0f);

	int getRecordByKey(const char* pTableName, DbRecordSetPtr pResultSet,
		const char* pWhereColumn = NULL, int iWhereValue = -1,
		const char* pOrderColumn = NULL, EDbSort eSort = EDbSort::ASC);

	int getRecordByKey(const char* pTableName, DbRecordSetPtr pResultSet,
		const char* pWhereColumn = NULL, const char* pWhereValue = NULL,
		const char* pOrderColumn = NULL, EDbSort eSort = EDbSort::ASC);

	int getRecordById(const char* pTableName, DbRecordSetPtr pResultSet,
		int id,
		const char* orderColumn = NULL, EDbSort eSort = EDbSort::ASC);

	int getRecord(const char* pTableName, DbRecordSetPtr pResultSet,
		DbKVPtr pWhere = NULL, DbKVPtr pOrder = NULL);

	int getRecordBySql(const char* sql, DbRecordSetPtr pResultSet);

	static void freeResultSet(DbRecordSetPtr pResultSet);

protected:
	DbHelper();
	virtual ~DbHelper();

protected:
	static DbHelper* m_pInstance;

	sqlite3* m_pDb;
};