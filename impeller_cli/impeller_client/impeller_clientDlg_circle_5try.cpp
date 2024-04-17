
// impeller_clientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "impeller_client.h"
#include "impeller_clientDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CimpellerclientDlg 대화 상자



CimpellerclientDlg::CimpellerclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPELLER_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CimpellerclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_PICUTRE, m_Picture);
}

BEGIN_MESSAGE_MAP(CimpellerclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CimpellerclientDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_START, &CimpellerclientDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_CUT, &CimpellerclientDlg::OnBnClickedBtnCut)
END_MESSAGE_MAP()


// CimpellerclientDlg 메시지 처리기

BOOL CimpellerclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Capture = new cv::VideoCapture(0); // 첫 번째 장비로 하는 캡처 객체
	if (!Capture->isOpened()) {
		MessageBox(_T("웹캠을 열 수 없습니다."));
	} // 웹캠을 열기 실패하면 메시지 상자 표시

	Capture->set(cv::CAP_PROP_FRAME_WIDTH, 320); // 웹캠 가로(너비)를 320으로 설정
	Capture->set(cv::CAP_PROP_FRAME_HEIGHT, 240); // 웹캠 세로(높이)를 240으로 설정

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CimpellerclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CimpellerclientDlg::OnPaint()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CimpellerclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CimpellerclientDlg::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetTimer(1000, 30, NULL);
	// 이벤트 아이디가 1인 타이머 0.03초(30ms)마다 OnTimer() 함수 작동
	// 타이머 설정 | SetTimer(타이머 ID, ms 설정, 실행할 함수명)
}

void CimpellerclientDlg::OnBnClickedBtnExit() // 종료 버튼을 누르면
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxGetMainWnd()->PostMessage(WM_CLOSE); // 종료 메시지 보내서 종료
}


void CimpellerclientDlg::OnDestroy() // 메모리 누수 방지 코드
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Capture->release(); // 캡처 없으면 캡처 객체 삭제하여 메모리 누수 방지
}

void CimpellerclientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
		for(int i = 0; i < 256; i++) {
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

	CDialogEx::OnTimer(nIDEvent);
}

void CimpellerclientDlg::OnBnClickedBtnCut()
{
	//TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	


	//// 허프 함수 3회 // !!! 통과 !!!
	cv::Mat input_img; // 이미지 입력받을 Mat
	Capture->read(input_img); // 캠 이미지를 읽어서 저장

	cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 흑백화함

	std::vector<cv::Vec3f> small_circles; // 작은 원 정보 담는 벡터
	cv::HoughCircles(gr_img, small_circles, cv::HOUGH_GRADIENT, 1, 30, 150, 30, 0, 30); 
	// 허프 원 변환 알고리즘을 통한 원 검출 | 허프 원함수
	// (입력받은 영상<그레이스케일만 받음>, 원 정보 담을 벡터, 방법<고정>, 입력/축적의 비율, 인접 원 중심 최소 거리, Canny 검출기 높은 임곗값, 
	// 중심 임곗값(원 검출 위한 정보로 낮으면 더 많은 원, 높으면 더 적은<정확한> 원), 검출할 원 최소 반지름, 검출할 원 최대 반지름)

	std::vector<cv::Vec3f> mid_circles;
	cv::HoughCircles(gr_img, mid_circles, cv::HOUGH_GRADIENT, 1, 30, 150, 60, 50, 70);

	std::vector<cv::Vec3f> big_circles;
	cv::HoughCircles(gr_img, big_circles, cv::HOUGH_GRADIENT, 1, 30, 150, 47, 90, 123);


	cv::Mat color_img; // 컬러 이미지 받을 Mat
	cvtColor(gr_img, color_img, cv::COLOR_GRAY2BGR);
	
	for (const auto& cs : small_circles) { // 원 그리기
		cv::Point centers(cvRound(cs[0]), cvRound(cs[1])); // 중심점 좌표 포인트
		int radiuss = cvRound(cs[2]); // 반지름 포인트
		//cv::circle(color_img, centers, 2, cv::Scalar(255, 255, 255), 1, cv::LINE_AA); // 중심점 찍기
		cv::circle(color_img, centers, radiuss, cv::Scalar(0, 0, 255), 3, cv::LINE_AA); 
		// 원 그리기 함수 | 원함수
		// (원그림을 받을 이미지, 원의 중심 좌표, 원의 반지름, 선 색상(BGR 순) 입력, 선 굵기, 선 타입)
		cv::putText(color_img, std::to_string((int)cs[2]), cv::Point((int)cs[0] - 10, (int)cs[1] + 50), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
		// 영상에 문자열 출력하는 함수 | 텍스트 출력 함수
		// (텍스트가 쓰일 이미지, 입력할 텍스트, 텍스트 출력할 위치<x 좌표, y 좌표>, 폰트 종류, 폰트 크기(확대 비율), 폰트 색상, 문자 두께)
	}

	for (const auto& cm : mid_circles) {
		cv::Point centerm(cvRound(cm[0]), cvRound(cm[1]));
		int radiusm = cvRound(cm[2]);
		cv::circle(color_img, centerm, radiusm, cv::Scalar(0, 255, 255), 3, cv::LINE_AA);
		cv::putText(color_img, std::to_string((int)cm[2]), cv::Point((int)cm[0] - 15, (int)cm[1] + 85), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1);
	}

	for (const auto& cb : big_circles) {
		cv::Point centerb(cvRound(cb[0]), cvRound(cb[1]));
		int radiusb = cvRound(cb[2]);
		cv::circle(color_img, centerb, radiusb, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
		cv::putText(color_img, std::to_string((int)cb[2]), cv::Point((int)cb[0] - 15, (int)cb[1] + 121), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
	}

	cv::imshow("result", color_img); // 이미지 출력 함수 | ("창 이름", 불러올 이미지)
	//// 허프 함수 3회 // !!! 통과 !!!



	//// 이진화 진행하여 윤곽선 검출 (원 반지름이 측정이 안 되어서 스킵)
	//cv::Mat input_img; // 이미지 입력받을 Mat
	//Capture->read(input_img); // 캠 이미지를 읽어서 저장

	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	//cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 흑백화 함
	////cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	//cv::Mat median;
	//cv::medianBlur(gr_img, median, 5);

	//cv::Mat bin;
	//cv::threshold(median, bin, 120, 255, cv::THRESH_BINARY);

	//cv::Mat open, close;
	//cv::Mat element_open = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	//cv::Mat element_close = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(31, 31));
	//
	//cv::morphologyEx(bin, open, cv::MORPH_OPEN, element_open);
	//cv::morphologyEx(open, close, cv::MORPH_CLOSE, element_close);

	//cv::Mat label, stats, centroids;
	//int num = cv::connectedComponentsWithStats(close, label, stats, centroids, 8, CV_32S);

	//cv::Mat src_color;
	//cv::cvtColor(gr_img, src_color, cv::COLOR_GRAY2BGR);

	//cv::RNG rng(12345);
	//for (int i = 1; i < num; ++i) {
	//	int* p = stats.ptr<int>(i);
	//	double* c = centroids.ptr<double>(i);

	//	int radius = static_cast<int>(p[2] / 2);

	//	cv::circle(src_color, cv::Point((int)c[0], (int)c[1]), radius, cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)), 4);
	//	
	//	if (radius < 70) {
	//		cv::putText(src_color, "inside", cv::Point((int)c[0], (int)c[1]), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
	//	}
	//	else if (70 < radius && radius < 250) {
	//		cv::putText(src_color, "center", cv::Point((int)c[0], (int)c[1]), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
	//	}
	//	else if(radius < 300) {
	//		cv::putText(src_color, "outside", cv::Point((int)c[0], (int)c[1]), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
	//	}
	//}

	//cv::imshow("test", src_color);
	//// 이진화 진행하여 윤곽선 검출 (원 반지름이 측정이 안 되어서 스킵)



	//// 모든 외곽선 검출 (폐곡선이 아니고 완벽하지 않아서 스킵)
	//cv::Mat input_img; // 이미지 입력받을 Mat
	//Capture->read(input_img); // 캠 이미지를 읽어서 저장

	//cv::Mat in; // 그레이스케일 이미지 받을 Mat
	//cv::cvtColor(input_img, in, cv::COLOR_BGR2GRAY); // 이미지 흑백화 함
	////cv::Mat in = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);
	//cv::Mat binary;
	//cv::threshold(in, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	//std::vector<std::vector<cv::Point>> point_contours;
	//std::vector<cv::Vec4i> hierarcy;
	//cv::findContours(binary, point_contours, hierarcy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

	//cv::Mat out;
	//cvtColor(in, out, cv::COLOR_GRAY2BGR);

	//for (int i = 0; i < point_contours.size(); i++)
	//{
	//	cv::Scalar c(0, 0, 255);
	//	drawContours(out, point_contours, i, c, 2);
	//}

	//cv::imshow("test", out);
	//// 모든 외곽선 검출 (폐곡선이 아니고 완벽하지 않아서 스킵)



	//// 윤곽선 검출 (아예 못 읽어서 폐기)
	//cv::Mat input_img; // 이미지 입력받을 Mat
	//Capture->read(input_img); // 캠 이미지를 읽어서 저장

	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	//cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 흑백화 함
	////cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	//std::vector<std::vector<cv::Point>> contours;
	//std::vector<cv::Vec4i> hierarchy;

	//int mode = cv::RETR_EXTERNAL;
	//int method = cv::CHAIN_APPROX_SIMPLE;

	//cv::findContours(gr_img, contours, hierarchy, mode, method);

	//cv::Mat src_color;
	//cv::cvtColor(gr_img, src_color, cv::COLOR_GRAY2BGR);

	//cv::RNG rng(12345);
	//for (int i = 0; i < contours.size(); ++i) {
	//	cv::drawContours(src_color, contours, i, cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)), 2);

	//	for (int j = 0; j < contours[i].size(); ++j)
	//		cv::circle(src_color, contours[i][j], 1, cv::Scalar(0, 0, 255), cv::FILLED);
	//}

	//cv::imshow("test", src_color);
	//// 윤곽선 검출 (아예 못 읽어서 폐기)



	//// 허프 함수 원 검출 (동심원 불가로 스킵)
	//cv::Mat input_img; // 이미지 입력받을 Mat
	//Capture->read(input_img); // 캠 이미지를 읽어서 저장

	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	//cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 흑백화 함
	//cv::medianBlur(gr_img, gr_img, 5); // 메디안 블러로 노이즈 제거

	////cv::Mat img_houghC; // 원 잡을 Mat
	////gr_img.copyTo(img_houghC); // 흑백 이미지 복사

	//std::vector<cv::Vec3f> circles; // 원 정보 저장 변수
	//cv::HoughCircles(gr_img, circles, cv::HOUGH_GRADIENT, 1, 60, 50, 30, 0, 0);
	//// (검출할 흑백 이미지, 원 정보 저장할 벡터, 원 검출 방법<고정>, 영상과 축적크기배율, 인접원과 중심의 최소거리,  높은 임계값, 축적 배열 임계값, 원 최소 반지름, 원 최대 반지름

	//cv::Mat color_img;
	//cvtColor(gr_img, color_img, cv::COLOR_GRAY2BGR);

	//for (size_t i = 0; i < circles.size(); i++) {
	//	cv::Vec3i c = circles[i];
	//	cv::Point center(c[0], c[1]);
	//	int radius = c[2];

	//	cv::circle(color_img, center, radius, cv::Scalar(0, 255, 0), 2);
	//	cv::circle(color_img, center, 2, cv::Scalar(0, 0, 255), 3);
	//}

	//cv::imshow("result", color_img);
	//// 허프 함수 원 검출 (동심원 불가로 스킵)
}	
