#pragma once

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define MAX_POINT 100

class CRobot
{
public:
    int    m_selectedProblem;   // 0: forward, 1: inverse

    // Thông số hình học (mm)
    double a2, a3;
    double theta1, d2, theta3;  // 1 pose

    // Biến khớp theo thời gian
    double q1[MAX_POINT], q2[MAX_POINT], q3[MAX_POINT];

    // Vị trí điểm cuối
    double pEx[MAX_POINT], pEy[MAX_POINT], pEz[MAX_POINT];

    // Vận tốc điểm cuối
    double pExv[MAX_POINT], pEyv[MAX_POINT], pEzv[MAX_POINT];

    // Gia tốc điểm cuối
    double pExa[MAX_POINT], pEya[MAX_POINT], pEza[MAX_POINT];

    // Vận tốc khớp
    double q1v[MAX_POINT], q2v[MAX_POINT], q3v[MAX_POINT];

    // Gia tốc khớp
    double q1a[MAX_POINT], q2a[MAX_POINT], q3a[MAX_POINT];

    // Index hiện tại trên quỹ đạo (cho OpenGL)
    int idCurrentPoint;

    CRobot();

    void init();

    // Bài toán thuận
    void SetVariables();              // Tạo quỹ đạo khớp q1,q2,q3
    void SolvingForwardKinematics();  // Từ q -> p

    // Bài toán nghịch (bắt đầu từ quỹ đạo điểm cuối)
    void InitTrajectory();            // Tạo quỹ đạo p, v, a điểm cuối
    void SolvingInverseKinematics();  // Từ p -> q
    void SolvingVelocity();           // Từ vE -> q̇
    void SolvingAcceleration();       // Từ aE -> q̈
};

// Con trỏ global để FK/IK dùng (Newton_Raphson & Jacobian)
extern CRobot* pRobot;
