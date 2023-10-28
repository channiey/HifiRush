#include "MainApp_Converter.h"
#include "Converter.h"
#include "ConverterCustom.h"

CMainApp_Converter::CMainApp_Converter()
{
}

CMainApp_Converter::~CMainApp_Converter()
{
}

HRESULT CMainApp_Converter::Start_Convert()
{
	cout << "Start Binarization\n\n\n\n";
	{
		/*if (FAILED(Convert_Character()))
			return E_FAIL;

		/*if (FAILED(Convert_Env_Static()))
			return E_FAIL;

		if (FAILED(Convert_Env_Anim()))
			return E_FAIL;

		if (FAILED(Convert_Weapon()))
			return E_FAIL;
		
		if (FAILED(Convert_SelectiveModel()))
			return E_FAIL;*/

		if (FAILED(Convert_Custom()))
			return E_FAIL;

	}
	cout << "\n\n\nComplete Entire Binarization";
	int temp(getchar());//Sleep(10000);
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Character()
{
	const MODEL_TYPE	type = MODEL_TYPE::ANIM;

	const string		filePath	= "../Assets/Character";
	vector<string>		fileNames	= Utils_File::Get_AllFolderNames(filePath);
	const string		savePath	= "Character/";

	for (string& name : fileNames)
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(name, savePath + name, type)))
			ASSERT_LOG();
	}
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Env_Static()
{
	const MODEL_TYPE	type = MODEL_TYPE::STATIC;

	const string		filePath = "../Assets/Environment/Static";
	vector<string>		fileNames = Utils_File::Get_AllFolderNames(filePath);
	const string		savePath = "Environment/Static/";

	for (string& name : fileNames)
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(name, savePath + name, type)))
			ASSERT_LOG();
	}
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Env_Anim()
{
	const MODEL_TYPE	type = MODEL_TYPE::ANIM;

	const string		filePath = "../Assets/Environment/Anim";
	vector<string>		fileNames = Utils_File::Get_AllFolderNames(filePath);
	const string		savePath = "Environment/Anim/";

	for (string& name : fileNames)
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(name, savePath + name, type)))
			ASSERT_LOG();
	}
	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Weapon()
{
	const MODEL_TYPE	type = MODEL_TYPE::STATIC;

	const string		filePath = "../Assets/Weapon/";
	vector<string>		fileNames = Utils_File::Get_AllFolderNames(filePath);
	const string		savePath = "Weapon/";

	for (string& name : fileNames)
	{
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(name, savePath + name, type)))
			ASSERT_LOG();
	}
	return S_OK;

}


HRESULT CMainApp_Converter::Convert_SelectiveModel()
{
	/*{
		const MODEL_TYPE	type = MODEL_TYPE::STATIC;
		const string		savePath = "Environment/Static/";
		const string		fileName = "Container_Closed_Blue";
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(fileName, savePath + fileName, type)))
			ASSERT_LOG();
	}

	{
		const MODEL_TYPE	type = MODEL_TYPE::STATIC;
		const string		savePath = "Environment/Static/";
		const string		fileName = "Container_Closed_Green";
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(fileName, savePath + fileName, type)))
			ASSERT_LOG();
	}

	{
		const MODEL_TYPE	type = MODEL_TYPE::STATIC;
		const string		savePath = "Environment/Static/";
		const string		fileName = "Container_Closed_Yellow";
		shared_ptr<CConverter> converter = make_shared<CConverter>();
		if (FAILED(converter->Binarize_Model(fileName, savePath + fileName, type)))
			ASSERT_LOG();
	}*/

	return S_OK;
}

HRESULT CMainApp_Converter::Convert_Custom()
{

	const MODEL_TYPE	type = MODEL_TYPE::ANIM;
	const string		savePath = "Character/Chai/";
	const string		fileName = "Chai";
	vector<string>		fileMeshesPath;

	fileMeshesPath.push_back("Bini");
	fileMeshesPath.push_back("Top_Hood");
	fileMeshesPath.push_back("Pants");
	fileMeshesPath.push_back("Shoes_Running");
	fileMeshesPath.push_back("Arm"); /* 이거 순서 바뀌면 매태리얼 쪽 다시 순서 맞춰줘야 한다.*/
	//fileMeshesPath.push_back("Glasses");




	//fileMeshesPath.push_back("Inner");



	//fileMeshesPath.push_back("Scalf");
	//fileMeshesPath[6] = "Shoes";
	//fileMeshesPath[8] = "Top_Baseball";


	shared_ptr<CConverterCustom> converter = make_shared<CConverterCustom>();
	if (FAILED(converter->Binarize_Model(fileName, fileMeshesPath, savePath, type)))
		ASSERT_LOG();

	return S_OK;
}
