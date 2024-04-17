
// impeller_clientDlg.h: 헤더 파일
//

#pragma once

// OpenCv를 위한 헤더 파일
#include "opencv2/opencv.hpp"

// C++를 위한 헤더 파일
#include <iostream>

// CimpellerclientDlg 대화 상자
class CimpellerclientDlg : public CDialogEx
{
// 생성입니다.
public:
	CimpellerclientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 캠을 위한 멤버 변수
	cv::VideoCapture *Capture; // VideoCapture 캡처 포인터
	cv::Mat Cam; // Mat 클래스 Cam
	CImage Camimg; // CImage 클래스 Camimg

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPELLER_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	CStatic m_Picture;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnCut();
};
