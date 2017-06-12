/**
* @file   : threadpool.h
* @brief  : 线程池类，主要解决多线程的管理问题
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


#ifndef __OETHREADPOOL_H__
#define __OETHREADPOOL_H__



#include "Task.h"
#include "TaskQueue.h"


// 1. 线程池的初始化  (init)
//  while:
//    1. 创建任务类
//    2. 添加任务到线程池当中  addTask
//    3. 线程池异步处理
// 2. 清理线程池资源  (release)

/// 任务管理类
class OEThreadPool
{
public:
	/// 线程池配置参数
	typedef struct tagThreadPoolConfig {
		int nMaxThreadsNum;		    /// 最大线程数量
		int nMinThreadsNum;		    /// 最小线程数量
		double dbTaskAddThreadRate;   /// 增 最大线程任务比 (任务数量与线程数量，什么比例的时候才加)
		double dbTaskSubThreadRate;   /// 减 最小线程任务比 (任务数量与线程数量，什么比例的时候才减)
	} ThreadPoolConfig;

private:
	/// 任务队列
	OETaskQueue taskQueue_;

	/// 线程池配置（如果最小线程数量为1，则表示需要一个常驻的处理线程）
	ThreadPoolConfig threadPoolConfig_;
	/// 线程池是否被要求结束
	std::atomic<bool> atcWorking_;
	/// 当前线程个数
	std::atomic<int>  atcCurTotalThrNum_;
	/// 互斥量
	std::mutex mutex_;

public:

	OEThreadPool(void);
	~OEThreadPool(void);

	/**
	* @brief ：线程池资源配置初始化
	* @param ：config 初始化的配置信息
	* @return：0 执行成功  非0 执行失败
	*/
	int init(const ThreadPoolConfig& config);
	/**
	* @brief ：释放资源（释放线程池、释放任务队列）
	* @return：true 执行成功  false 执行失败
	*/
	bool release(void);

	/**
	* @brief ：添加任务
	* @param ：taskptr 任务类
	* @param ：priority 是否有限处理 true：优先处理
	* @return：0 执行成功  非0 执行失败
	*/
	int addTask(std::shared_ptr<OETask> taskptr, bool priority = false);

	/**
	* @brief ：删除任务（从就绪队列删除，如果就绪队列没有，则看执行队列有没有，有的话置下取消状态位）
	* @param ：nID 任务编号
	* @return：0 执行成功  非0 执行失败
	*/
	int deleteTask(int nID);

	/**
	* @brief ：删除所有任务
	* @return：0 执行成功  非0 执行失败
	*/
    int deleteAllTasks(void);
	/**
	* @brief ：判断任务是否执行完毕
	* @param ：nID 任务编号
	* @return：执行完毕，执行完返回null，否则返回任务指针
	*/
    std::shared_ptr<OETask> isTaskProcessed(int nId);

private:
	/**
	* @brief ：获取当前线程任务比
	* @return：线程任务比
	*/
	double getThreadTaskRate(void);
	/**
	* @brief ：当前线程是否需要结束
	* @return：true:可以结束 false:不可以结束
	* @note  ：已考虑到最小线程数量
	*/
	bool shouldEnd(void);
	/**
	* @brief ：添加指定数量的处理线程 
	* @param ：nThreadsNum 添加的线程数量
	* @return：0 执行成功  非0 执行失败
	*/
	int addProThreads(int nThreadsNum);
	/**
	* @brief ：释放线程池
	* @return：true 执行成功  false 执行失败
	*/
	bool releaseThreadPool(void);
	/**
	* @brief ：任务处理线程函数
	*/
	void taskProcThread(void);

};

// extern OEThreadPool SystemThreadPool;

#endif // __OETHREADPOOL_H__