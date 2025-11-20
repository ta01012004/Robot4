// RobotDemooView.cpp : implementation of the CRobotDemooView class
//

#include "pch.h"
#include "framework.h"

#ifndef SHARED_HANDLERS
#include "RobotDemoo.h"
#endif

#include "RobotDemooDoc.h"
#include "RobotDemooView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CDlgGraph1.h"

// CRobotDemooView

IMPLEMENT_DYNCREATE(CRobotDemooView, CView)

BEGIN_MESSAGE_MAP(CRobotDemooView, CView)
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

    // Demo controls
    ON_COMMAND(ID_DEMO_PAUSE, &CRobotDemooView::OnDemoPause)
    ON_UPDATE_COMMAND_UI(ID_DEMO_PAUSE, &CRobotDemooView::OnUpdateDemoPause)
    ON_COMMAND(ID_DEMO_RUN, &CRobotDemooView::OnDemoRun)
    ON_UPDATE_COMMAND_UI(ID_DEMO_RUN, &CRobotDemooView::OnUpdateDemoRun)
    ON_COMMAND(ID_DEMO_STOP, &CRobotDemooView::OnDemoStop)
    ON_UPDATE_COMMAND_UI(ID_DEMO_STOP, &CRobotDemooView::OnUpdateDemoStop)

    // Kinematics graphs
    ON_COMMAND(ID_FORWARDKINEMATICS_ENDPOINT, &CRobotDemooView::OnForwardkinematicsEndpoint)
    ON_UPDATE_COMMAND_UI(ID_FORWARDKINEMATICS_ENDPOINT, &CRobotDemooView::OnUpdateForwardkinematicsEndpoint)
    ON_COMMAND(ID_FORWARDKINEMATICS_VARIABLE, &CRobotDemooView::OnForwardkinematicsVariable)
    ON_UPDATE_COMMAND_UI(ID_FORWARDKINEMATICS_VARIABLE, &CRobotDemooView::OnUpdateForwardkinematicsVariable)

    ON_COMMAND(ID_INVERSEKINEMATICS_ACCEOFEP, &CRobotDemooView::OnInversekinematicsAcceofep)
    ON_UPDATE_COMMAND_UI(ID_INVERSEKINEMATICS_ACCEOFEP, &CRobotDemooView::OnUpdateInversekinematicsAcceofep)
    ON_COMMAND(ID_INVERSEKINEMATICS_ACCEOFJV, &CRobotDemooView::OnInversekinematicsAcceofjv)
    ON_UPDATE_COMMAND_UI(ID_INVERSEKINEMATICS_ACCEOFJV, &CRobotDemooView::OnUpdateInversekinematicsAcceofjv)

    ON_COMMAND(ID_INVERSEKINEMATICS_ENDPOINTS, &CRobotDemooView::OnInversekinematicsEndpoints)
    ON_UPDATE_COMMAND_UI(ID_INVERSEKINEMATICS_ENDPOINTS, &CRobotDemooView::OnUpdateInversekinematicsEndpoints)
    ON_COMMAND(ID_INVERSEKINEMATICS_JOINTVAR, &CRobotDemooView::OnInversekinematicsJointvar)
    ON_UPDATE_COMMAND_UI(ID_INVERSEKINEMATICS_JOINTVAR, &CRobotDemooView::OnUpdateInversekinematicsJointvar)

    ON_COMMAND(ID_INVERSEKINEMATICS_VARIABLES, &CRobotDemooView::OnInversekinematicsVeloofep)
    ON_UPDATE_COMMAND_UI(ID_INVERSEKINEMATICS_VARIABLES, &CRobotDemooView::OnUpdateInversekinematicsVeloofep)
    ON_COMMAND(ID_INVERSEKINEMATICS_VELOOFJV, &CRobotDemooView::OnInversekinematicsVeloofjv)
    ON_UPDATE_COMMAND_UI(ID_INVERSEKINEMATICS_VELOOFJV, &CRobotDemooView::OnUpdateInversekinematicsVeloofjv)

    // Select forward / inverse problem
    ON_COMMAND(ID_SELECTPROBLEM_FORWARDKINEMATICS, &CRobotDemooView::OnSolvingForwardkinematics)
    ON_UPDATE_COMMAND_UI(ID_SELECTPROBLEM_FORWARDKINEMATICS, &CRobotDemooView::OnUpdateSelectproblemForwardkinematics)
    ON_COMMAND(ID_SELECTPROBLEM_INVERSEKINEMATICS, &CRobotDemooView::OnSolvingInversekinematics)
    ON_UPDATE_COMMAND_UI(ID_SELECTPROBLEM_INVERSEKINEMATICS, &CRobotDemooView::OnUpdateSelectproblemInversekinematics)

    ON_COMMAND(ID_SELECTEDPROBLEM_FORWARDKINEMATICS, &CRobotDemooView::OnSelectedproblemForwardkinematics)
    ON_UPDATE_COMMAND_UI(ID_SELECTEDPROBLEM_FORWARDKINEMATICS, &CRobotDemooView::OnUpdateSelectedproblemForwardkinematics)
    ON_COMMAND(ID_SELECTEDPROBLEM_INVERSEKINEMATICS, &CRobotDemooView::OnSelectedproblemInversekinematics)
    ON_UPDATE_COMMAND_UI(ID_SELECTEDPROBLEM_INVERSEKINEMATICS, &CRobotDemooView::OnUpdateSelectedproblemInversekinematics)

    // OpenGL child + timer
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_TIMER()
END_MESSAGE_MAP()

// CRobotDemooView construction/destruction

CRobotDemooView::CRobotDemooView() noexcept
{
    // Demo state: 0 = stop, 1 = run, 2 = pause
    m_DemoState = 0;
    // Direction on trajectory: +1 forward, -1 backward
    m_direction = 1;
}

CRobotDemooView::~CRobotDemooView()
{
}

BOOL CRobotDemooView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CView::PreCreateWindow(cs);
}

// drawing

void CRobotDemooView::OnDraw(CDC* pDC)
{
    CRobotDemooDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // nothing to draw in the view; all graphics are in CMyOpenGLCtrl
}

// printing

BOOL CRobotDemooView::OnPreparePrinting(CPrintInfo* pInfo)
{
    return DoPreparePrinting(pInfo);
}

void CRobotDemooView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CRobotDemooView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

// diagnostics

#ifdef _DEBUG
void CRobotDemooView::AssertValid() const
{
    CView::AssertValid();
}

void CRobotDemooView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CRobotDemooDoc* CRobotDemooView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRobotDemooDoc)));
    return (CRobotDemooDoc*)m_pDocument;
}
#endif

// ====================== DEMO RUN / PAUSE / STOP ======================

void CRobotDemooView::OnDemoPause()
{
    m_DemoState = 2;
    KillTimer(10000);
}

void CRobotDemooView::OnUpdateDemoPause(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_DemoState == 2);
}

void CRobotDemooView::OnDemoRun()
{
    m_DemoState = 1;
    SetTimer(10000, 100, nullptr);   // 100 ms per frame
}

void CRobotDemooView::OnUpdateDemoRun(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_DemoState == 1);
}

void CRobotDemooView::OnDemoStop()
{
    m_DemoState = 0;
    m_direction = 1;
    m_robot.idCurrentPoint = 0;

    KillTimer(10000);
    m_openGL.Invalidate(FALSE);
}

void CRobotDemooView::OnUpdateDemoStop(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_DemoState == 0);
}

// ====================== FORWARD KINEMATICS GRAPHS ====================

void CRobotDemooView::OnForwardkinematicsEndpoint()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Forward kinematics: Endpoint";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, m_robot.pEx[i]));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.pEy[i]));
        dlg.m_pl3.Add(CPoint2d(i, m_robot.pEz[i]));
    }
    dlg.m_sl1 = "Ex (mm)";
    dlg.m_sl2 = "Ey (mm)";
    dlg.m_sl3 = "Ez (mm)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateForwardkinematicsEndpoint(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 0);
}

void CRobotDemooView::OnForwardkinematicsVariable()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Forward kinematics: Joint variables";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, Rad2Deg(m_robot.q1[i])));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.q2[i]));
        dlg.m_pl3.Add(CPoint2d(i, Rad2Deg(m_robot.q3[i])));
    }
    dlg.m_sl1 = "q1 = theta1 (deg)";
    dlg.m_sl2 = "q2 = d2 (mm)";
    dlg.m_sl3 = "q3 = theta3 (deg)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateForwardkinematicsVariable(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 0);
}

// ====================== INVERSE KINEMATICS GRAPHS ====================

void CRobotDemooView::OnInversekinematicsAcceofep()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Inverse kinematics: Acceleration of endpoint";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, m_robot.pExa[i]));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.pEya[i]));
        dlg.m_pl3.Add(CPoint2d(i, m_robot.pEza[i]));
    }
    dlg.m_sl1 = "aEx (mm/s^2)";
    dlg.m_sl2 = "aEy (mm/s^2)";
    dlg.m_sl3 = "aEz (mm/s^2)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateInversekinematicsAcceofep(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 1);
}

void CRobotDemooView::OnInversekinematicsAcceofjv()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Inverse kinematics: Acceleration of joint variables";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, m_robot.q1a[i]));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.q2a[i]));
        dlg.m_pl3.Add(CPoint2d(i, m_robot.q3a[i]));
    }
    dlg.m_sl1 = "a theta1 (deg/s^2)";
    dlg.m_sl2 = "a d2 (mm/s^2)";
    dlg.m_sl3 = "a theta3 (deg/s^2)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateInversekinematicsAcceofjv(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 1);
}

void CRobotDemooView::OnInversekinematicsEndpoints()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Inverse kinematics: Endpoint";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, m_robot.pEx[i]));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.pEy[i]));
        dlg.m_pl3.Add(CPoint2d(i, m_robot.pEz[i]));
    }
    dlg.m_sl1 = "Ex (mm)";
    dlg.m_sl2 = "Ey (mm)";
    dlg.m_sl3 = "Ez (mm)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateInversekinematicsEndpoints(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 1);
}

void CRobotDemooView::OnInversekinematicsJointvar()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Inverse kinematics: Joint variables";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, Rad2Deg(m_robot.q1[i])));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.q2[i]));
        dlg.m_pl3.Add(CPoint2d(i, Rad2Deg(m_robot.q3[i])));
    }
    dlg.m_sl1 = "q1 = theta1 (deg)";
    dlg.m_sl2 = "q2 = d2 (mm)";
    dlg.m_sl3 = "q3 = theta3 (deg)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateInversekinematicsJointvar(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 1);
}

void CRobotDemooView::OnInversekinematicsVeloofep()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Inverse kinematics: Velocity of endpoint";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, m_robot.pExv[i]));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.pEyv[i]));
        dlg.m_pl3.Add(CPoint2d(i, m_robot.pEzv[i]));
    }
    dlg.m_sl1 = "vEx (mm/s)";
    dlg.m_sl2 = "vEy (mm/s)";
    dlg.m_sl3 = "vEz (mm/s)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateInversekinematicsVeloofep(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 1);
}

void CRobotDemooView::OnInversekinematicsVeloofjv()
{
    CDlgGraph1 dlg;
    dlg.m_sDlgCaption = "Inverse kinematics: Velocity of joint variables";

    for (int i = 0; i < MAX_POINT; ++i)
    {
        dlg.m_pl1.Add(CPoint2d(i, m_robot.q1v[i]));
        dlg.m_pl2.Add(CPoint2d(i, m_robot.q2v[i]));
        dlg.m_pl3.Add(CPoint2d(i, m_robot.q3v[i]));
    }
    dlg.m_sl1 = "v theta1 (deg/s)";
    dlg.m_sl2 = "v d2 (mm/s)";
    dlg.m_sl3 = "v theta3 (deg/s)";

    dlg.DoModal();
}

void CRobotDemooView::OnUpdateInversekinematicsVeloofjv(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_robot.m_selectedProblem == 1);
}

// ====================== SELECT PROBLEM ======================

void CRobotDemooView::OnSolvingForwardkinematics()
{
    // Optional: call the "selected forward" handler
    OnSelectedproblemForwardkinematics();
}

void CRobotDemooView::OnUpdateSelectproblemForwardkinematics(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_robot.m_selectedProblem == 0);
}

void CRobotDemooView::OnSolvingInversekinematics()
{
    OnSelectedproblemInversekinematics();
}

void CRobotDemooView::OnUpdateSelectproblemInversekinematics(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_robot.m_selectedProblem == 1);
}

void CRobotDemooView::OnSelectedproblemInversekinematics()
{
    m_DemoState = 0;
    KillTimer(10000);
    m_direction = 1;
    m_robot.idCurrentPoint = 0;

    m_robot.m_selectedProblem = 1;
    m_robot.init();
    m_robot.InitTrajectory();
    m_robot.SolvingInverseKinematics();
    m_robot.SolvingVelocity();
    m_robot.SolvingAcceleration();

    m_openGL.DrawTrajectory();
    m_openGL.DrawWorkSpace();
    Invalidate(FALSE);
}

void CRobotDemooView::OnUpdateSelectedproblemInversekinematics(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_robot.m_selectedProblem == 1);
}

void CRobotDemooView::OnSelectedproblemForwardkinematics()
{
    m_DemoState = 0;
    KillTimer(10000);
    m_direction = 1;
    m_robot.idCurrentPoint = 0;

    m_robot.m_selectedProblem = 0;
    m_robot.init();
    m_robot.SetVariables();
    m_robot.SolvingForwardKinematics();

    m_openGL.DrawTrajectory();
    // workspace optional here
    // m_openGL.DrawWorkSpace();
    Invalidate(FALSE);
}

void CRobotDemooView::OnUpdateSelectedproblemForwardkinematics(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_robot.m_selectedProblem == 0);
}

// ====================== OPENGL CHILD + TIMER ======================

int CRobotDemooView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rect;
    GetClientRect(rect);

    // create OpenGL control covering the whole view
    m_openGL.Create(rect, this, 100);
    m_openGL.SetScaleWithClient();
    m_openGL.SetMouseStyleDone(CMouseEvent::MOUSE_ROTATE);

    // connect robot to OpenGL and load models
    m_openGL.m_pRobot = &m_robot;
    m_openGL.LoadRobotModel();

    // initial data (forward kinematics) is already computed in CRobot ctor,
    // so we can draw trajectory and workspace now
    m_openGL.DrawTrajectory();
    m_openGL.DrawWorkSpace();

    return 0;
}

void CRobotDemooView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if (::IsWindow(m_openGL.m_hWnd))
    {
        CRect rect;
        GetClientRect(rect);
        m_openGL.MoveWindow(rect);
    }
}

void CRobotDemooView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 10000)
    {
        m_robot.idCurrentPoint += m_direction;

        if (m_robot.idCurrentPoint >= MAX_POINT)
        {
            m_robot.idCurrentPoint = MAX_POINT - 1;
            m_direction = -1;
        }

        if (m_robot.idCurrentPoint < 0)
        {
            m_robot.idCurrentPoint = 0;
            m_direction = 1;
        }

        m_openGL.Invalidate(FALSE);
    }

    CView::OnTimer(nIDEvent);
}
