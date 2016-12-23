
// VarianceLineSetDlg.h : ͷ�ļ�
//

#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#define areaNum 2
// CVarianceLineSetDlg �Ի���
class CVarianceLineSetDlg : public CDialogEx
{
// ����
public:
	CVarianceLineSetDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VARIANCELINESET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedvarianceset();
	afx_msg void OnBnClickedreflect();
	afx_msg void OnBnClickedopenpic();
	afx_msg void OnBnClickedsaveit();

private:
	CString _openImage;
	CString _writeIn;
	cv::Mat _inImg;
	int _roadWidth;
	
};
