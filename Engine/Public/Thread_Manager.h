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

	/* 반환값을 받지 않는 버전 */
	void Add_Command(std::function<void()> job)
	{
		if (m_bStopAll) 
			throw std::runtime_error("ThreadPool 사용 중지됨");
		
		std::lock_guard<std::mutex> lock(m_mutexCommand);
		m_queueCommand.push(std::move(job));

		m_cvCommand.notify_one();
	}

	void Finish_MultiThreading(); // 단지 Join 하는 역할

private:
	vector<thread>			m_vecThread;
	/* 전체 쓰레드 개수 */
	size_t					m_iThreadNum = 0;	
	/* 쓰레드들이 처리해야 할 명령 큐 */
	/* 모든 쓰레드들이 접근하므로 조건변수와 뮤텍스로 보호한다.*/
	queue<function<void()>> m_queueCommand;		
	/* 명령 큐에 대한 조건 변수 */
	condition_variable		m_cvCommand;		
	/* 명령 큐에 대한 뮤텍스 */
	mutex					m_mutexCommand;		
	/* 모든 쓰레드를 종료하기 위한 변수*/
	_bool					m_bStopAll = FALSE;

private:
	/* thread 클래스는 생성시 태스크 함수를 부여해야 한다. */
	/* 위 용도의 함수이고, 이 함수의 역할은 큐에 있는 작업을 꺼내 처리한다. */
	void					Execute_Command();

public:
	virtual void			Free() override;
};

END




//template <class F, class... Args>
//void Add_Command(F&& f, Args&&... args)
//{
//	/* 클라이언트에서는 함수를 쓰레드 매니저에게 던지게 된다. 이때 반환값이 다양할 수 있다. */
//	/* 또한 내가 직접 수정하기 어려운 함수도 있을 수 있기에 반환값을 받는 버전으로 간다. */

//	if (m_bStopAll) return;

//	/* 편의상 재정의 */
//	/* C++14 'return_type' -> C++20 'invoke_result' */
//	using return_type = typename invoke_result<F(Args...)>::type;

//	/* forward는 레퍼런스 전달을 위해 사용한다. */
//	/* packaged_task는 비동기적으로 실행되는 함수의 리턴값을 받아내기 위해 사용한다. */
//	/* packaged_task의 생성자는 함수만을 받기 때문에 인자들을 bind한다. */
//	auto command = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));

//	future<return_type> future_Command_Result = command->get_future(); // command의 실행 결과를 보관한다.
//	{
//		/* 뮤텍스 락 - lock_guard는 지역변수이므로 소멸자 호출시 자동으로 뮤텍스 해제*/
//		lock_guard<mutex> lockGuard(m_mutexCommand);

//		/* 명령 큐에 명령을 추가한다.*/
//		m_queueCommand.push([command]() {(*command)(); });
//	}

//	/* 대기 중인 쓰레드 하나를 활성화 한다.*/
//	m_cvCommand.notify_one();

//	/* 이후 위 쓰레드가 명령을 수행한다. Execute_Command() */
//	/* 해당 명령령의 리턴값은 future_Command_Result에 저장되고, 이는 풀 사용자가 접근할 수 있다. */
//	
//	//return S_OK;
//}














// 반환값 받는 버전 

///* 쓰레드가 수행해야할 명령을 추가한다. */
//template <class F, class... Args>
//future<typename result_of<F(Args...)>::type> Add_Command(F&& f, Args&&... args)
//{
//	/* 임의 형태의 함수와 매개변수를 받고, 리턴 값을 보관하는 feture를 리턴한다. */
//	/* 리턴 값 형식 = future<typename result_of<F(Args...)>::type> */
//	/* template <class F, class... Args>은 가변길이 템플릿 형식이다. */
//	/* 따라서 위 구문은 std:future<함수 f의 리턴 타입>Add_Command()*/
//
//	if (m_bStopAll)	throw runtime_error("Error : All Thread Finished");
//
//	/* 편의상 재정의 */
//	using return_type = typename result_of<F(Args...)>::type;
//	/* forward는 레퍼런스 전달을 위해 사용한다. */
//	/* packaged_task는 비동기적으로 실행되는 함수의 리턴값을 받아내기 위해 사용한다. */
//	/* packaged_task의 생성자는 함수만을 받기 때문에 인자들을 bind한다. */
//	auto command = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));
//
//	future<return_type> future_Command_Result = command->get_future(); // command의 실행 결과를 보관한다.
//	{
//		/* 뮤텍스 락 - lock_guard는 지역변수이므로 소멸자 호출시 자동으로 뮤텍스 해제*/
//		lock_guard<mutex> lockGuard(m_mutexCommand);
//
//		/* 명령 큐에 명령을 추가한다.*/
//		m_queueCommand.push([command]() {(*command)(); })
//	}
//
//	/* 대기 중인 쓰레드 하나를 활성화 한다.*/
//	m_cvCommand->notify_one();
//
//	/* 이후 위 쓰레드가 명령을 수행한다. Execute_Command() */
//	/* 해당 명령령의 리턴값은 future_Command_Result에 저장되고, 이는 풀 사용자가 접근할 수 있다. */
//
//	return future_Command_Result;
//}

