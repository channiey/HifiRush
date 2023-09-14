#pragma once

#include "Component.h"

/* ��ü�� ���� ��ȯ ��� (���� �����̽��� ���� ���� ��ȯ ���)�� �����Ѵ�. */

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
	/* ���� �����󿡼��� ����� �� �� ����(��, ��, ��, ��)�� �����Ѵ�. */
	_vector Get_State(STATE eState) { return XMLoadFloat4x4(&m_WorldMatrix).r[eState]; }

	const _float& Get_Speed() const { return m_tTrans.fSpeedPerSec; }
	const _float& Get_RotRad() const { return m_tTrans.fRotRadPerSec; }

	/* ������Ŀ� �� �࿡ �ʿ��� ������ �־��ٲ���. */
	void Set_State(STATE eState, _fvector vState);

	void Set_Speed(const _float& fSpeed) { m_tTrans.fSpeedPerSec = fSpeed; }
	void Set_RotRad(const _float& fRotRad) { m_tTrans.fRotRadPerSec = fRotRad; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Move_Forward(_float fTimeDelta);
	void Move_Backward(_float fTimeDelta);
	void Move_Left(_float fTimeDelta);
	void Move_Right(_float fTimeDelta);

	void Fix_Rotation(_fvector vAxis, _float fRadian);
	void Turn(_fvector vAxis, _float fTimeDelta);

private:
	/* DX���� ���� �ڷ����� ũ�� 2������ ������. ~float�� ~vector(matrix) */
	/* �ڰ� ~float���� �����ٸ� �����, ~vector�� ~matrix�� �����ٸ� ���� + ������̴�. */
	_float4x4			m_WorldMatrix = {};
	TRANSFORM_DESC		m_tTrans = {};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END