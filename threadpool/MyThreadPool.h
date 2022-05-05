#pragma once

#include "logs/log/LogInfo.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
using namespace std;

#define EVENT


#define MAX_THREADS_NUM             100
#define MIX_THREADS_NUM             10
#define TASK_ADD_THREAD_RATE        4
#define TASK_SUB_THREAD_RATE        0.4


struct QueueCell
{
    char*   pbuf;
    int     bufsize;
    QueueCell()
    {
        pbuf = nullptr;
        bufsize = 0;
    }
    ~QueueCell()
    {
        if (pbuf)
        {
            delete[]pbuf;
            pbuf = NULL;
            bufsize = 0;
        }
    }
};

class MyThreadPool
{
public:
	MyThreadPool();
	~MyThreadPool();

	void	CreateWorkThread();
	void	StopWorkThread();
	void	WorkerThread(int threadID);

	int		AddTaskThreads(int nThreadNum);
	void	TaskProcThread();
    void    TaskProcThreadEx();
    void    DoTask(std::shared_ptr<QueueCell> cell);   //�����ص�

    void    PushBackMessageQueue(std::shared_ptr<QueueCell>& cell);
    int     PopFrontMessageQueue(std::shared_ptr<QueueCell>& cell);
    bool    WaitForTask(std::chrono::milliseconds millsec);
    bool    WaitForTaskEx();
    void    ClearMessageQueue();
    int     GetSizeMessageQueue();

    //��ȡ��ǰ���õ������߳�����
    int     GetSizeCurTask();

    void    StartEvent();

    double  GetThreadTaskRate();
    bool    ShouldEnd();

    void    TestPushQueue();

private:
	atomic<bool>	m_bRun;
    atomic<int>     m_nThreadUsing;     
    atomic<int>     m_nThreadEvent;     //�����¼����̼߳���
	int				m_nThreadNum;		//��ȡӲ��֧�ֲ�����
	thread*			m_pThreadWorker;
    mutex           m_QueueMutex;
    mutex           m_MutexDataTest;
    mutex           m_MutexCv;
    int             m_nDataTest;        //��������

    int             m_MaxThreadsNum;      
    int             m_MinThreadsNum;      
    double          m_TaskAddThreadRate;  
    double          m_TaskSubThreadRate;  

    HANDLE          m_hEvent;

    std::queue<std::shared_ptr<QueueCell>>  m_MessageQueue;
    std::condition_variable                 m_ConditPut;
    std::condition_variable                 m_cv;

};

