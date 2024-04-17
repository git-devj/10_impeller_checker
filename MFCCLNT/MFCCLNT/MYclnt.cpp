#include "pch.h"
#include "MYclnt.h"
#include "iostream"
#include "atlimage.h"
#include <fstream>

void MYclnt::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	ShutDown(2);
	Close();
}
