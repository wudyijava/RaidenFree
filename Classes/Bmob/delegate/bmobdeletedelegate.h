/**
* create by zh_u 2015-10-25
* ɾ�����ݻص��ӿ�
*/
#ifndef BMOBSDK_BMOBDELETEDELEGATE_H_
#define BMOBSDK_BMOBDELETEDELEGATE_H_

class BmobDeleteDelegate{
public:
	/**
	* ɾ���ɹ��Ļص�����
	* @param data ���ص�����
	*/
	virtual void onDeleteSucess(const void* data) = 0;
	/**
	* ɾ��ʧ�ܻص�����
	* @param code ���ش�����
	* @param data ���صĴ�������
	*/
	virtual void onDeleteError(int code,const void* data) = 0;
};

#endif