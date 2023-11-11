#include "..\Default\stdafx.h"
#include "State_Blader_Base.h"

CState_Blader_Base::CState_Blader_Base()
{
}

CState_Blader_Base::CState_Blader_Base(const CState_Blader_Base& rhs)
{
}

HRESULT CState_Blader_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pBlader = dynamic_cast<CBlader*>(pOwner);

	if (nullptr == m_pBlader)
		return E_FAIL;

	m_pModel = m_pBlader->Get_Model();

	if (nullptr == m_pModel)
		return E_FAIL;

	return S_OK;
}

const _float CState_Blader_Base::Get_Distance()
{
	return Vec4::Distance(m_pBlader->Get_Transform()->Get_FinalPosition(),
		m_pBlader->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition());
}

void CState_Blader_Base::Look_Target()
{
	if (!m_pModel->Is_Tween())
	{
		const _float fRotConstMax = 66.f;
		const _float fRotConstNormal = 50.f;

		Vec4 vLook, vDir, vRotDir;

		vLook = m_pBlader->Get_Transform()->Get_Forward();
		vDir = m_pBlader->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
			- m_pBlader->Get_Transform()->Get_FinalPosition();

		vLook.y = 0.f;
		vLook.z = 0.f;

		vDir.y = 0.f;
		vDir.w = 0.f;

		vLook.Normalize();
		vDir.Normalize();


		if (3.f < acos(vDir.Dot((vLook))))
			vRotDir = Vec4::Lerp(vLook, vDir, fRotConstMax);
		else
			vRotDir = Vec4::Lerp(vLook, vDir, fRotConstNormal * 0.5f);


		m_pBlader->Get_Transform()->Set_Look(vDir);
	}
}

CState* CState_Blader_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Base::Free()
{
	__super::Free();
}
