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


#ifndef __OETASKQUEUE_H__
#define __OETASKQUEUE_H__

#include "Task.h"
#include <deque>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>

/// 任务队列类
class OETaskQueue
{
public:
	OETaskQueue();
	~OETaskQueue();

private:
	/// 就绪任务队列
	std::deque<std::shared_ptr<OETask> > queue_;
	/// 运行任务map
	std::unordered_map<int, std::shared_ptr<OETask>> mapDoingTask_;
	/// 互斥量
	std::mutex mutex_;
	/// 条件变量
	std::condition_variable conditPut_;

public:
	/**
	* @brief ：向队列的末尾插入任务
	* @param ：task 任务类
	*/
	void put_back(std::shared_ptr<OETask> task);
	/**
	* @brief ：向队列的头部插入任务
	* @param ：task 任务类
	*/
	void put_front(std::shared_ptr<OETask> task);
	/**
	* @brief ：获取队首（并将任务加到运行任务列表中）
	* @return：任务类
	*/
	std::shared_ptr<OETask> get(void);
	/**
	* @brief ：获取整个双向链表的大小
	* @return：大小
	*/
	size_t size(void);

	/**
	* @brief ：释放队列
	*/
	void release(void);
	/**
	* @brief ：删除任务（从就绪队列删除，如果就绪队列没有，则看执行队列有没有，有的话置下取消状态位）
	* @param ：nID 任务的编号
	* @return：成功返回0 失败返回非0
	*/
	int deleteTask(int nID);
	/**
	* @brief ：删除所有任务
	* @return：成功返回0 失败返回非0
	*/
	int deleteAllTasks(void);
	/**
	* @brief ：任务完成回调（从运行列表中删除指定任务）
	* @param ：nID 任务的编号
	* @return：成功返回0 失败返回非0
	*/
	int onTaskFinished(int nID);
	/**
	* @brief ：判断任务是否执行完毕
	* @param ：nID 任务的编号
	* @return：任务类
	*/
    std::shared_ptr<OETask> isTaskProcessed(int nId);

	/**
	* @brief ：等待有任务到达（带超时：超时自动唤醒）
	* @param ：millsec 超时时间（毫秒）
	* @return：成功返回true 失败返回false
	*/
	bool wait(std::chrono::milliseconds millsec);

};

#endif // __OETASKQUEUE_H__