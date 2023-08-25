#include "MiniMap.h"
#include "GameInstance.h"
#include "Image.h"
_uint MiniMap::MiniMap_Id = 0;

MiniMap::MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
	, m_RaderPos(_float2())
{
	XMStoreFloat4x4(&m_WorldMatrixInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

MiniMap::MiniMap(const MiniMap& rhs)
	: CGameObject(rhs)
	, m_WorldMatrixInverse(rhs.m_WorldMatrixInverse)
	, m_WorldMatrix(rhs.m_WorldMatrix)
	, m_RaderPos(rhs.m_RaderPos)
{
}

HRESULT MiniMap::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT MiniMap::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++MiniMap_Id;
	m_tInfo.wstrName = TO_WSTR("MiniMap" + to_string(MiniMap_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = MiniMap_Id;

	tagMiniMapDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagMiniMapDesc*)pArg;

	return S_OK;
}

void MiniMap::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CalaulateMatrices();
	CalculateMonsterImagesPosition();
	m_pRaderImage->Tick(TimeDelta);
	m_pPlayerImage->Tick(TimeDelta);
	for (auto& Image : m_pRenderImages)
		Image->Tick(TimeDelta);
}

void MiniMap::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRaderImage->Late_Tick(TimeDelta);
	m_pPlayerImage->Late_Tick(TimeDelta);
	for (auto& Image : m_pRenderImages)
		Image->Late_Tick(TimeDelta);
}

HRESULT MiniMap::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	Image::tagImageDesc tImageDesc;
	tImageDesc.Pos = m_RaderPos = _float2(g_iWinSizeX - 150, g_iWinSizeY - 607);
	tImageDesc.Size = _float2(210, 210);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_UI_KS_Radar");
	FAILED_CHECK_RETURN(__super::Add_Composite(Image::ProtoTag(), L"Com_Radar", (CComponent**)&m_pRaderImage, &tImageDesc), E_FAIL);

	tImageDesc.Pos = _float2(g_iWinSizeX - 150, (g_iWinSizeY - 607));
	tImageDesc.Size = _float2(20, 20);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_ui_radar_player_icon");
	FAILED_CHECK_RETURN(__super::Add_Composite(Image::ProtoTag(), L"Com_PlayerImage", (CComponent**)&m_pPlayerImage, &tImageDesc), E_FAIL);

	const _tchar* ImageTag[30] = { L"Com_MonsterImage1", L"Com_MonsterImage2", L"Com_MonsterImage3", L"Com_MonsterImage4", L"Com_MonsterImage5"
		, L"Com_MonsterImage6", L"Com_MonsterImage7", L"Com_MonsterImage8", L"Com_MonsterImage9", L"Com_MonsterImage10", L"Com_MonsterImage11"
		, L"Com_MonsterImage12", L"Com_MonsterImage13", L"Com_MonsterImage14", L"Com_MonsterImage15", L"Com_MonsterImage16", L"Com_MonsterImage17"
		, L"Com_MonsterImage18", L"Com_MonsterImage19", L"Com_MonsterImage20", L"Com_MonsterImage21", L"Com_MonsterImage22", L"Com_MonsterImage23"
		, L"Com_MonsterImage24", L"Com_MonsterImage25", L"Com_MonsterImage26", L"Com_MonsterImage27", L"Com_MonsterImage28", L"Com_MonsterImage29"
		, L"Com_MonsterImage30"};

	for (_uint i = 0; i, i < 30; ++i)
	{
		Image* pImage = { nullptr };
		tImageDesc.Pos = _float2(g_iWinSizeX >> 1, g_iWinSizeY >> 1);
		tImageDesc.Size = _float2(20, 20);
		tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_UI_radar_mEnemy");
		FAILED_CHECK_RETURN(__super::Add_Composite(Image::ProtoTag(), ImageTag[i], (CComponent**)&pImage, &tImageDesc), E_FAIL);
		m_pMonsterImages.push_back(pImage);
	}
	
	Safe_Release(pGameInstance);
	return S_OK;
}

void MiniMap::CalaulateMatrices()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_Transform"));
	_matrix WorldMatrixInverse = pPlayerTransform->Get_WorldMatrix_Inverse();
	_matrix WorldMatrix = pPlayerTransform->Get_WorldMatrix();
	XMStoreFloat4x4(&m_WorldMatrixInverse, WorldMatrixInverse);
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);

	Safe_Release(pGameInstance);
}

void MiniMap::CalculateMonsterImagesPosition()
{
	m_pRenderImages.clear();
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*>::iterator LayerBegin = pGameInstance->GetLayerBegin(pGameInstance->Get_CurLevelIndex(), L"Layer_Monster");
	list<CGameObject*>::iterator LayerEnd = pGameInstance->GetLayerEnd(pGameInstance->Get_CurLevelIndex(), L"Layer_Monster");
	
	Safe_Release(pGameInstance);
	if (LayerBegin == LayerEnd)
		return;
	_uint iIndex = { 0 };
	_vector PlayerPos = Single->GetClintPosition();
	// 몬스터 레이어 반복문
	for (auto iter = LayerBegin; iter != LayerEnd; ++iter)
	{
		// 몬스터 트랜스폼 얻음
		 CTransform* pTransform = dynamic_cast<CTransform*>((*iter)->Get_Component(L"Com_Transform"));
		 if (pTransform && iIndex != m_pMonsterImages.size())
		 {
			 _vector vMonsterPos = pTransform->Get_State(CTransform::STATE_POSITION);
			 _vector vOffsetPos = vMonsterPos - PlayerPos;
			 
			 _float fLength = XMVectorGetX(XMVector3Length(vOffsetPos));

			 if (fLength >= 25.f)
				 continue;

			 _vector RaderPos =  105.f * ((vOffsetPos)) / 25.f;
			 m_pMonsterImages[iIndex]->SetPosition(m_RaderPos.x + XMVectorGetX(XMVector3Dot(RaderPos, WorldAxisX()))
				 , m_RaderPos.y + XMVectorGetX(XMVector3Dot(RaderPos, WorldAxisZ())));

			 //if(fabs(vOffsetPos.m128_f32[2] * 15.f) < 105.f && fabs(vOffsetPos.m128_f32[0] * 15.f) < 105.f)
				 m_pRenderImages.push_back(m_pMonsterImages[iIndex++]);
		 }
	}
}

MiniMap* MiniMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	MiniMap* pInstance = new MiniMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created MiniMap");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* MiniMap::Clone(void* pArg)
{
	MiniMap* pInstance = new MiniMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned MiniMap");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MiniMap::Free(void)
{
	__super::Free();

	--MiniMap_Id;
	Safe_Release(m_pBossImage);
	Safe_Release(m_pArrowImage);
	Safe_Release(m_pPlayerImage);
	Safe_Release(m_pRaderImage);

	for (auto Monster : m_pMonsterImages)
		Safe_Release(Monster);
	m_pMonsterImages.clear();
	m_pRenderImages.clear();
}