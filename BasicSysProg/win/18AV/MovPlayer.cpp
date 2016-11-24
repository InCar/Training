#include "stdafx.h"
#include "MovPlayer.h"
#include "resource.h"
#include "App.h"

CMovPlayer::CMovPlayer()
{
}


CMovPlayer::~CMovPlayer()
{
}

LRESULT CMovPlayer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_RBUTTONUP, OnRButtonUp);
        break;
    default:
        return CXWnd::WndProc(hWnd, uMsg, wParam, lParam);
    }
}

void CMovPlayer::OnRButtonUp(HWND hwnd, int x, int y, UINT flags)
{
    HMENU hMenu = LoadMenu(CApp::GetApp()->GetInstance(), MAKEINTRESOURCE(IDR_MENU_VIDEO));
    HMENU hVideo = GetSubMenu(hMenu, 0);

    POINT ptClick;
    ptClick.x = x;
    ptClick.y = y;
    ClientToScreen(hwnd, &ptClick);

    TrackPopupMenu(hVideo, TPM_LEFTALIGN, ptClick.x, ptClick.y, 0, hwnd, NULL);
    DestroyMenu(hMenu);
}

void CMovPlayer::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_VIDEO_OPEN:
    {
        wstring wstrMovFile;
        if (OpenMovFile(wstrMovFile)) {
            // TODO: Play
        }
        break;
    }
    case ID_VIDEO_CLOSE:
    {
        break;
    }
    default:
        CXWnd::OnCommand(hwnd, id, hwndCtl, codeNotify);
    }
}

BOOL CMovPlayer::OpenMovFile(wstring &wstrMov)
{
    BOOL bRet = FALSE;
    IFileDialog *pFileDialog = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (SUCCEEDED(hr)) {
        COMDLG_FILTERSPEC filter[2];
        filter[0].pszName = L"mkv(*.mkv)";
        filter[0].pszSpec = L"*.mkv";
        filter[1].pszName = L"All Files(*.*)";
        filter[1].pszSpec = L"*.*";

        hr = pFileDialog->SetFileTypes(sizeof(filter) / sizeof(COMDLG_FILTERSPEC), filter);
        hr = pFileDialog->SetFileTypeIndex(1);
        hr = pFileDialog->Show(m_hwnd);
        if (SUCCEEDED(hr)) {
            IShellItem *psi = NULL;
            hr = pFileDialog->GetResult(&psi);
            if (SUCCEEDED(hr)) {
                PWSTR pwszFilePath = NULL;
                hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pwszFilePath);
                if (SUCCEEDED(hr)) {
                    wstrMov.assign(pwszFilePath);
                    CoTaskMemFree(pwszFilePath);
                    bRet = TRUE;
                }
                psi->Release();
            }
        }
    }
    pFileDialog->Release();

    return bRet;
}