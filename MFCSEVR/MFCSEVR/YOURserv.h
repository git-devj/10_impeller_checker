#pragma once
#include <afxsock.h>
#include "CPPserv.h"

class YOURserv : public CSocket
{
private:
    CPPserv m_cpp;
    static int count[];

public:
    virtual void OnAccept(int nErrorCode);
    void RecvSearch();
};

