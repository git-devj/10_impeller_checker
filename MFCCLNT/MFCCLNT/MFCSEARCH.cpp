// MFCSEARCH.cpp: 구현 파일
//

#include "pch.h"
#include "MFCCLNT.h"
#include "afxdialogex.h"
#include "MFCSEARCH.h"
#include "MYclnt.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

const double pi = 3.1415926535;

// MFCSEARCH 대화 상자

IMPLEMENT_DYNAMIC(MFCSEARCH, CDialogEx)

MFCSEARCH::MFCSEARCH(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSEARCH, pParent)
{

}

MFCSEARCH::~MFCSEARCH()
{
}

void MFCSEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(MFCSEARCH, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SSEARCH, &MFCSEARCH::OnBnClickedBtnSsearch)
END_MESSAGE_MAP()


// MFCSEARCH 메시지 처리기


BOOL MFCSEARCH::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW(_T("CLIENT"));
	SetWindowPos(NULL, 100, 500, 0, 0, SWP_NOSIZE);

	//m_clnt.Create();
	//if (m_clnt.Connect(_T("127.0.0.1"), 20201) == FALSE) {
	//	AfxMessageBox(_T("ERROR : Failed to connect Server"));
	//	PostQuitMessage(0);
	//	return FALSE;
	//}
	//else
	//	AfxMessageBox(_T("Success to connect Server"));

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T("no"), 0, 30);
	m_list.InsertColumn(1, _T("file"), 0, 110);
	m_list.InsertColumn(2, _T("status"), 0, 50);
	m_list.InsertColumn(3, _T("reason"), 0, 60);

	return TRUE;
}


void MFCSEARCH::OnBnClickedBtnSsearch()
{
	m_clnt.Create();
	m_clnt.Connect(L"10.10.20.99", 20201);

	CString strdate;
	GetDlgItemTextW(IDC_DATETIMEPICKER1, strdate);

	int num = 5;
	m_clnt.Send(&num, sizeof(int));
	std::cout << "조회 5번이야" << std::endl;
	Sleep(1000);
	char cdate[100];
	sprintf_s(cdate, "%S", (LPCTSTR)strdate);
	if (0 != m_clnt.Send(&cdate, sizeof(cdate))) {
		std::cout << "Send ok----------------------" << cdate << std::endl;
	}

	char strBuffer[1024];
	int nLen = m_clnt.Receive(strBuffer, sizeof(strBuffer));
	strBuffer[nLen] = 0;

	std::cout << "recv : " << strBuffer << std::endl;

	std::vector<std::string> tokens;
	std::stringstream ss(strBuffer);
	std::string token;
	int nCnt = 0;
	while (std::getline(ss, token, ','))
	{
		tokens.push_back(token);
		nCnt++;
	}

	m_list.DeleteAllItems();
	int normalCnt = 0;
	int defectCnt = 0;

	int index = 0;
	for (int i = 0; i < nCnt / 3; i++)
	{
		if (strcmp(tokens[0].c_str(), "no") == 0)
		{
			AfxMessageBox(_T("No data on selected date"));
			break;
		}
		CString strindex;
		strindex.Format(_T("%d"), index + 1);

		m_list.InsertItem(index, strindex, 0);
		for (int j = 0; j < 3; j++)
		{
			m_list.SetItemText(index, j + 1, CString(tokens[(i * 3) + j].c_str()));
			if (j == 1 && strcmp(tokens[(i * 3) + j].c_str(), "normal") == 0)
			{
				normalCnt++;
			}
			if (j == 1 && strcmp(tokens[(i * 3) + j].c_str(), "defect") == 0)
			{
				defectCnt++;
			}
		}
		index++;
	}

	if (strcmp(tokens[0].c_str(), "no") != 0)
		DrawPieChart(normalCnt, defectCnt);
	else
	{
		CRect rect(1, 1, 253, 253);
		GetDlgItem(IDC_PICTURE)->GetDC()->FillSolidRect(&rect, RGB(240, 240, 240));
	}
	m_clnt.Close();
}

void MFCSEARCH::DrawPieChart(int normalCnt, int defectCnt)
{
	CClientDC dc(this);
	CRect rect;
	GetDlgItem(IDC_PICTURE)->GetWindowRect(&rect);
	ScreenToClient(&rect);


	double total = normalCnt + defectCnt;
	double normalrate = (normalCnt / total) * 100;
	double defectrate = (defectCnt / total) * 100;

	int centerX = rect.Width() / 2;
	int centerY = rect.Height() / 2;

	int Xpos = static_cast<int>(centerX * cos((360 * normalrate / 100) * pi / 180)) + rect.CenterPoint().x;
	int Ypos = static_cast<int>(centerY * sin((360 * normalrate / 100) * pi / 180)) + rect.CenterPoint().y;

	int normalX = static_cast<int>(centerX / 2 * cos((360 * normalrate / 2 / 100) * pi / 180)) + rect.CenterPoint().x;;
	int normalY = static_cast<int>(centerY / 2 * sin((360 * normalrate / 2 / 100) * pi / 180)) + rect.CenterPoint().y;;

	int defectX = static_cast<int>(centerX / 2 * cos((360 * (normalrate + defectrate / 2) / 100) * pi / 180)) + rect.CenterPoint().x;
	int defectY = static_cast<int>(centerY / 2 * sin((360 * (normalrate + defectrate / 2) / 100) * pi / 180)) + rect.CenterPoint().y;

	CBrush greenbrush, redbrush;
	greenbrush.CreateSolidBrush(RGB(0, 106, 213));
	redbrush.CreateSolidBrush(RGB(203, 34, 10));

	if (normalCnt != 0)
	{
		dc.SelectObject(&greenbrush);
		dc.Ellipse(&rect);
	}
	else
	{
		dc.SelectObject(&redbrush);
		dc.Ellipse(&rect);
	}

	if (defectCnt != 0 && normalCnt != 0)
	{
		dc.SelectObject(&redbrush);
		dc.Pie(rect.TopLeft().x, rect.TopLeft().y, rect.BottomRight().x, rect.BottomRight().y, rect.CenterPoint().x, rect.CenterPoint().y, Xpos, Ypos);
	}

	CString strnormalrate, strdefectrate;
	strnormalrate.Format(_T("%.1f%%"), normalrate);
	strdefectrate.Format(_T("%.1f%%"), defectrate);

	dc.SetBkMode(TRANSPARENT);
	if (normalCnt != 0)
	{
		dc.TextOutW(normalX, normalY, _T("normal"));
		dc.TextOutW(normalX, normalY + 15, strnormalrate);
	}
	if (defectCnt != 0)
	{
		dc.TextOutW(defectX, defectY, _T("defect"));
		dc.TextOutW(defectX, defectY + 15, strdefectrate);
	}
}