#ifndef __OETASK_H__
#define __OETASK_H__

#include <atomic>

// �������
class OETask
{

protected:

    // �����Ψһ��ʶ
    int id_;

private:
    static int nRequestID_;
    // ����ȡ��״̬
    std::atomic<bool>  bIsCancelRequired_;

public:
	OETask() :id_(nRequestID_++), bIsCancelRequired_(false) {};
	virtual ~OETask() {};

public:
	virtual int doWork() = 0;

	// ������ȡ���ص�
	virtual int onCanceled(){ return 1; }
	// ���������
	virtual int onCompleted(int code){ return 1; }

	// ��ȡID
	int getID(){ return id_; }
	//����ID
	void setID(int nID){ nRequestID_ = nID; }
	// ��ȡ����ȡ��״̬
	bool isCancelRequired(){ return bIsCancelRequired_; }
	// ��������ȡ��״̬
	void setCancelRequired(){ bIsCancelRequired_ = true; }


};

__declspec(selectany) int OETask::nRequestID_ = 100000;

#endif // __OETASK_H__