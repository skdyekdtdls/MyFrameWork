//#pragma once
//
//#include "Client_Defines.h"
//#include "Model.h"
//
//BEGIN(Client)
//
//class CModel final : public CModel
//{
//public:
//	typedef struct tagCModelDesc : public tagCModelDesc
//	{
//		tagCModelDesc() : tagCModelDesc() {};
//	}CLINT_MODEL_DESC;
//
//private:
//	explicit CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
//	explicit CModel(const CModel& rhs);
//	virtual ~CModel() = default;
//
//public: /* Getter*/
//	//virtual class CBone* GetBoneByIndex(_uint iIndex) override;
//
//
//	virtual HRESULT Initialize(void* pArg) override;
//
//public:
//	virtual void ResetAnimation(_int iIndex = -1, BODY eBody = LOWER) override;
//
//public:
//	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _fmatrix PivotMatrix, _int RootIndex = -1);
//	virtual CComponent* Clone(void* pArg) override;
//	virtual void Free() override;
//};
//
//END