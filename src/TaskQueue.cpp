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


#include "TaskQueue.h"

#include <thread>

OETaskQueue::OETaskQueue() {
}


OETaskQueue::~OETaskQueue() {
}

//插入队列 
void OETaskQueue::put_back(std::shared_ptr<OETask> task) {
	std::unique_lock<std::mutex> lock(mutex_);

	queue_.push_back(task);
	
	conditPut_.notify_one();
}
void OETaskQueue::put_front(std::shared_ptr<OETask> task) {
	std::unique_lock<std::mutex> lock(mutex_);

	queue_.push_front(task);

	conditPut_.notify_one();
}
//获取队首（并将任务加到运行任务列表中）
std::shared_ptr<OETask> OETaskQueue::get(void) {
	std::unique_lock<std::mutex> lock(mutex_);
	if (queue_.empty()) {
		return nullptr;
	}

	std::shared_ptr<OETask> task = queue_.front();
	mapDoingTask_.insert(make_pair(task->getID(), task));
	queue_.pop_front();

	return task;
}

//获取大小
size_t OETaskQueue::size(void) {
	std::unique_lock<std::mutex> lock(mutex_);
	return queue_.size();
}

//释放队列
void OETaskQueue::release(void) {
	//将就绪队列中的任务删除
	std::unique_lock<std::mutex> lock(mutex_);
	if (!queue_.empty()) {
		queue_.clear();//清空
	}

	if (!mapDoingTask_.empty()) {
		// std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
		auto it_map = mapDoingTask_.begin();
		for (; it_map != mapDoingTask_.end(); ++it_map) {
			it_map->second->setCancelRequired();
		}
	}

	conditPut_.notify_all();
}

//删除任务（从就绪队列删除，如果就绪队列没有，则看执行队列有没有，有的话置下取消状态位）
int OETaskQueue::deleteTask(int nID) {
	std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
	lock.lock();
	// std::deque<std::shared_ptr<OETask>>::iterator
	auto it = queue_.begin();
	for (; it != queue_.end(); ++it) {
		if ((*it)->getID() == nID) {
			queue_.erase(it);
			lock.unlock();
			return 0;
		}
	}

	// std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
	auto it_map = mapDoingTask_.find(nID);
	if (it_map != mapDoingTask_.end()) {
		it_map->second->setCancelRequired();
	}
	//下面的逻辑可能会造成死锁，这里要及时释放
	lock.unlock();
	//任务执行完后再返回
	while (mapDoingTask_.count(nID)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	return 0;
}

//删除所有任务
int OETaskQueue::deleteAllTasks(void) {
	std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
	lock.lock();
	if (!queue_.empty()) {
		queue_.clear();//清空
	}

	if (!mapDoingTask_.empty()) {
		// std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
		auto it_map = mapDoingTask_.begin();
		for (; it_map != mapDoingTask_.end(); ++it_map) {
			it_map->second->setCancelRequired();
		}
	}
	//下面的逻辑可能会造成死锁，这里要及时释放
	lock.unlock();
	//任务执行完后再返回
	while (mapDoingTask_.empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}

//任务完成回调（从运行列表中删除指定任务）
int OETaskQueue::onTaskFinished(int nID) {
	std::unique_lock<std::mutex> lock(mutex_);
	// std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
	auto it_map = mapDoingTask_.find(nID);
	if (it_map != mapDoingTask_.end()) {
		mapDoingTask_.erase(it_map);
	}
	return 0;
}

//判断任务是否执行完毕
std::shared_ptr<OETask> OETaskQueue::isTaskProcessed(int nId) {
	std::unique_lock<std::mutex> lock(mutex_);
	// std::deque<std::shared_ptr<OETask>>::iterator
	auto it = queue_.begin();
	for (; it != queue_.end(); ++it) {
		if ((*it)->getID() == nId) {
			return *it;
		}
	}

	// std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
	auto it_map = mapDoingTask_.find(nId);
	if (it_map != mapDoingTask_.end()) {
		return it_map->second;
	}

	return nullptr;
}

//等待有任务到达（带超时）
bool OETaskQueue::wait(std::chrono::milliseconds millsec) {
	std::unique_lock<std::mutex> lock(mutex_);
	conditPut_.wait_for(lock, millsec);

	return true;
}