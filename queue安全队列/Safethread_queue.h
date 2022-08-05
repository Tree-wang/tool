
/*
	FileName: Threadsafe_queue.h
*/


#pragma once

//�϶���������queue������������������
#include <queue>
#include <mutex>
#include <condition_variable>

namespace NM_Tools {
	//ģ����
	template <typename T>
	class Threadsafe_queue
	{
	public:


		
		//���
		void Push(T value)
		{
			std::unique_lock<std::mutex> lk(m_mutex);
			m_queue.push(value);
			lk.unlock();
			m_cond.notify_one();
		}

		//�ȴ���ֱ�������ݣ�Ȼ�����
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

		//���Գ���
		bool Try_pop(T& value)
		{
			//��ǰ�治ͬ���ǣ�����Ҫ����wait
			std::lock_guard<std::mutex> lk(m_mutex);
			if (m_queue.empty())
				return false;
			value = m_queue.front();
			m_queue.pop();
			return true;
		}

		//�ж��Ƿ�Ϊ��
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

