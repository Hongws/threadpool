#include "pch.h"
#include "MyThreadPool.h"

void clear(queue<std::shared_ptr<QueueCell>>& q) {
	queue<std::shared_ptr<QueueCell>> empty;
	swap(empty, q);
}

MyThreadPool::MyThreadPool()
{
	InitLogInfo();
	InitMiniDumpInfo();

	m_bRun = false;
	m_nThreadNum = 0;
	m_pThreadWorker = nullptr;
	m_nThreadUsing = 0;
	m_nDataTest = 0;
	m_nThreadEvent = 0;
	m_hEvent = NULL;

	clear(m_MessageQueue);
}

MyThreadPool::~MyThreadPool()
{
	StopWorkThread();
		
}

void MyThreadPool::DoTask(std::shared_ptr<QueueCell> cell)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	WriteLogExA(__FILENAME__, DBG_LVL_INFO, "taskthread[%d] Dotask:%d", GetCurrentThreadId(), cell->bufsize);
}

void MyThreadPool::TaskProcThread()
{	
	m_nThreadUsing.fetch_add(1);
	int iRet = 0;
	std::chrono::milliseconds mills_sleep(1);
	
	while (m_bRun)
	{		
		//WriteLogExA(__FILENAME__, DBG_LVL_INFO, "taskthread[%d] start", GetCurrentThreadId());
		
		std::shared_ptr<QueueCell> cell;
		iRet = PopFrontMessageQueue(cell);
		if (iRet == -1)
		{
			WaitForTask(mills_sleep);			//这里相当于Sleep(1);有可能更短
			continue;			
		}

		DoTask(cell);
		WriteLogExA(__FILENAME__, DBG_LVL_INFO, "taskthread[%d] end", GetCurrentThreadId());

		if (ShouldEnd())
		{
			break;
		}
	}
	m_nThreadUsing.fetch_sub(1);
}

void MyThreadPool::TaskProcThreadEx()
{
	m_nThreadUsing.fetch_add(1);
	int iRet = 0;
	std::chrono::milliseconds mills_sleep(1);

	while (m_bRun)
	{
		//WriteLogExA(__FILENAME__, DBG_LVL_INFO, "taskthread[%d] start", GetCurrentThreadId());
		if (0 == GetSizeMessageQueue())
		{
			WaitForTaskEx();
		}

		std::shared_ptr<QueueCell> cell;
		int iRet = PopFrontMessageQueue(cell);
		if (-1 != iRet)
			DoTask(cell);
		WriteLogExA(__FILENAME__, DBG_LVL_INFO, "taskthread[%d] end", GetCurrentThreadId());

		if (ShouldEnd())
		{
			break;
		}
	}
	m_nThreadUsing.fetch_sub(1);
}

int MyThreadPool::AddTaskThreads(int nThreadNum)
{
	try
	{
		for (int i = 0; i < nThreadNum; i++)
		{
			//小区别 两种方法
			//thread(&MyThreadPool::TaskProcThread, this).detach();
			thread(&MyThreadPool::TaskProcThreadEx, this).detach();
		}
	}
	catch (...)
	{
		WriteLogExA(__FILENAME__, DBG_LVL_ERROR, "addtaskthreads fail!");
		return -1;
	}

	return 0;
}

void MyThreadPool::WorkerThread(int threadID)
{
	while (m_bRun)
	{
		WriteLogExA(__FILENAME__, DBG_LVL_INFO, "workthread working[%d]...", threadID);

		//有一个信号 进程用事件  进程内用事件或者条件变量  
#ifdef EVENT
		WaitForSingleObject(m_hEvent, INFINITE);
#else
		std::unique_lock<std::mutex> lkcv(m_MutexCv);
		m_cv.wait(lkcv);
		lkcv.unlock();
#endif

		if (!m_bRun)
			break;

		//std::unique_lock<std::mutex> lk(m_MutexDataTest);				
		auto temp = std::make_shared<QueueCell>();
		temp->bufsize = m_nDataTest;
		PushBackMessageQueue(temp);
		WriteLogExA(__FILENAME__, DBG_LVL_INFO, "get envet m_nDataTest[%d]", m_nDataTest++);

		//lk.unlock();

		double  rate = GetThreadTaskRate();
		if (m_nThreadUsing < m_MaxThreadsNum
			&& rate > m_TaskAddThreadRate)
		{
			AddTaskThreads(1);
		}
	}
}

void MyThreadPool::CreateWorkThread()
{
	//为了得到最大利用率 一般cpu*2
	m_nThreadNum = thread::hardware_concurrency()/**2*/;
	if (0 == m_nThreadNum)
	{
		WriteLogExA(__FUNCTION__, DBG_LVL_ERROR, "this hardware don't support!");
		return;
	}
	
	WriteLogExA(__FILENAME__, DBG_LVL_INFO, "workthread start");

	//创建事件
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_bRun = true;
	m_pThreadWorker = new thread[m_nThreadNum];
	for (int i = 0; i < m_nThreadNum; i++)
	{
		m_pThreadWorker[i] = thread(&MyThreadPool::WorkerThread, this, i);
	}

	m_MaxThreadsNum = MAX_THREADS_NUM;
	m_MinThreadsNum = MIX_THREADS_NUM;
	m_TaskAddThreadRate = TASK_ADD_THREAD_RATE;
	m_TaskSubThreadRate = TASK_SUB_THREAD_RATE;

	AddTaskThreads(m_MinThreadsNum);
}

void MyThreadPool::StopWorkThread()
{
	if (m_bRun)
	{
		m_bRun = false;

#ifdef EVENT
		for (int i = 0; i < m_nThreadNum; i++)
			SetEvent(m_hEvent);
#else
		m_cv.notify_all();
#endif		

		for (int i = 0; i < m_nThreadNum; i++)
		{
			if (m_pThreadWorker[i].joinable())
			{
				m_pThreadWorker[i].join();
			}
		}
		if (m_pThreadWorker)
		{
			delete[]m_pThreadWorker;
			m_pThreadWorker = nullptr;
		}
		ClearMessageQueue();
				
		int nLimitTimes = 120;
		while (m_nThreadUsing && nLimitTimes)
		{
			nLimitTimes--;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		int nTemp = m_nThreadUsing;
		WriteLogExA(__FUNCTION__, DBG_LVL_INFO, "ThreadUsing:%d", nTemp);

		nLimitTimes = 120;
		while (m_nThreadEvent && nLimitTimes)
		{
			nLimitTimes--;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}		
	}
	if (m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}

void MyThreadPool::PushBackMessageQueue(std::shared_ptr<QueueCell>& cell)
{
	std::lock_guard<std::mutex>    lk(m_QueueMutex);
	m_MessageQueue.push(cell);
	m_ConditPut.notify_one();
}

int MyThreadPool::PopFrontMessageQueue(std::shared_ptr<QueueCell>& cell)
{
	std::lock_guard<std::mutex>    lk(m_QueueMutex);
	if (m_MessageQueue.size() > 0)
	{
		cell = m_MessageQueue.front();
		m_MessageQueue.pop();
	}
	else
	{
		return -1;
	}
	return 0;
}

bool MyThreadPool::WaitForTask(std::chrono::milliseconds millsec)
{
	std::unique_lock<std::mutex> lk(m_QueueMutex);
	m_ConditPut.wait_for(lk, millsec);
	return true;
}

bool MyThreadPool::WaitForTaskEx()
{
	std::unique_lock<std::mutex> lk(m_QueueMutex);
	m_ConditPut.wait(lk);
	return true;
}


void MyThreadPool::ClearMessageQueue()
{
	std::lock_guard<std::mutex>		lk(m_QueueMutex);
	m_ConditPut.notify_all();
	clear(m_MessageQueue);
}

int MyThreadPool::GetSizeMessageQueue()
{
	std::lock_guard<std::mutex>		lk(m_QueueMutex);	
	return m_MessageQueue.size();
}

int MyThreadPool::GetSizeCurTask()
{
	return m_nThreadUsing;
}

void MyThreadPool::StartEvent()
{
	std::thread startEvetThread([this]{
		m_nThreadEvent.fetch_add(1);
		for (int i = 0; i < 10000; i++)
		{
			WriteLogExA(__FILENAME__, DBG_LVL_INFO, "chufa Event id[%d]", i);
#ifdef EVENT
			SetEvent(m_hEvent);
#else
			m_cv.notify_one();

#endif // EVENT			
		}
		m_nThreadEvent.fetch_sub(1);
	});
	startEvetThread.detach();
}

double MyThreadPool::GetThreadTaskRate()
{
	std::lock_guard<std::mutex>    lk(m_QueueMutex);
	if (m_nThreadUsing != 0)
	{
		return m_MessageQueue.size() * 1.0 / m_nThreadUsing;
	}
	return 0;
}

bool MyThreadPool::ShouldEnd()
{
	bool bFlag = false;
	double dbThreadTaskRate = GetThreadTaskRate();
	if (!m_bRun || (m_nThreadUsing > m_MinThreadsNum &&
		dbThreadTaskRate < m_TaskSubThreadRate))
	{
		bFlag = true;
	}
	return bFlag;
}

void MyThreadPool::TestPushQueue()
{
	for (int i = 0; i < 10000; i++)
	{
		auto temp = std::make_shared<QueueCell>();
		temp->bufsize = i;
		PushBackMessageQueue(temp);
		WriteLogExA(__FILENAME__, DBG_LVL_INFO, "get envet m_nDataTest[%d]", i);

		double  rate = GetThreadTaskRate();
		if (m_nThreadUsing < m_MaxThreadsNum
			&& rate > m_TaskAddThreadRate)
		{
			AddTaskThreads(1);
		}
	}

}

