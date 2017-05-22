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
    virtual int onCompleted(int){ return 1; }

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
