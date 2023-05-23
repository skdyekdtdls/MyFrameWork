#pragma once

#include "Shader.h"
#include "Renderer.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"

BEGIN(Engine)
class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	~CComponent_Manager() = default;

public:
	HRESULT Reserve_Containers(_uint iLevelIndex);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pCompoent);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	void Clear_LevelResources(_uint iLevelIndex);

private:
	_uint m_iNumLevels = { 0 };
	typedef unordered_map<const _tchar*, CComponent*> PROTOTYPES;
	PROTOTYPES* m_pPrototypes = { nullptr };
private:
	CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free(void) override;
};

END