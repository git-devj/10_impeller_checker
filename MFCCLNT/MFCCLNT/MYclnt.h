#pragma once
#include <afxsock.h>
class MYclnt : public CSocket
{
public:
	virtual void OnClose(int nErrorCode);
};

