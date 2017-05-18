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