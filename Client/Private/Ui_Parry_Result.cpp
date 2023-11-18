//#include "..\Default\stdafx.h"
//#include "..\Public\Parry_Rhythm.h"
//
//#include "EngineInstance.h"
//#include "ImGui_Manager.h"
//
//CParry_Rhythm::CParry_Rhythm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//	: CUi(pDevice, pContext)
//{
//
//}
//
//CParry_Rhythm::CParry_Rhythm(const CGameObject& rhs)
//	: CUi(rhs)
//{
//
//}
//
//HRESULT CParry_Rhythm::Initialize_Prototype()
//{
//	return S_OK;
//}
//
//HRESULT CParry_Rhythm::Initialize(void* pArg)
//{
//	if (FAILED(__super::Initialize(pArg)))
//		return E_FAIL;
//
//	if (FAILED(Ready_Components()))
//		return E_FAIL;
//
//	if (FAILED(Load()))
//		return E_FAIL;
//
//	/* memcpy하면 주소 복사라 누적된다. */
//	m_TextureLocalDescOrigin.resize(m_TextureLocalDesc.size());
//	for (size_t i = 0; i < m_TextureLocalDesc.size(); i++)
//		m_TextureLocalDescOrigin[i] = m_TextureLocalDesc[i];
//
//	return S_OK;
//}
//
//void CParry_Rhythm::Tick(_double fTimeDelta)
//{
//	__super::Tick(fTimeDelta);
//}
//
//void CParry_Rhythm::LateTick(_double fTimeDelta)
//{
//	__super::LateTick(fTimeDelta);
//}
//
//HRESULT CParry_Rhythm::Render()
//{
//	if (FAILED(__super::Render()))
//		return E_FAIL;
//
//	Matrix matWorldOrigin = m_pTransformCom->Get_WorldMat();
//
//	const size_t iSize = (size_t)m_pTextureComs.size();
//	for (size_t i = 0; i < iSize; i++)
//	{
//		if (nullptr == m_pTextureComs[i]) continue;
//
//		/* 텍스처의 로컬 정보를 Ui 월드로 올린다. */
//		{
//			const Matrix matNewWorld = Matrix::CreateScale(m_TextureLocalDesc[i].vSize.x, m_TextureLocalDesc[i].vSize.y, 1.f)
//				* Matrix::CreateTranslation(m_TextureLocalDesc[i].vPos.x, m_TextureLocalDesc[i].vPos.y, 0.f)
//				* matWorldOrigin;
//
//			m_pTransformCom->Set_WorldMat(matNewWorld);
//
//			if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
//				return E_FAIL;
//		}
//
//		if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
//			return E_FAIL;
//
//		if (i == FULL_HEALTH)
//			m_pShaderCom->Begin(1);
//		else
//			m_pShaderCom->Begin(0);
//
//		m_pVIBufferCom->Render();
//	}
//
//	m_pTransformCom->Set_WorldMat(matWorldOrigin);
//
//	return S_OK;
//}
//
//HRESULT CParry_Rhythm::Ready_Components()
//{
//	/* Vector Resize */
//	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
//
//	/* Load Texture */
//	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_GuiterBody_DropShadow"),
//		TEXT("Com_Texture_DROP_LOGO"), (CComponent**)&m_pTextureComs[TEX_TYPE::DROP_LOGO])))
//		return E_FAIL;
//
//	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HealthBar_DropShadow_10"),
//		TEXT("Com_Texture_DROP_HEALTH"), (CComponent**)&m_pTextureComs[TEX_TYPE::DROP_HEALTH])))
//		return E_FAIL;
//
//	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_GuiterBack"),
//		TEXT("Com_Texture_LOGO"), (CComponent**)&m_pTextureComs[TEX_TYPE::LOGO])))
//		return E_FAIL;
//
//	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HealthBar_Bg_10"),
//		TEXT("Com_Texture_EMPTY_HEALTH"), (CComponent**)&m_pTextureComs[TEX_TYPE::EMPTY_HEALTH])))
//		return E_FAIL;
//
//	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HealthBar_Bar_10"),
//		TEXT("Com_Texture_FULL_HEALTH"), (CComponent**)&m_pTextureComs[TEX_TYPE::FULL_HEALTH])))
//		return E_FAIL;
//
//	return S_OK;
//}
//
//HRESULT CParry_Rhythm::Bind_ShaderResources()
//{
//	return S_OK;
//}
//
//void CParry_Rhythm::Drop(_double fTimeDelta)
//{
//	if (!m_tLerpDesc.bActive)
//	{
//		if (!m_bMax)
//			m_tLerpDesc.Start(0.f, 10.f, CBeatManager::GetInstance()->Get_SPB() * 0.2f, LERP_MODE::EASE_OUT);
//		else
//			m_tLerpDesc.Start(10.f, 0.f, CBeatManager::GetInstance()->Get_SPB() * 0.8f, LERP_MODE::EASE_OUT);
//
//		m_bMax = !m_bMax;
//	}
//
//	m_tLerpDesc.Update(fTimeDelta);
//
//	m_TextureLocalDesc[TEX_TYPE::DROP_LOGO].vPos.x = m_TextureLocalDescOrigin[TEX_TYPE::DROP_LOGO].vPos.x + m_tLerpDesc.fCurValue;
//	m_TextureLocalDesc[TEX_TYPE::DROP_LOGO].vPos.y = m_TextureLocalDescOrigin[TEX_TYPE::DROP_LOGO].vPos.y - m_tLerpDesc.fCurValue;
//
//	m_TextureLocalDesc[TEX_TYPE::DROP_HEALTH].vPos.x = m_TextureLocalDescOrigin[TEX_TYPE::DROP_HEALTH].vPos.x + m_tLerpDesc.fCurValue;
//	m_TextureLocalDesc[TEX_TYPE::DROP_HEALTH].vPos.y = m_TextureLocalDescOrigin[TEX_TYPE::DROP_HEALTH].vPos.y - m_tLerpDesc.fCurValue;
//}
//
//void CParry_Rhythm::Set_Player()
//{
//	CGameObject* pObj = ENGINE_INSTANCE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_PLAYER]);
//	if (nullptr != pObj)
//	{
//		m_pPlayer = dynamic_cast<CCharacter*>(pObj);
//	}
//}
//
//CParry_Rhythm* CParry_Rhythm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//{
//	CParry_Rhythm* pInstance = new CParry_Rhythm(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created : CParry_Rhythm");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CParry_Rhythm* CParry_Rhythm::Clone(void* pArg)
//{
//	CParry_Rhythm* pInstance = new CParry_Rhythm(*this);
//
//	if (FAILED(pInstance->Initialize(pArg)))
//	{
//		MSG_BOX("Failed to Cloned : CParry_Rhythm");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//void CParry_Rhythm::Free()
//{
//	__super::Free();
//}
