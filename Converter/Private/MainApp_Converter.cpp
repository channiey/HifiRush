#include "MainApp_Converter.h"
#include "Converter.h"

static string g_srcPath = "../Assets/";
static string g_destPath = "../../Client/Bin/Resources/Models/";

CMainApp_Converter::CMainApp_Converter()
{
}

CMainApp_Converter::~CMainApp_Converter()
{
}

HRESULT CMainApp_Converter::Start_Convert()
{
	if (FAILED(Convert_Player()))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Player()
{
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model("Chai", "Character/Chai", MODEL_TYPE::ANIM)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Monster()
{
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Npc()
{
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Env()
{
	return S_OK;
}

