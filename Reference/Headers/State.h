#pragma once
#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CState : public CBase
{

protected:
	CState();
	virtual ~CState() = default;

public:
	virtual HRESULT Enter() PURE;
	virtual CState* Tick(const _float& fimeDelta)  PURE;
	virtual CState* LateTick() PURE;
	virtual void Exit() PURE;

//protected:
//	virtual Check_Condition(const _float& fimeDelta) {};

private:
	class CStateMachine* m_pStateMachine = { nullptr };
	wstring	m_pTag = {};

private:
	virtual void Free();
};

END		