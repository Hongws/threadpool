#pragma once

#include "logs/log/LogInfo.h"
#include "ThreadPool_gai.h"
#include <atomic>
#include <queue>
#include <set>

#define THREADS_NUM             8
#define THREADS_NUM_SECOND      THREADS_NUM*2


typedef struct tagStruPerson
{
    int     nNo;
    char*   szName;       //这个需要new
    char    szID[20];
    std::string str;
    tagStruPerson()
    {
        nNo = 0;
        szName = nullptr;
        memset(szID, 0, 20);
    }
    ~tagStruPerson()
    {
        if (szName)
        {
            nNo = 0;
            delete [] szName;
            szName = nullptr;            
        }
    }
}StruPerson, *lpStruPerson;



class CThreadUse
{
public:
    CThreadUse();
    ~CThreadUse();

    void    CreateTaskThread();
    void    StopThread();

    void    DoTask(std::shared_ptr<StruPerson> struPerson);
    void    PushQueue();
    void    PushQueueEx();

    int     GetTaskThreadSize();
    int     GetQueueSize();

    void    AddQueue(std::shared_ptr<StruPerson> temp);

private:
    ThreadPool_XGM* m_pthreadxgm;
    std::atomic<bool>	m_bRun;

    std::queue<std::shared_ptr<StruPerson>> m_queueTaskInfo;
    //std::set<std::shared_ptr<StruPerson>> m_setTaskInfo;
    //std::set<int> m_setTaskInfo;
    //比对int string  不能比对struct char 需不需要去重
    std::set<std::string> m_setTaskInfo;
    std::mutex m_mutexQueue;
};

