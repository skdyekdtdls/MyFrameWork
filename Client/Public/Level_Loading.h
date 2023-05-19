
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	HRESULT Initialize(LEVELID eNextLevelID);
	void Tick(_double TimeDelta) override;
	void Late_Tick(_double TimeDelta) override;
	HRESULT Render() override;

private:
	class CLoader* m_pLoader = { nullptr };
	LEVELID	m_eNextLevelID = { LEVEL_END };

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	void Free() override;
};

END