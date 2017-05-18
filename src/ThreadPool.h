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


#ifndef __OETHREADPOOL_H__
#define __OETHREADPOOL_H__



#include "Task.h"
#include "TaskQueue.h"



/// ���������
class OEThreadPool
{
public:
	/// �̳߳����ò���
	typedef struct tagThreadPoolConfig {
		int nMaxThreadsNum;		    /// ����߳�����
		int nMinThreadsNum;		    /// ��С�߳�����
		double dbTaskAddThreadRate;   /// �� ����߳������ (�����������߳�������ʲô������ʱ��ż�)
		double dbTaskSubThreadRate;   /// �� ��С�߳������ (�����������߳�������ʲô������ʱ��ż�)
	} ThreadPoolConfig;

private:
    /// �������
    OETaskQueue taskQueue_;

    /// �̳߳����ã������С�߳�����Ϊ1�����ʾ��Ҫһ����פ�Ĵ����̣߳�
    ThreadPoolConfig threadPoolConfig_;
    /// �̳߳��Ƿ�Ҫ�����
    std::atomic<bool> atcWorking_;
    /// ��ǰ�̸߳���
    std::atomic<int>  atcCurTotalThrNum_;
    /// ������
    std::mutex mutex_;

public:
	OEThreadPool(void);
	~OEThreadPool(void);

	/**
	* @brief ���̳߳���Դ���ó�ʼ��
	* @param ��config ��ʼ����������Ϣ
	* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
	*/
	int init(const ThreadPoolConfig& config);
	/**
	* @brief ���ͷ���Դ���ͷ��̳߳ء��ͷ�������У�
	* @return��true ִ�гɹ�  false ִ��ʧ��
	*/
	bool release(void);

	/**
	* @brief ���������
	* @param ��taskptr ������
	* @param ��priority �Ƿ����޴��� true�����ȴ���
	* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
	*/
	int addTask(std::shared_ptr<OETask> taskptr, bool priority = false);

	/**
	* @brief ��ɾ�����񣨴Ӿ�������ɾ���������������û�У���ִ�ж�����û�У��еĻ�����ȡ��״̬λ��
	* @param ��nID ������
	* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
	*/
	int deleteTask(int nID);

	/**
	* @brief ��ɾ����������
	* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
	*/
	int deleteAllTasks(void);
	/**
	* @brief ���ж������Ƿ�ִ�����
	* @param ��nID ������
	* @return��ִ����ϣ�ִ���귵��null�����򷵻�����ָ��
	*/
    std::shared_ptr<OETask> isTaskProcessed(int nId);

private:
	/**
	* @brief ����ȡ��ǰ�߳������
	* @return���߳������
	*/
	double getThreadTaskRate(void);
	/**
	* @brief ����ǰ�߳��Ƿ���Ҫ����
	* @return��true:���Խ��� false:�����Խ���
	* @note  ���ѿ��ǵ���С�߳�����
	*/
	bool shouldEnd(void);
	/**
	* @brief �����ָ�������Ĵ����߳� 
	* @param ��nThreadsNum ��ӵ��߳�����
	* @return��0 ִ�гɹ�  ��0 ִ��ʧ��
	*/
	int addProThreads(int nThreadsNum);
	/**
	* @brief ���ͷ��̳߳�
	* @return��true ִ�гɹ�  false ִ��ʧ��
	*/
	bool releaseThreadPool(void);
	/**
	* @brief ���������̺߳���
	*/
	void taskProcThread(void);

};

//extern OEThreadPool SystemThreadPool;

#endif // __OETHREADPOOL_H__