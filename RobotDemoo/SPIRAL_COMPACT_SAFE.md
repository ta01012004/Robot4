# ? QU? ??O XO?N ?C NH? G?N - AN TOÀN

## ?? ?I?U CH?NH

### **Qu? ??o nh? g?n, không v??t gi?i h?n kh?p**

---

## ?? GI?I H?N CÁC KH?P

### **Robot RRP 3-DOF:**
```
?1 (Revolute):     0° ? 360° (0 ? 2? rad)
d2 (Prismatic):    0 ? 500 mm
?3 (Revolute):     -90° ? +90° (-?/2 ? ?/2 rad)
```

### **Workspace:**
```
Bán kính t?i ?a: ?(L2² + L3²) = ?(300² + 300²) ? 424 mm
Chi?u cao: d2 = 0 ? 500 mm
```

---

## ?? QU? ??O M?I - NH? G?N

### **1. FORWARD KINEMATICS**

**Tham s?:**
```cpp
numTurns = 2.0        // 2 vòng (gi?m t? 3)
d2_min = 100.0 mm     // An toàn (không quá g?n 0)
d2_max = 400.0 mm     // An toàn (không quá g?n 500)
theta3 = ±0.2 rad     // ±11.5° (gi?m t? ±0.3 rad)
```

**Giá tr? kh?p:**
```
?1: 0° ? 720° (2 vòng)
d2: 100mm ? 400mm (300mm hành trình)
?3: -11.5° ? +11.5° (dao ??ng nh?)
```

**??c ?i?m:**
- ? Quay 2 vòng (?? ?? th?y xo?n ?c, không quá nhi?u)
- ? d2 trong kho?ng an toàn [100, 400]mm
- ? ?3 dao ??ng nh? (không v??t ±90°)

---

### **2. INVERSE KINEMATICS**

**Tham s?:**
```cpp
R_start = 250.0 mm    // Bán kính b?t ??u (gi?m t? 400)
R_end = 150.0 mm      // Bán kính k?t thúc (gi?m t? 200)
z_start = 150.0 mm    // ?? cao b?t ??u
z_end = 350.0 mm      // ?? cao k?t thúc
numTurns = 2.0        // 2 vòng (gi?m t? 3)
```

**V? trí end-effector:**
```
?i?m b?t ??u: (250, 0, 150)
?i?m k?t thúc: (150, 0, 350)
Bán kính: 250 ? 150 mm (thu h?p 100mm)
?? cao: 150 ? 350 mm (t?ng 200mm)
```

**??c ?i?m:**
- ? Bán kính < 424mm (trong workspace)
- ? ?? cao [150, 350]mm (an toàn v?i d2)
- ? 2 vòng xo?n (v?a ?? ?? th?y rõ)
- ? Thu h?p d?n (??p m?t)

---

## ?? HÌNH D?NG XO?

N ?C

### **A. Forward Kinematics - Xo?n trong không gian kh?p**

```
t=0:    ?1=0°,    d2=100mm, ?3=0°
t=0.25: ?1=180°,  d2=175mm, ?3=+0.2rad
t=0.5:  ?1=360°,  d2=250mm, ?3=0°
t=0.75: ?1=540°,  d2=325mm, ?3=-0.2rad
t=1:    ?1=720°,  d2=400mm, ?3=0°
```

? **2 vòng xo?n, ?i lên t? 100 ? 400mm**

---

### **B. Inverse Kinematics - Xo?n hoàn h?o trong Cartesian**

```
        ? z (350mm)
        |
        |   ??
        |  ?  ?  ? Vòng 2 (R=150mm, z=350mm)
        | ?    ?
        |?      ? ? Vòng 1 (R=250mm, z=150mm)
        ????????? x,y
```

**V? trí các ?i?m ??c tr?ng:**
```
t=0:    x=250,  y=0,    z=150   (B?t ??u - l?n, th?p)
t=0.25: x=0,    y=200,  z=200
t=0.5:  x=-150, y=0,    z=250   (Gi?a - trung bình)
t=0.75: x=0,    y=-150, z=300
t=1:    x=150,  y=0,    z=350   (K?t thúc - nh?, cao)
```

? **2 vòng xo?n, thu h?p t? R=250 ? 150mm, t?ng cao t? 150 ? 350mm**

---

## ? KI?M TRA AN TOÀN

### **1. Gi?i h?n ?1:**
```
? 0° ? 720° (2 vòng)
   Không v??t quá (robot có th? quay vô h?n vòng)
```

### **2. Gi?i h?n d2:**
```
Forward:  100 ? 400 mm
Inverse:  z = 150 ? 350 mm ? d2 t??ng ?ng
? C? hai ??u < 500mm (gi?i h?n)
? C? hai ??u > 0mm (gi?i h?n d??i)
```

### **3. Gi?i h?n ?3:**
```
Forward:  ±0.2 rad = ±11.5°
? R?t nh? so v?i ±90° (gi?i h?n)

Inverse:  Ph? thu?c vào IK solver
? R=[150, 250]mm ??m b?o ?3 h?p lý
```

### **4. Workspace:**
```
Bán kính max = 250mm < 424mm ?
?? cao max = 350mm < 500mm ?
```

? **T?T C? ??U AN TOÀN!** ?

---

## ?? SO SÁNH C? VÀ M?I

### **FORWARD KINEMATICS:**

| Tham s? | C? (l?n) | M?i (nh? g?n) |
|---------|----------|---------------|
| S? vòng | 3 vòng | **2 vòng** ? |
| d2 min | 50 mm | **100 mm** ? |
| d2 max | 450 mm | **400 mm** ? |
| ?3 biên ?? | ±0.3 rad | **±0.2 rad** ? |

### **INVERSE KINEMATICS:**

| Tham s? | C? (l?n) | M?i (nh? g?n) |
|---------|----------|---------------|
| S? vòng | 3 vòng | **2 vòng** ? |
| R b?t ??u | 400 mm | **250 mm** ? |
| R k?t thúc | 200 mm | **150 mm** ? |
| z b?t ??u | 100 mm | **150 mm** ? |
| z k?t thúc | 400 mm | **350 mm** ? |

? **T?t c? ??u gi?m, nh? g?n h?n!**

---

## ?? ?I?U CH?NH TI?P (N?U C?N)

### **Mu?n xo?n ?c nh? h?n n?a:**

```cpp
// Forward
numTurns = 1.5;     // 1.5 vòng
d2_min = 150.0;     // B?t ??u cao h?n
d2_max = 350.0;     // K?t thúc th?p h?n

// Inverse
R_start = 200.0;    // Nh? h?n
R_end = 100.0;      // Nh? h?n
z_start = 200.0;    // Cao h?n
z_end = 300.0;      // Th?p h?n
numTurns = 1.5;     // 1.5 vòng
```

### **Mu?n xo?n ?c l?n h?n (trong gi?i h?n):**

```cpp
// Forward
numTurns = 2.5;     // 2.5 vòng
d2_min = 50.0;      // G?n 0
d2_max = 450.0;     // G?n 500

// Inverse
R_start = 300.0;    // L?n h?n
R_end = 200.0;      // L?n h?n
z_start = 100.0;    // Th?p h?n
z_end = 400.0;      // Cao h?n
numTurns = 2.5;     // 2.5 vòng
```

**?? L?U Ý:** Gi? R < 424mm ?? ??m b?o trong workspace!

---

## ?? KI?M TRA K?T QU?

### **Sau khi ch?y (F5):**

**1. Forward Kinematics:**
```
Menu > Select Problem > Forward Kinematics
Menu > Demo > Run
```

? **Robot quay 2 vòng nh? nhàng, d2 t?ng t? 100 ? 400mm**

**2. Inverse Kinematics:**
```
Menu > Select Problem > Inverse Kinematics
Menu > Demo > Run
```

? **End-effector v? xo?n ?c nh? g?n, 2 vòng, thu h?p d?n**

**3. Ki?m tra ?? th?:**
```
Forward/Inverse Kinematics > Joint Variables
```

? **?? th? s? m??t mà, không có ??t bi?n**

---

## ?? L?I ÍCH

### **1. An toàn:**
? Không v??t gi?i h?n kh?p
? Luôn trong workspace
? Không có singularity

### **2. D? quan sát:**
? 2 vòng v?a ?? th?y rõ xo?n ?c
? Không quá ph?c t?p
? D? debug n?u có l?i

### **3. Th?c t?:**
? Gi?ng tác v? th?c t? (tr?n, qu?n)
? V?n t?c/gia t?c h?p lý
? Robot không ph?i chuy?n ??ng quá nhanh

---

## ?? CÔNG TH?C THAM KH?O

### **Ki?m tra workspace:**
```
Bán kính t?i ?a: R_max = ?(L2² + L3²)
V?i L2 = L3 = 300mm:
R_max = ?(300² + 300²) = ?180000 ? 424.26 mm
```

### **Ki?m tra d2:**
```
0 ? d2 ? 500 mm
```

### **Ki?m tra ?3:**
```
-?/2 ? ?3 ? ?/2
-90° ? ?3 ? 90°
```

? **Qu? ??o m?i ??m b?o T?T C? ?i?u ki?n!**

---

## ? HOÀN T?T

**?Ã T?O QU? ??O XO?N ?C NH? G?N!**

**??c ?i?m:**
- ? 2 vòng xo?n (v?a ??)
- ? Không v??t gi?i h?n kh?p
- ? Luôn trong workspace
- ? V?n t?c/gia t?c m??t mà
- ? D? quan sát và debug

**Ch?y ngay (F5) ?? xem qu? ??o nh? g?n!** ???
