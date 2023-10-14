#pragma once

#include "..\Default\pch.h"

class CMainApp_Converter
{
public:
	CMainApp_Converter();
	virtual ~CMainApp_Converter();

public:
	HRESULT Start_Convert();

private:

	HRESULT Convert_Character();
	HRESULT	Convert_Env_Static();
	HRESULT Convert_Env_Anim();
	HRESULT	Convert_Weapon();

	HRESULT Convert_SelectiveModel();

private:

private:


};

