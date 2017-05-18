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


#include "TaskQueue.h"

#include <thread>

OETaskQueue::OETaskQueue() {
}


OETaskQueue::~OETaskQueue() {
}

//������� 
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
//��ȡ���ף���������ӵ����������б��У�
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

//��ȡ��С
size_t OETaskQueue::size(void) {
	std::unique_lock<std::mutex> lock(mutex_);
	return queue_.size();
}

//�ͷŶ���
void OETaskQueue::release(void) {
	//�����������е�����ɾ��
	std::unique_lock<std::mutex> lock(mutex_);
	if (!queue_.empty()) {
		queue_.clear();//���
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

//ɾ�����񣨴Ӿ�������ɾ���������������û�У���ִ�ж�����û�У��еĻ�����ȡ��״̬λ��
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
	//������߼����ܻ��������������Ҫ��ʱ�ͷ�
	lock.unlock();
	//����ִ������ٷ���
	while (mapDoingTask_.count(nID)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	return 0;
}

//ɾ����������
int OETaskQueue::deleteAllTasks(void) {
	std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
	lock.lock();
	if (!queue_.empty()) {
		queue_.clear();//���
	}

	if (!mapDoingTask_.empty()) {
		// std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
		auto it_map = mapDoingTask_.begin();
		for (; it_map != mapDoingTask_.end(); ++it_map) {
			it_map->second->setCancelRequired();
		}
	}
	//������߼����ܻ��������������Ҫ��ʱ�ͷ�
	lock.unlock();
	//����ִ������ٷ���
	while (mapDoingTask_.empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}

//������ɻص����������б���ɾ��ָ������
int OETaskQueue::onTaskFinished(int nID) {
	std::unique_lock<std::mutex> lock(mutex_);
	// std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
	auto it_map = mapDoingTask_.find(nID);
	if (it_map != mapDoingTask_.end()) {
		mapDoingTask_.erase(it_map);
	}
	return 0;
}

//�ж������Ƿ�ִ�����
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

//�ȴ������񵽴����ʱ��
bool OETaskQueue::wait(std::chrono::milliseconds millsec) {
	std::unique_lock<std::mutex> lock(mutex_);
	conditPut_.wait_for(lock, millsec);

	return true;
}