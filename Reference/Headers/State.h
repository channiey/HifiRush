#pragma once
   
/* �ش� Ŭ������ ��ӹ޾� Ŭ���̾�Ʈ���� ���¸� �����Ѵ�. */
/* ���´� enum�� enum string���� ��Ī�ϸ�, ������Ʈ�ӽſ��� Ŭ���̾�Ʈ�κ��� �Ѿ�� enum string�� ������ ������Ʈ�� �����Ѵ�.*/

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
	virtual const wstring& Tick(const _float& fimeDelta)  PURE;
	virtual const wstring& LateTick() PURE;
	virtual void Exit() PURE;

public:
	const wstring& Get_Name() const { return m_pName; }
	void Set_Name(const wstring& strName) { m_pName = strName; }

//protected:
//	virtual Check_Condition(const _float& fimeDelta) {};

private:
	class CStateMachine* m_pStateMachine = { nullptr };
	wstring	m_pName = {};

private:
	virtual void Free();
};

END		