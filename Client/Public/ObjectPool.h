
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

    void PushPool(CLASSNAME* object)
    {
        m_ObjectList.push_back(object);
    }

    CLASSNAME* PopPool(const _tchar* pTag, void* pArg)
    {
        CLASSNAME* object;
        if (m_ObjectList.empty())
        {
            CGameInstance* pGameInstance = CGameInstance::GetInstance();
            Safe_AddRef(pGameInstance);
            
            object = static_cast<CLASSNAME*>(pGameInstance->Clone_GameObject(pTag, pArg));
            ++m_ObjectCount;

            Safe_Release(pGameInstance);
        }
        else
        {
            object = m_ObjectList.front();
            m_ObjectList.pop_front();
            object->Reset(pArg);
        }

        return object;
    }

    _uint GetCount()
    {
        return m_ObjectCount;
    }

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    static std::list<CLASSNAME*> m_ObjectList;
    static int m_ObjectCount;

    virtual void Free() override
    {
        Safe_Release(m_pDevice);
        Safe_Release(m_pContext);

        for (auto object : m_ObjectList)
        {
            Safe_Release(object);
        }

        m_ObjectList.clear();
    }
};

template<typename CLASSNAME>
std::list<CLASSNAME*> ObjectPool<CLASSNAME>::m_ObjectList;

template<typename CLASSNAME>
int ObjectPool<CLASSNAME>::m_ObjectCount = 0;



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