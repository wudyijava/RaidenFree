/**
* �ַ�������������
*/
#ifndef BMOBSDK_BMOBSTUTIL_H_
#define BMOBSDK_BMOBSTUTIL_H_

#include <iostream>
#include <string>

using namespace std;

class BmobStrUtil{
public:
	/**
	* �Ƴ��ַ����е�tab��enter��space�ʲ�
	* @param str Ҫ�������ַ���
	*/
	static string remove_tab_enter_space(string* str);
};

#endif