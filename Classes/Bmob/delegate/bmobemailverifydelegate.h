/**
* create 201511-6
* ������֤�Ļص��ӿ�
*/
#ifndef BMOBSDK_BMOBEMAILVERIFYDELEGATE_H_
#define BMOBSDK_BMOBEMAILVERIFYDELEGATE_H_

class BmobEmailVerifyDelegate{
public:
	/**
	* ��֤�ɹ��Ļص��ӿ�
	* @param data ���ص�����
	*/
	virtual void onEmailVerifySucess(const void* data) = 0;
	/**
	* ��֤ʧ�ܻص�
	* @param code ���صĴ�����
	* @param data ���صĴ�������
	*/
	virtual void onEmailVerifyError(int code,const void* data) = 0;
};

#endif