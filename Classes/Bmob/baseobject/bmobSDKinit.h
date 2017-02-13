/**
* create zh_u 2015-10-25
* BmobSDK������ʼ������
*/

#ifndef BMOBSDK_BMOBSDKINIT_H_
#define BMOBSDK_BMOBSDKINIT_H_

#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

class BmobSDKInit{
public:
	/**
	* ʹ��appID��appKey��ʼ������
	*/
	static void initialize(string app_id,string app_key);
	/**
	* ��ѯ�����Ƿ��Ѿ���ʼ��
	*/
	static bool isInitialize();

	static string APP_ID;			//SDK APP_ID//
	static string APP_KEY;			//SDK APP_KEY //
	static string URL;
	static string BASE_URL;
	static string USER_URL;
	static string LOGIN_URL;
	static string USER_TABLE;
	static string RESET_URL;
	static string REQUEST_SMS_CODE_URL;
	static string RESET_BY_CODE_URL;
	static string UPDATE_PWD_URL;
	static string EMAIL_VERIFY_URL;
	static string CLOUD_CODE_URL;

};
#endif
