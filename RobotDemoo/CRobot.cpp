#include "pch.h"
#include "CRobot.h"
#include "Transformations.h"
#include <cmath>

CRobot* pRobot = NULL;

// =============================================================
// FK giải tích cho robot RRP theo DH (không dùng offset -10)
// DH:
// 0->1: theta = q1, alpha = 0,      a = 0,  d = 0
// 1->2: theta = 0,  alpha = pi/2,   a = a2, d = d2
// 2->3: theta = q3, alpha = 0,      a = a3, d = 0
//
// Kết quả:
// px = (a2 + a3*cos q3)*cos q1
// py = (a2 + a3*cos q3)*sin q1
// pz = d2 + a3*sin q3
// =============================================================
static void FK_RRP(double q1, double d2, double q3,
    double& px, double& py, double& pz)
{
    const double c1 = std::cos(q1);
    const double s1 = std::sin(q1);
    const double c3 = std::cos(q3);
    const double s3 = std::sin(q3);

    const double r = pRobot->a2 + pRobot->a3 * c3;

    px = r * c1;
    py = r * s1;
    pz = d2 + pRobot->a3 * s3;
}

// =============================================================
// Hàm f và Jacobian cho Newton–Raphson (bài toán nghịch)
// x = [theta1; d2; theta3]
// f(x) = FK(x) - p_des
// F    = df/dx = J
// =============================================================
void funcJacobi(smatrix& F, vectorm& f, const vectorm& x)
{
    if (!pRobot) return;

    // Lấy điểm cần tới từ quỹ đạo điểm cuối
    const int i = pRobot->idCurrentPoint;
    const double px_des = pRobot->pEx[i];
    const double py_des = pRobot->pEy[i];
    const double pz_des = pRobot->pEz[i];

    const double q1 = x(0);
    const double d2 = x(1);
    const double q3 = x(2);

    double px, py, pz;
    FK_RRP(q1, d2, q3, px, py, pz);

    // f = FK(x) - p_des
    f.SetSize(3);
    f(0) = px - px_des;
    f(1) = py - py_des;
    f(2) = pz - pz_des;

    // Jacobian giải tích
    const double c1 = std::cos(q1);
    const double s1 = std::sin(q1);
    const double c3 = std::cos(q3);
    const double s3 = std::sin(q3);

    const double r = pRobot->a2 + pRobot->a3 * c3;

    F.SetSize(3);

    // Hàng 1: ∂px/∂q
    F(0, 0) = -r * s1;             // ∂px/∂θ1
    F(0, 1) = 0.0;                 // ∂px/∂d2
    F(0, 2) = -pRobot->a3 * c1 * s3; // ∂px/∂θ3

    // Hàng 2: ∂py/∂q
    F(1, 0) = r * c1;             // ∂py/∂θ1
    F(1, 1) = 0.0;                 // ∂py/∂d2
    F(1, 2) = -pRobot->a3 * s1 * s3; // ∂py/∂θ3

    // Hàng 3: ∂pz/∂q
    F(2, 0) = 0.0;                 // ∂pz/∂θ1
    F(2, 1) = 1.0;                 // ∂pz/∂d2
    F(2, 2) = pRobot->a3 * c3;      // ∂pz/∂θ3
}

// =============================================================
// Constructor
// =============================================================
CRobot::CRobot()
{
    pRobot = this;                // rất quan trọng – cho IK dùng

    m_selectedProblem = 0;

    init();
    SetVariables();
    SolvingForwardKinematics();
}

// =============================================================
// Khởi tạo
// =============================================================
void CRobot::init()
{
    a2 = 300.0;
    a3 = 300.0;

    theta1 = 0.0;
    d2 = 0.0;
    theta3 = 0.0;

    idCurrentPoint = 0;

    for (int i = 0; i < MAX_POINT; ++i)
    {
        q1[i] = q2[i] = q3[i] = 0.0;
        pEx[i] = pEy[i] = pEz[i] = 0.0;
        pExv[i] = pEyv[i] = pEzv[i] = 0.0;
        pExa[i] = pEya[i] = pEza[i] = 0.0;
        q1v[i] = q2v[i] = q3v[i] = 0.0;
        q1a[i] = q2a[i] = q3a[i] = 0.0;
    }
}

// =============================================================
// Quỹ đạo KHỚP cho bài toán thuận
// q1: 0 → 90°, d2: 150 → 250mm, q3: -30° → +30°
// dùng polynomial bậc 5 (mượt)
// =============================================================
void CRobot::SetVariables()
{
    const double t1s = 0.0;
    const double t1e = PI / 2.0;
    const double d2s = 150.0;
    const double d2e = 250.0;
    const double t3s = -PI / 6.0;
    const double t3e = PI / 6.0;

    auto poly5 = [](double tau)->double
        {
            return 6 * tau * tau * tau * tau * tau
                - 15 * tau * tau * tau * tau
                + 10 * tau * tau * tau;
        };

    for (int i = 0; i < MAX_POINT; ++i)
    {
        const double tau = (MAX_POINT > 1) ? (double)i / (MAX_POINT - 1) : 0.0;
        const double s = poly5(tau);

        q1[i] = t1s + (t1e - t1s) * s;
        q2[i] = d2s + (d2e - d2s) * s;
        q3[i] = t3s + (t3e - t3s) * s;
    }
}

// =============================================================
// Bài toán thuận: từ q1,q2,q3 → pEx,pEy,pEz
// =============================================================
void CRobot::SolvingForwardKinematics()
{
    for (int i = 0; i < MAX_POINT; ++i)
    {
        double px, py, pz;
        FK_RRP(q1[i], q2[i], q3[i], px, py, pz);

        pEx[i] = px;
        pEy[i] = py;
        pEz[i] = pz;
    }
}

// =============================================================
// Quỹ đạo điểm cuối cho bài toán nghịch
// – xoắn ốc 3D (spiral) + poly5 mượt
// – tính luôn v(t), a(t) analytic (để dùng trong IK vận tốc/gia tốc)
// =============================================================
void CRobot::InitTrajectory()
{
    idCurrentPoint = 0;

    const double T = 10.0;          // "thời gian" tổng

    const double xC = 0.0;
    const double yC = 0.0;
    const double zStart = 50.0;
    const double zEnd = 200.0;

    const double numTurns = 2.0;
    const double radiusStart = 250.0;
    const double radiusEnd = 150.0;

    const double totalAngle = numTurns * 2.0 * PI;

    auto poly5 = [](double tau)->double
        {
            return 6 * tau * tau * tau * tau * tau
                - 15 * tau * tau * tau * tau
                + 10 * tau * tau * tau;
        };
    auto poly5_dot = [](double tau, double T)->double
        {
            return (30 * tau * tau * tau * tau - 60 * tau * tau * tau + 30 * tau * tau) / T;
        };
    auto poly5_ddot = [](double tau, double T)->double
        {
            return (120 * tau * tau * tau - 180 * tau * tau + 60 * tau) / (T * T);
        };

    for (int i = 0; i < MAX_POINT; ++i)
    {
        const double tau = (MAX_POINT > 1) ? (double)i / (MAX_POINT - 1) : 0.0;

        const double s = poly5(tau);
        const double s_dot = poly5_dot(tau, T);
        const double s_ddot = poly5_ddot(tau, T);

        const double theta = tau * totalAngle;
        const double theta_dot = totalAngle / T;
        const double theta_ddot = 0.0;

        const double r = radiusStart + (radiusEnd - radiusStart) * s;
        const double r_dot = (radiusEnd - radiusStart) * s_dot;
        const double r_ddot = (radiusEnd - radiusStart) * s_ddot;

        const double z = zStart + (zEnd - zStart) * s;
        const double z_dot = (zEnd - zStart) * s_dot;
        const double z_ddot = (zEnd - zStart) * s_ddot;

        const double c = std::cos(theta);
        const double s_t = std::sin(theta);

        // Vị trí
        pEx[i] = xC + r * c;
        pEy[i] = yC + r * s_t;
        pEz[i] = z;

        // Vận tốc
        pExv[i] = r_dot * c - r * s_t * theta_dot;
        pEyv[i] = r_dot * s_t + r * c * theta_dot;
        pEzv[i] = z_dot;

        // Gia tốc
        pExa[i] = r_ddot * c - 2 * r_dot * s_t * theta_dot
            - r * c * theta_dot * theta_dot
            - r * s_t * theta_ddot;

        pEya[i] = r_ddot * s_t + 2 * r_dot * c * theta_dot
            - r * s_t * theta_dot * theta_dot
            + r * c * theta_ddot;

        pEza[i] = z_ddot;
    }
}

// =============================================================
// Bài toán nghịch: dùng Newton–Raphson với funcJacobi
// (dùng nghiệm trước làm initial cho nghiệm sau)
// =============================================================
void CRobot::SolvingInverseKinematics()
{
    if (!pRobot) pRobot = this;

    vectorm x(3);
    x(0) = PI / 4.0;   // initial theta1
    x(1) = 200.0;      // initial d2
    x(2) = 0.0;        // initial theta3

    const double eps = 1e-5;
    const int    maxloop = 50;

    for (int i = 0; i < MAX_POINT; ++i)
    {
        idCurrentPoint = i;

        int result = Newton_Raphson(x, funcJacobi, eps, maxloop);

        if (result == 0)
        {
            q1[i] = x(0);
            q2[i] = x(1);
            q3[i] = x(2);
        }
        else
        {
            // nếu fail, giữ nghiệm trước
            if (i > 0)
            {
                q1[i] = q1[i - 1];
                q2[i] = q2[i - 1];
                q3[i] = q3[i - 1];

                x(0) = q1[i];
                x(1) = q2[i];
                x(2) = q3[i];
            }
        }
    }
}

// =============================================================
// Vận tốc: J(q) * q̇ = vE  →  q̇ = J^{-1} vE
// =============================================================
void CRobot::SolvingVelocity()
{
    const int n = 3;
    smatrix A(n);
    vectorm b(n), x(n);

    for (int i = 0; i < MAX_POINT; ++i)
    {
        const double q1i = q1[i];
        const double q2i = q2[i];
        const double q3i = q3[i];

        const double c1 = std::cos(q1i);
        const double s1 = std::sin(q1i);
        const double c3 = std::cos(q3i);
        const double s3 = std::sin(q3i);

        const double r = a2 + a3 * c3;

        // Jacobian tại điểm i
        A(0, 0) = -r * s1;
        A(0, 1) = 0.0;
        A(0, 2) = -a3 * c1 * s3;

        A(1, 0) = r * c1;
        A(1, 1) = 0.0;
        A(1, 2) = -a3 * s1 * s3;

        A(2, 0) = 0.0;
        A(2, 1) = 1.0;
        A(2, 2) = a3 * c3;

        b(0) = pExv[i];
        b(1) = pEyv[i];
        b(2) = pEzv[i];

        // Kiểm tra singular
        double det = A(0, 0) * (A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1))
            - A(0, 1) * (A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0))
            + A(0, 2) * (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0));

        if (std::fabs(det) < 1e-6)
        {
            if (i > 0)
            {
                q1v[i] = q1v[i - 1];
                q2v[i] = q2v[i - 1];
                q3v[i] = q3v[i - 1];
            }
            else
            {
                q1v[i] = q2v[i] = q3v[i] = 0.0;
            }
            continue;
        }

        int result = Gauss_Jordan(A, b, x);
        if (result == 0)
        {
            q1v[i] = x(0);
            q2v[i] = x(1);
            q3v[i] = x(2);
        }
        else
        {
            if (i > 0)
            {
                q1v[i] = q1v[i - 1];
                q2v[i] = q2v[i - 1];
                q3v[i] = q3v[i - 1];
            }
            else
            {
                q1v[i] = q2v[i] = q3v[i] = 0.0;
            }
        }
    }
}

// =============================================================
// Gia tốc:  aE = J*q̈ + J̇*q̇  →  q̈ = J^{-1}(aE - J̇*q̇)
// J̇*q̇ được tính giải tích đúng theo FK trên
// =============================================================
void CRobot::SolvingAcceleration()
{
    const int n = 3;
    smatrix A(n);
    vectorm b(n), x(n);

    for (int i = 0; i < MAX_POINT; ++i)
    {
        const double q1i = q1[i];
        const double q2i = q2[i];
        const double q3i = q3[i];
        const double dq1 = q1v[i];
        const double dq2 = q2v[i];
        const double dq3 = q3v[i];

        const double c1 = std::cos(q1i);
        const double s1 = std::sin(q1i);
        const double c3 = std::cos(q3i);
        const double s3 = std::sin(q3i);

        const double r = a2 + a3 * c3;

        // Jacobian J
        A(0, 0) = -r * s1;
        A(0, 1) = 0.0;
        A(0, 2) = -a3 * c1 * s3;

        A(1, 0) = r * c1;
        A(1, 1) = 0.0;
        A(1, 2) = -a3 * s1 * s3;

        A(2, 0) = 0.0;
        A(2, 1) = 1.0;
        A(2, 2) = a3 * c3;

        // J̇*q̇ – lấy từ đạo hàm analytic (tính sẵn bằng sympy)
        const double Jdx =
            -a2 * dq1 * dq1 * std::cos(q1i)
            - a3 * dq1 * dq1 * std::cos(q1i) * c3
            + 2.0 * a3 * dq1 * dq3 * std::sin(q1i) * s3
            - a3 * dq3 * dq3 * std::cos(q1i) * c3;

        const double Jdy =
            -a2 * dq1 * dq1 * std::sin(q1i)
            - a3 * dq1 * dq1 * std::sin(q1i) * c3
            - 2.0 * a3 * dq1 * dq3 * s3 * std::cos(q1i)
            - a3 * dq3 * dq3 * std::sin(q1i) * c3;

        const double Jdz =
            -a3 * dq3 * dq3 * s3;

        // b = aE - J̇*q̇
        b(0) = pExa[i] - Jdx;
        b(1) = pEya[i] - Jdy;
        b(2) = pEza[i] - Jdz;

        // Định thức
        double det = A(0, 0) * (A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1))
            - A(0, 1) * (A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0))
            + A(0, 2) * (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0));

        if (std::fabs(det) < 1e-6)
        {
            if (i > 0)
            {
                q1a[i] = q1a[i - 1];
                q2a[i] = q2a[i - 1];
                q3a[i] = q3a[i - 1];
            }
            else
            {
                q1a[i] = q2a[i] = q3a[i] = 0.0;
            }
            continue;
        }

        int result = Gauss_Jordan(A, b, x);
        if (result == 0)
        {
            q1a[i] = x(0);
            q2a[i] = x(1);
            q3a[i] = x(2);
        }
        else
        {
            if (i > 0)
            {
                q1a[i] = q1a[i - 1];
                q2a[i] = q2a[i - 1];
                q3a[i] = q3a[i - 1];
            }
            else
            {
                q1a[i] = q2a[i] = q3a[i] = 0.0;
            }
        }
    }
}
