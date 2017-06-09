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


#ifndef __TASKTEST_H__
#define __TASKTEST_H__

#include <iostream>

#include "OE/EasyPool/Task.h"

class OETaskTest
	: public OETask
{
public:
    OETaskTest() {
        std::cout << "task : " << id_ << " is create." << std::endl;
    }
    virtual ~OETaskTest() {}

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
    virtual int onCompleted(int) {
		// 任务已完成，返回码为code
		std::cout << "task : " << id_ << " completed." << std::endl;
		return 0;
	}
};

#endif // __TASKTEST_H__
