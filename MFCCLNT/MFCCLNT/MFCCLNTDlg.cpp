
// MFCCLNTDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCCLNT.h"
#include "MFCCLNTDlg.h"
#include "afxdialogex.h"
#include "MFCSEARCH.h"
#include "ctime"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
// CMFCCLNTDlg 대화 상자



CMFCCLNTDlg::CMFCCLNTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCLNT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCCLNTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_PICUTRE, m_Picture);
}

BEGIN_MESSAGE_MAP(CMFCCLNTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, &CMFCCLNTDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CMFCCLNTDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_START, &CMFCCLNTDlg::OnBnClickedBtnStart)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CMFCCLNTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	Capture = new cv::VideoCapture(0); // 첫 번째 장비로 하는 캡처 객체
	if (!Capture->isOpened()) {
		MessageBox(_T("웹캠을 열 수 없습니다."));
	} // 웹캠을 열기 실패하면 메시지 상자 표시

	Capture->set(cv::CAP_PROP_FRAME_WIDTH, 320); // 웹캠 가로(너비)를 320으로 설정
	Capture->set(cv::CAP_PROP_FRAME_HEIGHT, 240); // 웹캠 세로(높이)를 240으로 설정

	SetWindowPos(NULL, 100, 500, 0, 0, SWP_NOSIZE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CMFCCLNTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMFCCLNTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCCLNTDlg::OnBnClickedBtnSend()
{
	m_clnt.Create();
	m_clnt.Connect(L"10.10.20.99", 20201);

	const char* filePath = "C:/Users/lms/Documents/Visual Studio 2022/up/MFCCLNT/MFCCLNT/SEND/save.jpg";
	cv::imwrite(filePath, Cam);

	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		std::cout << "Not open file: " << filePath << std::endl;
		return;
	}

	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	m_clnt.Send((const char*)&fileSize, sizeof(fileSize));

	char* buf = new char[fileSize];
	file.read(buf, fileSize);
	m_clnt.Send(buf, fileSize);

	delete[] buf;
	file.close();
	m_clnt.Close();
}


void CMFCCLNTDlg::OnBnClickedBtnSearch()
{
	m_clnt.Close();
	MFCSEARCH dlg;
	dlg.DoModal();
}


void CMFCCLNTDlg::OnBnClickedBtnStart()
{
	SetTimer(1000, 30, NULL);
}


void CMFCCLNTDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
	if (nIDEvent == 1000) {
		Capture->read(Cam); // 캡처를 읽어서 Mat Cam에 담음

		int bpp = 8 * Cam.elemSize(); // Bit per pixel = 8 * Cam의 행렬 요소 하나의 바이트 크기(채널) / 3채널(BGR)이므로 24
		assert((bpp == 8 || bpp == 24 || bpp == 32)); // bpp가 8/24/32 중 하나가 아니면 에러 | assert 함수(): 식이 참이면 통과, 거짓이면 에러

		int padding = 0; // 캠 안쪽 여백(테두리와 캠 사이) 크기
		if (bpp < 32)
			padding = 4 - (Cam.cols % 4); // bpp 32보다 작으면 여백을 4 - (캠의 가로 픽셀 크기 % 4)

		if (padding == 4)
			padding = 0; // 여백이 4면 0으로 설정

		int border = 0; // 캠 바깥(테두리) 크기
		if (bpp < 32)
			border = 4 - (Cam.cols % 4); // bpp 32보다 작으면 테두리를 4 - (캠의 가로 픽셀 크기 % 4)

		cv::Mat Mat_temp; // Mat 클래스 Mat_temp
		if (border > 0 || Cam.isContinuous() == false) // border가 0 보다 크거나 원소가 연속적으로 저장되어 있지 않으면 
			cv::copyMakeBorder(Cam, Mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
		// 입력 Cam의 영상을 <0, 0, 0, 테두리> 크기의 단색 경계, 검은색으로 테두리 만들어 Mat_temp에 담음
		// 테두리 만드는 함수 | copyMakeBorder(입력 Mat, 저장될 Mat, <테두리>(위 크기, 아래 크기, 왼쪽 크기, 오른쪽 크기), 테두리 형식, 색상)
		else
			Mat_temp = Cam; // 위에 해당하지 않으면 Mat_temp = Cam

		RECT sz_rect; // 사각형 정의 구조체 sz_rect;
		m_Picture.GetClientRect(&sz_rect); // m_Picture의 변수 정보(IDC_PIC_PICUTRE의 크기)를 sz_rect에 저장

		cv::Size winSize(sz_rect.right, sz_rect.bottom);
		// 픽셀을 그룹화하기 위해 사용되는 창 크기를 sz_rect의 오른쪽, 아래 크기로 만듦
		// 픽셀 그룹화를 위한 창 크기 설정 함수 | winSize(오른쪽, 하단)

		Camimg.Create(winSize.width, winSize.height, 24); // 위의 창으로 크기를 정하고 24bpp로 만듦
		// 이미지 생성 함수 | Create(너비<가로>, 높이<세로>, bpp<그레이 레벨>)

		BITMAPINFO* BitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)); // 비트맵 넓이 및 컬러 포맷 정보 저장
		BitInfo->bmiHeader.biBitCount = bpp; // 픽셀 근처의 비트 수 지정
		BitInfo->bmiHeader.biWidth = Mat_temp.cols; // 비트맵 너비(가로) 지정
		BitInfo->bmiHeader.biHeight = -Mat_temp.rows; // 비트맵 높이(세로) 지정
		BitInfo->bmiHeader.biPlanes = 1; // 장치의 면 수, 반드시 1
		BitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); // 필요로 하는 바이트 수 저장
		BitInfo->bmiHeader.biCompression = BI_RGB; // 비트맵 압축 시 압축 지정 | BI_RGB: 비압축 RGB
		BitInfo->bmiHeader.biClrImportant = 0; // 비트맵 표시를 위한 중요 컬러 인덱스 수 지정 | 0이면 모든 색 중요
		BitInfo->bmiHeader.biClrUsed = 0; // 컬러 테이블의 컬러 인덱스 중 비트맵에서 실제로 사용하는 인덱스 수 지정
		BitInfo->bmiHeader.biSizeImage = 0; // 이미지 크기 지정(바이트 단위) | 비압축 RGB라면 0 설정 가능
		BitInfo->bmiHeader.biXPelsPerMeter = 0; // 비트맵 타깃 장치의 수평 해상도, 1미터당 픽셀 단위로 지정
		BitInfo->bmiHeader.biYPelsPerMeter = 0; // 비트맵 타깃 장치의 수직 해상도, 1미터당 픽셀 단위로 지정

		if (bpp == 8)
		{
			RGBQUAD* grpalette = BitInfo->bmiColors;
			for (int i = 0; i < 256; i++) {
				grpalette[i].rgbBlue = grpalette[i].rgbGreen = grpalette[i].rgbRed = (BYTE)i;
				grpalette[i].rgbReserved = 0;
			}
		} // 그레이 스케일이라면 팔레트 지정

		if (Mat_temp.cols == winSize.width && Mat_temp.rows == winSize.height)
		{
			SetDIBitsToDevice(Camimg.GetDC(), 0, 0, winSize.width, winSize.height, 0, 0, 0, Mat_temp.rows, Mat_temp.data, BitInfo, DIB_RGB_COLORS);
		} // 이미지와 창 크기가 같으면 설정대로 화면 출력
		// 화면 출력 함수 | // SetDIBitsToDevice(출력 대상의 DC<디바이스 컨텍스트> 핸들, 출력할 위치의 x 좌표, 출력할 위치의 y 좌표, 비트맵 너비, 비트맵 높이, 출력할 원본 비트맵의 x 좌표, 
		// 출력할 원본 비트맵의 y 좌표, 출력할 원본 비트맵의 시작 라인, 출력할 원본 비트맵의 라인 수, 바이트 배열로 저장된 비트맵 포인터, 비트맵 정보가 저장된 BITMAPINFO 포인터, RGB나 팔레트 인덱스 정보)
		// RGB는 DIB_RGB_COLORS / 팔레트는 DIB_PAL_COLORS
		else
		{
			int destX = 0;
			int destY = 0;
			int destW = winSize.width;
			int destH = winSize.height;

			int imgx = 0;
			int imgy = 0;
			int imgWidth = Mat_temp.cols - border;
			int imgHeight = Mat_temp.rows;

			StretchDIBits(Camimg.GetDC(), destX, destY, destW, destH, imgx, imgy, imgWidth, imgHeight, Mat_temp.data, BitInfo, DIB_RGB_COLORS, SRCCOPY);
			// 비트맵을 설정대로 출력
			// 비트맵 확대/축소 출력 함수 | StretchDIBit(출력 대상의 DC<디바이스 컨텍스트> 핸들, 출력할 위치의 x 좌표, 출력할 위치의 y 좌표, 출력 대상의 사각형 너비, 출력 대상의 사각형 높이, 출력할 원본 비트맵의 x 좌표, 
			// 출력할 원본 비트맵의 y 좌표, 출력할 원본 비트맵의 사각형 너비, 출력할 원본 비트맵의 사각형 높이, 바이트 배열로 저장된 비트맵 포인터, 비트맵 정보가 저장된 BITMAPINFO 포인터,  RGB나 팔레트 인덱스 정보, 래스터 연산 코드)
		}

		HDC dc = ::GetDC(m_Picture.m_hWnd);
		// 그림 창을 얻어옴
		// 창의 DC를 얻는 함수 | ::GetDC(창 핸들)
		Camimg.BitBlt(dc, 0, 0); // 캠 이미지를 dc로 복사함 | DC 간 복사 함수
		// BitBlt(사본 DC, 사본 비트맵의 복사 시작 x 좌표, 사본 비트맵의 복사 시작 y 좌표), 복사할 만큼의 원본 그림 폭, 복사할 만큼의 원복 그림 높이, 
		// 원본 DC, 원본 비트맵의 복사 시작 x 좌표, 원본 비트맵의 복사 시작 y 좌표, 복사 방법 설정

		::ReleaseDC(m_Picture.m_hWnd, dc);
		// m_hWnd에 생성한 dc 제거
		// DC 제거 함수 | ::ReleaseDC(창 핸들, 제거할 DC)

		Camimg.ReleaseDC();
		Camimg.Destroy();
	}
}


void CMFCCLNTDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	Capture->release(); // 캡처 없으면 캡처 객체 삭제하여 메모리 누수 방지
}

