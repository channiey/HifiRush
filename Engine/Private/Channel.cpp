#include "..\Public\Channel.h"
#include "HierarchyNode.h"

CChannel::CChannel()
{
}

//HRESULT CChannel::Initialize(aiNodeAnim * pAIChannel)
//{ 
//	strcpy_s(m_szName, pAIChannel->mNodeName.data);
//
//	/* SRT 키프레임 갯수 맞추기 (부족한 키프레임은 이전 키프레임 상태 사용)*/
//	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
//	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);
//
//	/* 키프레임이 부족하면 이전 프레임 상태 사용하기 위해 루프 밖에 선언 */
//	_float3			vScale{};
//	_float4			vRotation{};
//	_float3			vPosition{};
//
//	/* 모든 키프레임 정보 복사 */
//	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
//	{
//		KEYFRAME			KeyFrame;
//		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));
//
//		if(i < pAIChannel->mNumScalingKeys)
//		{
//			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
//			KeyFrame.fTime = pAIChannel->mScalingKeys[i].mTime;
//		}
//		if (i < pAIChannel->mNumRotationKeys)
//		{
//			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
//			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
//			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
//			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
//			KeyFrame.fTime = pAIChannel->mRotationKeys[i].mTime;
//		}
//		if (i < pAIChannel->mNumPositionKeys)
//		{
//			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
//			KeyFrame.fTime = pAIChannel->mPositionKeys[i].mTime;
//		}
//
//		KeyFrame.vScale = vScale;
//		KeyFrame.vRotation = vRotation;
//		KeyFrame.vPosition = vPosition;
//
//		m_KeyFrames.push_back(KeyFrame);
//	}	
//
//	return S_OK;
//}

HRESULT CChannel::Initialize(const string& strName, vector<KEYFRAME>& Keyframes)
{
	strcpy_s(m_szName, strName.c_str());

	memcpy(&m_KeyFrames, &Keyframes, sizeof(Keyframes));

	m_iNumKeyFrames = (_int)m_KeyFrames.size();

	return S_OK;
}

_uint CChannel::Update_Transformation(_float fPlayTime, _uint iCurrentKeyFrame, CHierarchyNode* pNode)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	/* 마지막 키프레임이상으로 넘어갔을때 : 마지막 키프레임 자세로 고정할 수 있도록 한다. */
	if (fPlayTime >= m_KeyFrames.back().fTime)
	{
		vScale = m_KeyFrames.back().vScale;
		vRotation = m_KeyFrames.back().vRotation;
		vPosition = m_KeyFrames.back().vPosition;		
	}
	else
	{
		/* 키프레임 보간 */
		while (fPlayTime >= m_KeyFrames[iCurrentKeyFrame + 1].fTime)
			++iCurrentKeyFrame;

		_float		fRatio = (fPlayTime - m_KeyFrames[iCurrentKeyFrame].fTime) /
			(m_KeyFrames[iCurrentKeyFrame + 1].fTime - m_KeyFrames[iCurrentKeyFrame].fTime);

		_float3		vSourScale, vDestScale;
		_float4		vSourRotation, vDestRotation;
		_float3		vSourPosition, vDestPosition;

		vSourScale = m_KeyFrames[iCurrentKeyFrame].vScale;
		vDestScale = m_KeyFrames[iCurrentKeyFrame + 1].vScale;

		vSourRotation = m_KeyFrames[iCurrentKeyFrame].vRotation;
		vDestRotation = m_KeyFrames[iCurrentKeyFrame + 1].vRotation;

		vSourPosition = m_KeyFrames[iCurrentKeyFrame].vPosition;
		vDestPosition = m_KeyFrames[iCurrentKeyFrame + 1].vPosition;

		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vSourPosition), XMLoadFloat3(&vDestPosition), fRatio));
	}
	_matrix	TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));	

	if (nullptr != pNode)
		pNode->Set_Transformation(TransformationMatrix);

	return iCurrentKeyFrame;
}

//CChannel * CChannel::Create(aiNodeAnim * pAIChannel)
//{
//	CChannel*			pInstance = new CChannel();
//
//	if (FAILED(pInstance->Initialize(pAIChannel)))
//	{
//		MSG_BOX("Failed To Created : CChannel");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CChannel* CChannel::Create(const string& strName, vector<KEYFRAME>& Keyframes)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(strName, Keyframes)))
	{
		MSG_BOX("Failed To Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{

}

