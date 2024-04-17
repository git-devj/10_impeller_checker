#pragma once
#include "afxdialogex.h"
#include "MYclnt.h"


// MFCSEARCH 대화 상자

class MFCSEARCH : public CDialogEx
{
	DECLARE_DYNAMIC(MFCSEARCH)

public:
	MFCSEARCH(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MFCSEARCH();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSEARCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//세정
	MYclnt m_clnt;
	CListCtrl m_list;
	void DrawPieChart(int normalCnt, int defectCnt);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSsearch();
};
