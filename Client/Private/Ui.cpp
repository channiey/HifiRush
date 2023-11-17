#include "..\Default\stdafx.h"
#include "..\Public\Ui.h"

#include "EngineInstance.h"

CUi::CUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CUi::CUi(const CGameObject& rhs)
	: CGameObject(rhs)
{

}

HRESULT CUi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 직교 투영을 위한 변수 세팅 (윈도우 좌표 기준 크기, 중점) */
	{
		_float fPosX = g_iWinSizeX * 0.5f;
		_float fPosY = g_iWinSizeY * 0.5f;

		m_pTransformCom->Set_Scale(Vec3(1.f, 1.f, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(fPosX - g_iWinSizeX * 0.5f, -fPosY + g_iWinSizeY * 0.5f, 0.f, 1.f));
	}
	
	/* UI는 별도의 뷰 행렬과 투영행렬로 그린다. */
	{
		m_ViewMatrix = Matrix::Identity;
		m_ProjMatrix = Matrix::CreateOrthographic(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);
	}

	return S_OK;
}

void CUi::Tick(_double fTimeDelta)
{

}

void CUi::LateTick(_double fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUi::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi::Save()
{
	wstring strName = Util_String::Remove_LastNumChar(m_strName, CLONE_PIN_MAX_DIGIT);

	const wstring strSaveFilePath = UiFilePath + strName + L".bin";

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(strSaveFilePath, FileMode::Write);

	file->Write<size_t>(m_TextureLocalDesc.size());
	for (auto desc : m_TextureLocalDesc)
	{
		file->Write<Vec2>(desc.vPos);
		file->Write<Vec2>(desc.vSize);
	}

	return S_OK;
}

HRESULT CUi::Load()
{
	const wstring strLoadFilePath = UiFilePath + m_strName + L".bin";

	if (!Util_File::IsExistFile(Util_String::ToString(strLoadFilePath)))
	{
		MSG_BOX("Nothing Trigger File");
		return E_FAIL;
	}

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(strLoadFilePath, FileMode::Read);

	const size_t iSize = file->Read<size_t>();
	for (size_t i = 0; i < iSize; i++)
	{
		m_TextureLocalDesc[i].vPos = file->Read<Vec2>();
		m_TextureLocalDesc[i].vSize = file->Read<Vec2>();
	}

	return S_OK;
}

HRESULT CUi::Bind_ShaderResources()
{
	/*if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;*/

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	return S_OK;
}

void CUi::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

	for (auto& pTextureCom : m_pTextureComs)
		Safe_Release(pTextureCom);

}
