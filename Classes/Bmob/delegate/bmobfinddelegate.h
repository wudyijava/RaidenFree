/**
* create by zh_u 2015-10-25
* ��ѯ���ݵĻص��ӿ�
*/
#ifndef BMOBSDK_BMOBFINDDELEGATE_H_
#define BMOBSDK_BMOBFINDDELEGATE_H_

class BmobFindDelegate{
public:
	/**
	* �ɹ��ص��ӿ�
	* @param data ���ص�����
	*/
	virtual void onFindSucess(const void* data) = 0;
	/**
	* ����ص�
	* @param code ���صĴ�����
	* @param data ���صĴ�������
	*/
	virtual void onFindError(int code,const void* data) = 0;
};

#endif