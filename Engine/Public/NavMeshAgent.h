#pragma once

#include "Component.h"

BEGIN(Engine)
class CNavMesh;

class ENGINE_DLL CNavMeshAgent final : public CComponent
{
public:
	typedef struct tagNavMeshAgentDesc
	{
		_int			iCurIndex = { -1 };

		tagNavMeshAgentDesc(const _int& _iCurIndex) : iCurIndex(_iCurIndex) {};

	}NAVMESHAGENT_DESC;

private:
	CNavMeshAgent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavMeshAgent(const CNavMeshAgent& rhs);
	virtual ~CNavMeshAgent() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

public:
	const _bool		Can_Move(_fvector vPoint);

private:
	_int			m_iCurIndex = { -1 };
	CNavMesh*		m_pNavMesh = { nullptr };

public:
	static CNavMeshAgent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END