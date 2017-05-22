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


#include "ThreadPool.h"
#include <iostream>
#include <thread>


//OEThreadPool SystemThreadPool;

OEThreadPool::OEThreadPool(void)
 :atcCurTotalThrNum_(0), atcWorking_(true) {
}

OEThreadPool::~OEThreadPool(void) {
    release();
}

// ��ʼ����Դ
int OEThreadPool::init(const ThreadPoolConfig& threadPoolConfig) {
	// ���������
	int ret = 0;
	if (threadPoolConfig.dbTaskAddThreadRate < threadPoolConfig.dbTaskSubThreadRate) {
		return 87;
	}

	threadPoolConfig_.nMaxThreadsNum = threadPoolConfig.nMaxThreadsNum;
	threadPoolConfig_.nMinThreadsNum = threadPoolConfig.nMinThreadsNum;
	threadPoolConfig_.dbTaskAddThreadRate = threadPoolConfig.dbTaskAddThreadRate;
	threadPoolConfig_.dbTaskSubThreadRate = threadPoolConfig.dbTaskSubThreadRate;
	// �����̳߳�
	if (threadPoolConfig_.nMinThreadsNum > 0) {
		ret = addProThreads(threadPoolConfig_.nMinThreadsNum);
	}

	return ret;
}

// �������
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
		// �����������
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

// ɾ�����񣨴Ӿ�������ɾ���������������û�У���ִ�ж�����û�У��еĻ�����ȡ��״̬λ��
int OEThreadPool::deleteTask(int nID) {
	return taskQueue_.deleteTask(nID);
}
// ɾ����������
int OEThreadPool::deleteAllTasks(void) {
	return taskQueue_.deleteAllTasks();
}

std::shared_ptr<OETask> OEThreadPool::isTaskProcessed(int nId) {
	return taskQueue_.isTaskProcessed(nId);
}


// �ͷ���Դ���ͷ��̳߳ء��ͷ�������У�
bool OEThreadPool::release(void) {
	// 1��ֹͣ�̳߳ء�2������������С�3���ȴ�ִ�ж���Ϊ0
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

// ��ȡ��ǰ�߳������
double OEThreadPool::getThreadTaskRate(void) {
	std::unique_lock<std::mutex> lock(mutex_);
	double dbThreadTaskRate = 0.0;

	if (atcCurTotalThrNum_ != 0) {
		int nTaskQueSize = taskQueue_.size();
		dbThreadTaskRate = nTaskQueSize* 1.0 / atcCurTotalThrNum_;
	}
	
	return dbThreadTaskRate;
}
// ��ǰ�߳��Ƿ���Ҫ����
bool OEThreadPool::shouldEnd(void) {
	bool bFlag = false;
	double dbThreadTaskRate = getThreadTaskRate();
	if (!atcWorking_ || atcCurTotalThrNum_ > threadPoolConfig_.nMinThreadsNum
		&& dbThreadTaskRate < threadPoolConfig_.dbTaskSubThreadRate) {
		bFlag = true;
	}
	return bFlag;
}
// ���ָ�������Ĵ����߳�
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
// �ͷ��̳߳�
bool OEThreadPool::releaseThreadPool(void) {
	threadPoolConfig_.nMinThreadsNum = 0;
	threadPoolConfig_.dbTaskSubThreadRate = 0;
	atcWorking_ = false;
	return true;
}

// �������̺߳���
void OEThreadPool::taskProcThread(void) {
	int nTaskProcRet = 0;
	// �߳�����
	atcCurTotalThrNum_.fetch_add(1);
	std::chrono::milliseconds mills_sleep(500);

	std::shared_ptr<OETask> pTask;
	while (atcWorking_) {
		// ��ȡ����
		pTask = taskQueue_.get();
		if (pTask == nullptr) {
			if (shouldEnd()) {
				break;
			}
			taskQueue_.wait(mills_sleep);
			continue;
		}
		// �������ȡ��״̬
		if (pTask->isCancelRequired()) {
			pTask->onCanceled();
		}
		else {
			// ��������
			nTaskProcRet = pTask->doWork();
			pTask->onCompleted(nTaskProcRet);
		}

		// ����������������Ƴ�����
		taskQueue_.onTaskFinished(pTask->getID());

		// �ж��߳��Ƿ���Ҫ����
		if (shouldEnd()) {
			break;
		}
	}
	// �̸߳�����һ
	atcCurTotalThrNum_.fetch_sub(1);
}