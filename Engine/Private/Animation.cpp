#include "..\Public\Animation.h"
#include "Model.h"
#include "Bone.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation & rhs)
	: m_strName(rhs.m_strName)
	, m_fDuration(rhs.m_fDuration)
	, m_Channels(rhs.m_Channels) /* ������ ŭ, ���� �����ص� ��� ������ ��������� ���� */
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_fPlayTime(rhs.m_fPlayTime)
	, m_iMaxFrameCount(rhs.m_iMaxFrameCount)
{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

HRESULT CAnimation::Initialize_Prototype(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels, const string& strName)
{
	m_strName = strName;
	m_fDuration = fDuration;// / 60.f;
	m_fTickPerSecond = fTickPerSecond;
	
	/* ���Ϳ� ����� �� ä��� ��ī�� �ϸ� ū�ϳ���. */
	//memcpy(&m_Channels, &Channels, sizeof(Channels)); 

	/* ���� �� ȿ�����ϱ� */

	/*m_Channels.resize(Channels.size());
	memcpy(&m_Channels, &Channels, sizeof(Channels));*/

	m_Channels.reserve(Channels.size());
	for (auto& iter : Channels)
		m_Channels.push_back(iter);

	for (auto& pChannel : Channels)
		m_iMaxFrameCount = max(m_iMaxFrameCount, (_uint)pChannel->Get_KeyFrames().size());

	return S_OK;
}

HRESULT CAnimation::Initialize(CModel* pModel)
{
	/* �ִϸ��̼��� ����ϱ� ���� ���Ǵ� ���� ��� �����Ѵ�.  */
	for (_uint i = 0; i < m_Channels.size(); ++i)
	{
		m_ChannelKeyFrames.push_back(0);

		CBone*	pBone = pModel->Get_Bone(m_Channels[i]->Get_Name().c_str());
		{
			if (nullptr == pBone)
				return E_FAIL;		

			m_Bones.push_back(pBone);
		}
		Safe_AddRef(pBone);
	}
	return S_OK;
}

HRESULT CAnimation::Clear_Channels()
{
	/* Channel */
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();


	m_ChannelKeyFrames.clear();

	return S_OK;
}

HRESULT CAnimation::Clear_Bones()
{
	/* HierarachyNode */
	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	return S_OK;
}

HRESULT CAnimation::Calculate_Animation(_uint iFrame)
{
	/* ��� ä���� Ű�������� iFrame���� �����Ѵ�. */
	for (auto& pChannel : m_Channels)
	{
		for (auto& iCurrentKeyFrame : m_ChannelKeyFrames)
			iCurrentKeyFrame = iFrame;
	}

	/* ������ ������ Ű�����Ӵ��, ��� ä���� Ű�������� �����Ѵ�. (���� �θ� ����) */
	_uint iChannelIndex = 0;
	for (auto& pChannel : m_Channels)
	{	
		m_ChannelKeyFrames[iChannelIndex]
			= pChannel->Update_Transformation_NoneLerp(m_ChannelKeyFrames[iChannelIndex], m_Bones[iChannelIndex]);
		
		++iChannelIndex;
	}

	return S_OK;
}

CAnimation* CAnimation::Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels, const string& strName)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize_Prototype(fDuration, fTickPerSecond, Channels, strName)))
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
	///* Channel */
	//for (auto& pChannel : m_Channels)
	//	Safe_Release(pChannel);
	//m_Channels.clear();

	///* HierarachyNode */
	//for (auto& pBone : m_Bones)
	//	Safe_Release(pBone);
	//m_Bones.clear();
}
