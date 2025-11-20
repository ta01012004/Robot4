#include "pch.h"
#include "Transformations.h"

#include <cmath>
#include <iostream>

using namespace std;

// =======================================================
// ===============  HÀM CƠ BẢN 4x4  =======================
// =======================================================

void SetIdentity(smatrix& m)
{
    m.SetSize(4);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m(i, j) = (i == j) ? 1.0 : 0.0;
}

void SetIdentity(vectorm& v)
{
    v.SetSize(4);
    for (int i = 0; i < 4; i++)
        v[i] = (i == 3) ? 1.0 : 0.0;
}

void SetVector(vectorm& v, double tx, double ty, double tz)
{
    SetIdentity(v);
    v[0] = tx;
    v[1] = ty;
    v[2] = tz;
}

void SetTranslationMatrix(smatrix& m, double tx, double ty, double tz)
{
    SetIdentity(m);
    m(0, 3) = tx;
    m(1, 3) = ty;
    m(2, 3) = tz;
}

void SetScalationMatrix(smatrix& m, double sx, double sy, double sz)
{
    SetIdentity(m);
    m(0, 0) = sx;
    m(1, 1) = sy;
    m(2, 2) = sz;
}

void SetRotationXMatrix(smatrix& m, double angle)
{
    SetIdentity(m);
    double c = cos(angle);
    double s = sin(angle);
    m(1, 1) = c;  m(1, 2) = -s;
    m(2, 1) = s;  m(2, 2) = c;
}

void SetRotationYMatrix(smatrix& m, double angle)
{
    SetIdentity(m);
    double c = cos(angle);
    double s = sin(angle);
    m(0, 0) = c;  m(0, 2) = s;
    m(2, 0) = -s; m(2, 2) = c;
}

void SetRotationZMatrix(smatrix& m, double angle)
{
    SetIdentity(m);
    double c = cos(angle);
    double s = sin(angle);
    m(0, 0) = c;  m(0, 1) = -s;
    m(1, 0) = s;  m(1, 1) = c;
}

// =======================================================
// ===============  MA TRẬN D-H CHUẨN  ====================
// =======================================================
//
// Quy ước: A = RotZ(theta) * TransZ(d) * TransX(a) * RotX(alpha)
//
// giống 100% lý thuyết DH và đúng với code FK của bạn
//
void SetDHmatrix(smatrix& A, double theta, double alpha, double a, double d)
{
    SetIdentity(A);

    double c = cos(theta);
    double s = sin(theta);
    double ca = cos(alpha);
    double sa = sin(alpha);

    A(0, 0) = c;
    A(0, 1) = -s * ca;
    A(0, 2) = s * sa;
    A(0, 3) = a * c;

    A(1, 0) = s;
    A(1, 1) = c * ca;
    A(1, 2) = -c * sa;
    A(1, 3) = a * s;

    A(2, 0) = 0;
    A(2, 1) = sa;
    A(2, 2) = ca;
    A(2, 3) = d;

    A(3, 0) = 0;
    A(3, 1) = 0;
    A(3, 2) = 0;
    A(3, 3) = 1;
}

// =======================================================
// ===============  RIÊNG CHO ROBOT R-P-P  ===============
// =======================================================
//
// CẤU HÌNH RPP (Rot – Prismatic – Prismatic)
//
//   Khớp 1: θ1 (quay quanh Z)
//   Khớp 2: d2 (trượt theo Z)
//   Khớp 3: d3 (trượt theo X)
//   Chiều dài khâu ngang: L2
//
// DH theo bảng của bạn:
//    A01 = DH(θ1, 0, 0, 0)
//    A12 = DH( 0, 0, L2, d2)
//    A23 = DH( 0, 0, d3, 0)

void SetRPP_A01(smatrix& A01, double theta1)
{
    SetDHmatrix(A01, theta1, 0.0, 0.0, 0.0);
}

void SetRPP_A12(smatrix& A12, double L2, double d2)
{
    SetDHmatrix(A12, 0.0, 0.0, L2, d2);
}

void SetRPP_A23(smatrix& A23, double d3)
{
    SetDHmatrix(A23, 0.0, 0.0, d3, 0.0);
}

// =======================================================
// ==========  NHÂN MA TRẬN 4x4 (CHO RPP hoặc chung) =====
// =======================================================

void MulMatrix4x4(const smatrix& A, const smatrix& B, smatrix& C)
{
    C.SetSize(4);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            double s = 0;
            for (int k = 0; k < 4; k++)
                s += A(i, k) * B(k, j);
            C(i, j) = s;
        }
}

// =======================================================
// ===============  TÍNH T03 CHO RPP  =====================
// =======================================================
//
//   T03 = A01 * A12 * A23
//
void SetRPP_T03(double theta1, double d2, double d3, double L2,
    smatrix& T03)
{
    smatrix A01, A12, A23, T01_2;

    SetRPP_A01(A01, theta1);
    SetRPP_A12(A12, L2, d2);
    SetRPP_A23(A23, d3);

    MulMatrix4x4(A01, A12, T01_2);
    MulMatrix4x4(T01_2, A23, T03);
}
