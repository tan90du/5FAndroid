//
// Created by 郑伟 on 2021/5/20.
//

#ifndef ANDROID_TCPSERVERIMP_H
#define ANDROID_TCPSERVERIMP_H

#include "TcpPackServer.h"
//#include "TcpPullServer.h"
typedef std::function<void(int, const char*)> onDataCallback;
typedef std::function<void()> OnAcceptCallBack;
typedef std::function<void()> OnCloseCallBack;


class TcpServerImp  : public CTcpServerListener{
public:
    TcpServerImp();
    ~TcpServerImp();

    void setOnAcceptCallBack(OnAcceptCallBack callback) { mOnAcceptCallBack = callback; };
    void setOnCloseCallBack(OnCloseCallBack callback) { mOnCloseCallBack = callback; };
    void setDataCallBack(onDataCallback callback) { mDataCallback = callback; };

    EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen) override;
    EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient) override;
    EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID) override;
    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
    EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
    EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;
    EnHandleResult OnShutdown(ITcpServer* pSender) override;

    CONNID getConnectId() { return mConnID; }
private:
    CONNID mConnID = 0;
    onDataCallback mDataCallback;
    OnAcceptCallBack mOnAcceptCallBack;
    OnCloseCallBack mOnCloseCallBack;
};


#endif //ANDROID_TCPSERVERIMP_H
