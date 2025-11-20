#include "pch.h"
#include "CMyOpenGLCtrl.h"
#include "Transformations.h"
#include "CRobot.h"
#include "STLLoader.h"
#include <afx.h>
#include <cmath>        // std::cos, std::sin

// ============================================================
// HÀM VẼ CHÍNH TRONG OPENGL
// ============================================================
void CMyOpenGLCtrl::DrawInOpenGL()
{
    // Kiểm tra robot
    if (!m_pRobot)
    {
        TRACE(_T("DrawInOpenGL: m_pRobot = NULL\n"));
        return;
    }

    // ---------------- Trục tọa độ gốc ----------------
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // X - đỏ
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(200.0f, 0.0f, 0.0f);

    // Y - xanh lá
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 200.0f, 0.0f);

    // Z - xanh dương
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 200.0f);
    glEnd();

    // ---------------- Thiết lập OpenGL ----------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    m_glLight.SetUpLight();

    // ---------------- Vẽ quỹ đạo + workspace ----------------
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glScaled(0.01, 0.01, 0.01);   // thu nhỏ quỹ đạo và workspace
    m_objTrajectory.Draw();
    m_objWorkSpace.Draw();
    glPopMatrix();
    glEnable(GL_LIGHTING);

    // =======================================================
    // LẤY GIÁ TRỊ KHỚP RRP TỪ CRobot (RRP: theta1, d2, theta3)
    // =======================================================
    const int idx = m_pRobot->idCurrentPoint;

    const double theta1 = m_pRobot->q1[idx];   // [rad]
    const double d2 = m_pRobot->q2[idx];   // [mm]
    const double theta3 = m_pRobot->q3[idx];   // [rad]

    const double a2 = m_pRobot->a2;            // mm
    const double a3 = m_pRobot->a3;            // mm

    const double theta1_deg =
        theta1 * 180.0 / 3.14159265358979323846;
    const double theta3_deg =
        theta3 * 180.0 / 3.14159265358979323846;

    // =======================================================
    // VẼ ROBOT (scale mm -> đơn vị vẽ)
    // =======================================================
    glPushMatrix();
    {
        glScaled(0.01, 0.01, 0.01);

        // -------- LINK 0 (BASE) ----------
        glPushMatrix();
        m_objLink0.m_glMaterial.SetUpMaterial();
        m_objLink0.Draw();
        glPopMatrix();

        // -------- KHỚP 1: REVOLUTE quanh Z (theta1) ----------
        glRotated(theta1_deg, 0.0, 0.0, 1.0);

        // -------- RAIL: Cột đứng dọc Z sau khớp 1 ----------
        glPushMatrix();
        m_objRail.m_glMaterial.SetUpMaterial();
        m_objRail.Draw();
        glPopMatrix();

        // -------- KHỚP 2: PRISMATIC theo Z (d2) ----------
        glTranslated(0.0, 0.0, d2);

        // CARRIAGE trượt trên RAIL
        glPushMatrix();
        m_objCarriage.m_glMaterial.SetUpMaterial();
        m_objCarriage.Draw();
        glPopMatrix();

        // -------- LINK1: BEAM dọc +X (chiều dài ~ a2) --------
        glPushMatrix();
        m_objLink1.m_glMaterial.SetUpMaterial();
        m_objLink1.Draw();
        glPopMatrix();

        // Đến gốc khớp 3: dịch a2 theo +X
        glTranslated(a2, 0.0, 0.0);

        // -------- KHỚP 3: REVOLUTE quanh Y (theta3) ----------
        glRotated(theta3_deg, 0.0, 1.0, 0.0);

        // -------- LINK3: BEAM cuối dọc +X (chiều dài ~ a3) ---
        glPushMatrix();
        m_objLink3.m_glMaterial.SetUpMaterial();
        m_objLink3.Draw();
        glPopMatrix();

        // Đến gốc End-effector: dịch a3 theo +X
        glTranslated(a3, 0.0, 0.0);

        // -------- END-EFFECTOR ----------
        glPushMatrix();
        m_objEndEffector.m_glMaterial.SetUpMaterial();
        m_objEndEffector.Draw();
        glPopMatrix();

        // -------- Điểm vàng đánh dấu E ----------
        // CRobot dùng pE3 = [0,0,-10,1]^T trong frame cuối,
        // nên ở đây vẽ điểm tại (0,0,-10) trong hệ hiện tại.
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 0.0f);
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        glVertex3f(0.0f, 0.0f, -10.0f);
        glEnd();
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();
}

// ============================================================
// LOAD MODEL STL (CSTLLoader)
// ============================================================
void CMyOpenGLCtrl::LoadModel(const CString& sfilename,
    STRUCT_OBJECTGLLIST* pObjectDraw)
{
    CStringA filenameA(sfilename);
    const char* filename = filenameA.GetString();

    CSTLLoader stlLoader;

    if (!stlLoader.LoadSTL(filename))
    {
        TRACE(_T("Cannot load STL file: %s\n"), sfilename);
        return;
    }

    float minX, minY, minZ, maxX, maxY, maxZ;
    stlLoader.GetBounds(minX, minY, minZ, maxX, maxY, maxZ);

    TRACE(_T("Loaded STL: %s\n"), sfilename);
    TRACE(_T("  Size: X[%.1f, %.1f] Y[%.1f, %.1f] Z[%.1f, %.1f]\n"),
        minX, maxX, minY, maxY, minZ, maxZ);
    TRACE(_T("  Triangles: %d\n"), (int)stlLoader.triangles.size());

    MakeCurrent();
    pObjectDraw->BeginNewList();
    glPushMatrix();
    stlLoader.Draw();
    glPopMatrix();
    pObjectDraw->EndNewList();
    MakeNotCurrent();
}

// ============================================================
// LOAD 6 FILE STL ROBOT
// ============================================================
void CMyOpenGLCtrl::LoadRobotModel()
{
    TRACE(_T("\n========================================\n"));
    TRACE(_T("Begin loading STL models\n"));
    TRACE(_T("========================================\n"));

    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, MAX_PATH);
    CString exePath(szPath);
    int pos = exePath.ReverseFind('\\');
    CString exeDir = exePath.Left(pos + 1);

    CString fileBase = exeDir + _T("base_joint1.stl");
    CString fileLink1 = exeDir + _T("link1_a2_beam.stl");
    CString fileRail = exeDir + _T("rail_fixed_column.stl");
    CString fileCarriage = exeDir + _T("carriage_slider.stl");
    CString fileLink3 = exeDir + _T("link3_a3_beam.stl");
    CString fileEndEffector = exeDir + _T("end_effector_stub.stl");

    TRACE(_T("Exe folder: %s\n"), exeDir);

    TRACE(_T("\n1) BASE\n"));
    LoadModel(fileBase, &m_objLink0);

    TRACE(_T("\n2) LINK1 BEAM\n"));
    LoadModel(fileLink1, &m_objLink1);

    TRACE(_T("\n3) RAIL\n"));
    LoadModel(fileRail, &m_objRail);

    TRACE(_T("\n4) CARRIAGE\n"));
    LoadModel(fileCarriage, &m_objCarriage);

    TRACE(_T("\n5) LINK3\n"));
    LoadModel(fileLink3, &m_objLink3);

    TRACE(_T("\n6) END-EFFECTOR\n"));
    LoadModel(fileEndEffector, &m_objEndEffector);

    // Màu vật liệu
    m_objLink0.m_glMaterial.SetAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    m_objLink1.m_glMaterial.SetAmbient(0.9f, 0.4f, 0.1f, 1.0f);
    m_objRail.m_glMaterial.SetAmbient(0.2f, 0.2f, 0.8f, 1.0f);
    m_objCarriage.m_glMaterial.SetAmbient(0.2f, 0.9f, 0.2f, 1.0f);
    m_objLink3.m_glMaterial.SetAmbient(0.8f, 0.2f, 0.8f, 1.0f);
    m_objEndEffector.m_glMaterial.SetAmbient(1.0f, 0.8f, 0.0f, 1.0f);

    // Ánh sáng
    m_glLight.SetStyleDefault(1);
    m_glLight.Default();
    m_glLight.constant_atten = 1.8;
    m_glLight.quad_atten = 0.0;
    m_glLight.Position[0] = 1000.0;
    m_glLight.Position[1] = 1000.0;
    m_glLight.Position[2] = 1000.0;
    m_glLight.Position[3] = 1.0;

    TRACE(_T("\nFinished loading STL models\n"));
    TRACE(_T("========================================\n\n"));
}

// ============================================================
// VẼ QUỸ ĐẠO ĐIỂM CUỐI
// ============================================================
void CMyOpenGLCtrl::DrawTrajectory()
{
    if (!m_pRobot)
    {
        TRACE(_T("DrawTrajectory: m_pRobot = NULL\n"));
        return;
    }

    TRACE(_T("DrawTrajectory: start\n"));

    MakeCurrent();
    m_objTrajectory.BeginNewList();
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < MAX_POINT; ++i)
    {
        glVertex3d(m_pRobot->pEx[i],
            m_pRobot->pEy[i],
            m_pRobot->pEz[i]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
    m_objTrajectory.EndNewList();
    MakeNotCurrent();

    TRACE(_T("DrawTrajectory: done (%d points)\n"), MAX_POINT);
}

// ============================================================
// WORKSPACE – dùng đúng công thức FK analytic RRP
// ============================================================
void CMyOpenGLCtrl::DrawWorkSpace()
{
    if (!m_pRobot)
    {
        TRACE(_T("DrawWorkSpace: m_pRobot = NULL\n"));
        return;
    }

    TRACE(_T("DrawWorkSpace: start\n"));

    const double theta1min = 0.0;
    const double theta1max = 2.0 * PI;

    const double d2min = 10.0;
    const double d2max = 200.0;

    const double theta3min = -PI / 2.0;
    const double theta3max = PI / 2.0;

    const int numDiv = 15;

    const double a2 = m_pRobot->a2;
    const double a3 = m_pRobot->a3;

    MakeCurrent();
    m_objWorkSpace.BeginNewList();
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(0.5f, 0.5f, 1.0f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);

    for (int i = 0; i < numDiv; ++i)
    {
        double theta1 = theta1min +
            i * (theta1max - theta1min) / (numDiv - 1);

        for (int j = 0; j < numDiv; ++j)
        {
            double d2 = d2min +
                j * (d2max - d2min) / (numDiv - 1);

            for (int k = 0; k < numDiv; ++k)
            {
                double theta3 = theta3min +
                    k * (theta3max - theta3min) / (numDiv - 1);

                // FK analytic RRP (không offset -10):
                // x = (a2 + a3*cosθ3)*cosθ1
                // y = (a2 + a3*cosθ3)*sinθ1
                // z = d2 + a3*sinθ3
                double c1 = std::cos(theta1);
                double s1 = std::sin(theta1);
                double c3 = std::cos(theta3);
                double s3 = std::sin(theta3);

                double r = a2 + a3 * c3;
                double x = r * c1;
                double y = r * s1;
                double z = d2 + a3 * s3;

                glVertex3d(x, y, z);
            }
        }
    }

    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
    m_objWorkSpace.EndNewList();
    MakeNotCurrent();

    TRACE(_T("DrawWorkSpace: done\n"));
}
