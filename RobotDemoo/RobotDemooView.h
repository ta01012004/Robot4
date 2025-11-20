
// RobotDemooView.h : interface of the CRobotDemooView class
//

#pragma once

#include "CMyOpenGLCtrl.h"
class CRobotDemooView : public CView
{
protected: // create from serialization only
	CRobotDemooView() noexcept;
	DECLARE_DYNCREATE(CRobotDemooView)

// Attributes
public:
	CRobotDemooDoc* GetDocument() const;

// Operations
public:
	CRobot m_robot;

	CMyOpenGLCtrl m_openGL;

	int m_DemoState;
	int m_direction;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRobotDemooView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDemoPause();
	afx_msg void OnUpdateDemoPause(CCmdUI* pCmdUI);
	afx_msg void OnDemoRun();
	afx_msg void OnUpdateDemoRun(CCmdUI* pCmdUI);
	afx_msg void OnDemoStop();
	afx_msg void OnUpdateDemoStop(CCmdUI* pCmdUI);
	afx_msg void OnForwardkinematicsEndpoint();
	afx_msg void OnUpdateForwardkinematicsEndpoint(CCmdUI* pCmdUI);
	afx_msg void OnForwardkinematicsVariable();
	afx_msg void OnUpdateForwardkinematicsVariable(CCmdUI* pCmdUI);
	afx_msg void OnInversekinematicsAcceofep();
	afx_msg void OnUpdateInversekinematicsAcceofep(CCmdUI* pCmdUI);
	afx_msg void OnInversekinematicsAcceofjv();
	afx_msg void OnUpdateInversekinematicsAcceofjv(CCmdUI* pCmdUI);
	afx_msg void OnInversekinematicsEndpoints();
	afx_msg void OnUpdateInversekinematicsEndpoints(CCmdUI* pCmdUI);
	afx_msg void OnInversekinematicsJointvar();
	afx_msg void OnUpdateInversekinematicsJointvar(CCmdUI* pCmdUI);
	afx_msg void OnInversekinematicsVeloofep();
	afx_msg void OnUpdateInversekinematicsVeloofep(CCmdUI* pCmdUI);
	afx_msg void OnInversekinematicsVeloofjv();
	afx_msg void OnUpdateInversekinematicsVeloofjv(CCmdUI* pCmdUI);
	afx_msg void OnSolvingForwardkinematics();
	afx_msg void OnUpdateSelectproblemForwardkinematics(CCmdUI* pCmdUI);
	afx_msg void OnSolvingInversekinematics();
	afx_msg void OnUpdateSelectproblemInversekinematics(CCmdUI* pCmdUI);
	afx_msg void OnSelectedproblemInversekinematics();
	afx_msg void OnUpdateSelectedproblemInversekinematics(CCmdUI* pCmdUI);
	afx_msg void OnSelectedproblemForwardkinematics();
	afx_msg void OnUpdateSelectedproblemForwardkinematics(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in RobotDemooView.cpp
inline CRobotDemooDoc* CRobotDemooView::GetDocument() const
   { return reinterpret_cast<CRobotDemooDoc*>(m_pDocument); }
#endif

