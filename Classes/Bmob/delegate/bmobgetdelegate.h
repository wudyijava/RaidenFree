/**
* create by zh_u_ 2015-10-25
* ��ѯ�������ݵĻص��ӿ�
*/
#ifndef BMOBSDK_BMOBGETDELEGATE_H_
#define BMOBSDK_BMOBGETDELEGATE_H_

class BmobGetDelegate{
public:
	/**
	* ��ѯ�ɹ��Ļص�
	* @param data ���ص�����
	*/
	virtual void onGetSucess(const void* data) = 0;
	/**
	* ʧ�ܻص�
	* @param code ���ش�����
	* @param data ���صĴ�������
	*/
	virtual void onGetError(int code,const void* data) = 0;
};

#endif