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

