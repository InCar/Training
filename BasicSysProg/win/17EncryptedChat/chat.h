#pragma once

#define MAX_NICK_LEN 32
#define MAX_MESSAGE_LEN 256

typedef struct tagMsgPack
{
    wchar_t wszNick[MAX_NICK_LEN]; // 昵称
    wchar_t wszMsg[MAX_MESSAGE_LEN]; // 内容
} MsgPack;

typedef struct tagChat
{
    HWND hwndMain;
    HWND hwndHistory;
    HWND hwndList;
    HWND hwndInput;
    HWND hwndSend;
    HWND hwndRefresh;
    HFONT hfontText;

    wchar_t wszNick[MAX_NICK_LEN];

    HANDLE ev15673;
    HANDLE ev15683;
} Chat;

extern Chat g_chat;

typedef struct tagBroadcastPack
{
    char tag[8]; // 标志位 IC2016IC
    char cmd[4]; // REPT 请求全体广播自己身份; MYID 报告自己的身份
    unsigned char len; // 消息体长度
    char buf[256]; // 消息体
} BroadcastPack;

void ChatStartup(Chat*);
void ChatShutdown(Chat*);
// 局域网内广播消息
void ChatBroadcast(char*, int);
void OnBroadcast();
// 发送信息
void ChatAcceptOrRead();
void ChatSend(wchar_t *pIP, wchar_t *pwszText);