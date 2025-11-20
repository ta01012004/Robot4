# ? QU? ??O XO?N ?C (SPIRAL TRAJECTORY)

## ?? THI?T K? M?I

### **Thay th? qu? ??o c? b?ng qu? ??o xo?n ?c 3D**

---

## ?? 2 LO?I QU? ??O

### **1. FORWARD KINEMATICS - Xo?n ?c trong không gian kh?p**
```cpp
void SetVariables()
{
    // ?1: Quay 3 vòng
    q1[i] = 3.0 * 2? * t
    
    // d2: T?ng t? 50mm ? 450mm
    q2[i] = 50 + 400 * t
    
    // ?3: Dao ??ng ±0.3 rad
    q3[i] = 0.3 * sin(4? * t)
}
```

### **2. INVERSE KINEMATICS - Xo?n ?c trong không gian Cartesian**
```cpp
void InitTrajectory()
{
    // Bán kính: 400mm ? 200mm (thu h?p)
    R = 400 + (200 - 400) * s(t)
    
    // Góc: Quay 3 vòng
    ? = 3 * 2? * t
    
    // ?? cao: 100mm ? 400mm (t?ng)
    z = 100 + 300 * s(t)
    
    // V? trí Cartesian
    x = R * cos(?)
    y = R * sin(?)
    z = z
}
```

---

## ?? HÌNH D?NG QU? ??O

### **A. FORWARD KINEMATICS**

**??c ?i?m:**
- ? Xo?n ?c trong không gian kh?p
- ? ?? quay 3 vòng (0 ? 6? rad)
- ? d? t?ng tuy?n tính (50 ? 450 mm)
- ? ?? dao ??ng (t?o u?n l??n)

**Hình d?ng end-effector:**
- Qu? ??o ph?c t?p do k?t h?p 3 chuy?n ??ng
- Không ph?i xo?n ?c hoàn h?o (vì qua FK)

**Ví d?:**
```
?i?m 0:   q1 = 0°,    d2 = 50mm,  q3 = 0°
?i?m 25:  q1 = 270°,  d2 = 150mm, q3 = 0.3 rad
?i?m 50:  q1 = 540°,  d2 = 250mm, q3 = 0°
?i?m 75:  q1 = 810°,  d2 = 350mm, q3 = -0.3 rad
?i?m 100: q1 = 1080°, d2 = 450mm, q3 = 0°
```

---

### **B. INVERSE KINEMATICS**

**??c ?i?m:**
- ? Xo?n ?c hoàn h?o trong không gian Cartesian
- ? Bán kính thu h?p d?n (400 ? 200 mm)
- ? Quay 3 vòng quanh tr?c Z
- ? ?? cao t?ng (100 ? 400 mm)

**Hình d?ng end-effector:**
```
        ? z
        |
        |    ??
        |   ?  ?  ? Vòng 3 (R=200mm, z=400mm)
        |  ?    ?
        | ?      ?  ? Vòng 2 (R=300mm, z=250mm)
        ?          ?
       ?            ?  ? Vòng 1 (R=400mm, z=100mm)
      ???????????????? x,y
```

**Ví d?:**
```
?i?m 0:   x=400,  y=0,    z=100   (Start)
?i?m 25:  x=0,    y=350,  z=175
?i?m 50:  x=-300, y=0,    z=250
?i?m 75:  x=0,    y=-250, z=325
?i?m 100: x=200,  y=0,    z=400   (End)
```

---

## ?? CÔNG TH?C TOÁN H?C

### **1. Polynomial B?c 5 (M??t Mà)**

```
s(t) = 6t? - 15t? + 10t³

??c ?i?m:
- s(0) = 0,  s(1) = 1        (??u/cu?i ?úng v? trí)
- s'(0) = 0, s'(1) = 0       (V?n t?c ??u/cu?i = 0)
- s''(0) = 0, s''(1) = 0     (Gia t?c ??u/cu?i = 0)
```

? **Chuy?n ??ng m??t mà, không gi?t!**

### **2. T?a ?? tr? ? Cartesian**

```
x = R(t) * cos(?(t))
y = R(t) * sin(?(t))
z = z(t)

V?i:
- R(t) = R_start + (R_end - R_start) * s(t)
- ?(t) = n * 2? * t  (n = s? vòng)
- z(t) = z_start + (z_end - z_start) * s(t)
```

### **3. V?n t?c (v = dx/dt)**

```
v_x = R' * cos(?) - R * sin(?) * ?'
v_y = R' * sin(?) + R * cos(?) * ?'
v_z = z'

V?i:
- R' = dR/dt
- ?' = d?/dt = n * 2? / T
- z' = dz/dt
```

### **4. Gia t?c (a = dv/dt)**

```
a_x = R'' * cos(?) - 2R' * sin(?) * ?' - R * cos(?) * ?'² - R * sin(?) * ?''
a_y = R'' * sin(?) + 2R' * cos(?) * ?' - R * sin(?) * ?'² + R * cos(?) * ?''
a_z = z''
```

? **Bao g?m:**
- Thành ph?n h??ng tâm: `-R * ?'²`
- Thành ph?n Coriolis: `2R' * ?'`
- Thành ph?n ti?p tuy?n: `R * ?''`

---

## ?? THAM S? ?I?U CH?NH

### **A. Forward Kinematics (SetVariables)**

```cpp
const double numTurns = 3.0;  // S? vòng xo?n (thay ??i ?? có nhi?u/ít vòng)
const double d2_min = 50.0;   // d2 b?t ??u (mm)
const double d2_max = 450.0;  // d2 k?t thúc (mm)
```

**?i?u ch?nh:**
- `numTurns = 1.0` ? 1 vòng xo?n (ít ph?c t?p)
- `numTurns = 5.0` ? 5 vòng xo?n (r?t ph?c t?p)
- `d2_max = 300.0` ? Xo?n th?p h?n

### **B. Inverse Kinematics (InitTrajectory)**

```cpp
const double R_start = 400.0;   // Bán kính b?t ??u (mm)
const double R_end = 200.0;     // Bán kính k?t thúc (mm)
const double z_start = 100.0;   // ?? cao b?t ??u (mm)
const double z_end = 400.0;     // ?? cao k?t thúc (mm)
const double numTurns = 3.0;    // S? vòng xo?n
```

**?i?u ch?nh:**

#### **1. Xo?n ?c m? r?ng (thay vì thu h?p):**
```cpp
const double R_start = 200.0;   // B?t ??u nh?
const double R_end = 400.0;     // K?t thúc l?n
```

#### **2. Xo?n ?c xu?ng (thay vì lên):**
```cpp
const double z_start = 400.0;   // B?t ??u cao
const double z_end = 100.0;     // K?t thúc th?p
```

#### **3. Xo?n ?c nhi?u vòng:**
```cpp
const double numTurns = 5.0;    // 5 vòng
```

#### **4. Xo?n ?c hình tr? (bán kính không ??i):**
```cpp
const double R_start = 300.0;
const double R_end = 300.0;     // Gi?ng nhau
```

---

## ?? SO SÁNH V?I QU? ??O C?

### **QU? ??O C? (A ? A1 ? B ? B1):**

| ??c ?i?m | Mô t? |
|----------|-------|
| Hình d?ng | 3 ?o?n th?ng + 1 cung tròn |
| ?? ph?c t?p | Trung bình |
| Tính th?c t? | Gi?ng tác v? pick-and-place |
| V?n t?c | M??t (polynomial 5) |
| Gia t?c | Liên t?c |

### **QU? ??O M?I (Xo?n ?c):**

| ??c ?i?m | Mô t? |
|----------|-------|
| Hình d?ng | Xo?n ?c 3D |
| ?? ph?c t?p | Cao |
| Tính th?c t? | Gi?ng tác v? tr?n, qu?n dây |
| V?n t?c | M??t (polynomial 5) |
| Gia t?c | Liên t?c (bao g?m thành ph?n h??ng tâm) |

---

## ?? KI?M TRA K?T QU?

### **Sau khi build và ch?y:**

**1. Forward Kinematics:**
```
Menu > Select Problem > Forward Kinematics
Menu > Demo > Run
```

? B?n s? th?y:
- Robot quay nhi?u vòng quanh tr?c Z
- Kh?p t?nh ti?n d? t?ng d?n
- Kh?p ?? dao ??ng (u?n l??n)
- End-effector v? qu? ??o ph?c t?p

**2. Inverse Kinematics:**
```
Menu > Select Problem > Inverse Kinematics
Menu > Demo > Run
```

? B?n s? th?y:
- End-effector v? xo?n ?c hoàn h?o
- Bán kính thu h?p d?n
- ?? cao t?ng d?n
- Robot chuy?n ??ng m??t mà

**3. Xem ?? th?:**
```
Forward Kinematics > Endpoint
Inverse Kinematics > Endpoints
```

? ?? th? s? hi?n th?:
- X, Y, Z theo d?ng sin/cos
- Chu k? t??ng ?ng v?i s? vòng xo?n

---

## ?? ?NG D?NG TH?C T?

### **1. Tr?n/khu?y:**
- Xo?n ?c trong bình ch?a
- Bán kính thay ??i ?? tr?n ??u

### **2. Qu?n dây:**
- Xo?n ?c quanh lõi
- ?? cao t?ng d?n

### **3. Painting/coating:**
- Xo?n ?c trên b? m?t
- Ph? ??u s?n ph?m

### **4. Assembly:**
- L?p ráp theo ???ng xo?n
- V?n ?c

---

## ? ?U ?I?M

### **1. M??t mà:**
? Dùng polynomial b?c 5
? V?n t?c ??u/cu?i = 0
? Gia t?c liên t?c

### **2. T?ng quát:**
? D? ?i?u ch?nh tham s?
? Có th? t?o nhi?u bi?n th?
? Áp d?ng cho nhi?u tác v?

### **3. Ki?m tra IK/Velocity/Acceleration:**
? Qu? ??o ph?c t?p ? test t?t thu?t toán
? Bao g?m thành ph?n h??ng tâm và Coriolis
? Ki?m tra singularity

---

## ?? L?U Ý

### **1. Workspace:**
??m b?o qu? ??o n?m trong workspace:
```
d2: [0, 500] mm
R: T?i ?a ?(L2² + L3²) ? 424 mm
```

? **R_start = 400mm là OK!**

### **2. Singularity:**
- Tránh R = 0 (tr?c Z)
- Tránh ?? = ±90° (gi?i h?n)

### **3. Initial Guess cho IK:**
```cpp
x(0) = PI / 4.0;  // ?1 = 45°
x(1) = 200;       // d2 = 200mm (gi?a workspace)
x(2) = PI / 6.0;  // ?3 = 30°
```

? **?? g?n ?? Newton-Raphson h?i t?!**

---

## ? HOÀN T?T

**?Ã THI?T K? QU? ??O XO?N ?C!**

**Forward Kinematics:**
- ? Xo?n ?c trong không gian kh?p
- ? 3 vòng, d?: 50-450mm
- ? ?? dao ??ng

**Inverse Kinematics:**
- ? Xo?n ?c hoàn h?o trong Cartesian
- ? R: 400?200mm, z: 100?400mm
- ? V?n t?c/gia t?c m??t mà

**Ch?y ch??ng trình (F5) ?? xem robot v? xo?n ?c!** ????
