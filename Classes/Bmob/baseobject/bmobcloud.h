/**
* create by zhou,2016-01-18
* cloud code operator
* ���ƶ˴���Ĳ���
*/
#ifndef BMOBSDK_BMOBCLOUD_H_
#define BMOBSDK_BMOBCLOUD_H_

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "network/HttpRequest.h"

#include "../delegate/bmobdelegate.h"
#include "../jsoncpp/include/json.h"
#include "bmobSDKinit.h"
#include "../util/bmobjsonutil.h"

using namespace std;
USING_NS_CC;

class BmobCloud:public Ref{
public:
  /**
  * �ƶ˴���Ĳ������ͣ�ִ�д��롢�������롢ɾ������
  */
  enum EXEC_Type{
    EXEC_Exec,
    EXEC_Create,
    EXEC_Delete,
  };

public:
  BmobCloud();
  ~BmobCloud();

  /**
  * ִ���ƶ˴���
  * @param cloudName  �ƶ˴���ķ�����������Ǵ������룬����NULL
  * @param param      �ƶ˴���Ĳ���
  * @param  delegate  ����ִ�еĻص�����
  * @param  type (Ĭ����EXEC_Exec)     ִ�еĲ�����ִ�д��롢�������롢ɾ������
  * ע����ִ�д����ƶ˴����ʱ�򣬲����д洢���Ƿ��������⣬����ʹ�ü�ֵ�Ķ�Ӧ��ϵ
  * ��Ϊcode��ֵ�Ǵ��룬param ���洢һ��ֵ;������õ��Ǵ������룬��ôcloudName
  * ����NULL����
  */
  void execCloudCode(string cloudName,std::map<string, Ref*> param,\
    BmobCloudDelegate *delegate,BmobCloud::EXEC_Type type = EXEC_Type::EXEC_Exec);

protected:

  /**
	* ���������BmobSDK��̨
	*/
  void send();

  /**
  * ʹ��HTTP���ͷ�������
  * @param type http��������,GET��POST��PUT��DELETE etc.
  */
  void send(network::HttpRequest::Type type);

  /**
  * HTTP����ص�����
  * @param pSender
  * @param data �ص���������
  */
  virtual void onHttpRequestCompleted(cocos2d::Node *pSender,void *data);

private:
  void setHeader(std::vector<string> v){}
  std::vector<string> getHeader();

  void enJson(Json::Value* value){
      BmobJsonUtil::dictionary2Json(value, &(this->m_mapData));
  }

private:
  EXEC_Type m_exec_type;

  BmobCloudDelegate* m_delegate;
  std::map<string, cocos2d::Ref *> m_mapData;    //���������//
  std::vector<string> m_header;   //����ͷ��//
  string m_url;
};

#endif
