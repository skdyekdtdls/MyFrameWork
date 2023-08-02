#pragma once

#include "Shader.h"
#include "Renderer.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Ray.h"
#include "Texture.h"
#include "Transform.h"
#include "Model.h"
#include "Navigation.h"
#include "ColliderSphere.h"
#include "ColliderAABB.h"
#include "Raycast.h"
#include "VIBuffer_Rect_Instance.h"
#include "VIBuffer_Point_Instance.h"
#include "Health.h"
#include "TimeCounter.h"
#include "VIBuffer_DynamicRect.h"
#include "Transform2D.h"

BEGIN(Engine)
class ENGINE_DLL CComponent_Manager final : public CBase
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
	CComponent* Get_ProtoComponent(_uint iLevelIndex, const _tchar * pProtoTag);
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