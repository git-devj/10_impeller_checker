#pragma once
#include <afxsock.h>

#define DB_HOST "127.0.0.1"	// ���� ������
#define DB_USER "root"	// DB ���Ӱ���
#define DB_PASS "1234"	// DB ������ȣ
#define DB_NAME "defect_test"	// DB �̸�

class CPPserv : public CSocket
{

public:
	virtual void OnClose(int nErrorCode);
};

