#include "stdafx.h"
#include "chat.h"
#include "resource.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <IPHlpApi.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHlpApi.lib")

static wchar_t s_wsaLocalIP[32];

static SOCKET s_skt15673 = INVALID_SOCKET;
static SOCKADDR_IN s_addr15673;

static SOCKET s_skt15683 = INVALID_SOCKET;
static SOCKADDR_IN s_addr15683;

void ChatStartup(Chat *pChat)
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 初始用计算机名称
    int len = MAX_NICK_LEN;
    GetComputerName(pChat->wszNick, &len);

    PADDRINFOT pInfo;
    GetAddrInfo(pChat->wszNick, NULL, NULL, &pInfo);
    PADDRINFOT pCur = pInfo;
    while (pCur) {
        if (pCur->ai_family == AF_INET) {
            DWORD dwLen = 32;
            WSAAddressToString(pCur->ai_addr, sizeof(SOCKADDR), NULL, s_wsaLocalIP, &dwLen);
            break;
        }
        pCur = pCur->ai_next;
    }
    FreeAddrInfo(pInfo);

    // 创建广播socket
    s_skt15673 = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
    s_addr15673.sin_family = AF_INET;
    s_addr15673.sin_port = htons(15673);
    s_addr15673.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(s_skt15673, (PSOCKADDR)&s_addr15673, sizeof(SOCKADDR_IN));

    int nOptVal = 1;
    setsockopt(s_skt15673, SOL_SOCKET, SO_REUSEADDR|SO_BROADCAST, (char*)&nOptVal, sizeof(int));

    pChat->ev15673 = CreateEvent(NULL, TRUE, FALSE, NULL);
    WSAEventSelect(s_skt15673, pChat->ev15673, FD_READ);

    // 监听TCP
    s_skt15683 = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    s_addr15683.sin_family = AF_INET;
    s_addr15683.sin_port = htons(15683);
    s_addr15683.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(s_skt15683, (PSOCKADDR)&s_addr15683, sizeof(SOCKADDR_IN));
    listen(s_skt15683, 32);

    pChat->ev15683 = CreateEvent(NULL, TRUE, FALSE, NULL);
    WSAEventSelect(s_skt15683, pChat->ev15683, FD_ACCEPT|FD_READ);
}

void ChatShutdown(Chat *pChat)
{
    if (s_skt15673 != INVALID_SOCKET)
        closesocket(s_skt15673);
    if (s_skt15683 != INVALID_SOCKET)
        closesocket(s_skt15683);

    WSACleanup();

    CloseHandle(pChat->ev15673);
    CloseHandle(pChat->ev15683);
}


void ChatBroadcast(char *buf, int size)
{
    SOCKADDR_IN addr255;
    addr255.sin_family = AF_INET;
    addr255.sin_port = htons(15673);
    InetPton(AF_INET, s_wsaLocalIP, &addr255.sin_addr);

    for (int i = 1; i < 255; i++) {
        addr255.sin_addr.S_un.S_un_b.s_b4 = i; // 广播好像被交换机阻了，只好每个都发一个包好了
        DWORD dwSent = 0;
        int nRet = sendto(s_skt15673, buf, size, 0, (PSOCKADDR)&addr255, sizeof(SOCKADDR_IN));
        if (nRet == 0) {
            int error = WSAGetLastError();
        }
    }
}

void OnBroadcast()
{
    BroadcastPack pack;
    SOCKADDR from;
    int fromLen = sizeof(SOCKADDR);
    recvfrom(s_skt15673, (char*)&pack, sizeof(BroadcastPack), 0, &from, &fromLen);

    if (memcmp(&pack.tag, "IC2016IC", 8) != 0) return;
    if (memcmp(&pack.cmd, "REPT", 4) == 0) {
        PostMessage(g_chat.hwndMain, WM_COMMAND, MAKEWPARAM(ID_MSG_MYID, 0), 0);
    }
    else if (memcmp(&pack.cmd, "MYID", 4) == 0) {
        wchar_t xfrom[32];
        DWORD dwLen = 32;
        WSAAddressToString(&from, sizeof(SOCKADDR), NULL, xfrom, &dwLen);
        SendMessage(g_chat.hwndMain, WM_UPDATE_ID, (WPARAM)xfrom, (LPARAM)(pack.buf));
    }
    else {
        OutputDebugString(L"不识别的命令\r\n");
    }
}

void ChatAcceptOrRead()
{
    WSANETWORKEVENTS ev;
    WSAEnumNetworkEvents(s_skt15683, g_chat.ev15683, &ev);
    if (ev.lNetworkEvents == FD_ACCEPT) {
        SOCKADDR from;
        int nLen = sizeof(SOCKADDR);
        static SOCKET skt2;
        skt2 = accept(s_skt15683, &from, &nLen);

        wchar_t xfrom[32];
        DWORD dwLen = 32;
        WSAAddressToString(&from, sizeof(SOCKADDR), NULL, xfrom, &dwLen);

        int nTry = 0;
        char buf[4096];
        while (recv(skt2, buf, sizeof(buf), 0) == SOCKET_ERROR) {
            nTry++;
            if (nTry > 5)
            {
                StringCchPrintf((wchar_t*)buf, 2048, L"接受失败");
                break;
            }
            else
             Sleep(500);
        }

        SendMessage(g_chat.hwndMain, WM_INCOMING_MSG, (WPARAM)xfrom, (LPARAM)buf);
        closesocket(skt2);
    }
}

void ChatSend(wchar_t *pIP, wchar_t *pwszText)
{
    SOCKET skt = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(15683);
    InetPton(AF_INET, pIP, &addr.sin_addr);

    if (connect(skt, (PSOCKADDR)&addr, sizeof(SOCKADDR_IN)) == 0) {
        send(skt, (char*)pwszText, (int)(wcslen(pwszText)+1) * sizeof(wchar_t), 0);
        closesocket(skt);
    }
}