#include "..\public\ModelPaser.h"

HRESULT CModelPaser::Pasing(TYPE eModelType, const char* pModelFilePath)
{
	_uint			iFlag = 0;

	m_eModelType = eModelType;

	if (TYPE_NONANIM == eModelType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;


}