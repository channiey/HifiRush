#pragma once

#include "Component.h"

/* 객체의 월드 스페이스로의 변환을 위한 행렬을 보관한다. */
/* 객체의 월드 스페이스로의 변환을 위한 행렬 == 월드행렬. == 월드상태에서의 Right, Up, Look, Position의 정보를 각 행에 저장한다. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec = { 0.f };
		_float		fRotRadPerSec = { 0.0f };
	}TRANSFORM_DESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	/*
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
	*/

	/* 월드 공간상에서의 행렬의 각 행 정보(라, 업, 룩, 포)를 리턴한다. */
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	/* 월드행렬에 각 행에 필요한 정보를 넣어줄꺼야. */
	void Set_State(STATE eState, _fvector vState);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Fix_Rotation(_fvector vAxis, _float fRadian);
	void Turn(_fvector vAxis, _float fTimeDelta);

private:
	/* D3DXVECTOR3 : 저장 + 연산 */
	/* XMFLOAT2, XMFLOAT3, XMFLOAT4 : 저장용벡터. */
	/* XMFLOAT4X4 : 단순 float 16개를 저장하고 있는 구조체. */
	/* 실제 연산의 기능은 가지고 있지 않다. */
	/* 행렬의 정보를 보관하는 기능을 위해 선언하는 타입. */

	/* 1차(16.4), 2차(67.4), 3차(178) 임신수치를 확인. */
	/* 4차 아기집 */
	/* 5차 난황, 아기를 확인해야돼 (심장소리를 들어야돼) return E_FAIL; */
	
	/* XMVECTOR : 연산용벡터. */
	/* XMMATRIX : 행렬의 연산을 위해 제공되는 타입. */	

	_float4x4			m_WorldMatrix;
	
	TRANSFORM_DESC		m_tTransformDesc = {};
	
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END