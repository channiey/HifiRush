#pragma once

#include "..\Default\pch.h"

#include "asTypes.h"

class CConverter
{

public:
	CConverter();
	virtual ~CConverter();

public: /* 1 ���� ���������� �о� �޸𸮿� �ø���. */
	HRESULT Read_AssetFile(wstring filePath, const MODEL_TYPE modelType);

public: /* 2. �޸� ��, �����ϰ� ���� �����͸� ���Ϸ� �����Ѵ�. */
	HRESULT Export_MeshData(wstring savePath);
	HRESULT Export_MaterialData(wstring savePath);


private:
	/* Meshes */
	HRESULT Read_MeshData();
	HRESULT Write_MeshData(wstring savePath);

	/* Materials */
	HRESULT Read_MaterialData();
	HRESULT Write_MaterialData(wstring savePath);


private:
	shared_ptr<Assimp::Importer>	m_pImporter;
	const aiScene*					m_pAiScene = { nullptr };

	MODEL_TYPE						m_eModelType = { MODEL_TYPE::TYPEEND };

	vector<shared_ptr<asMesh>>		m_Meshes;


private:
	wstring m_assetPath = L"../Bin/Resources/Assets/";
	wstring m_savePath	= L"../Bin/Resources/Fbx/";


};

