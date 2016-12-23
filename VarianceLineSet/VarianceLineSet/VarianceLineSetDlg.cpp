
// VarianceLineSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VarianceLineSet.h"
#include "VarianceLineSetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
// 函数原型
void varianceSetCall(int event, int x, int y, int flags, void *param);

// 全局变量
cv::Point areaPt[areaNum * 2];
int ptCnt = 0;
cv::Mat varImg;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVarianceLineSetDlg 对话框




CVarianceLineSetDlg::CVarianceLineSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVarianceLineSetDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVarianceLineSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVarianceLineSetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(varianceSet, &CVarianceLineSetDlg::OnBnClickedvarianceset)
	ON_BN_CLICKED(reflect, &CVarianceLineSetDlg::OnBnClickedreflect)
	ON_BN_CLICKED(openPic, &CVarianceLineSetDlg::OnBnClickedopenpic)
	ON_BN_CLICKED(saveIt, &CVarianceLineSetDlg::OnBnClickedsaveit)
END_MESSAGE_MAP()


// CVarianceLineSetDlg 消息处理程序

BOOL CVarianceLineSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	AllocConsole();  
	FILE *stream;  
	freopen_s( &stream , "CONOUT$", "w+t" , stdout );  
	freopen_s( &stream ,"CONIN$" , "r+t" , stdin );


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVarianceLineSetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVarianceLineSetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVarianceLineSetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVarianceLineSetDlg::OnBnClickedvarianceset()
{
	// TODO: 在此添加控件通知处理程序代码
	_inImg.copyTo(varImg);
	if(!varImg.data) throw "img not loaded";
	cv::namedWindow("VarSet", 0);
	cv::imshow("VarSet", varImg);
	cv::setMouseCallback("VarSet", varianceSetCall, this);
}

void varianceSetCall(int event, int x, int y, int flags, void *param){
	CVarianceLineSetDlg *MyDlg = (CVarianceLineSetDlg *)param;
	switch(event){
	case CV_EVENT_LBUTTONDOWN:
		{
			areaPt[ptCnt].x = x; areaPt[ptCnt].y = y; 
			cout<<x<<endl;cout<<y<<endl;
			cv::circle(varImg, cv::Point(x, y), 1, cv::Scalar(255, 255, 0), 1);
			cv::imshow("VarSet", varImg);
			ptCnt++;
		}
		break;
	case CV_EVENT_RBUTTONDOWN:
		{
			areaPt[ptCnt].x = x; areaPt[ptCnt].y = y;
			cout<<x<<endl; cout<<y<<endl;
			cv::circle(varImg, cv::Point(areaPt[ptCnt].x, areaPt[ptCnt].y), 
						1, cv::Scalar(255, 255, 0), 1);
			cv::line(varImg, cv::Point(areaPt[ptCnt-1].x, areaPt[ptCnt-1].y),
				cv::Point(x, y), cv::Scalar(255, 255, 0), 1);
			cv::imshow("VarSet", varImg);
			ptCnt++;
			if(ptCnt == areaNum * 2){
				ptCnt = 0;
				AfxMessageBox("DONE!");
			}
		}
		break;
	}
}


void CVarianceLineSetDlg::OnBnClickedreflect()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CVarianceLineSetDlg::OnBnClickedopenpic()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal()==IDOK)
		_openImage = dlg.GetPathName();

	string openPath = _openImage.GetBuffer();
	_openImage.ReleaseBuffer();
	_inImg = cv::imread(openPath);
	if(_inImg.data) AfxMessageBox("OK,Loaded!", MB_ICONINFORMATION);

}


void CVarianceLineSetDlg::OnBnClickedsaveit()
{
	// TODO: 在此添加控件通知处理程序代码
	// 写入的配置文件
	_writeIn = "E:\\wuxi.ini";

	CString tempStr;
	char *name[areaNum*4] = {	// 点的数量*2
		"A0P1.X", "A0P1.Y",
		"A0P2.X", "A0P2.Y",
		"A1P1.X", "A1P1.Y",
		"A1P2.X", "A1P2.Y"
	};

	for(int i = 0; i < areaNum*2; i+=1){
		tempStr.Format("%d", areaPt[i].x);
		WritePrivateProfileString("VarianceLine",name[i*2],tempStr,_writeIn);
		tempStr.Format("%d", areaPt[i].y);
		WritePrivateProfileString("VarianceLine",name[i*2+1],tempStr,_writeIn);
	}

	AfxMessageBox("OK,Saved!", MB_ICONINFORMATION);
	cv::destroyAllWindows(); // 不清除窗口退出会中断

}