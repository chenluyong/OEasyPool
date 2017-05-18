#ifndef __OETASKQUEUE_H__
#define __OETASKQUEUE_H__

#include "Task.h"
#include <deque>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>

/// ���������
class OETaskQueue
{
public:
	OETaskQueue();
	~OETaskQueue();

private:
	/// �����������
	std::deque<std::shared_ptr<OETask> > queue_;
	/// ��������map
	std::unordered_map<int, std::shared_ptr<OETask>> mapDoingTask_;
	/// ������
	std::mutex mutex_;
	/// ��������
	std::condition_variable conditPut_;

public:
	/**
	* @brief ������е�ĩβ��������
	* @param ��task ������
	*/
	void put_back(std::shared_ptr<OETask> task);
	/**
	* @brief ������е�ͷ����������
	* @param ��task ������
	*/
	void put_front(std::shared_ptr<OETask> task);
	/**
	* @brief ����ȡ���ף���������ӵ����������б��У�
	* @return��������
	*/
	std::shared_ptr<OETask> get(void);
	/**
	* @brief ����ȡ����˫������Ĵ�С
	* @return����С
	*/
	size_t size(void);

	/**
	* @brief ���ͷŶ���
	*/
	void release(void);
	/**
	* @brief ��ɾ�����񣨴Ӿ�������ɾ���������������û�У���ִ�ж�����û�У��еĻ�����ȡ��״̬λ��
	* @param ��nID ����ı��
	* @return���ɹ�����0 ʧ�ܷ��ط�0
	*/
	int deleteTask(int nID);
	/**
	* @brief ��ɾ����������
	* @return���ɹ�����0 ʧ�ܷ��ط�0
	*/
	int deleteAllTasks(void);
	/**
	* @brief ��������ɻص����������б���ɾ��ָ������
	* @param ��nID ����ı��
	* @return���ɹ�����0 ʧ�ܷ��ط�0
	*/
	int onTaskFinished(int nID);
	/**
	* @brief ���ж������Ƿ�ִ�����
	* @param ��nID ����ı��
	* @return��������
	*/
    std::shared_ptr<OETask> isTaskProcessed(int nId);

	/**
	* @brief ���ȴ������񵽴����ʱ����ʱ�Զ����ѣ�
	* @param ��millsec ��ʱʱ�䣨���룩
	* @return���ɹ�����true ʧ�ܷ���false
	*/
	bool wait(std::chrono::milliseconds millsec);

};

#endif // __OETASKQUEUE_H__