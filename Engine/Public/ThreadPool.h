#pragma once
#include "Base.h"
#include "EngineInstance.h"
#include "Component_Manager.h"
#include "GameObject.h"
#include "Component.h"
#include "Model.h"

BEGIN(Engine)
class ENGINE_DLL CThreadPool final : public CBase
{

public:
	CThreadPool(const _uint& numThreads);
	~CThreadPool();

    template <class F, class... Args>
    void Push_Command(F&& f, Args&&... args) 
    {
        using return_type = typename invoke_result<F(Args...)>::type;
        //using return_type = typename result_of<F(Args...)>::type;
        auto command = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));

        future<return_type> job_result_future = command->get_future();
        {
            lock_guard<mutex> lock(_mutex);
            _commands.push([command]() { (*command)(); });
        }

        _cv.notify_one();
    }

private:
	void					Execute_Command();

private:
	_uint					_numThreads;
	vector<thread>			_threads;
	queue<function<void()>> _commands; 

	condition_variable		_cv;
	mutex					_mutex;

	_bool					_stop;
};

END

