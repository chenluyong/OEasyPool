/**
* @file   : task.h
* @brief  : 任务的基础类，定义了最基础的接口
* @version: 1.0
* @author : 陈鲁勇
* @create : 2017年04月
* @date   : 2017年6月3日
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


#ifndef __OETASK_H__
#define __OETASK_H__

#include <time.h>
#include <atomic>

// 任务基类
class OETask
{

protected:

    // 任务的唯一标识
    unsigned int id_;
    // 任务创建时间 （非Unix时间戳）
    clock_t createTime_;

private:

    static unsigned int nRequestID_;
    // 任务取消状态
    std::atomic<bool>  isCancelRequired_;

public:
    OETask() :id_(nRequestID_++), isCancelRequired_(false),
        createTime_(clock()){}
	virtual ~OETask() {};

public:
    // 任务类虚接口，继承这个类的必须要实现这个接口
	virtual int doWork() = 0;

	// 任务已取消回调
	virtual int onCanceled(){ return 1; }
	// 任务已完成
    virtual int onCompleted(int){ return 1; }

    // 任务是否超时
    virtual bool isTimeout(const clock_t& now) {
        return ((now - createTime_) > 5000);
    }

	// 获取任务ID
    unsigned int getID(void){ return id_; }
    // 获取任务取消状态
    bool isCancelRequired(){ return isCancelRequired_; }
	// 设置任务取消状态
    void setCancelRequired(){ isCancelRequired_ = true; }


};

__declspec(selectany) unsigned int OETask::nRequestID_ = 100000;

#endif // __OETASK_H__
