
/*
	FileName: Threadsafe_queue.h
*/


#pragma once

//肯定基于容器queue、条件变量、互斥量
#include <queue>
#include <mutex>
#include <condition_variable>

namespace NM_Tools {
	//模板编程
	template <typename T>
	class Threadsafe_queue
	{
	public:


		
		//入队
		void Push(T value)
		{
			std::unique_lock<std::mutex> lk(m_mutex);
			m_queue.push(value);
			lk.unlock();
			m_cond.notify_one();
		}

		//等待，直到有数据，然后出队
		void Wait_for_pop(T& value)
		{
			std::unique_lock<std::mutex> lk(m_mutex);
			m_cond.wait(lk, [this]
				{
					return !m_queue.empty();
				});
			value = m_queue.front();
			m_queue.pop();
		}

		//尝试出队
		bool Try_pop(T& value)
		{
			//与前面不同的是，不需要阻塞wait
			std::lock_guard<std::mutex> lk(m_mutex);
			if (m_queue.empty())
				return false;
			value = m_queue.front();
			m_queue.pop();
			return true;
		}

		//判断是否为空
		bool IsEmpty() const
		{
			std::lock_guard<std::mutex> lk(m_mutex);
			return m_queue.empty();
		}
	private:
		std::mutex m_mutex;
		std::condition_variable m_cond;
		std::queue<T> m_queue;
	};
}

