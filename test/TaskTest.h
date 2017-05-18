/**
* @author : ��³��
* @date   : 2017��04��
* @version: 1.0
* @note   : ���� Apache ���֤ 2.0 �棨���¼�ơ����֤������Ȩ;
*           �������ر���ɣ�����������ʹ������ļ���
* @remarks: �����Ի�ø���ɵĸ�����
*           http://www.apache.org/licenses/LICENSE-2.0
*           �������÷�����Ҫ��������ͬ�⣬������ɷַ������
*           Ҫ����ԭ�����ַ���û���κ���ʽ�ģ���ȷ�������ĵ������
*           �μ����ձ���ɿ������Ȩ�޼����Ƶ��ض����Ե����֤��
*
*   ����Ի�øô�������°汾��
*
*        http://git.oschina.net/Mr_ChenLuYong/oeasypool
*
*   ��Դ�����������˶��ڴ�����Ĺ�ͬά����
*
*
*   ��������Щ���뻹�в����ĵط�����ͨ�����µ����½���ѧϰ��
*
*        ���͵�ַ��http://blog.csdn.net/csnd_ayo
*
*        ���µ�ַ��http://blog.csdn.net/csnd_ayo/article/details/72457190
*
* 	 �ڴ����ύBug����ӭIssues��
*
*/


#ifndef __TASKTEST_H__
#define __TASKTEST_H__

#include <iostream>

#include "Task.h"

class OETaskTest
	: public OETask
{
public:
	OETaskTest();
	virtual ~OETaskTest();

	virtual int doWork() {
		// ��ʼ����
		std::cout << "task : " << id_ << " do something." << std::endl;
		return 0;
	}

	virtual int onCanceled(void) {
		// ����ȡ��
		std::cout << "task : " << id_ << " canceled." << std::endl;
		return 0;
	}
	// ���������
	virtual int onCompleted(int code) {
		// ��������ɣ�������Ϊcode
		std::cout << "task : " << id_ << " completed." << std::endl;
		return 0;
	}
};

#endif // __TASKTEST_H__