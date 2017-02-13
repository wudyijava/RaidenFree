#include "DbHelper.h"
#include "Macro.h"

DbHelper* DbHelper::m_pInstance = NULL;

namespace DbConstants
{
	const char* DbFilePath = "cfg/raiden.db";
	const char* FileName = "raiden.db";
}

DbHelper::DbHelper()
{
	m_pDb = nullptr;
}

DbHelper::~DbHelper()
{
}

bool DbHelper::open()
{

	std::string destPath = CCFileUtils::getInstance()->getWritablePath() + DbConstants::FileName;

	Data data = FileUtils::getInstance()->getDataFromFile(DbConstants::DbFilePath);
	if (data.getSize() <= 0)
	{
		return false;
	}

	FILE *pFp = fopen(destPath.c_str(), "wb");
	fwrite(data.getBytes(), sizeof(char), data.getSize(), pFp);
	fclose(pFp);

	//打开数据库
	int ret = sqlite3_open(destPath.c_str(), &m_pDb);
	if (ret != SQLITE_OK) 
	{
		const char* err = sqlite3_errmsg(m_pDb);
		CCLOG("sqlite open error: %s", err);
		sqlite3_close(m_pDb);
		m_pDb = nullptr;
		return false;
	}

	return true;
}

bool DbHelper::close()
{
	//关闭并删除数据库
	if (m_pDb != nullptr)
	{
		sqlite3_close(m_pDb);
		m_pDb = nullptr;
	}

	FileUtils::getInstance()->removeFile(CCFileUtils::getInstance()->getWritablePath() + DbConstants::FileName);

	return true;
}

const std::string DbHelper::getValueByKey(const char* pTableName, const char* pResultColumn,
	const char* pWhereColumn, const char* pWhereValue, const char* defaultValue)
{
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = getRecordByKey(pTableName, pRecordSet, pWhereColumn, pWhereValue);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initGlobal : no record");
		return defaultValue;
	}

	DbKVPtr pRecord = pRecordSet->at(0);
	DbKVIter it = pRecord->find(pResultColumn);
	if (it != pRecord->end())
	{
		std::string value = it->second;
		delete pRecordSet;
		return value;
	}
	else
	{
		return defaultValue;
	}
}

const std::string DbHelper::getValueByKey(const char* pTableName, const char* pResultColumn,
	const char* pWhereColumn, int iWhereValue, const char* defaultValue)
{
	char pWhereValue[16] = { 0 };
	snprintf(pWhereValue, 15, "%d", iWhereValue);

	return getValueByKey(pTableName, pResultColumn,
		pWhereColumn, pWhereValue, defaultValue);
}

int DbHelper::getValueByKeyToInt(const char* pTableName, const char* pResultColumn,
	const char* pWhereColumn, const char* pWhereValue, int defaultValue)
{
	char pDefaultValue[16] = { 0 };
	snprintf(pDefaultValue, 15, "%d", defaultValue);

	return Utils::str2int(getValueByKey(pTableName, pResultColumn,
		pWhereColumn, pWhereValue, pDefaultValue));
}

int DbHelper::getValueByKeyToInt(const char* pTableName, const char* pResultColumn,
	const char* pWhereColumn, int iWhereValue, int defaultValue)
{
	char pDefaultValue[16] = { 0 };
	snprintf(pDefaultValue, 15, "%d", defaultValue);

	return Utils::str2int(getValueByKey(pTableName, pResultColumn,
		pWhereColumn, iWhereValue, pDefaultValue));
}

float DbHelper::getValueByKeyToFloat(const char* pTableName, const char* pResultColumn,
	const char* pWhereColumn, const char* pWhereValue, float defaultValue)
{
	char pDefaultValue[32] = { 0 };
	snprintf(pDefaultValue, 21, "%f", defaultValue);

	return Utils::str2float(getValueByKey(pTableName, pResultColumn,
		pWhereColumn, pWhereValue, pDefaultValue));
}

float DbHelper::getValueByKeyToFloat(const char* pTableName, const char* pResultColumn,
	const char* pWhereColumn, int iWhereValue, float defaultValue)
{
	char pDefaultValue[32] = { 0 };
	snprintf(pDefaultValue, 21, "%f", defaultValue);

	return Utils::str2float(getValueByKey(pTableName, pResultColumn,
		pWhereColumn, iWhereValue, pDefaultValue));
}


int DbHelper::getRecordById(const char* pTableName, DbRecordSetPtr pResultSet, 
	int id,
	const char* pOrderColumn, EDbSort eSort)
{
	char sz[16] = { 0 };
	snprintf(sz, 15, "%d", id);
	return getRecordByKey(pTableName, pResultSet, "id", sz, pOrderColumn, eSort);
}

int DbHelper::getRecordByKey(const char* pTableName, DbRecordSetPtr pResultSet,
	const char* pWhereColumn, int iWhereValue,
	const char* pOrderColumn, EDbSort eSort)
{
	char pWhereValue[16] = { 0 };
	snprintf(pWhereValue, 15, "%d", iWhereValue);

	return getRecordByKey(pTableName, pResultSet,
		pWhereColumn, pWhereValue,
		pOrderColumn, eSort);
}

int DbHelper::getRecordByKey(const char* pTableName, DbRecordSetPtr pResultSet,
	const char* pWhereColumn, const char* pWhereValue,
	const char* pOrderColumn, EDbSort eSort)
{
	int max = 1023;
	int len = 0;
	char sql[1024] = { 0 };

	len += snprintf(sql + len, max - len, "select * from %s where 1 = 1 ", pTableName);
	if (pWhereColumn != NULL && pWhereValue != NULL)
	{
		len += snprintf(sql + len, max - len, " and %s = \'%s\' ", pWhereColumn, pWhereValue);
	}

	if (pOrderColumn != NULL && (eSort == EDbSort::ASC || eSort == EDbSort::DESC))
	{
		if (EDbSort::ASC == eSort)
		{
			len += snprintf(sql + len, max - len, " order by %s asc ", pOrderColumn);
		}
		else
		{
			len += snprintf(sql + len, max - len, " order by %s desc ", pOrderColumn);
		}
	}

	return getRecordBySql(sql, pResultSet);
}


int DbHelper::getRecord(const char* pTableName, DbRecordSetPtr pResultSet, DbKVPtr pWhere, DbKVPtr pOrder)
{
	int max = 1023;
	char sql[1024] = { 0 };
	int len = 0;
	len += snprintf(sql, max, "select * from %s where 1 = 1 ", pTableName);

	if (pWhere != NULL)
	{
		DbKV::iterator it = pWhere->begin();
		for (; it != pWhere->end(); it++)
		{
			len += snprintf(sql + len, max - len, " and %s = '%s' ", it->first.c_str(), it->second.c_str());
		}
	}

	if (pOrder != NULL)
	{
		len += snprintf(sql + len, max - len, " order by 1 ");
		DbKV::iterator it2 = pOrder->begin();
		for (; it2 != pOrder->end(); it2++)
		{
			len += snprintf(sql + len, max - len, " , %s %s ", it2->first.c_str(), it2->second.c_str());
		}
	}

	return getRecordBySql(sql, pResultSet);
}

void DbHelper::freeResultSet(DbRecordSetPtr pResultSet)
{
	if (!pResultSet)
	{
		return;
	}

	for (unsigned i = 0; i < pResultSet->size(); i++)
	{
		CC_SAFE_DELETE(pResultSet->at(i));
	}

	CC_SAFE_DELETE(pResultSet);
}

int DbHelper::getRecordBySql(const char* sql, DbRecordSetPtr pResultSet)
{
	char** table; // 查询结果
	int row, col;     // 行数、列数

	int ret = sqlite3_get_table(m_pDb, sql, &table, &row, &col, nullptr);
	if (ret != SQLITE_OK)
	{
		CCLOG("SQL: %s", sql);
		DEBUG_LOG("sqlite open error: %s", sqlite3_errmsg(m_pDb));
		return -1;
	}
	CCLOG("RLT: %d\tSQL: %s", row, sql);

	if (0 == row)
	{
		sqlite3_free_table(table);
		return 0;
	}

	// 第0行（0 ~ c-1），为字段名
	// 第1行（c ~ 2*c-1），第一条记录
	// ......
	std::vector<std::string> colName;
	for (int j = 0; j < col; j++)
	{
		std::string s(table[j]);
		transform(s.begin(), s.end(), s.begin(), toupper);
		colName.push_back(s);
	}

	for (int i = 1; i <= row; i++)
	{
		DbKVPtr row = new DbKV;

		for (int j = 0; j < col; j++)
		{
			const char* value = table[i * col + j];
			if (!value)
			{
				value = "";
			}

			row->insert(DbKV::value_type(colName.at(j), value));
		}

		pResultSet->push_back(row);
	}

	// 记得是否查询表
	sqlite3_free_table(table);

	return row;
}
