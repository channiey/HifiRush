#include "StringUtils.h"

bool StringUtils::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool StringUtils::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

void StringUtils::Replace(OUT string& str, string comp, string rep)
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

void StringUtils::Replace(OUT wstring& str, wstring comp, wstring rep)
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

const wstring StringUtils::Combine(const wstring& str1, const wstring& str2)
{
	return str1 + str2;
}

const wstring StringUtils::Remove_LastNumChar(const wstring& str, const _uint& iNumCutWord)
{
	size_t iLength = str.length();

	if (iLength <= iNumCutWord) 
		return L"";

	wstring res = str.substr(0, iLength - iNumCutWord);
	
	return res;
}

const wstring StringUtils::Get_LastNumChar(const wstring& str, const _uint& iNumCutWord)
{
	size_t iLength = str.length();

	if (iLength <= iNumCutWord) 
		return str;

	wstring res = str.substr(iLength - iNumCutWord);
	
	return res;
}


const _bool& StringUtils::Is_Digit(const wstring& str)
{
	/* atoi�� �����ϰ�� 0, ������ ��� �ش� ���ڸ� ��ȯ�Ѵ�. */
	/* ������ ���� 0�� ��쿡�� 0�� ��ȯ�ϹǷ� �߰� ������ �Ǵ�. */
	/* compare�� ���� ���ٸ� 0, �ٸ��� 1�� �����Ѵ�. */
	
	if (ToString(str).compare("0") == 0
		|| ToString(str).compare("00") == 0
		|| ToString(str).compare("000") == 0
		|| ToString(str).compare("0000") == 0) /* ���� ���� 0, 00, 000�� ��� true*/
		return true;
	else if (atoi(ToString(str).c_str()) != 0) /* ���� atoi����� 0�� �ƴ� ��� true */
		return true;
	else
		return false;
}

const _bool StringUtils::Has_ClonePin(const wstring& str)
{
	/* ���� ��ȣ �տ� _�� �پ������Ƿ� �ڸ����� 1�� �����Ѵ�. */
	_bool b = Is_Digit(Get_LastNumChar(str, CLONE_PIN_MAX_DIGIT - 1));

	return b;
}

std::wstring StringUtils::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string StringUtils::ToString(wstring value)
{
	return string(value.begin(), value.end());
}
