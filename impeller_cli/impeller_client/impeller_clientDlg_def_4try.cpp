
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
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
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



	// 사출불량(Burr, 이바리, 가장자리 성형 불량) / 이물질(Impurity) 검출 | 탬플릿 매칭으로 시도 | 성공
	cv::Mat input_img; // 이미지 입력받을 Mat
	Capture->read(input_img); // 캠 이미지를 읽어서 저장

	cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 회색 조로 만듦
	//cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	cv::Mat burr_img = cv::imread("./def/burr.bmp", cv::IMREAD_GRAYSCALE); // Burr 이미지 회색 조로 만듦

	cv::Mat impurity_img = cv::imread("./def/impurity.bmp", cv::IMREAD_GRAYSCALE);

	cv::Mat tmpl_burr_img; // 템플릿 매칭 받을 Mat
	cv::matchTemplate(gr_img, burr_img, tmpl_burr_img, cv::TM_CCOEFF_NORMED); // 템플릿 매칭 함수 | 유사도 계산
	// matchTemplate(입력 Mat, 템플릿 Mat, 유사도 결과 Mat, 유사도 계산 방법, 마스크 영상<생략 시 noArray()>)
	// gr_img와 burr_img의 유사도를 TM_CCOEFF_NORMED 방법으로 계산하여 res_img에 저장

	cv::Mat tmpl_impurity_img;
	cv::matchTemplate(gr_img, impurity_img, tmpl_impurity_img, cv::TM_CCOEFF_NORMED);

	double min_bVal, max_bVal; // 최솟값, 최댓값을 받을 doule형 변수 선언
	//cv::Point minLoc, maxLoc; // 최솟값, 최댓값 좌표를 받을 Point형 변수 선언
	cv::minMaxLoc(tmpl_burr_img, 0, &max_bVal, 0, 0); // 행렬의 최솟값/최댓값을 찾는 함수
	// minMaxLoc(입력 Mat, <주소의 경우 필요 없으면 0 지정>최솟값을 받을 double형 변수 주소, 최댓값을 받을 double형 변수 주소, 
	// 최솟값 위치 좌표 받을 Point형 변수 주소, 최댓값 위치 좌표 받을 Point형 변수 주소, 마스크 영상<생략 가능>)
	// tmpl_img의 최솟값/최댓값을 각 변수 주소에, 최솟값/최댓값 위치 좌표를 각 Point형 주소에 마스크 영상의 픽셀값이 0이 아닌 위치에서 연산 수행
	// Val를 이용하면 유사도를 알 수 있다.

	double max_iVal;
	cv::minMaxLoc(tmpl_impurity_img, 0, &max_iVal, 0, 0);

	cv::Mat color_img; // 컬러 이미지 담을 Mat
	cv::cvtColor(gr_img, color_img, cv::COLOR_GRAY2BGR); // 회색 조 이미지 컬러로 만듦

	if (max_bVal * 100 > 50) {
		std::cout << "Burr 불량" << std::endl;
	}

	else if (max_iVal * 100 > 70) {
		std::cout << "impurity 불량" << std::endl;
	}

	else {
		std::cout << "void 불량" << std::endl;
	}

	cv::imshow("result", color_img); // 이미지 출력 함수 | ("창 이름", 불러올 이미지)
	// 사출불량(Burr, 이바리, 가장자리 성형 불량) / 이물질(Impurity) 검출 | 탬플릿 매칭으로 시도 | 성공



	////// 사출불량(Burr, 이바리, 가장자리 성형 불량) | 특징 검출로 시도 | 유의미한 검출이 없어 실패
	//cv::Mat input_img; // 이미지 입력받을 Mat
	//Capture->read(input_img); // 캠 이미지를 읽어서 저장

	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	//cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 흑백화함
	////cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	//cv::Mat burr_img = cv::imread("./def/bur.bmp", cv::IMREAD_GRAYSCALE);

	//std::vector<cv::KeyPoint> grkp, brkp;
	//cv::Mat grcomp, brcomp;

	//cv::Ptr<cv::Feature2D> detector;
	//detector = cv::ORB::create();

	//detector->detect(gr_img, grkp);
	//detector->detect(burr_img, brkp);

	//cv::Mat draw_grkp, draw_brkp;

	//cv::KeyPointsFilter::retainBest(grkp, 25);
	//cv::KeyPointsFilter::retainBest(brkp, 25);

	//cv::drawKeypoints(gr_img, grkp, draw_grkp, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
	//cv::drawKeypoints(burr_img, brkp, draw_brkp, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
	//
	//detector->compute(gr_img, grkp, grcomp);
	//detector->compute(burr_img, brkp, brcomp);

	//std::vector<cv::DMatch> matches;
	//cv::BFMatcher matcher(cv::NORM_HAMMING);
	//matcher.match(grcomp, brcomp, matches);

	//if (matches.size() < 4)
	//	AfxGetMainWnd()->PostMessage(WM_CLOSE);

	//cv::Mat allmatches;
	//cv::drawMatches(gr_img, grkp, burr_img, brkp, matches, allmatches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//std::vector<cv::DMatch> goodmatches;
	//std::sort(matches.begin(), matches.end());
	//if (matches.size() > 60) {
	//	goodmatches.assign(matches.begin(), matches.begin() + 60);
	//}
	//else
	//	goodmatches.assign(matches.begin(), matches.end());
	//
	//if(goodmatches.size() < 4)
	//	AfxGetMainWnd()->PostMessage(WM_CLOSE);

	//cv::Mat imgmatch;
	//cv::drawMatches(gr_img, grkp, burr_img, brkp, goodmatches, imgmatch, cv::Scalar::all(-1), cv::Scalar::all(-1));

	//std::vector<cv::Point2f> obj;
	//std::vector<cv::Point2f> scene;
	//for (const auto& g : goodmatches) {
	//	obj.emplace_back(grkp[g.queryIdx].pt);
	//	scene.emplace_back(brkp[g.trainIdx].pt);
	//}

	//cv::Mat H = cv::findHomography(obj, scene, cv::RANSAC);

	//std::vector<cv::Point2f> objP(4);
	//objP[0] = cv::Point2f(0, 0);
	//objP[1] = cv::Point2f(gr_img.cols, 0);
	//objP[2] = cv::Point2f(gr_img.cols, gr_img.rows);
	//objP[3] = cv::Point2f(0, gr_img.rows);

	//std::vector<cv::Point2f> sceneP(4);
	//cv::perspectiveTransform(objP, sceneP, H);

	//for (int i = 0; i < 4; ++i) {
	//	sceneP[i] += cv::Point2f(gr_img.cols, 0);
	//}
	//for (int i = 0; i < 4; ++i) {
	//	cv::line(imgmatch, sceneP[i], sceneP[(i + 1) % 4], cv::Scalar(0, 0, 255), 4);
	//}

	//cv::imshow("result", imgmatch); // 이미지 출력 함수 | ("창 이름", 불러올 이미지)
	////// 사출불량(Burr, 이바리, 가장자리 성형 불량) | 특징 검출로 시도 | 유의미한 검출이 없어 실패



	//// 사출불량(Burr, 이바리, 가장자리 성형 불량) | 캐니 에지로 시도 | 사진 크기 비율이 달라서 실패
	//cv::Mat input_img; // 이미지 입력받을 Mat
	//Capture->read(input_img); // 캠 이미지를 읽어서 저장

	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	//cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 흑백화함
	////cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	//cv::Mat res_img;
	//cv::cvtColor(gr_img, res_img, cv::COLOR_GRAY2BGR);
	// 
	//cv::Mat canny_img;
	//cv::Canny(gr_img, canny_img, 5, 468);

	//std::vector<std::vector<cv::Point>> contours;

	//cv::findContours(canny_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	//for (auto& pts : contours) {
	//	if (cv::contourArea(pts) > 37768) {
	//		for (int i = 0; i < pts.size(); ++i) {
	//			cv::line(res_img, pts[i % pts.size()], pts[(i + 1) % pts.size()], cv::Scalar(255, 0, 0), 2);
	//		}
	//	}
	//}
	//cv::imshow("canny_img", canny_img);
	//cv::imshow("result", res_img); // 이미지 출력 함수 | ("창 이름", 불러올 이미지)
	//// 사출불량(Burr, 이바리, 가장자리 성형 불량) | 캐니 에지로 시도 | 사진 크기 비율이 달라서 실패



	//// 사출불량(Burr, 이바리, 가장자리 성형 불량) | 면적 측정 | 적응형 이진화로 시도 | 초점 오류로 실패
	//cv::Mat input_img; // 이미지 입력받을 Mat
	//Capture->read(input_img); // 캠 이미지를 읽어서 저장

	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	//cv::cvtColor(input_img, gr_img, cv::COLOR_BGR2GRAY); // 이미지 흑백화함
	////cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	//cv::Mat res_img;
	//cv::cvtColor(gr_img, res_img, cv::COLOR_GRAY2BGR);

	//cv::Mat bin_img;
	//cv::adaptiveThreshold(gr_img, bin_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37, 5);

	//std::vector<std::vector<cv::Point>> contours;
	//cv::findContours(bin_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	//for (auto& pts : contours) {

	//	for (int i = 0; i < pts.size(); ++i) {
	//		cv::line(res_img, pts[i % pts.size()], pts[(i + 1) % pts.size()], cv::Scalar(255, 0, 0), 3);
	//	}
	// 
	//}
	//cv::imshow("result", res_img); // 이미지 출력 함수 | ("창 이름", 불러올 이미지)
	//// 사출불량(Burr, 이바리, 가장자리 성형 불량) | 면적 측정 | 적응형 이진화로 시도 | 초점 오류로 실패
}	