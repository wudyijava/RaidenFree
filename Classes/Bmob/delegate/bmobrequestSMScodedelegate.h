/**
* create 2015-11-6
* ����SMS��ص��ӿ�
*/
#ifndef BMOBSDK_BMOBREQUESTSMSCODEDELEGATE_H_
#define BMOBSDK_BMOBREQUESTSMSCODEDELEGATE_H_

class BmobRequestSMSCodeDelegate{
public:
	virtual void onRequestDone(int code,const void* data) = 0;
};

#endif