#pragma once
   
/* �ش� Ŭ������ ��ӹ޾� Ŭ���̾�Ʈ���� ���¸� �����Ѵ�. */
/* ���´� enum�� enum string���� ��Ī�ϸ�, ������Ʈ�ӽſ��� Ŭ���̾�Ʈ�κ��� �Ѿ�� enum string�� ������ ������Ʈ�� �����Ѵ�.*/
/* �÷��̾��� ���¶��, �÷��̾� ���� �����Ѵ�. */

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
	/* Tick, LateTick ����*/
	//virtual const wstring& Check_Transition() PURE; 

private:
	//virtual CState* Clone(void* pArg) = 0;
	virtual void Free();
};

END		