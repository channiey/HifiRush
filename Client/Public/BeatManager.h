#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
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
	HRESULT			Update(const _float fTimedelta);

public:
	const _uint		Get_BPM();
	const _double	Get_BPS();
	const _float	Get_AnimTimePerFrameSyncBeat(class CAnimation* pAnim);



public:
	const _bool&	Is_Hit() const { return m_bHit; }
	void			Reset();

private:
	_uint			m_iCurBpm	= 0;
	_double			m_fCurTime	= 0;
	_bool			m_bHit		= FALSE;


public:
	virtual void	Free() override;
};

END


