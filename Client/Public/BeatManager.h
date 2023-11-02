#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CModel;
class CAnimation;
END

BEGIN(Client)

class CBeatManager final : public CBase
{
	DECLARE_SINGLETON(CBeatManager)

private:
	CBeatManager();
	virtual ~CBeatManager() = default;

public:
	HRESULT			Initialize();
	HRESULT			Update(const _double fTimedelta);

public:
	const _uint		Get_BPM();
	const _double	Get_BPS(); /* 초당 비트 수 */
	const _double	Get_SPB(); /* 비트 당 초 */
	const _double	Get_SPB(_uint iNumBeat); /* 비트 개수당 초 */
	const _double	Get_AnimTimePerFrame(class CAnimation* pAnim);

public:
	const _bool&	Is_SteadyBeat() const { return m_bSteadyBeat; }
	const _bool&	Is_HalfBeat() const { return m_bHalfBeat; }

public:
	void			Reset();

private:
	void			Update_Beat(const _double fTimedelta);

private:
	_uint			m_iCurBpm	= 0;
	_double			m_dCurTimeSteadyBeat	= 0;
	_double			m_dCurTimeHalfBeat = 0;

	_bool			m_bSteadyBeat	= FALSE; /* 정박자 */
	_bool			m_bHalfBeat		= FALSE; /* 반박자 */

public:
	virtual void	Free() override;
};

END


