#include "pch.h"
#include "STLLoader.h"

// Undefine Windows macros ?? dùng std::min/max
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <algorithm>

CSTLLoader::CSTLLoader()
{
}

CSTLLoader::~CSTLLoader()
{
    Clear();
}

void CSTLLoader::Clear()
{
    triangles.clear();
}

bool CSTLLoader::LoadSTL(const char* filename)
{
    // Th? load binary tr??c
    if (LoadBinarySTL(filename))
        return true;
    
    // N?u không ???c thì th? ASCII
    return LoadASCIISTL(filename);
}

bool CSTLLoader::LoadBinarySTL(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        return false;

    // ??c header (80 bytes)
    char header[80];
    file.read(header, 80);

    // ??c s? l??ng triangles
    unsigned int numTriangles;
    file.read((char*)&numTriangles, sizeof(unsigned int));

    // Ki?m tra kích th??c file
    file.seekg(0, std::ios::end);
    size_t fileSize = (size_t)file.tellg();
    size_t expectedSize = 80 + 4 + numTriangles * 50;
    
    if (fileSize != expectedSize) {
        file.close();
        return false;
    }

    file.seekg(84, std::ios::beg);
    triangles.reserve(numTriangles);

    // ??c t?ng triangle
    for (unsigned int i = 0; i < numTriangles; i++)
    {
        Triangle tri;
        
        // Normal vector
        file.read((char*)tri.normal, 3 * sizeof(float));
        
        // Vertex 1
        file.read((char*)tri.v1, 3 * sizeof(float));
        
        // Vertex 2
        file.read((char*)tri.v2, 3 * sizeof(float));
        
        // Vertex 3
        file.read((char*)tri.v3, 3 * sizeof(float));
        
        // Attribute byte count (skip)
        unsigned short attributeByteCount;
        file.read((char*)&attributeByteCount, sizeof(unsigned short));
        
        triangles.push_back(tri);
    }

    file.close();
    return true;
}

bool CSTLLoader::LoadASCIISTL(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string line;
    Triangle tri;
    int vertexIndex = 0;

    while (std::getline(file, line))
    {
        if (line.find("facet normal") != std::string::npos)
        {
            sscanf_s(line.c_str(), " facet normal %f %f %f", 
                     &tri.normal[0], &tri.normal[1], &tri.normal[2]);
            vertexIndex = 0;
        }
        else if (line.find("vertex") != std::string::npos)
        {
            float* vertex = (vertexIndex == 0) ? tri.v1 : 
                           (vertexIndex == 1) ? tri.v2 : tri.v3;
            
            sscanf_s(line.c_str(), " vertex %f %f %f", 
                     &vertex[0], &vertex[1], &vertex[2]);
            vertexIndex++;
        }
        else if (line.find("endfacet") != std::string::npos)
        {
            triangles.push_back(tri);
        }
    }

    file.close();
    return !triangles.empty();
}

void CSTLLoader::Draw()
{
    if (triangles.empty())
        return;

    glBegin(GL_TRIANGLES);
    
    for (size_t i = 0; i < triangles.size(); i++)
    {
        const Triangle& tri = triangles[i];
        
        // Set normal
        glNormal3fv(tri.normal);
        
        // Draw vertices
        glVertex3fv(tri.v1);
        glVertex3fv(tri.v2);
        glVertex3fv(tri.v3);
    }
    
    glEnd();
}

void CSTLLoader::GetBounds(float& minX, float& minY, float& minZ, 
                           float& maxX, float& maxY, float& maxZ)
{
    if (triangles.empty())
    {
        minX = minY = minZ = 0;
        maxX = maxY = maxZ = 0;
        return;
    }

    minX = minY = minZ = FLT_MAX;
    maxX = maxY = maxZ = -FLT_MAX;

    for (const auto& tri : triangles)
    {
        for (int i = 0; i < 3; i++)
        {
            const float* v = (i == 0) ? tri.v1 : (i == 1) ? tri.v2 : tri.v3;
            
            minX = (minX < v[0]) ? minX : v[0];
            minY = (minY < v[1]) ? minY : v[1];
            minZ = (minZ < v[2]) ? minZ : v[2];
            
            maxX = (maxX > v[0]) ? maxX : v[0];
            maxY = (maxY > v[1]) ? maxY : v[1];
            maxZ = (maxZ > v[2]) ? maxZ : v[2];
        }
    }
}
