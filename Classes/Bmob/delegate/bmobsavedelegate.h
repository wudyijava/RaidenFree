/**
* �������ݻص��ӿ�
*/
#ifndef BMOBSDK_BMOBSAVEDELGATE_H_
#define BMOBSDK_BMOBSAVEDELGATE_H_

class BmobSaveDelegate{
public:
	/**
	* �������ݳɹ��Ļص��ӿ�
	* @param data ���ص�����
	*/
	virtual void onSaveSucess(const void* data) = 0;
	/**
	* �������ص��ӿ�
	* @param code ���صĴ�����
	* @param data ���صĴ�����Ϣ
	*/
	virtual void onSaveError(int code,const void* data) = 0;
};

#endif