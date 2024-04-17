#include "pch.h"
#include "MYserv.h"

void MYserv::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	Accept(m_user);
}
