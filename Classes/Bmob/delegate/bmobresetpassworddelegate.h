/**
* create 2015-11-6
* ��������ص��ӿ�
*/

#ifndef BMOBSDK_BMOBRESETPASSWORDDELEGATE_H_
#define BMOBSDK_BMOBRESETPASSWORDDELEGATE_H_

class BmobResetPasswordDelegate{
public:
	/**
	* ���óɹ��Ļص��ӿ�
	*/
	virtual void onResetSucess(const void* data) = 0;
	/**
	* ����ʧ�ܻص��ӿ�
	*/
	virtual void onResetError(int code,const void* data) = 0;
};


#endif