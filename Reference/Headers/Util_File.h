#pragma once

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


	/* 파일 존재 여부 */
	static const _bool IsExistFile(const string& path);

	/* 경로가 존재하는지 확인하고 없다면 생성 */
	static HRESULT CheckOrCreatePath(const string& path);

	/* 주어진 경로 내의 모든 폴더명을 읽어 반환한다. */
	static vector<string> GetAllFolderNames(const string& path);
	static vector<wstring> GetAllFolderNames(const wstring& path);

	static vector<string> GetAllFileNames(const string& path);
	static vector<wstring> GetAllFileNames(const wstring& path);

private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};

END