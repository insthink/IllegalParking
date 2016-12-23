
// VarianceLineSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VarianceLineSet.h"
#include "VarianceLineSetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
// ����ԭ��
void varianceSetCall(int event, int x, int y, int flags, void *param);

// ȫ�ֱ���
cv::Point areaPt[areaNum * 2];
int ptCnt = 0;
cv::Mat varImg;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CVarianceLineSetDlg �Ի���




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


// CVarianceLineSetDlg ��Ϣ�������

BOOL CVarianceLineSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	AllocConsole();  
	FILE *stream;  
	freopen_s( &stream , "CONOUT$", "w+t" , stdout );  
	freopen_s( &stream ,"CONIN$" , "r+t" , stdin );


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVarianceLineSetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVarianceLineSetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVarianceLineSetDlg::OnBnClickedvarianceset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CVarianceLineSetDlg::OnBnClickedopenpic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal()==IDOK)
		_openImage = dlg.GetPathName();

	string openPath = _openImage.GetBuffer();
	_openImage.ReleaseBuffer();
	_inImg = cv::imread(openPath);
	if(_inImg.data) AfxMessageBox("OK,Loaded!", MB_ICONINFORMATION);

}


void CVarianceLineSetDlg::OnBnClickedsaveit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// д��������ļ�
	_writeIn = "E:\\wuxi.ini";

	CString tempStr;
	char *name[areaNum*4] = {	// �������*2
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
	cv::destroyAllWindows(); // ����������˳����ж�

}