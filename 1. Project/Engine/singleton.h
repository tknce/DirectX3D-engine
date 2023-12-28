#pragma once



template<typename T>
class CSingleton
{
private:
	static T* g_Mgr;

public:
	static T* GetInst()
	{
		if (nullptr == g_Mgr)
		{
			g_Mgr = new T;
			void(*pFunc)() = &T::Destroy;
			atexit(pFunc);
		}
		return g_Mgr;
	}

	static void Destroy()
	{
		if (nullptr != g_Mgr)
		{
			delete g_Mgr;
			g_Mgr = nullptr;
		}
	}

protected:
	CSingleton() {}
	CSingleton(const T& ref) {}
	CSingleton& operator=(const T& ref) {}
	~CSingleton(){}
};

template<typename T>
T* CSingleton<T>::g_Mgr = nullptr;