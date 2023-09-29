#pragma once

/* '바이너리 파일' 입출력을 편하게 사용하기 위함 */

#include "Base.h"

BEGIN(Engine)

enum FileMode : uint8
{
	Write,
	Read,
};

class ENGINE_DLL Util_File final : public CBase
{
public:
	Util_File();
	~Util_File();

	void Open(wstring filePath, FileMode mode);

	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;

#ifdef _DEBUG
		assert(::WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
#else
		WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr);
#endif // _DEBUG

	}

	template<>
	void Write<string>(const string& data)
	{
		return Write(data);
	}

	void Write(void* data, uint32 dataSize);
	void Write(const string& data);

	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;

#ifdef _DEBUG
		assert(::ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
#else
		ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr);
#endif // _DEBUG

	}

	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}

	void Read(void** data, uint32 dataSize);
	void Read(OUT string& data);

	static const _bool IsExistFile(const string& path);

	static void CheckOrCreatePath(const string& path);

private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};

END