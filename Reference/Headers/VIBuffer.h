#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	typedef struct tagCVIBufferDesc : public tagComponentDesc
	{
		tagCVIBufferDesc() : tagComponentDesc() {};
	}CVIBUFFER_DESC;
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	void SaveAssimp(HANDLE hFile, DWORD & dwByte);
	void LoadAssimp(HANDLE hFile, DWORD & dwByte);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

	ID3D11Buffer* GetVB() {
		return m_pVB;
	}

protected:
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

protected:

	D3D11_BUFFER_DESC	m_BufferDesc;
	D3D11_SUBRESOURCE_DATA	m_SubResourceData;

	/* For VertexBuffer*/
	_uint	m_iStride = { 0 };
	_uint	m_iNumVertices = { 0 };

	/* For Index Buffer */
	_uint	m_iNumIndices = { 0 };
	_uint	m_iIndexStride = { 0 };
	DXGI_FORMAT	m_eFormat;
	
	_uint	m_iNumVertexBuffers = { 0 };
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;

protected:
	HRESULT Create_Buffer(ID3D11Buffer** ppOut);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END