#include "..\Public\Animation.h"
#include "Model.h"
#include "HierarchyNode.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation & rhs)
	: m_fDuration(rhs.m_fDuration)
	, m_Channels(rhs.m_Channels)
	, m_iNumChannels(rhs.m_iNumChannels)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_fPlayTime(rhs.m_fPlayTime)
{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

//HRESULT CAnimation::Initialize_Prototype(aiAnimation * pAIAnimation)
//{
//	m_fDuration = pAIAnimation->mDuration;
//	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;
//
//	/* ä�� ������ �����Ѵ�. */
//	m_iNumChannels = pAIAnimation->mNumChannels;
//	for (_uint i = 0; i < m_iNumChannels; ++i)
//	{
//		CChannel*		pChannel = CChannel::Create(pAIAnimation->mChannels[i]);
//		if (nullptr == pChannel)
//			return E_FAIL; 
//
//		/* Ư�� �ִϸ��̼� ������ ��, ��� ���� ���¸� �����ϴ� ���� �ƴ϶�(�̳� ������), �ش� �ִϸ��̼��� �����ϱ� ���� �ʿ��� ���븸 �������ֱ� ���� ä���� ��Ƶд�. */
//		m_Channels.push_back(pChannel);
//	}
//
//	return S_OK;
//}

HRESULT CAnimation::Initialize_Prototype(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels)
{
	m_fDuration = fDuration;
	m_fTickPerSecond = fTickPerSecond;

	/* ���Ϳ� ����� �� ä��� ��ī�� �ϸ� ū�ϳ���. */
	//memcpy(&m_Channels, &Channels, sizeof(Channels)); 

	m_Channels.reserve(Channels.size());
	for (auto& iter : Channels)
		m_Channels.push_back(iter);

	m_iNumChannels = (_int)m_Channels.size();

	return S_OK;
}

HRESULT CAnimation::Initialize(CModel* pModel)
{
	/* �ִϸ��̼��� ����ϱ� ���� ���Ǵ� ���� ��� �����Ѵ�.  */
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_ChannelKeyFrames.push_back(0);

		CHierarchyNode*		pNode = pModel->Get_HierarchyNode(m_Channels[i]->Get_Name().c_str());
		{
			if (nullptr == pNode)
				return E_FAIL;		

			m_HierarchyNodes.push_back(pNode);
		}
		Safe_AddRef(pNode);
	}

	return S_OK;
}


HRESULT CAnimation::Play_Animation(_float fTimeDelta)
{
	/* ���� ��� �ð��� ���Ѵ�. */
	m_fPlayTime += m_fTickPerSecond * fTimeDelta;

	/* �ִϸ��̼��� �����ٸ� */
	if (m_fPlayTime >= m_fDuration)
	{
		m_fPlayTime = 0.f;

		for (auto& pChannel : m_Channels)
		{
			for (auto& iCurrentKeyFrame : m_ChannelKeyFrames)
				iCurrentKeyFrame = 0;			
		}
	}

	/* �� �ִϸ��̼��� ��� ä���� Ű�������� �����Ѵ�. (���� �θ� ����)*/
	_uint		iChannelIndex = 0;
	for (auto& pChannel : m_Channels)
	{
		m_ChannelKeyFrames[iChannelIndex] = pChannel->Update_Transformation(m_fPlayTime, m_ChannelKeyFrames[iChannelIndex], m_HierarchyNodes[iChannelIndex]);

		++iChannelIndex;
	}

	return S_OK;
}

//CAnimation * CAnimation::Create(aiAnimation * pAIAnimation)
//{
//	CAnimation*			pInstance = new CAnimation();
//
//	if (FAILED(pInstance->Initialize_Prototype(pAIAnimation)))
//	{
//		MSG_BOX("Failed To Created : CAnimation");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CAnimation* CAnimation::Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize_Prototype(fDuration, fTickPerSecond, Channels)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}



CAnimation * CAnimation::Clone(CModel* pModel)
{
	CAnimation*			pInstance = new CAnimation(*this);

	if (FAILED(pInstance->Initialize(pModel)))
	{
		MSG_BOX("Failed To Cloned : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	/* Channel */
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();

	/* HierarachyNode */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	m_HierarchyNodes.clear();
}
