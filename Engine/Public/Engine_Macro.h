#pragma once

#ifndef		MSG_BOX
#define		MSG_BOX(_message)	MessageBox(nullptr, TEXT(_message), L"System Message", MB_OK)
#endif // !MSG_BOX

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define		USING(NAMESPACE)	using namesapce NAMESPACE;

#ifdef _DEBUG
#define CONSOLE_MSG(_message) cout << _message << endl;
#else
#define CONSOLE_MSG(_message) 
#endif

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif // ENGINE_EXPORTS

#define NULL_CHECK(_ptr) \
	{if(0 == _ptr) {__debugbreak(); return;}}

#define NULL_CHECK_RETURN( _ptr, _return) \
	{if(0 == _ptr) {__debugbreak(); return _return;}}

#define NULL_CHECK_MSG(_ptr, _message) \
	{if(0 == _ptr) {MSG_BOX(_message);__debugbreak();}}

#define NULL_CHECK_CONSOLE_MSG(_ptr, _message) \
	{if(0 == _ptr) {CONSOLE_MSG(_message);__debugbreak();}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message ) \
	{if (0 == _ptr) {MSG_BOX(_message); __debugbreak(); return _return;}}

#define NULL_CHECK_RETURN_CONSOLE_MSG( _ptr, _return, _message ) \
	{if (0 == _ptr) {CONSOLE_MSG(_message); __debugbreak(); return _return;}}

#define FAILED_CHECK(_hr) \
	{if ((HRESULT)_hr < 0) { __debugbreak(); return; }}

#define FAILED_CHECK_RETURN(_hr, _return) \
	{if ((HRESULT)_hr < 0) { __debugbreak(); return _return; }}

#define FAILED_CHECK_MSG(_hr, _message) \
	{if ((HRESULT)_hr < 0) { MSG_BOX(_message); __debugbreak(); return; }}

#define FAILED_CHECK_RETURN_MSG(_hr, _return, _message) \
	{if ((HRESULT)_hr < 0) { MSG_BOX(_message); __debugbreak(); return _return; }}

#define FAILED_CHECK_RETURN_CONSOLE_MSG(_hr, _return, _message) \
	{if ((HRESULT)_hr < 0) { CONSOLE_MSG(_message); __debugbreak(); return _return; }}

#define BOOL_CHECK(_hr) \
	{if ((bool)_hr == false) { __debugbreak(); return; }}

#define NO_COPY(CLASSNAME)										\
private:														\
	CLASSNAME(const CLASSNAME&) = delete;						\
	CLASSNAME& operator = (const CLASSNAME&) = delete;			

#define DECLARE_SINGLETON(CLASSNAME)		\
	NO_COPY(CLASSNAME)						\
	private:								\
	static CLASSNAME* m_pInstance;			\
	public:									\
	static CLASSNAME* GetInstance(void);	\
	static unsigned long DestroyInstance(void);


#define IMPLEMENT_SINGLETON(CLASSNAME)					\
	CLASSNAME*	CLASSNAME::m_pInstance = nullptr;		\
	CLASSNAME*	CLASSNAME::GetInstance(void) {			\
		if(nullptr == m_pInstance) {					\
			m_pInstance = new CLASSNAME;				\
		}												\
		return m_pInstance;								\
	}													\
	unsigned long CLASSNAME::DestroyInstance(void) {	\
		unsigned long dwRefCnt = { 0 };					\
		if (nullptr != m_pInstance)						\
		{												\
			dwRefCnt = m_pInstance->Release();			\
			if(0 == dwRefCnt)							\
				m_pInstance = { nullptr };				\
		}												\
		return dwRefCnt;								\
	}

#define ZeroStruct(Desc) ZeroMemory((&Desc), (sizeof(Desc)));