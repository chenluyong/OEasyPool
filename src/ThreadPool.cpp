#include "ThreadPool.h"
#include <iostream>
#include <thread>


//OEThreadPool SystemThreadPool;

OEThreadPool::OEThreadPool(void)
 :atcCurTotalThrNum_(0), atcWorking_(true) {
}

OEThreadPool::~OEThreadPool(void) {

}

// 初始化资源
int OEThreadPool::init(const ThreadPoolConfig& threadPoolConfig) {
	// 错误的设置
	int ret = 0;
	if (threadPoolConfig.dbTaskAddThreadRate < threadPoolConfig.dbTaskSubThreadRate) {
		return 87;
	}

	threadPoolConfig_.nMaxThreadsNum = threadPoolConfig.nMaxThreadsNum;
	threadPoolConfig_.nMinThreadsNum = threadPoolConfig.nMinThreadsNum;
	threadPoolConfig_.dbTaskAddThreadRate = threadPoolConfig.dbTaskAddThreadRate;
	threadPoolConfig_.dbTaskSubThreadRate = threadPoolConfig.dbTaskSubThreadRate;
	// 创建线程池
	if (threadPoolConfig_.nMinThreadsNum > 0) {
		ret = addProThreads(threadPoolConfig_.nMinThreadsNum);
	}

	return ret;
}

// 添加任务
int OEThreadPool::addTask(std::shared_ptr<OETask> taskptr, bool priority) {
	const double& rate = getThreadTaskRate();
	int ret = 0;
	if (priority) {
		if (rate > 1000) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		taskQueue_.put_front(taskptr);
	}
	else {
		// 检测任务数量
		if (rate > 888) {
			return 298;
		}
		taskQueue_.put_back(taskptr);
	}

	if (atcCurTotalThrNum_ < threadPoolConfig_.nMaxThreadsNum 
		&& rate > threadPoolConfig_.dbTaskAddThreadRate) {
		ret = addProThreads(1);
	}

	return ret;
}

// 删除任务（从就绪队列删除，如果就绪队列没有，则看执行队列有没有，有的话置下取消状态位）
int OEThreadPool::deleteTask(int nID) {
	return taskQueue_.deleteTask(nID);
}
// 删除所有任务
int OEThreadPool::deleteAllTasks(void) {
	return taskQueue_.deleteAllTasks();
}

std::shared_ptr<OETask> OEThreadPool::isTaskProcessed(int nId) {
	return taskQueue_.isTaskProcessed(nId);
}


// 释放资源（释放线程池、释放任务队列）
bool OEThreadPool::release(void) {
	// 1、停止线程池。2、清楚就绪队列。3、等待执行队列为0
	releaseThreadPool();
	taskQueue_.release();
	while (atcCurTotalThrNum_ != 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		static int i = 0;
		if (i++ == 10) {
			std::cout << "Waiting for the error." << std::endl;
			exit(23);
		}		
	}

	atcCurTotalThrNum_ = 0;
	return true;
}

// 获取当前线程任务比
double OEThreadPool::getThreadTaskRate(void) {
	std::unique_lock<std::mutex> lock(mutex_);
	double dbThreadTaskRate = 0.0;

	if (atcCurTotalThrNum_ != 0) {
		int nTaskQueSize = taskQueue_.size();
		dbThreadTaskRate = nTaskQueSize* 1.0 / atcCurTotalThrNum_;
	}
	
	return dbThreadTaskRate;
}
// 当前线程是否需要结束
bool OEThreadPool::shouldEnd(void) {
	bool bFlag = false;
	double dbThreadTaskRate = getThreadTaskRate();
	if (!atcWorking_ || atcCurTotalThrNum_ > threadPoolConfig_.nMinThreadsNum
		&& dbThreadTaskRate < threadPoolConfig_.dbTaskSubThreadRate) {
		bFlag = true;
	}
	return bFlag;
}
// 添加指定数量的处理线程
int OEThreadPool::addProThreads(int nThreadsNum) {
	std::thread add_thread;
	try {
		for (; nThreadsNum > 0; --nThreadsNum) {
			add_thread = std::thread(&OEThreadPool::taskProcThread, this);
			add_thread.detach();
		}
	}
	catch (...) {
		return 155;
	}
	return 0;
}
// 释放线程池
bool OEThreadPool::releaseThreadPool(void) {
	threadPoolConfig_.nMinThreadsNum = 0;
	threadPoolConfig_.dbTaskSubThreadRate = 0;
	atcWorking_ = false;
	return true;
}

// 任务处理线程函数
void OEThreadPool::taskProcThread(void) {
	int nTaskProcRet = 0;
	// 线程增加
	atcCurTotalThrNum_.fetch_add(1);
	std::chrono::milliseconds mills_sleep(500);

	std::shared_ptr<OETask> pTask;
	while (atcWorking_) {
		// 获取任务
		pTask = taskQueue_.get();
		if (pTask == nullptr) {
			if (shouldEnd()) {
				break;
			}
			taskQueue_.wait(mills_sleep);
			continue;
		}
		// 检测任务取消状态
		if (pTask->isCancelRequired()) {
			pTask->onCanceled();
		}
		else {
			// 处理任务
			nTaskProcRet = pTask->doWork();
			pTask->onCompleted(nTaskProcRet);
		}

		// 从运行任务队列中移除任务
		taskQueue_.onTaskFinished(pTask->getID());

		// 判断线程是否需要结束
		if (shouldEnd()) {
			break;
		}
	}
	// 线程个数减一
	atcCurTotalThrNum_.fetch_sub(1);
}