#include "ThreadPool.h"

CThreadPool::CThreadPool(const _uint& numThreads)
	: _numThreads(numThreads), _stop(FALSE)
{
	_threads.reserve(_numThreads);

	for (size_t i = 0; i < _numThreads; i++)
		_threads.emplace_back([this]() { this->Execute_Command(); });
}

CThreadPool::~CThreadPool()
{
	_stop = TRUE;

	_cv.notify_all();

	for (auto& thread : _threads)
		thread.join();
}

void CThreadPool::Execute_Command()
{
    while (true)
    {
        unique_lock<mutex> lock(_mutex);

        _cv.wait(lock, [this]() { return !this->_commands.empty() || _stop; });
       
        if (_stop && this->_commands.empty())
            return;

        function<void()> job = move(_commands.front()); 
        {
            _commands.pop();
            lock.unlock();

            job();
        }
    }
}
