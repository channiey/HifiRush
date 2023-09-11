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
	virtual void Tick(const _float& fimeDelta)  PURE;
	virtual void LateTick(const _float& fimeDelta) PURE;
	virtual void Exit() PURE;

//protected:
//	virtual Check_Condition(const _float& fimeDelta);

private:
	class CStateMachine* m_pStateMachine = { nullptr };

private:
	virtual void Free();
};

END		