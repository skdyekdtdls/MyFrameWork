#ifdef _DEBUG
#include "..\Public\ImWindow.h"
#include "EditCamera.h"

CImWindow::CImWindow(ImGuiIO* pIO)
    : m_pIO(pIO)
    , m_IsShow(true)
{

}

HRESULT CImWindow::Initialize(void* pArg)
{
    return S_OK;
}

void CImWindow::LateTick()
{
}

void CImWindow::Free(void)
{

}
#endif