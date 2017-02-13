/**
* create by 2015-11-09
* �����û�ע����࣬��Ҫ����ע�ᡢ��½����֤���󶨡������û���Ϣ����ȡ��ǰ�û�
*/
#ifndef BMOBSDK_BMOBUSER_H_
#define BMOBSDK_BMOBUSER_H_

#include <iostream>
#include "cocos2d.h"
#include "bmobobject.h"
#include "../delegate/bmobdelegate.h"

using namespace std;
USING_NS_CC;

class BmobUser:public BmobObject{
public:

	BmobUser();
	virtual ~BmobUser();

	/**
	* �û�ע��
	* @param delegate  ����ע��ص��ӿ�
	*/
	virtual void signUp(BmobSaveDelegate* delegate);
	/**
	* �û�ʹ���û����������½
	* @param delegate ������½�ص��ӿ�
	*/
	virtual void login(BmobSaveDelegate* delegate);
	/**
	* ͨ���û�objectId�����û���Ϣ
	* @param objectId 	�û�objectId
	*/
	virtual void update(string objectId,BmobUpdateDelegate* delegate);
	/**
	* �����û�����
	* @param delegate ���»ص��ӿ�
	*/
    virtual void update(BmobUpdateDelegate* delegate);
    
    /**
    * ͨ�������ʼ���������
    * @param email �����ַ
    * @param delegate ���ûص��ӿ�
    */
    void resetPasswordByEmail(string email,BmobResetPasswordDelegate* delegate);
    /**
    * ����SMS��
    * @param meblieNumber �ֻ�����
    * @param template_name ģ����
    * @param delegate ����ص��ӿ�
    */
    void requestSMSCode(string meblieNumber,string template_name,BmobRequestSMSCodeDelegate* delegate);
    /**
    * ͨ��SMS����������
    * @param pw ����
    * @param code SMS��
    * @param delegate ���ûص��ӿ�
    */
    void resetPasswordBySMSCode(string pw,string code,BmobResetPasswordByCodeDelegate* delegate);
    /**
    * ���µ�ǰ�û�������
    * @param old_pwd ԭ����
    * @param new_pwd ������
    * @param delegate �ص��ӿ�
    */
    void updateCurrentUserPassword(string old_pwd,string new_pwd,BmobUpdateDelegate* delegate);
    /**
    * ����������֤
    * @param email �����ַ
    * @param delegate �ص��ӿ�
    */
    void requestEmailVerify(string email,BmobEmailVerifyDelegate* delegate);
    /**
    * ʹ���˺�ֱ�ӵ�¼
    * @param meblieNumber  �˺������ֻ�������..��
    * @param pwd ����
    */
    void loginByAccount(string mebileNumber,string pwd,BmobLoginDelegate* delegate);
    /**
    * ʹ��SMS���¼
    * @param mebileNumber �ֻ���
    * @param code SMS��
    * @param delegate �ص��ӿ�
    */
    void loginBySMSCode(string mebileNumber,string code,BmobLoginDelegate* delegate);
    /**
    * ʹ���ֻ���ע�Ტֱ�ӵ�¼
    * @param mebileNumber �ֻ���
    * @param code SMS��
    * @param delegate �ص��ӿ�
    */
    void signOrLoginByMobilePhone(string mebileNumber,string code,BmobLoginDelegate* delegate);

	string getPassword();
	void setPassword(string password);

	/**
	* ��ȡ��ǰ�û�
	*/
	static BmobUser* getCurrentUser();
	static BmobUser* currentUser;
	static string __USER_FILE;
	/**
	* �˳���¼
	*/
	static void logOut();

protected:


private:
	CC_SYNTHESIZE(string,m_username,UserName);
	// CC_SYNTHESIZE(string,m_password,Password);
	string m_password;
	CC_SYNTHESIZE(string,m_email,Email);
	CC_SYNTHESIZE(string,m_emailVerified,EmailVerified);
	CC_SYNTHESIZE(string,m_mobilePhoneNumber,MobilePhoneNumber);
	CC_SYNTHESIZE(bool,m_mobilePhoneNumberVerified,MobilePhoneNumberVerified);
};

#endif