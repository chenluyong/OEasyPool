// ThreadPoolDemo.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include "ThreadPool.h"
#include "../test/TaskTest.h"
#include <memory>
using namespace std;


#include <time.h>

int main(void)
{
	OEThreadPool::ThreadPoolConfig threadPoolConfig;
	threadPoolConfig.nMaxThreadsNum = 100;
	threadPoolConfig.nMinThreadsNum = 5;
	threadPoolConfig.dbTaskAddThreadRate = 3;
    threadPoolConfig.dbTaskSubThreadRate = 0.5;
    clock_t start = clock();
	{

		std::shared_ptr<OEThreadPool> threadPool(new OEThreadPool);
		threadPool->init(threadPoolConfig);

		int nID = 0;
		while (true)
		{
			std::shared_ptr<OETaskTest> request = std::shared_ptr<OETaskTest>(new OETaskTest());
            
			threadPool->addTask(request);
            if (request->getID() == 101000) {
				break;
            }
		}

		threadPool->release();
    }
    clock_t finish = clock();
    std::cout << "duration:" << finish << "ms - " << start << "ms = " << finish - start << "ms"<< std::endl;
	getchar();
	return 0;
}

