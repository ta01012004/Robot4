#pragma once
#include "afxdialogex.h"


// CDlgGraph1 dialog

class CDlgGraph1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGraph1)

public:
	CDlgGraph1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgGraph1();

	CString m_sDlgCaption;
	CGraph m_graph;

	CPline m_pl1, m_pl2, m_pl3; // cac duong pline luu tru du lieu duong cong
	CString m_sl1, m_sl2, m_sl3; // chuoi tieu de cua duong cong
		
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CStatic m_staticGraph;
	virtual BOOL OnInitDialog();
};
