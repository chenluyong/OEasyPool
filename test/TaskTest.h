#ifndef __TASKTEST_H__
#define __TASKTEST_H__

#include "Task.h"
#include <iostream>

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