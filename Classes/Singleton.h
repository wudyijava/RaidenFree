#pragma once

template <typename T>
class Singleton
{
public:
	static T *getInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new (T)();
			atexit(destory);
		}

		return m_pInstance;
	}

	static void destory()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static T* volatile m_pInstance;
};

template <typename T>
T* volatile Singleton<T>::m_pInstance = nullptr;
