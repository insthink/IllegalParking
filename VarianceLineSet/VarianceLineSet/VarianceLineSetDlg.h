
// VarianceLineSetDlg.h : 头文件
//

#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#define areaNum 2
// CVarianceLineSetDlg 对话框
class CVarianceLineSetDlg : public CDialogEx
{
// 构造
public:
	CVarianceLineSetDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VARIANCELINESET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
