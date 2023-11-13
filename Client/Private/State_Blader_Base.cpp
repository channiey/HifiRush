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

void CState_Blader_Base::Turn_ToTarget()
{

}

void CState_Blader_Base::Look_Target()
{
	if (!m_pModel->Is_Tween())
	{
		const _float fRotConstNormal = 20.f * 0.016f;

		Vec4 vLook, vDir, vLerpDir;

		vLook = m_pBlader->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		vDir = m_pBlader->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
			- m_pBlader->Get_Transform()->Get_FinalPosition();

		vLook.y = vDir.y = 0.f;

		vLook.Normalize();
		vDir.Normalize();

		vLerpDir = Vec4::Lerp(vLook, vDir, fRotConstNormal);

		m_pBlader->Get_Transform()->Set_Look(vLerpDir);
	}
}

const _bool CState_Blader_Base::Is_On_Right(CTransform* pTargetTransform)
{
	Vec4 vLook = m_pBlader->Get_Transform()->Get_Forward();
	Vec4 vDirToTarget = pTargetTransform->Get_FinalPosition() - m_pBlader->Get_Transform()->Get_FinalPosition();
	
	const _float fDot = Vec3::Up.Dot(XMVector3Cross(vLook.xyz(), vDirToTarget.Normalized().xyz()));

	if (0.f <= fDot)
		return TRUE;

	return FALSE;
}

const _bool CState_Blader_Base::Is_On_Front(CTransform* pTargetTransform)
{
	Vec3 vLook = m_pBlader->Get_Transform()->Get_Forward().xyz();
	Vec4 vDirToTarget = pTargetTransform->Get_FinalPosition() - m_pBlader->Get_Transform()->Get_FinalPosition();

	const _float fDot = vLook.Dot(vDirToTarget.xyz().Normalized());

	if (0 < fDot) // ¾Õ 
		return TRUE;

	return FALSE;
}

CState* CState_Blader_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Base::Free()
{
	__super::Free();
}
