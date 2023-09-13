#pragma once

#include "Component.h"

/* ��ü�� ���� �����̽����� ��ȯ�� ���� ����� �����Ѵ�. */
/* ��ü�� ���� �����̽����� ��ȯ�� ���� ��� == �������. == ������¿����� Right, Up, Look, Position�� ������ �� �࿡ �����Ѵ�. */

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

	/* ���� �����󿡼��� ����� �� �� ����(��, ��, ��, ��)�� �����Ѵ�. */
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	/* ������Ŀ� �� �࿡ �ʿ��� ������ �־��ٲ���. */
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
	/* D3DXVECTOR3 : ���� + ���� */
	/* XMFLOAT2, XMFLOAT3, XMFLOAT4 : ����뺤��. */
	/* XMFLOAT4X4 : �ܼ� float 16���� �����ϰ� �ִ� ����ü. */
	/* ���� ������ ����� ������ ���� �ʴ�. */
	/* ����� ������ �����ϴ� ����� ���� �����ϴ� Ÿ��. */

	/* 1��(16.4), 2��(67.4), 3��(178) �ӽż�ġ�� Ȯ��. */
	/* 4�� �Ʊ��� */
	/* 5�� ��Ȳ, �Ʊ⸦ Ȯ���ؾߵ� (����Ҹ��� ���ߵ�) return E_FAIL; */
	
	/* XMVECTOR : ����뺤��. */
	/* XMMATRIX : ����� ������ ���� �����Ǵ� Ÿ��. */	

	_float4x4			m_WorldMatrix;
	
	TRANSFORM_DESC		m_tTransformDesc = {};
	
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END