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
	, m_Channels(rhs.m_Channels) /* 데이터 큼, 얕은 복사해도 상관 없으니 얕은복사로 진행 */
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
	
	/* 벡터에 사이즈도 안 채우고 멤카피 하면 큰일난다. */
	//memcpy(&m_Channels, &Channels, sizeof(Channels)); 

	/* 뭐가 더 효율적일까 */

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
	/* 애니메이션을 재생하기 위해 사용되는 뼈를 모두 저장한다.  */
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
	/* 모든 채널의 키프레임을 iFrame으로 세팅한다. */
	for (auto& pChannel : m_Channels)
	{
		for (auto& iCurrentKeyFrame : m_ChannelKeyFrames)
			iCurrentKeyFrame = iFrame;
	}

	/* 위에서 지전항 키프레임대로, 모든 채널의 키프레임을 보간한다. (아직 부모 기준) */
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
