/**
* create 2015-11
* ��ѯ�Ľӿ�
*/
#ifndef BMOBSDK_BMOBQUERYINTERFACE_H_
#define BMOBSDK_BMOBQUERYINTERFACE_H_

#include <iostream>
#include <string>
#include "cocos2d.h"

#include "../delegate/bmobdelegate.h"
#include "../jsoncpp/include/json.h"
#include "../util/bmobjsonutil.h"


using namespace std;
USING_NS_CC;


class BmobQueryInterafce:public cocos2d::Ref{
public:
	/**
	* ������������ͣ����忴�ĵ�
	*/
	enum class Type{
		IGNORE_CACHE,	
		CACHE_ONLY,
		NETWORK_ONLY,
		NETWORK_ELSE_CACHE,
		CACHE_ELSE_NETWORK,
		CACHE_THEN_NETWORK
	};

	
public:
	/**
	* ͨ��Ҫ�����ı�������һ��Query����
	*/
	BmobQueryInterafce(string tableName){
		m_tableName = tableName;
		_queryType = Type::IGNORE_CACHE;

		m_pFindDelegate = NULL;
		m_pGetDelegate = NULL;
		m_pCountDelegate = NULL;
		m_pStaticsDelegate = NULL;

		m_isHaving = false;
		m_groupCount = false;

		m_limit = 0;
		m_skip = 0;
	}

	

	/**
	* ��ѯ���ж���
	* @param delegate ��ѯ�ص��ӿ�
	*/
	virtual void findObjects(BmobFindDelegate* delegate) = 0;
	/**
	* ֱ��ͨ��������ѯ��Ӧ�����������
	* @param tableName ����
	* @param delegate ��ѯ�ص��ӿ�
	*/
	virtual void findObjects(string tableName,BmobFindDelegate* delegate) = 0;
	/**
	* ͨ��objectId ��ѯ��Ӧ������
	* @param objectid ��ѯ��objectId
	* @param delegate ��ѯ�Ļص��ӿ�
	*/
	virtual void getObject(string objectId,BmobGetDelegate* delegate) = 0;
	/**
	* ��ѯ����
	* @param delegate �ص��ӿ�
	* @param sign �Ƿ���Ҫ����������Ӧ�����ݱ�־ 
	*/
	virtual void count(BmobCountDelegate* delegate,bool sign = false) = 0;
	/**
	* ͳ�Ʋ���
	* @param delegate �ص��ӿ�
	*/
	virtual void findStatistics(BmobStaticsDelegate* delegate) = 0;

	/**
	* ��Ӳ�ѯ�����where����
	* @param seg �ֶ���
	* @param object ��Ӧ��ֵ
	* �磺��name="Harrier"��ӵ���ѯ��
	*/
	virtual void addWhereEqualTo(string seg,Ref *object) = 0;
	/**
	* ��Ӳ�ѯ�������where����
	* @param seg �ֶ���
	* @param object ��Ӧ��ֵ
	* �磺��name!="Harrier"��ӵ���ѯ��
	*/
	virtual void addWhereNotEqualTo(string seg,Ref *object) = 0;
	/**
	* 
	*/
	virtual void addWhereGreatorThan(string seg,Ref *object) = 0;
	/**
	* ���С��ĳ��ֵ��where����
	* @param seg �ֶ���
	* @param object ��Ӧ�ֶε�ֵ
	* �磺��age<30��ӵ���ѯ��
	*/
	virtual void addWhereLessThan(string seg,Ref *object) = 0;
	/**
	* ���С�ڵ���ĳ��ֵ��where����
	* @param seg �ֶ���
	* @param object ��Ӧ�ֶε�ֵ
	* �磺��age<=30��ӵ���ѯ��
	*/
	virtual void addWhereLessThanOrEqualTo(string seg,Ref *object) = 0;
	/**
	* ��Ӵ���ĳ��ֵ��where����
	* @param seg �ֶ���
	* @param object ��Ӧ��ֵ
	* �磺��age>20��ӵ���ѯ��
	*/
	virtual void addWhereGreaterThan(string seg,Ref *object) = 0;
	/**
	* ��Ӵ��ڵ���ĳ��ֵ��where����
	* @param seg �ֶ���
	* @param object ��Ӧ��ֵ
	* �磺��age>=20��ӵ���ѯ��
	*/
	virtual void addWhereGreaterThanOrEqualTo(string seg,Ref *object) = 0;
	/**
	* ��Ӱ���������ĳ��ֵ��where����
	* @param seg �ֶ���
	* @param array ��Ҫ��ѯ�ֶΰ�����ֵ
	*/
	virtual void addWhereContainedIn(string seg,Ref* array) = 0;
	/**
	* ��Ӳ�����������ĳ��ֵ��where����
	* @param seg �ֶ���
	* @param array ��Ҫ��ѯ�ֶβ�������ֵ
	*/
	virtual void addWhereNotContainedIn(string seg,Ref* array) = 0;
	/**
	* ��Ӱ�������������ֵ��where����
	* @param seg �ֶ���
	* @param array ��Ҫ��ѯ����������
	*/
	virtual void addWhereContainsAll(string seg,__Array* array) = 0;
	virtual void addWhereMatches() = 0;
	virtual void addWhereContains() = 0;
	/**
	* ��Ӳ�ѯĳ���ֶδ��ڵ�where����
	* @param column	��ѯ���ֶ���
	*/
	virtual void addWhereExists(string column) = 0;
	/**
	* ��Ӳ�ѯĳ���ֶβ����ڵ�where����
	* @param column ��ѯ���ֶ���
	*/
	virtual void addWhereDoesNotExists(string column) = 0;
	/**
	* ���ò�ѯ���ص��ֶ�
	* @param column �ֶ���
	*/
	virtual void addQueryKeys(string column) = 0;

	/**
	* where excute
	*/
	virtual void whereStartsWith() = 0;
	virtual void whereEndsWith() = 0;

	
	/**
	* ���ò�ѯ���ص����� 
	*/
	virtual void setLimit(int limit) = 0;

	/**
	* ���ò�ѯ��ת��ҳ������Ҫ��ѯ�ڼ�ҳ
	*/
	virtual void setSkip(int skip) = 0;
	/**
	* �������򷽷�
	*/
	virtual void order(string key) = 0;

	/**
	* 
	*/
	virtual void setCachePolicy() = 0;

	virtual bool hasCachedResult() = 0;
	virtual void clearCachedResult() = 0;
	virtual void clearAllCachedResults() = 0;
	
	/**
	* ����Ҫ��͵��ֶ�
	* @param column �ֶ���
	*/
	virtual void sum(string column) = 0;
	/**
	* ������ƽ��ֵ���ֶ�
	* @param column �ֶ���
	*/
	virtual void average(string column) = 0;
	/**
	* ����Ҫ��ѯ���ֵ���ֶ�
	* @param cloumn �ֶ���
	*/
	virtual void max(string column) = 0;
	/**
	* ����Ҫ��ѯ����Сֵ�ֶ�
	* @param column �ֶ���
	*/
	virtual void min(string column) = 0;
	/**
	* ���ð�ĳ���ֶν��з����ѯ
	* @param column �ֶ���
	*/
	virtual void groupby(string column) = 0;
	/**
	*
	*/
	virtual void having(bool isHaving = false) = 0;
	virtual void setHasGroupCount(bool groupCount) = 0;

	/**
	* ����HTTP����ͷ�� 
	*/
	virtual void setHeader(vector<string> v) {}
	/**
	* ����HTTP����ͷ��
	*/
    virtual vector<string> getHeader() {return m_header;}
  
    /**
    * 
    */
    virtual void enJson(Json::Value* value) = 0;
    /**
    * 
    */
    virtual void deJson(Json::Value* value) = 0;

	/**
	* ���ò�ѯ���������
	*/
	inline void setQueryType(BmobQueryInterafce::Type type)
    {
        _queryType = type;
    };
   
   	/**
   	* �õ���ѯ���������
   	*/
    inline BmobQueryInterafce::Type getQueryType()
    {
        return _queryType;
    };

    inline void clear(){
    	this->m_whereData.clear();
    }

protected:
	/**
	* HTTP ����Ļص�����
	*/
	virtual void onHttpRequestCompleted(cocos2d::Node *pSender,void *data) = 0;
	/**
	* ��ѯ
	*/
	virtual void query() = 0;

	BmobQueryInterafce(){}

protected:
	/**
	* ������ѯ����������
	*/
	enum class DelegateType{
		_bFIND_D,
		_bGET_D,
		_bCOUNT_D,
		_bSTATICS_D
	};

protected:
	BmobQueryInterafce::Type _queryType;			//��ѯ���������//
	BmobQueryInterafce::DelegateType _delegateType; //������ѯ������//

	CC_SYNTHESIZE(string,m_objectId,ObjectId);		//����objectId //
	CC_SYNTHESIZE(string,m_tableName,TableName);	//��Ӧ�ı���//

	CC_SYNTHESIZE(BmobFindDelegate*,m_pFindDelegate,FindDelegate);	//��ѯ�������ݵĻص��ӿ�//
	CC_SYNTHESIZE(BmobGetDelegate* ,m_pGetDelegate,GetDelegate);	//��ѯһ���������ݵĻص��ӿ�//
	CC_SYNTHESIZE(BmobCountDelegate* ,m_pCountDelegate,CountDelegate);	//������ѯ�Ļص��ӿ�//
	CC_SYNTHESIZE(BmobStaticsDelegate*, m_pStaticsDelegate,StaticDelegate);	 	//ͳ�Ʋ�ѯ�ص��ӿ�//

	std::vector<std::string> m_header;						//����ͷ��//
	std::multimap<string, cocos2d::Ref *> m_whereData; //���������//
	string m_url;		
	string m_order;		
	string m_queryKey;	
	string m_sum;	
	string m_group;
	string m_min;
	string m_max;
	string m_average;
	bool m_isHaving;
	bool m_groupCount;

	int m_limit;
	int m_skip;
};

#endif