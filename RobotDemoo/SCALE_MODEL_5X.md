# ? ?Ã THU NH? MÔ HÌNH 5 L?N

## ?? THAY ??I

### **Scale Factor: 0.2 (= 1/5)**

---

## ?? CODE ?Ã S?A

### **1. Thu nh? ROBOT 5 l?n**
```cpp
glPushMatrix();
{
    glScaled(0.2, 0.2, 0.2);  // ? THU NH? 5 L?N
    
    // V? toàn b? robot ? ?ây
    // BASE ? RAIL ? CARRIAGE ? LINK1 ? LINK3 ? END-EFFECTOR
}
glPopMatrix();
```

### **2. Thu nh? TRAJECTORY & WORKSPACE 5 l?n**
```cpp
glPushMatrix();
{
    glScaled(0.2, 0.2, 0.2);  // ? Thu nh? 5 l?n
    m_objTrajectory.Draw();
    m_objWorkSpace.Draw();
}
glPopMatrix();
```

---

## ?? SO SÁNH

### **TR??C KHI THU NH?:**
| Thành ph?n | Kích th??c |
|------------|-----------|
| Robot | 100% (kích th??c g?c STL) |
| Trajectory | 100% |
| Workspace | 100% |
| Tr?c XYZ | 200 mm |

**V?n ??:** Robot quá l?n, chi?m h?t màn hình!

### **SAU KHI THU NH?:**
| Thành ph?n | Kích th??c |
|------------|-----------|
| Robot | **20%** (= 1/5 kích th??c g?c) |
| Trajectory | **20%** |
| Workspace | **20%** |
| Tr?c XYZ | 200 mm (không ??i) |

**K?t qu?:** Robot nh? g?n, d? quan sát toàn c?nh!

---

## ?? KÍCH TH??C C? TH?

### **N?u file STL g?c có kích th??c:**
```
BASE: 100 × 100 × 100 mm
LINK1: 300 × 20 × 20 mm
RAIL: 30 × 30 × 500 mm
...
```

### **Sau khi scale 0.2:**
```
BASE: 20 × 20 × 20 mm (hi?n th?)
LINK1: 60 × 4 × 4 mm (hi?n th?)
RAIL: 6 × 6 × 100 mm (hi?n th?)
...
```

**L?U Ý:** ?ây là kích th??c **hi?n th?** trong OpenGL, không thay ??i file STL g?c!

---

## ?? TR?C T?A ?? KHÔNG ??I

```cpp
// Tr?c XYZ KHÔNG b? scale
glDisable(GL_LIGHTING);
glLineWidth(3.0f);
glBegin(GL_LINES);
    // X: 200mm (màu ??)
    // Y: 200mm (màu xanh lá)
    // Z: 200mm (màu xanh d??ng)
glEnd();
```

? **Tr?c XYZ gi? nguyên 200mm** ?? làm tham chi?u!

---

## ?? ?I?U CH?NH SCALE N?U C?N

### **Mu?n robot nh? h?n n?a (thu nh? 10 l?n):**
```cpp
glScaled(0.1, 0.1, 0.1);  // Scale = 1/10
```

### **Mu?n robot l?n h?n (thu nh? 2 l?n):**
```cpp
glScaled(0.5, 0.5, 0.5);  // Scale = 1/2
```

### **Mu?n robot kích th??c g?c:**
```cpp
glScaled(1.0, 1.0, 1.0);  // Scale = 1 (không ??i)
// Ho?c b? dòng glScaled() ?i
```

### **Mu?n robot PHÓNG TO 2 l?n:**
```cpp
glScaled(2.0, 2.0, 2.0);  // Scale = 2x
```

---

## ?? CÔNG TH?C TÍNH

```
Scale Factor = 1 / (S? l?n thu nh?)

Ví d?:
- Thu nh? 2 l?n  ? Scale = 1/2 = 0.5
- Thu nh? 5 l?n  ? Scale = 1/5 = 0.2
- Thu nh? 10 l?n ? Scale = 1/10 = 0.1

- Phóng to 2 l?n ? Scale = 2.0
- Phóng to 3 l?n ? Scale = 3.0
```

---

## ?? VÌ SAO C?N SCALE?

### **1. File STL quá l?n**
- File STL t? CAD software th??ng có kích th??c th?c (mm)
- Robot th?c có th? cao 500-1000mm
- Hi?n th? 1:1 s? quá l?n trên màn hình

### **2. Camera không phù h?p**
- Camera trong `COpenGLCtrl` có v? trí c? ??nh
- Near plane = 1.0, Far plane = 5000.0
- C?n scale ?? robot v?a v?n trong view frustum

### **3. D? quan sát**
- Scale nh? ? Nhìn ???c toàn c?nh
- Scale l?n ? Nhìn chi ti?t t?ng b? ph?n

---

## ?? KI?M TRA K?T QU?

### **Sau khi build và ch?y:**

**1. Robot s? nh? h?n 5 l?n**
- D? nhìn toàn b? c?u trúc
- Không b? che khu?t

**2. Trajectory s? nh? h?n 5 l?n**
- Qu? ??o vàng kh?p v?i robot
- Không b? l?ch t? l?

**3. Workspace s? nh? h?n 5 l?n**
- ?ám mây ?i?m xanh d??ng kh?p v?i robot
- Không b? l?ch t? l?

**4. Tr?c XYZ gi? nguyên**
- V?n 200mm
- Làm tham chi?u ?? so sánh kích th??c

---

## ?? SCALE CHO T?NG PH?N (N?U C?N)

### **N?u mu?n scale riêng t?ng ph?n:**

#### **A. Ch? scale ROBOT:**
```cpp
// Robot scale 0.2
glPushMatrix();
{
    glScaled(0.2, 0.2, 0.2);
    // V? robot
}
glPopMatrix();

// Trajectory & Workspace scale 1.0 (không ??i)
m_objTrajectory.Draw();
m_objWorkSpace.Draw();
```

#### **B. Scale khác nhau cho t?ng link:**
```cpp
// BASE scale 0.3
glPushMatrix();
{
    glScaled(0.3, 0.3, 0.3);
    m_objLink0.Draw();
}
glPopMatrix();

// RAIL scale 0.2
glRotated(q1, 0, 0, 1);
glPushMatrix();
{
    glScaled(0.2, 0.2, 0.2);
    m_objRail.Draw();
}
glPopMatrix();
```

**?? KHÔNG KHUY?N KHÍCH:** S? làm robot b? méo!

---

## ?? L?U Ý QUAN TR?NG

### **1. Scale KHÔNG thay ??i file STL**
- Ch? thay ??i cách hi?n th?
- File STL g?c v?n nguyên v?n

### **2. Scale ?NH H??NG ??n t?t c? transform**
```cpp
glScaled(0.2, 0.2, 0.2);
glTranslated(L2, 0, 0);  // ? Th?c t? d?ch 0.2 * L2 = 60mm
```

? **?ã tính toán ?úng trong code!**

### **3. Scale không ?nh h??ng ??n DH parameters**
```cpp
const double L2 = m_pRobot->a2;  // = 300 mm
const double L3 = m_pRobot->a3;  // = 300 mm
```

? **L2, L3 v?n là 300mm trong FK/IK!**

---

## ? HOÀN T?T

**?Ã THU NH? TOÀN B? MÔ HÌNH 5 L?N!**

**K?t qu?:**
- ? Robot nh? g?n, d? quan sát
- ? Trajectory kh?p v?i robot
- ? Workspace kh?p v?i robot
- ? Tr?c XYZ gi? nguyên làm tham chi?u
- ? Build thành công

**Ch?y ch??ng trình (F5) ?? xem k?t qu?!** ??
