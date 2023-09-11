#include "Thread_Manager.h"

IMPLEMENT_SINGLETON(CThread_Manager)


CThread_Manager::CThread_Manager()
{
}


HRESULT CThread_Manager::Set_MultiThreading(const _uint& iThreadNum)
{
	// �����带 ���� �� �����Ѵ� (���۽ÿ��� �½�ũ ����� ����ֱ� ������, ��� ������� �����·� ��ȯ�ȴ�)

	if (!m_bStopAll || !m_vecThread.empty()) return E_FAIL;

	m_bStopAll = FALSE;

	if (!m_vecThread.empty()) m_vecThread.clear();
	
	m_iThreadNum = iThreadNum;

	m_vecThread.reserve(m_iThreadNum);

	for (size_t i = 0; i < m_iThreadNum; ++i)
		m_vecThread.emplace_back([this]() {this->Execute_Command(); });

	/*
		thread Ŭ������ ������ �½�ũ�� �ο��ؾ� �Ѵ�.

		WorkerThread() �Լ��� thread ������ �־��� �Լ��̰�, �������� �۾� �Լ��� ť�� ����ִ�.

		WorkerThread() �Լ��� ť�� �ִ� �۾��� ���� �����ϴ� ������ �Ѵ�.

	*/
	return S_OK;
}

void CThread_Manager::Finish_MultiThreading()
{
	m_bStopAll = TRUE;

	/* ��� �����带 �����Ű�� ���� ��� �����°� �ƴ� Ȱ�� ���·� ��ȯ�Ѵ�. */
	m_cvCommand.notify_all();

	/* ��� �����尡 ����� �� ���� ȣ�� �����带 ��� ���·� �д�.*/
	for (auto& thread : m_vecThread)
		thread.join();

	/* ���͸� �ʱ�ȭ �Ѵ�. */
	m_vecThread.clear(); 

	/* ���⼭ ������ ������ Ǯ�� �Ҹ��Ѵ�. - ��� Ŀ�ǵ� ó�� �Ϸ� */
}

void CThread_Manager::Execute_Command()
{
	/* ������ �Ŵ����� Set �� Ÿ�ֿ̹��� ��� ť�� ��� �ֱ� ������ ��� ������� �����·� ��ȯ�ȴ�. */
	/* ��� ť�� ����� ��������� ����ϴٰ� ����� ������ notify�� ���� Ȱ��ȭ �� �� ����� �����Ѵ�. */

	while (TRUE)
	{
		unique_lock<mutex> mutex(m_mutexCommand);

		/* ����� ���ų� ��ž�� ��� ���ؽ��� ��� �ϰ� �����·� ��ȯ�Ѵ�. */
		/* cv.wait()�� ������ ture�� �� ������ ���� �����带 �����·� �д�. */
		/* �� ������ FALSE��� ���ؽ��� ����� �� ��� ���·� ��ȯ �ȴ�.*/
		m_cvCommand.wait(mutex, [this]() {return !this->m_queueCommand.empty() || m_bStopAll; });

		/* �������� ���� ���� : ��ž�� �����ǰ�, ��� ť�� ����ִٸ�*/
		if (m_bStopAll && this->m_queueCommand.empty())
			return;

		/* �������� �۾� ����, ���� �����ڸ� ȣ���� �� ���� ������ move ��� */
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
