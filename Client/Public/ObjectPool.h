
#include "Base.h"
#include "Client_Defines.h"
#include "GameInstance.h"

template<typename CLASSNAME>
class ObjectPool final : public CBase
{
    DECLARE_SINGLETON(ObjectPool<CLASSNAME>)

public:
    ObjectPool<CLASSNAME>() = default;
    ~ObjectPool<CLASSNAME>() = default;

    void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    {
        if (nullptr == m_pDevice)
        {
            m_pDevice = pDevice;
            Safe_AddRef(m_pDevice);
        }

        if (nullptr == m_pContext)
        {
            m_pContext = pContext;
            Safe_AddRef(m_pContext);
        }
    }

    void PushPool(CLASSNAME* pObject)
    {
        m_ObjectList.push_back(pObject);
    }

    // 내부적으로 ResetPool이 불린다.
    CLASSNAME* PopPool(const _tchar* pTag, void* pArg)
    {
        CLASSNAME* pObject;
        if (m_ObjectList.empty())
        {
            CGameInstance* pGameInstance = CGameInstance::GetInstance();
            Safe_AddRef(pGameInstance);

            pObject = static_cast<CLASSNAME*>(pGameInstance->Clone_GameObject(pTag, pArg));
            m_pInstances.push_back(pObject);

            Safe_Release(pGameInstance);
        }
        else
        {
            pObject = m_ObjectList.front();
            m_ObjectList.pop_front();
        }
        pObject->ResetPool(pArg);
        NULL_CHECK_RETURN(pObject, nullptr);
        return pObject;
    }

    _uint GetCount()
    {
        return m_pInstances.size();
    }

    _uint GetSize()
    {
        return m_ObjectList.size();
    }

    void Resize(_uint iSize)
    {
        CGameInstance* pGameInstance = CGameInstance::GetInstance();
        Safe_AddRef(pGameInstance);

        for (_uint i = 0; i < iSize; ++i)
        {
            CLASSNAME* pGameObject = dynamic_cast<CLASSNAME*>(pGameInstance->Clone_GameObject(CLASSNAME::ProtoTag(), nullptr));
            m_ObjectList.push_back(pGameObject);
            m_pInstances.push_back(pGameObject);
        }
       
        Safe_Release(pGameInstance);
    }
private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    list<CLASSNAME*> m_ObjectList; // 보관하고 내보내는 용도
    list<CLASSNAME*> m_pInstances; // 지우기용도

public:
    virtual void Free() override
    {
        Safe_Release(m_pDevice);
        Safe_Release(m_pContext);

        m_ObjectList.clear();
        for (auto pObject : m_pInstances)
        {
            Safe_Release(pObject);
        }
        m_pInstances.clear();
    }
};

// ImpleSingleton
template<typename CLASSNAME>
ObjectPool<CLASSNAME>* ObjectPool<CLASSNAME>::m_pInstance = nullptr;
template<typename CLASSNAME>
ObjectPool<CLASSNAME>* ObjectPool<CLASSNAME>::GetInstance(void) {
    if (nullptr == m_pInstance) {
        m_pInstance = new ObjectPool<CLASSNAME>;
    }
    return m_pInstance;
}
template<typename CLASSNAME>
unsigned long ObjectPool<CLASSNAME>::DestroyInstance(void) {
    unsigned long dwRefCnt = { 0 };
    if (nullptr != m_pInstance)
    {
        dwRefCnt = m_pInstance->Release();
        if (0 == dwRefCnt)
            m_pInstance = { nullptr };
    }
    return dwRefCnt;
}