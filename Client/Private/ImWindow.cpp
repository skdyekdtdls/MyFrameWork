#ifdef _DEBUG
#include "..\Public\ImWindow.h"

CImWindow::CImWindow(ImGuiIO* pIO)
    : m_pIO(pIO)
    , m_IsShow(true)
{

}

HRESULT CImWindow::Initialize(void* pArg)
{

    return S_OK;
}

void CImWindow::Free(void)
{

}
#endif