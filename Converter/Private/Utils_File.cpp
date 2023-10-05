#include "Utils_File.h"
#include "Utils_String.h"

Utils_File::Utils_File()
{

}

Utils_File::~Utils_File()
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
}


void Utils_File::Open(wstring filePath, FileMode mode)
{
	if (mode == FileMode::Write)
	{
		_handle = ::CreateFile(
			filePath.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}
	else
	{
		_handle = ::CreateFile
		(
			filePath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}

	assert(_handle != INVALID_HANDLE_VALUE);
}


void Utils_File::Write(void* data, uint32 dataSize)
{
	uint32 numOfBytes = 0;
	assert(::WriteFile(_handle, data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void Utils_File::Write(const string& data)
{
	uint32 size = (uint32)data.size();
	Write(size);

	if (data.size() == 0)
		return;

	Write((void*)data.data(), size);
}

void Utils_File::Read(void** data, uint32 dataSize)
{
	uint32 numOfBytes = 0;
	assert(::ReadFile(_handle, *data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void Utils_File::Read(OUT string& data)
{
	uint32 size = Read<uint32>();

	if (size == 0)
		return;

	char* temp = new char[size + 1];
	temp[size] = 0;
	Read((void**)&temp, size);
	data = temp;
	delete[] temp;
}

const bool Utils_File::IsExistFile(const string& path)
{
	auto p = filesystem::path(path);

	if (filesystem::exists(p))
		return true;

	return false;
}

void Utils_File::CheckOrCreatePath(const string& path)
{
	/* 2단계 위까지 검사한다. */

	auto p = filesystem::path(path);
	filesystem::create_directory(p.parent_path().parent_path()); 
	filesystem::create_directory(p.parent_path());
}

vector<string> Utils_File::Get_AllFolderNames(const string& path)
{
	vector<std::string> folderNames;
	try
	{
		for (const auto& entry : filesystem::directory_iterator(path))
		{
			if (filesystem::is_directory(entry))
			{
				folderNames.push_back(entry.path().filename().string());
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return folderNames;
}

vector<wstring> Utils_File::Get_AllFolderNames(const wstring& path)
{
	vector<wstring> folderNames;
	try
	{
		for (const auto& entry : filesystem::directory_iterator(path))
		{
			if (filesystem::is_directory(entry))
			{
				folderNames.push_back(Utils_String::ToWString(entry.path().filename().string()));
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return folderNames;
}
