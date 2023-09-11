#include "Thread_Manager.h"

IMPLEMENT_SINGLETON(CThread_Manager)


CThread_Manager::CThread_Manager()
{
}


HRESULT CThread_Manager::Set_MultiThreading(const _uint& iThreadNum)
{
	// 스레드를 생성 및 시작한다 (시작시에는 태스크 목록이 비어있기 때문에, 모든 쓰레드는 대기상태로 전환된다)

	if (!m_bStopAll || !m_vecThread.empty()) return E_FAIL;

	m_bStopAll = FALSE;

	if (!m_vecThread.empty()) m_vecThread.clear();
	
	m_iThreadNum = iThreadNum;

	m_vecThread.reserve(m_iThreadNum);

	for (size_t i = 0; i < m_iThreadNum; ++i)
		m_vecThread.emplace_back([this]() {this->Execute_Command(); });

	/*
		thread 클래스는 생성시 태스크를 부여해야 한다.

		WorkerThread() 함수는 thread 생성시 넣어줄 함수이고, 실질적인 작업 함수는 큐에 들어있다.

		WorkerThread() 함수는 큐에 있는 작업을 꺼내 수행하는 역할을 한다.

	*/
	return S_OK;
}

void CThread_Manager::Finish_MultiThreading()
{
	m_bStopAll = TRUE;

	/* 모든 쓰레드를 종료시키기 위해 모두 대기상태가 아닌 활성 사태로 전환한다. */
	m_cvCommand.notify_all();

	/* 모든 쓰레드가 종료될 때 까지 호출 쓰레드를 대기 상태로 둔다.*/
	for (auto& thread : m_vecThread)
		thread.join();

	/* 벡터를 초기화 한다. */
	m_vecThread.clear(); 

	/* 여기서 실제로 쓰레드 풀이 소멸한다. - 모든 커맨드 처리 완료 */
}

void CThread_Manager::Execute_Command()
{
	/* 쓰레드 매니저가 Set 된 타이밍에는 명령 큐가 비어 있기 때문에 모든 쓰레드는 대기상태로 전환된다. */
	/* 명령 큐에 명령이 들어오기까지 대기하다가 명령이 들어오면 notify를 통해 활성화 된 뒤 명령을 수핸한다. */

	while (TRUE)
	{
		unique_lock<mutex> mutex(m_mutexCommand);

		/* 명령이 없거나 스탑일 경우 뮤텍스를 언락 하고 대기상태로 전환한다. */
		/* cv.wait()은 조건이 ture가 될 때까지 현재 스레드를 대기상태로 둔다. */
		/* 즉 조건이 FALSE라면 뮤텍스를 언락한 뒤 대기 상태로 전환 된다.*/
		m_cvCommand.wait(mutex, [this]() {return !this->m_queueCommand.empty() || m_bStopAll; });

		/* 쓰레드의 종료 조건 : 스탑이 설정되고, 명령 큐가 비어있다면*/
		if (m_bStopAll && this->m_queueCommand.empty())
			return;

		/* 실질적인 작업 수행, 복사 생성자를 호출할 수 없기 때문에 move 사용 */
		function<void()> command = move(m_queueCommand.front());

		m_queueCommand.pop();
		mutex.unlock();

		command();
	}
}

void CThread_Manager::Free()
{
	__super::Free();
}
