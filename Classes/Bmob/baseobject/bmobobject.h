/**
* create by zh_u 2015-10-25
* BmobObject �������Զ������Ļ��࣬Ҳ��BmobSDK�Ļ���
*/

#ifndef BMOBSDK_BMOBOBJECT_H_
#define BMOBSDK_BMOBOBJECT_H_

#include <iostream>
#include <string>
#include <vector>
#include "cocos2d.h"
#include "network/HttpRequest.h"

#include "../delegate/bmobdelegate.h"
#include "../jsoncpp/include/json.h"
#include "bmobSDKinit.h"
#include "../util/bmobjsonutil.h"

using namespace std;
USING_NS_CC;

class BmobObject:public cocos2d::Ref{
protected:
    /**
    * save,update,delete's sign
    * HTTP���������ͣ���Ӧ���ǻص��ӿ�����
    */
    enum class HTTP_OP_Type{
        _bHTTP_SAVE,
        _bHTTP_UPDATE,
        _bHTTP_DELETE,
        _bHTTP_RESET,
        _bHTTP_REQUEST_CODE,
        _bHTTP_RESET_BY_CODE,
        _bHTTP_EMAIL_VERIFY,
        _bHTTP_LOGIN
    };

public:
	/**
    * ʹ�ö����������캯����Ҳ���Ǻ�̨��Ӧ�ı���
    * @param tableName �������
    */
	BmobObject(string tableName);
    /**
    * �鹹����
    */
    virtual ~BmobObject();

	/**
	* ���ø÷������������ݱ��浽SDK��̨
    * @param delegate �������ص��ӿ�
	*/
	virtual void save(BmobSaveDelegate* delegate);
    
    /**
    * ���¶�����÷���
    * @param column ���µ��ֶ�����������
    * @param value Ҫ���µ�ֵ��Ĭ������1
    * �÷���ֵ����������ݽ��в���
    */
    virtual void increment(string column,int value = 1);
    /**
    * ����Ҫ���µ��ֶκ�ֵ
    * @param key    ���õ��ֶ�����������
    * @param object ��Ҫ���õ�ֵ
    */
    void setValue(string key,cocos2d::Ref *object);
    /**
    * ������Ĳ���
    * @param key    Ҫ���õ��ֶ�����������
    * @param array  ����ֵ
    */
    void setValue(string key,cocos2d::__Array* array);
    /**
    * ʹ��objectId����BmobSDK��̨����
    * @param ObjectId   �������ݶ�Ӧ��objectId
    * @param delegate   �������ݻص��ӿ�
    */
    virtual void update(string objectId,BmobUpdateDelegate* delegate);
    /**
    * ����BmobSDK��̨����
    * @param    delegate    �������ݻص��ӿ�
    */
    virtual void update(BmobUpdateDelegate* delegate);
    /**
    * ����Ҫ�Ƴ���̨���ݶ�Ӧ���ֶ�
    * @param name   �ֶ���
    */
    virtual void remove(string name);
    /**
    * ����Ҫ�Ƴ���̨���ݶ�Ӧ���ֶ�
    * @param array  Ҫ�Ƴ����ֶ�����
    */
    virtual void removeAll(string name,__Array* array);
    /**
    *  ɾ��BmobSDK��̨����
    * @param    delegate    ɾ�����ݻص��ӿ�
    */
    virtual void del(BmobDeleteDelegate* delegate);
    /**
    * ֱ��ʹ��objectId��ɾ������
    * @param objectId Ҫɾ�����ݶ�Ӧ��ObjectID
    * @param delegate ɾ�����ݵĻص��ӿ�
    */
    virtual void del(string objectId,BmobDeleteDelegate* delegate);

    /**
    * ��BmobSDK����Ӱ���һ�����ݵ�����
    * @param column  �ֶ���
    * @param object  ��Ӧ�ֶ�����ֵ
    */
    virtual void add(string column,Ref* object);
    /**
    * ��BmobSDK����Ӱ���������ݵ�����
    * @param column Ҫ��ӵ��ֶ���
    * @param array Ҫ��ӵ�����ֵ
    */
    virtual void add(string column,__Array* array);
    /**
    * ��������Ҫ���������ѹ��HTTP������
    * @param key Ҫ�ں�̨������ֶ�
    * @param object  Ҫ�ں�̨������ֶζ�Ӧ��ֵ
    */
    void enParamsToHttp(std::string key,cocos2d::Ref *object);

	/**
	* ����HTTP�����ͷ��
    * @param v �Զ���HTTP��ͷ�� 
	*/
	virtual void setHeader(vector<string> v);
    /**
    * ��ȡ��ǰHTTP�����ͷ��
    */
    virtual vector<string> getHeader();

    /**
    * ��ȡkey��Ӧ������
    * @param key �ֶ���
    */
    virtual Ref* getParams(string key);
    /**
    * �����͸����������ת����json����
    * @param value 
    */
    virtual void enJson(Json::Value* value);
    /**
    * ��Json���ݽ�ѹ��map<string,Ref*>
    * @param value Ҫ������Json����
    */
    virtual void deJson(Json::Value* value);

    /**
    * ��������е�����
    */
    void clear(){
        this->m_mapData.clear();
    }

protected:
    /**
    * 
    */
    BmobObject();

public:
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

  
protected:
    HTTP_OP_Type _opType;                           //HTTP��������//

	CC_SYNTHESIZE(string, m_objectId, ObjectId);   //�����ӦBmobSDK��̨ObjectId//
    CC_SYNTHESIZE(string,m_createdAt,CreatedAt);    //��̨���󴴽���ʱ��//
    CC_SYNTHESIZE(string,m_updatedAt,UpdatedAt);    //��̨������µ�ʱ��///
    CC_SYNTHESIZE(string,m_ACL,ACL);            
    CC_SYNTHESIZE(string,m_url,Url);                //�����URL//

    CC_SYNTHESIZE(string,m_tableName,TableName);    //�����Ӧ�ı���//
    CC_SYNTHESIZE(string,m_session,Session);        //�û������Ӧ��session//

    CC_SYNTHESIZE(BmobSaveDelegate*,m_pSaveDelegate,SaveDelegate);  //�������ص��ӿ�//
    CC_SYNTHESIZE(BmobUpdateDelegate*,m_pUpdateDelegate,UpdateDelegate);    //���¶���ص��ӿ�//
    CC_SYNTHESIZE(BmobDeleteDelegate*,m_pDeleteDelegate,DeleteDelegate);    //ɾ������ص��ӿ�//
    CC_SYNTHESIZE(BmobResetPasswordDelegate*,m_pResetDelegate,ResetDelegate);   //��������ص��ӿ�//
    CC_SYNTHESIZE(BmobRequestSMSCodeDelegate*,m_pRequestSMSCodeDelegate,RequestSMSCodeDelegate);    //����SMS��ص��ӿ�//  
    CC_SYNTHESIZE(BmobResetPasswordByCodeDelegate*,m_pResetByMSMCodeDelegate,ResetByMSMCodeDelegate);   //ͨ��SMS����������Ļص��ӿ�//
    CC_SYNTHESIZE(BmobEmailVerifyDelegate*,m_pEmailVerifyDelegate,EmailVerifyDelegate); //ͨ��������֤�Ļص��ӿ�//
    CC_SYNTHESIZE(BmobLoginDelegate*,m_pLoginDelegate,LoginDelegate);   //��¼�Ļص��ӿ�//

    std::map<string, cocos2d::Ref *> m_mapData;    //���������//
    std::vector<string> m_header;   //����ͷ��//

};

#endif