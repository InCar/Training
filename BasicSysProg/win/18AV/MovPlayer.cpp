#include "stdafx.h"
#include "MovPlayer.h"
#include "resource.h"
#include "App.h"

#define WM_MOVPLAYER_EVENT    WM_USER+5

CMovPlayer::CMovPlayer()
{
    m_pSource = NULL;
    m_pSession = NULL;
}


CMovPlayer::~CMovPlayer()
{
}

LRESULT CMovPlayer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_RBUTTONUP, OnRButtonUp);
    case WM_MOVPLAYER_EVENT:
        return OnPlayerEvent((MediaEventType)wParam, (IMFMediaEvent*)lParam);
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

void CMovPlayer::OnPaint(HWND hwnd)
{
    CXWnd::OnPaint(hwnd);
}

void CMovPlayer::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_VIDEO_OPEN:
    {
        wstring wstrMovFile;
        if (OpenMovFile(wstrMovFile)) {
            // create media source
            MF_OBJECT_TYPE mfObjType = MF_OBJECT_INVALID;
            IMFSourceResolver *pMFSourceResolver = NULL;
            IUnknown *pSource = NULL;
            HRESULT hr = MFCreateSourceResolver(&pMFSourceResolver);
            hr = pMFSourceResolver->CreateObjectFromURL(wstrMovFile.c_str(), MF_RESOLUTION_MEDIASOURCE, NULL, &mfObjType, &pSource);
            hr = pSource->QueryInterface(IID_PPV_ARGS(&m_pSource));
            pSource->Release();
            pMFSourceResolver->Release();

            // create playback topologies
            DWORD dwStreamCount = 0;
            BOOL bSelected = FALSE;
            IMFPresentationDescriptor *pMFPresentDesc = NULL;
            IMFTopology *pMFTopology = NULL;
            hr = m_pSource->CreatePresentationDescriptor(&pMFPresentDesc);
            hr = MFCreateTopology(&pMFTopology);

            hr = pMFPresentDesc->GetStreamDescriptorCount(&dwStreamCount);
            for (DWORD i = 0; i < dwStreamCount; i++) {
                IMFStreamDescriptor *pMFStreamDesc = NULL;
                hr = pMFPresentDesc->GetStreamDescriptorByIndex(i, &bSelected, &pMFStreamDesc);
                if (bSelected) {
                    GUID guidMajor;
                    IMFMediaTypeHandler *pMFMediaTypeHandler = NULL;
                    IMFActivate *pMFActivate = NULL;
                    hr = pMFStreamDesc->GetMediaTypeHandler(&pMFMediaTypeHandler);
                    
                    hr = pMFMediaTypeHandler->GetMajorType(&guidMajor);
                    if (guidMajor == MFMediaType_Audio) {
                        hr = MFCreateAudioRendererActivate(&pMFActivate);
                    }
                    else if (guidMajor == MFMediaType_Video) {
                        hr = MFCreateVideoRendererActivate(hwnd, &pMFActivate);
                    }
                    // add source node
                    IMFTopologyNode *pTopoNodeSrc = NULL;
                    hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pTopoNodeSrc);
                    hr = pTopoNodeSrc->SetUnknown(MF_TOPONODE_SOURCE, m_pSource);
                    hr = pTopoNodeSrc->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pMFPresentDesc);
                    hr = pTopoNodeSrc->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pMFStreamDesc);
                    hr = pMFTopology->AddNode(pTopoNodeSrc);

                    // add output node
                    IMFTopologyNode *pTopoNodeOut = NULL;
                    hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pTopoNodeOut);
                    hr = pTopoNodeOut->SetObject(pMFActivate);
                    hr = pTopoNodeOut->SetUINT32(MF_TOPONODE_STREAMID, 0);
                    hr = pTopoNodeOut->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, TRUE);
                    hr = pMFTopology->AddNode(pTopoNodeOut);

                    // connect
                    hr = pTopoNodeSrc->ConnectOutput(0, pTopoNodeOut, 0);

                    pMFMediaTypeHandler->Release();
                }
                pMFStreamDesc->Release();
            }
            pMFPresentDesc->Release();

            // create session
            hr = MFCreateMediaSession(NULL, &m_pSession);
            hr = m_pSession->BeginGetEvent(this, NULL);
            hr = m_pSession->SetTopology(0, pMFTopology);
        }
        break;
    }
    case ID_VIDEO_CLOSE:
    {
        HRESULT hr = S_OK;

        if (m_pSession) {
            hr = m_pSession->Close();
            hr = m_pSession->Shutdown();
            m_pSession->Release();
            m_pSession = NULL;
        }
        
        if (m_pSource) {
            hr = m_pSource->Shutdown();
            m_pSource->Release();
            m_pSource = NULL;
        }
        break;
    }
    default:
        CXWnd::OnCommand(hwnd, id, hwndCtl, codeNotify);
    }
}

HRESULT CMovPlayer::Invoke(IMFAsyncResult *pResult)
{
    // MUST BE THREAD SAFE
    MediaEventType meType = MEUnknown;
    IMFMediaEvent *pMediaEvent = NULL;

    HRESULT hr = m_pSession->EndGetEvent(pResult, &pMediaEvent);
    if (FAILED(hr)) return hr;

    hr = pMediaEvent->GetType(&meType);
    if (meType != MESessionClosed) {
        hr = m_pSession->BeginGetEvent(this, NULL);
    }
    else {
    }

    PostMessage(m_hwnd, WM_MOVPLAYER_EVENT, (WPARAM)meType, (LPARAM)pMediaEvent);

    return S_OK;
}

LRESULT CMovPlayer::OnPlayerEvent(MediaEventType meType, IMFMediaEvent *pMediaEvent)
{
    HRESULT hrEvent = S_OK;
    HRESULT hr = pMediaEvent->GetStatus(&hrEvent);
    if (FAILED(hrEvent)) {
        pMediaEvent->Release();
        pMediaEvent = NULL;
        return 0L;
    }

    switch (meType)
    {
    case MESessionTopologyStatus:
    {
        UINT32 status = 0;
        HRESULT hr = pMediaEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &status);
        if (status == MF_TOPOSTATUS_READY){
            // start playback
            PROPVARIANT varStart;
            PropVariantInit(&varStart);
            hr = m_pSession->Start(&GUID_NULL, &varStart);
            PropVariantClear(&varStart);
        }
        break;
    }
    case MENewPresentation:
        break;
    case MEEndOfPresentation:
        break;
    default:
        break;
    }

    pMediaEvent->Release();
    pMediaEvent = NULL;

    return 0L;
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