/**
* create by zh_u 2015-10-25
* �������ݵĻص��ӿ�
*/
#ifndef BMOBSDK_BMOBUPDATEDELEGATE_H_
#define BMOBSDK_BMOBUPDATEDELEGATE_H_

class BmobUpdateDelegate{
public:
	/**
	* �������ݳɹ��ص�����
	* @param data ���ص�����
	*/
	virtual void onUpdateSucess(const void* data) = 0;
	/**
	* ����ʧ�ܻص�����
	* @param code ���صĴ�����
	* @param data ���صĴ�������
	*/
	virtual void onUpdateError(int code,const void* data) = 0;
};

#endif