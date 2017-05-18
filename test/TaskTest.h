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
		// 开始工作
		std::cout << "task : " << id_ << " do something." << std::endl;
		return 0;
	}

	virtual int onCanceled(void) {
		// 任务被取消
		std::cout << "task : " << id_ << " canceled." << std::endl;
		return 0;
	}
	// 任务已完成
	virtual int onCompleted(int code) {
		// 任务已完成，返回码为code
		std::cout << "task : " << id_ << " completed." << std::endl;
		return 0;
	}
};

#endif // __TASKTEST_H__