/**
* create by zh_u_ 2015-10-25
*/

#ifndef BMOBSDK_BMOBJSONUTIL_H_
#define BMOBSDK_BMOBJSONUTIL_H_

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "../jsoncpp/include/json.h"

using namespace std;
USING_NS_CC;


class BmobJsonUtil{
public:
	/**
	* ��objectת����json����
    * @param value �洢ת���������
    * @param key ��Ӧת�����ݵ�key
    * @param object ת�������ݶ���
	*/
    static void object2Json(Json::Value *value, std::string key, cocos2d::Ref *object);
    /**
    * ��map����ת����json����
    * @param value �洢ת���������
    * @param mapData ��Ҫת��������
    */
    static void dictionary2Json(Json::Value *value,std::map<std::string, cocos2d::Ref *> *mapData);
   	/**
   	* ����������ת����json���� 
    * @param jarray �洢ת���������
    * @param list ��Ҫת���������б�
   	*/
    static void array2Json(Json::Value *jarray, std::vector<cocos2d::Ref *> *list);
    
    /**
    * ��multimap����ת����json����
    * @param value �洢ת���������
    * @param mapData ��Ҫת����multimap����
    */
    static void dictionary2Json_m(Json::Value *value,std::multimap<std::string, cocos2d::Ref *> *mapData);

    /**
    * ��json����ת����Ref
    * @param value ��Ҫת����json����
    * @param key json��Ӧ�ļ�
    */
    static cocos2d::Ref* json2Object(Json::Value *value, std::string key);
    /**
    * ��json����ת����map����
    * @param value ��Ҫת����json����
    * @param mapData �洢ת���������
    */
    static void json2Dictionary(Json::Value *value,std::map<std::string, cocos2d::Ref *> *mapData);
    /**
    * ��json����ת��������
    * @param jarray 
    * @param list 
    */
    static void json2Array(Json::Value *jarray, std::vector<cocos2d::Ref *> *list);
    /**
    * jsonת�����ַ���
    * @param value
    */
    static string json2String(Json::Value *value);
};

#endif