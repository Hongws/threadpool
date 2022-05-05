#include "pch.h"
#include "ThreadUse.h"

CThreadUse::CThreadUse()
{
	InitLogInfo();
	InitMiniDumpInfo();
	m_pthreadxgm = nullptr;
	m_bRun = FALSE;
}

CThreadUse::~CThreadUse()
{
	StopThread();
}

void clearEx(std::queue<std::shared_ptr<StruPerson>>& q) {
	std::queue<std::shared_ptr<StruPerson>> empty;
	swap(empty, q);
}

void CThreadUse::StopThread()
{
	if (m_bRun)
	{
		m_bRun = FALSE;

		{
			std::lock_guard<std::mutex>    lk(m_mutexQueue);
			clearEx(m_queueTaskInfo);
			m_setTaskInfo.clear();
		}

		int nLimitTimes = 120;
		while (m_pthreadxgm->taskssize() && nLimitTimes)
		{
			nLimitTimes--;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

	}

	if (m_pthreadxgm)
	{
		delete m_pthreadxgm;
		m_pthreadxgm = nullptr;
	}
}


DWORD WINAPI DoTaskEx(std::shared_ptr<StruPerson>& struPerson, LPVOID lp = nullptr)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	WriteLogExA(__FILENAME__, DBG_LVL_INFO, "DoTaskEx[%d] Dotask:%d", GetCurrentThreadId(), struPerson->nNo);

	return 0;
}

void CThreadUse::DoTask(std::shared_ptr<StruPerson> struPerson)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	WriteLogExA(__FILENAME__, DBG_LVL_INFO, "taskthread[%d] Dotask:%d", GetCurrentThreadId(), struPerson->nNo);
}

void CThreadUse::CreateTaskThread()
{
	m_pthreadxgm = new ThreadPool_XGM(THREADS_NUM);
	m_bRun = TRUE;

	std::thread taskThread([this] {
		while (m_bRun)
		{
			{
				std::lock_guard<std::mutex>    lock(m_mutexQueue);
				if (m_queueTaskInfo.size() > 0)
				{
					if (m_pthreadxgm->taskssize() < THREADS_NUM_SECOND)
					{
						//auto struPerson = std::move(m_queueTaskInfo.front());
						auto struPerson = m_queueTaskInfo.front();
						m_queueTaskInfo.pop();
						//m_setTaskInfo.erase(struPerson);
						m_pthreadxgm->enqueue(&CThreadUse::DoTask, this, struPerson);

					}
				}
			}
			Sleep(1);
		}
	});
	taskThread.detach();
}

void CThreadUse::PushQueue()
{
	
	for (int i = 0; i < 100; i++)
	{
		auto temp = std::make_shared<StruPerson>();
		temp->nNo = i;
		temp->szName = new char[10];
		strcpy_s(temp->szName, 10, "123");
		strcpy_s(temp->szID, 20, "20220503");
		temp->str = "1111";

		//std::lock_guard<std::mutex>    lk(m_mutexQueue);
		//m_queueTaskInfo.push(temp);
		AddQueue(temp);
	}
}

void CThreadUse::PushQueueEx()
{
	for (int i = 50; i < 150; i++)
	{
		auto temp = std::make_shared<StruPerson>();
		temp->nNo = i;
		temp->szName = new char[10];
		strcpy_s(temp->szName, 10, "123");
		strcpy_s(temp->szID, 20, "20220503");
		temp->str = "1111";
		AddQueue(temp);
	}
}

int CThreadUse::GetTaskThreadSize()
{
	return m_pthreadxgm->taskssize();
}

int CThreadUse::GetQueueSize()
{
	std::lock_guard<std::mutex>    lk(m_mutexQueue);
	return m_queueTaskInfo.size();
}

void CThreadUse::AddQueue(std::shared_ptr<StruPerson> temp)
{
	//std::wstring str(strFilePath, dwPathLength);
	{
		std::lock_guard<std::mutex> lock(m_mutexQueue);
		if (m_setTaskInfo.count(temp->str) == 0)
		{
			m_queueTaskInfo.push(temp);
			m_setTaskInfo.insert(temp->str);
		}
	}
	//return 0;
}