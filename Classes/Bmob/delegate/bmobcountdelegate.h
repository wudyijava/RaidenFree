/**
* create by zh_u 2015-10-25
* ��ѯ�����ص��ӿ�
*/
#ifndef BOMSDK_BMOBCOUNTDELEGATE_H_
#define BOMSDK_BMOBCOUNTDELEGATE_H_

class BmobCountDelegate{
public:
	/**
	* ��ѯ�ɹ��ص�
	* @param data ���ص�����
	*/
	virtual void onCountSucess(const void* data) = 0;
	/**
	* ��ѯʧ�ܻص�
	* @param code ���صĴ�����
	* @param data ���صĴ�����Ϣ
	*/
	virtual void onCountError(int code,const void* data) = 0;
};

#endif