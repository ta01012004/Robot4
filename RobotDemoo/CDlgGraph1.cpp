// CDlgGraph1.cpp : implementation file
//

#include "pch.h"
#include "RobotDemoo.h"
#include "afxdialogex.h"
#include "CDlgGraph1.h"


// CDlgGraph1 dialog

IMPLEMENT_DYNAMIC(CDlgGraph1, CDialogEx)

CDlgGraph1::CDlgGraph1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDlgGraph1::~CDlgGraph1()
{
}

void CDlgGraph1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_staticGraph);
}


BEGIN_MESSAGE_MAP(CDlgGraph1, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgGraph1 message handlers

void CDlgGraph1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	
	double xmin = 0, xmax = 1, ymin = 0, ymax = 1;
	if (m_pl1.GetSize() > 0)
	{
	
		xmin = xmax = m_pl1[0].x;
		ymin = ymax = m_pl1[0].y;
	}

	for (int i = 0; i < m_pl1.GetSize(); i++)
	{
		xmin = min(xmin, min(m_pl1[i].x, min(m_pl2[i].x, m_pl3[i].x)));
		ymin = min(ymin, min(m_pl1[i].y, min(m_pl2[i].y, m_pl3[i].y)));

		xmax = max(xmax, max(m_pl1[i].x, max(m_pl2[i].x, m_pl3[i].x)));
		ymax = max(ymax, max(m_pl1[i].y, max(m_pl2[i].y, m_pl3[i].y)));

	}
	m_graph.SetRangeX(xmin, xmax);
	m_graph.SetRangeY(ymin, ymax);


	m_graph.SetRectControl(m_staticGraph);
	m_graph.DrawBackGround(RGB(255, 255, 255)); //đặt mầu nền
	m_graph.DrawTable(); //vẽ bảng lưới của đồ thị
	
	m_graph.SetColorGraph(RGB(255, 0, 0));
	m_graph.DrawPline(m_pl1);

	m_graph.SetColorGraph(RGB(0, 255, 0));
	m_graph.DrawPline(m_pl2);

	m_graph.SetColorGraph(RGB(0, 0, 255));
	m_graph.DrawPline(m_pl3);

	//viet chu ghi chu
	if (m_pl1.GetSize() > 0)
	{
		m_graph.DrawTextWithColor(m_pl1[0], m_sl1, RGB(255, 0, 0), TA_LEFT | TA_BOTTOM);
		m_graph.DrawTextWithColor(m_pl2[0], m_sl2, RGB(0, 255, 0), TA_LEFT | TA_BOTTOM);
		m_graph.DrawTextWithColor(m_pl3[0], m_sl3, RGB(0, 0, 255), TA_LEFT | TA_BOTTOM);
	}
}

BOOL CDlgGraph1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetWindowText(m_sDlgCaption);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
