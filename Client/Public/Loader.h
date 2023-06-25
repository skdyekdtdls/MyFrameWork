#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~CLoader() = default;

public:
	HRESULT Initialize(LEVELID eNextLevel);
	HRESULT Loading();

	_bool Get_Finished() { return m_isFinished; }
	const _tchar* Get_LoadingText() const {
		return m_szLoading;
	}
	void Set_LoadingText(const _tchar* text) 
	{
		lstrcpy(m_szLoading, text);
	}
private:
	HANDLE	m_hThread = { nullptr };
	CRITICAL_SECTION m_Critical_Section;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

private:
	LEVELID	m_eNextLevel = { LEVEL_END };
	_tchar m_szLoading[MAX_PATH] = { TEXT("") };
	_bool	m_isFinished = { false };

private:
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();
#ifdef _DEBUG
	HRESULT Loading_For_IMGUI();
#endif
public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVELID eNextLevel);
	void Free() override;
};

END