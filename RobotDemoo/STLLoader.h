#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <gl/GL.h>

// Triangle structure for STL
struct Triangle
{
    float normal[3];
    float v1[3];
    float v2[3];
    float v3[3];
};

class CSTLLoader
{
public:
    CSTLLoader();
    ~CSTLLoader();

    // Load STL file (automatically detects binary or ASCII)
    bool LoadSTL(const char* filename);
    
    // Draw the loaded STL model
    void Draw();
    
    // Get bounding box of the model
    void GetBounds(float& minX, float& minY, float& minZ, 
                   float& maxX, float& maxY, float& maxZ);
    
    // Clear loaded data
    void Clear();

    // Public access to triangles (for statistics)
    std::vector<Triangle> triangles;

private:
    bool LoadBinarySTL(const char* filename);
    bool LoadASCIISTL(const char* filename);
};
