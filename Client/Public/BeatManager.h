#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CModel;
class CAnimation;
class CStateMachine;
class CGameObject;
END

BEGIN(Client)

class CBeatManager final : public CBase
{
	DECLARE_SINGLETON(CBeatManager)

public:
	enum EVENT_TYPE { STATE_EVENT, EVENT_TYPEEND };

	typedef struct tagStateEventDesc
	{
		CStateMachine* pStateMachine = { nullptr };
		wstring		   strStateName = {};

		tagStateEventDesc() {};
		tagStateEventDesc(CStateMachine* _pStateMachine, const wstring& _strStateName)
			: pStateMachine(_pStateMachine), strStateName(strStateName) {};

	}STATE_EVENT_DESC;

private:
	CBeatManager();
	virtual ~CBeatManager() = default;

public:
	HRESULT			Initialize();
	HRESULT			Update(const _double fTimedelta);

public:
	const _uint		Get_BPM();
	const _double	Get_BPS(); /* �ʴ� ��Ʈ �� (136BPM -> 2.27)*/
	const _double	Get_SPB(); /* ��Ʈ �� �� (136BPM -> 0.44) */
	const _double	Get_SPB(_uint iNumBeat); /* ��Ʈ ������ �� */
	const _double	Get_AnimTimePerFrame(class CAnimation* pAnim);

public:
	const _bool&	Is_SteadyBeat() const { return m_bSteadyBeat; }
	const _bool&	Is_HalfBeat() const { return m_bHalfBeat; }

public:
	void			Reset();

public: /* Events */
	HRESULT			Add_StateEvent(const STATE_EVENT_DESC StateEvent);
	const _bool		Is_Reserve_StateEvent(CStateMachine* pStateMachine);

private:
	void			Update_Beat(const _double fTimedelta);

private:
	_uint						m_iCurBpm	= 0;
	_double						m_dCurTimeSteadyBeat	= 0;
	_double						m_dCurTimeHalfBeat = 0;

	_bool						m_bSteadyBeat	= FALSE; /* ������ */
	_bool						m_bHalfBeat		= FALSE; /* �ݹ��� */

	vector<STATE_EVENT_DESC>	m_StateEvents;
public:
	virtual void	Free() override;
};

END


