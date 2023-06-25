#ifdef _DEBUG

#include "VIBuffer.h"
#include "Client_Defines.h"

BEGIN(Client)

class CImVertex : public Engine::CVIBuffer
{
private:
	explicit CImVertex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CImVertex(const CImVertex& rhs);
	virtual ~CImVertex() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	

public:
	// If this component is the VIBuffer Com or the Shader Com, Delete ProtoTag().
	static const _tchar* ProtoTag() { return L"Prototype_Component_CImVertex"; }
	static CImVertex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#endif