/**
* @author : 陈鲁勇
* @date   : 2017年04月
* @version: 1.0
* @note   : 根据 Apache 许可证 2.0 版（以下简称“许可证”）授权;
*           除非遵守本许可，否则您不能使用这个文件。
* @remarks: 您可以获得该许可的副本：
*           http://www.apache.org/licenses/LICENSE-2.0
*           除非适用法律需要或者书面同意，按本许可分发的软件
*           要按“原样”分发，没有任何形式的，明确或隐含的担保条款。
*           参见按照本许可控制许可权限及限制的特定语言的许可证。
*
*   你可以获得该代码的最新版本：
*
*        http://git.oschina.net/Mr_ChenLuYong/oeasypool
*
*   开源社区的所有人都期待与你的共同维护。
*
*
*   如果你对这些代码还有不理解的地方可以通过最新的文章进行学习：
*
*        博客地址：http://blog.csdn.net/csnd_ayo
*
*        文章地址：http://blog.csdn.net/csnd_ayo/article/details/72457190
*
* 	 期待你提交Bug，欢迎Issues。
*
*/

#include <time.h>
#include <iostream>
#include <memory>
#include <thread>
#include "OE/EasyThreadPool"

#include "../../test/TaskTest.h"
using namespace std;



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

		while (true)
		{
			std::shared_ptr<OETaskTest> request = std::shared_ptr<OETaskTest>(new OETaskTest());
            
			threadPool->addTask(request);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
//            if (request->getID() == 101000) {
//				break;
//            }
		}

		threadPool->release();
    }
    clock_t finish = clock();
    std::cout << "duration:" << finish - start << "ms"<< std::endl;
	getchar();
	return 0;
}

