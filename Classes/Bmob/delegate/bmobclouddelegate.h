/**
* create by zhou,2016-01
* �ƶ˴���ִ�лص��ӿ�
*/
#ifndef BMOBSDK_BMOBCLOUDDELEGATE_H_
#define BMOBSDK_BMOBCLOUDDELEGATE_H_

class BmobCloudDelegate{
public:
  /**
  * ִ���ƶ˴���ɹ��Ļص�����
  * @param data   �ɹ��Ժ�ķ�������
  */
  virtual void onCloudSuccess(const void* data) = 0;
  /**
  * ִ���ƶ˴���ʧ�ܵĻص�����
  * @param code   ʧ�ܵķ��ش���
  * @param mas    ʧ�ܵķ�����Ϣ
  */
  virtual void onCloudFailure(const int code,const void* msg) = 0;
};

#endif
