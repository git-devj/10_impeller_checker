#pragma once
#include <afxsock.h>
#include "UserSock.h"

class MYserv : public CSocket
{
private:
	UserSock m_user;
	
public:
	virtual void OnAccept(int nErrorCode);
};

