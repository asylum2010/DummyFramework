//=============================================================================================================
#if !defined(_WIN32THREAD_H_) && defined(_WIN32)
#define _WIN32THREAD_H_

#include <windows.h>

namespace DummyFramework
{
	/**
	 * \brief Thread executor
	 */
	class CCallable
	{
	public:
		virtual ~CCallable() {}
		virtual void operator()() = 0;
	};

	/**
	 * \brief Thread object
	 */
	class CThread
	{
	private:
		HANDLE handle;
		unsigned long id;
		
		static unsigned long CALLBACK Run(void* param);

	public:
		CThread();
		~CThread();

		bool Attach(CCallable& obj);

		inline void Start() {
			ResumeThread(handle);
		}

		inline void Stop() {
			SuspendThread(handle);
		}

		inline void Close() {
			CloseHandle(handle);
		}

		inline int GetID() const {
			return id;
		}

		inline HANDLE GetHandle() const {
			return handle;
		}
	};
}

#endif
//=============================================================================================================
