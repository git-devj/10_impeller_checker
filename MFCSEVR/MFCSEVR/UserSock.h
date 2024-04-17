#pragma once

#include "opencv2/opencv.hpp"
#define DB_HOST "127.0.0.1"	// 서버 아이피
#define DB_USER "root"	// DB 접속계정
#define DB_PASS "1234"	// DB 계정암호
#define DB_NAME "defect_test"	// DB 이름

// UserSock 명령 대상

class UserSock : public CSocket
{
public:
	UserSock();
	virtual ~UserSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void ThreeCircles();
	int BurrImpurity();
	cv::Mat Cam; // Mat 클래스 Cam
	CImage Camimg; // CImage 클래스 Camimg

	void DatabaseOn();
	void Database_filename(std::string n_strImage);
	void Database_result(int num);

};


