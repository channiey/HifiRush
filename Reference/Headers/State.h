#pragma once
   
/* 해당 클래스를 상속받아 클라이언트에서 상태를 정의한다. */
/* 상태는 enum과 enum string으로 지칭하며, 스테이트머신에는 클라이언트로부터 넘어온 enum string을 가지고 스테이트를 관리한다.*/
/* 플레이어의 상태라면, 플레이어 내에 정의한다. */

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CState : public CBase
{

protected:
	CState();
	//CState(const CState& rhs);
	virtual ~CState() = default;

public:
	virtual HRESULT Enter() PURE;
	virtual const wstring& Tick(const _float& fimeDelta) PURE;
	virtual const wstring& LateTick() PURE;
	virtual void Exit() PURE;

public:
	const wstring& Get_Name() const { return m_pName; }
	void Set_Name(const wstring& strName) { m_pName = strName; }

private:
	class CStateMachine* m_pStateMachine = { nullptr };
	wstring	m_pName = {};

private:
	/* Tick, LateTick 공용*/
	//virtual const wstring& Check_Transition() PURE; 

private:
	//virtual CState* Clone(void* pArg) = 0;
	virtual void Free();
};

END		