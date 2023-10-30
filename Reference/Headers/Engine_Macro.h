#ifndef Engine_Macro_h__
#define Engine_Macro_h__

#define	VK_MAX				0xff

/* Clone */
#define			CLONE_PIN_MAX_DIGIT 4

/* Msg */
#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(nullptr, TEXT(_message), L"System Message", MB_OK)
#endif


/* Region */
#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}
#define			USING(NAMESPACE)		using namespace NAMESPACE;

/* Color */
#define D3DCOLOR_ARGB(a,b,g,r) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

/* Dll */
#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif


/* Null Check */
#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);nreturn _return;}}


/* Failed Check */
#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); return _return;}


/* Singleton*/
#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static unsigned long DestroyInstance( void );

#define IMPLEMENT_SINGLETON(CLASSNAME)							\
		CLASSNAME*	CLASSNAME::m_pInstance = nullptr;			\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
			if(nullptr == m_pInstance) {						\
				m_pInstance = new CLASSNAME;					\
			}													\
			return m_pInstance;									\
		}														\
		unsigned long CLASSNAME::DestroyInstance( void )		\
		{														\
			unsigned long dwRefCnt = 0;							\
			if(nullptr != m_pInstance)							\
			{													\
				dwRefCnt = m_pInstance->Release();				\
				if(0 == dwRefCnt)								\
					m_pInstance = nullptr;						\
			}													\
			return dwRefCnt;									\
		}


/* Instance */
#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();

#define ENGINE_INSTANCE CEngineInstance::GetInstance()

#endif // Engine_Macro_h__
