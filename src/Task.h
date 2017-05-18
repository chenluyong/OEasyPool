#ifndef __OETASK_H__
#define __OETASK_H__

#include <atomic>

// 任务基类
class OETask
{

protected:

    // 任务的唯一标识
    int id_;

private:
    static int nRequestID_;
    // 任务取消状态
    std::atomic<bool>  bIsCancelRequired_;

public:
	OETask() :id_(nRequestID_++), bIsCancelRequired_(false) {};
	virtual ~OETask() {};

public:
	virtual int doWork() = 0;

	// 任务已取消回调
	virtual int onCanceled(){ return 1; }
	// 任务已完成
	virtual int onCompleted(int code){ return 1; }

	// 获取ID
	int getID(){ return id_; }
	//设置ID
	void setID(int nID){ nRequestID_ = nID; }
	// 获取任务取消状态
	bool isCancelRequired(){ return bIsCancelRequired_; }
	// 设置任务取消状态
	void setCancelRequired(){ bIsCancelRequired_ = true; }


};

__declspec(selectany) int OETask::nRequestID_ = 100000;

#endif // __OETASK_H__