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
	HRESULT			Update(const _float fTimedelta);

public:
	const _uint		Get_BPM();
	const _double	Get_BPS(); /* 초당 비트 수 */
	const _double	Get_SPB(); /* 비트 당 초 */
	const _double	Get_AnimTimePerFrame(class CAnimation* pAnim);

public:
	const _bool&	Is_Hit() const { return m_bHit; }

public:
	void			Reset();

private:
	void			Update_Beat(const _float fTimedelta);

private:
	_uint			m_iCurBpm	= 0;
	_double			m_dCurTime	= 0;
	_bool			m_bHit		= FALSE;

public:
	virtual void	Free() override;
};

END


