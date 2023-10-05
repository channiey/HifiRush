#pragma once

#include "..\Default\pch.h"

enum FileMode : uint8
{
	Write,
	Read,
};

class Utils_File
{
public:
	Utils_File();
	~Utils_File();

	void Open(wstring filePath, FileMode mode);

	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;
		assert(::WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
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
		assert(::ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
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

	static const bool IsExistFile(const string& path);

	static void CheckOrCreatePath(const string& path);

	/* �־��� ��� ���� ��� �������� �о� ��ȯ�Ѵ�. */
	static vector<string> Get_AllFolderNames(const string& path);
	static vector<wstring> Get_AllFolderNames(const wstring& path);


private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};

