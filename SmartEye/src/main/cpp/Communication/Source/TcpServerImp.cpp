//
// Created by 郑伟 on 2021/5/20.
//

#include "TcpServerImp.h"

#include "Log.h"
TcpServerImp::TcpServerImp(){

}

TcpServerImp::~TcpServerImp(){

}

EnHandleResult TcpServerImp::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
{
    Log::info("TcpServerImp", "OnPrepareListen");
    return HR_OK;
}

EnHandleResult TcpServerImp::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
{
    Log::info("TcpServerImp", "收到TCP连接,%lld", dwConnID);
    mConnID = dwConnID;
    // 回调给使用方
    mOnAcceptCallBack();
    return HR_OK ;
}

EnHandleResult TcpServerImp::OnHandShake(ITcpServer* pSender, CONNID dwConnID)
{
    Log::info("TcpServerImp", "OnHandShake：%d", dwConnID);

    return HR_OK;
}

EnHandleResult TcpServerImp::OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
    mDataCallback(iLength, (const char*)pData);
    return HR_OK;
}

EnHandleResult TcpServerImp::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
  //  Log::info("TcpServerImp", "OnSend：%d, length:%d", dwConnID, iLength);
    return HR_OK;
}

EnHandleResult TcpServerImp::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
    Log::info("TcpServerImp", "OnClose:%d", dwConnID);
    mConnID = 0;
//    Log::info("TcpServerImp", "OnClose333");
//    mOnCloseCallBack();
//    Log::info("TcpServerImp", "OnClose444");
    return HR_OK;
}

EnHandleResult TcpServerImp::OnShutdown(ITcpServer* pSender)
{
    Log::info("TcpServerImp", "OnShutdown");
    mConnID = 0;
    return HR_OK;
}