#include "Utils_String.h"

bool Utils_String::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool Utils_String::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

void Utils_String::Replace(OUT string& str, string comp, string rep)
{
	string temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

void Utils_String::Replace(OUT wstring& str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

std::wstring Utils_String::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string Utils_String::ToString(wstring value)
{
	return string(value.begin(), value.end());
}

string Utils_String::RemoveFileExtension(string str)
{
	size_t first_not_dot = str.find_first_not_of('.');
	size_t last_dot = str.find_last_of(".");
	if (last_dot == std::string::npos
		|| last_dot <= first_not_dot)
	{
		return str;
	}
	return str.substr(0, last_dot);
}

wstring Utils_String::RemoveFileExtension(wstring str)
{
	string path = ToString(str);

	size_t first_not_dot = path.find_first_not_of('.');
	size_t last_dot = path.find_last_of(".");
	if (last_dot == std::string::npos
		|| last_dot <= first_not_dot)
	{
		return ToWString(path);
	}
	return ToWString(path.substr(0, last_dot));
}

