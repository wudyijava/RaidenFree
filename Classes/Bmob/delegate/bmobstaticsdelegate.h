/**
* create by zh_u 2015-10-25
* ͳ�ƵĻص��ӿ�
*/
#ifndef BMOBSDK_BMOBSTATICSDELEGATE_H_
#define BMOBSDK_BMOBSTATICSDELEGATE_H_

class BmobStaticsDelegate{
public:
	/**
	* ͳ�Ƴɹ��Ļص��ӿ�
	* @param data ���ص�����
	*/
	virtual void onStaticsSucess(const void* data) = 0;
	/**
	* ͳ�ƴ���Ļص��ӿ�
	* @param code ���صĴ�����
	* @param data ���صĴ�������
	*/
	virtual void onStaticsError(int code,const void* data) = 0;
};

#endif