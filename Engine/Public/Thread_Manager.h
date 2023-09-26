#pragma once

#include "Base.h"
BEGIN(Engine)

class CThread_Manager final : public CBase
{
	DECLARE_SINGLETON(CThread_Manager)

private:
	CThread_Manager();
	virtual ~CThread_Manager() = default;

public: /* For.GameInstance */
	HRESULT Set_MultiThreading(const _uint& iThreadNum);

	/* ��ȯ���� ���� �ʴ� ���� */
	void Add_Command(std::function<void()> job)
	{
		if (m_bStopAll) 
			throw std::runtime_error("ThreadPool ��� ������");
		
		std::lock_guard<std::mutex> lock(m_mutexCommand);
		m_queueCommand.push(std::move(job));

		m_cvCommand.notify_one();
	}

	void Finish_MultiThreading(); // ���� Join �ϴ� ����

private:
	vector<thread>			m_vecThread;
	/* ��ü ������ ���� */
	size_t					m_iThreadNum = 0;	
	/* ��������� ó���ؾ� �� ��� ť */
	/* ��� ��������� �����ϹǷ� ���Ǻ����� ���ؽ��� ��ȣ�Ѵ�.*/
	queue<function<void()>> m_queueCommand;		
	/* ��� ť�� ���� ���� ���� */
	condition_variable		m_cvCommand;		
	/* ��� ť�� ���� ���ؽ� */
	mutex					m_mutexCommand;		
	/* ��� �����带 �����ϱ� ���� ����*/
	_bool					m_bStopAll = FALSE;

private:
	/* thread Ŭ������ ������ �½�ũ �Լ��� �ο��ؾ� �Ѵ�. */
	/* �� �뵵�� �Լ��̰�, �� �Լ��� ������ ť�� �ִ� �۾��� ���� ó���Ѵ�. */
	void					Execute_Command();

public:
	virtual void			Free() override;
};

END




//template <class F, class... Args>
//void Add_Command(F&& f, Args&&... args)
//{
//	/* Ŭ���̾�Ʈ������ �Լ��� ������ �Ŵ������� ������ �ȴ�. �̶� ��ȯ���� �پ��� �� �ִ�. */
//	/* ���� ���� ���� �����ϱ� ����� �Լ��� ���� �� �ֱ⿡ ��ȯ���� �޴� �������� ����. */

//	if (m_bStopAll) return;

//	/* ���ǻ� ������ */
//	/* C++14 'return_type' -> C++20 'invoke_result' */
//	using return_type = typename invoke_result<F(Args...)>::type;

//	/* forward�� ���۷��� ������ ���� ����Ѵ�. */
//	/* packaged_task�� �񵿱������� ����Ǵ� �Լ��� ���ϰ��� �޾Ƴ��� ���� ����Ѵ�. */
//	/* packaged_task�� �����ڴ� �Լ����� �ޱ� ������ ���ڵ��� bind�Ѵ�. */
//	auto command = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));

//	future<return_type> future_Command_Result = command->get_future(); // command�� ���� ����� �����Ѵ�.
//	{
//		/* ���ؽ� �� - lock_guard�� ���������̹Ƿ� �Ҹ��� ȣ��� �ڵ����� ���ؽ� ����*/
//		lock_guard<mutex> lockGuard(m_mutexCommand);

//		/* ��� ť�� ����� �߰��Ѵ�.*/
//		m_queueCommand.push([command]() {(*command)(); });
//	}

//	/* ��� ���� ������ �ϳ��� Ȱ��ȭ �Ѵ�.*/
//	m_cvCommand.notify_one();

//	/* ���� �� �����尡 ����� �����Ѵ�. Execute_Command() */
//	/* �ش� ��ɷ��� ���ϰ��� future_Command_Result�� ����ǰ�, �̴� Ǯ ����ڰ� ������ �� �ִ�. */
//	
//	//return S_OK;
//}














// ��ȯ�� �޴� ���� 

///* �����尡 �����ؾ��� ����� �߰��Ѵ�. */
//template <class F, class... Args>
//future<typename result_of<F(Args...)>::type> Add_Command(F&& f, Args&&... args)
//{
//	/* ���� ������ �Լ��� �Ű������� �ް�, ���� ���� �����ϴ� feture�� �����Ѵ�. */
//	/* ���� �� ���� = future<typename result_of<F(Args...)>::type> */
//	/* template <class F, class... Args>�� �������� ���ø� �����̴�. */
//	/* ���� �� ������ std:future<�Լ� f�� ���� Ÿ��>Add_Command()*/
//
//	if (m_bStopAll)	throw runtime_error("Error : All Thread Finished");
//
//	/* ���ǻ� ������ */
//	using return_type = typename result_of<F(Args...)>::type;
//	/* forward�� ���۷��� ������ ���� ����Ѵ�. */
//	/* packaged_task�� �񵿱������� ����Ǵ� �Լ��� ���ϰ��� �޾Ƴ��� ���� ����Ѵ�. */
//	/* packaged_task�� �����ڴ� �Լ����� �ޱ� ������ ���ڵ��� bind�Ѵ�. */
//	auto command = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));
//
//	future<return_type> future_Command_Result = command->get_future(); // command�� ���� ����� �����Ѵ�.
//	{
//		/* ���ؽ� �� - lock_guard�� ���������̹Ƿ� �Ҹ��� ȣ��� �ڵ����� ���ؽ� ����*/
//		lock_guard<mutex> lockGuard(m_mutexCommand);
//
//		/* ��� ť�� ����� �߰��Ѵ�.*/
//		m_queueCommand.push([command]() {(*command)(); })
//	}
//
//	/* ��� ���� ������ �ϳ��� Ȱ��ȭ �Ѵ�.*/
//	m_cvCommand->notify_one();
//
//	/* ���� �� �����尡 ����� �����Ѵ�. Execute_Command() */
//	/* �ش� ��ɷ��� ���ϰ��� future_Command_Result�� ����ǰ�, �̴� Ǯ ����ڰ� ������ �� �ִ�. */
//
//	return future_Command_Result;
//}

