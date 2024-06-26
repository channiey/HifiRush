#include "..\Public\Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const string strName, vector<KEYFRAME>& Keyframes)
{
	m_szName = strName;

	/* 뭐가 더 효율적일까 */

	//m_KeyFrames.resize(Keyframes.size());
	//memcpy(&m_KeyFrames, &Keyframes, sizeof(Keyframes));

	m_KeyFrames.reserve(Keyframes.size());
	for (auto& iter : Keyframes)
		m_KeyFrames.push_back(iter);

	return S_OK;
}

_uint CChannel::Update_Transformation(_float fPlayTime, _uint iCurrentKeyFrame, CBone* pNode)
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

_uint CChannel::Update_Transformation_NoneLerp(_uint iCurrentKeyFrame, CBone* pNode)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	/* 마지막 키프레임이상으로 넘어갔을때 : 마지막 키프레임 자세로 고정할 수 있도록 한다. */
	if (iCurrentKeyFrame < m_KeyFrames.size())
	{
		vScale	= m_KeyFrames[iCurrentKeyFrame].vScale;
		vRotation = m_KeyFrames[iCurrentKeyFrame].vRotation;
		vPosition = m_KeyFrames[iCurrentKeyFrame].vPosition;
	}
	else
	{
		vScale = m_KeyFrames.back().vScale;
		vRotation = m_KeyFrames.back().vRotation;
		vPosition = m_KeyFrames.back().vPosition;
	}

	_matrix TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

	if (nullptr != pNode)
		pNode->Set_Transformation(TransformationMatrix);

	return iCurrentKeyFrame;
}

CChannel* CChannel::Create(const string strName, vector<KEYFRAME>& Keyframes)
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
	__super::Free();
}

