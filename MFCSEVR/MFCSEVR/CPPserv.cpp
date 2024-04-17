#include "pch.h"
#include "MFCSEVR.h"
#include "CPPserv.h"
#include "iostream"
#include <afx.h>
#include <fstream>
#include <mysql.h>
#include <sstream>
#include <vector>

void CPPserv::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	ShutDown(2);
	Close();
}
