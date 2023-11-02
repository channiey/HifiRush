//#include "Node_Action.h"
//#include "Node_Wait.h"
//
//CNode_Wait::CNode_Wait()
//{
//}
//
//CNode_Wait::CNode_Wait(const CNode_Wait& rhs)
//{
//}
//
//HRESULT CNode_Wait::Initialize_Node(const _float& fTime)
//{
//	if (FAILED(__super::Initialize_Node()))
//		return E_FAIL;
//
//	m_fLimitAcc = fTime;
//
//	m_eType = NODE_TYPE::ACTION;
//
//	return S_OK;
//}
//
//const NODE_STATE CNode_Wait::Evaluate(const _float& fTimeDelta)
//{
//	if (NODE_STATE::SUCCESS == m_eState)
//		return m_eState;
//
//	m_fAcc += fTimeDelta;
//
//	if (m_fLimitAcc <= m_fAcc)
//	{
//		m_eState = NODE_STATE::SUCCESS;
//		m_fAcc = 0.f;
//	}
//	else
//		m_eState = NODE_STATE::RUNNING;
//
//
//	return m_eState;
//}
//
//HRESULT CNode_Wait::Reset_Node()
//{
//	__super::Reset_Node();
//
//	m_fAcc = 0.f;
//
//	return S_OK;
//}
//
//CNode_Wait* CNode_Wait::Create(const _float& fTime)
//{
//	CNode_Wait* pInstance = new CNode_Wait();
//
//	if (FAILED(pInstance->Initialize_Node(fTime)))
//	{
//		MSG_BOX("Failed to Created : CNode_Wait");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//void CNode_Wait::Free()
//{
//	__super::Free();
//}
