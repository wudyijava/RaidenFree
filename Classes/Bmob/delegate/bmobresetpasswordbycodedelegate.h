/**
* create 2015-11-6
* ͨ��SMS����������ص��ӿ�
*/

#ifndef BMOBSDK_BMOBRESETPASSWORDBYCODEDELEGATE_H_
#define BMOBSDK_BMOBRESETPASSWORDBYCODEDELEGATE_H_

class BmobResetPasswordByCodeDelegate{
public:
	virtual void onResetDone(int code,const void* data) = 0;
};

#endif