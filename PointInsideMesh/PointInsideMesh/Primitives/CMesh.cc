#include <fstream>
#include <sstream>
#include "CMesh.h"
#include "../Utility/Utility.h"

#include "IntersectTest.h"

#define __TEST_INTERSECT__

/////////////////// Ray ///////////////////
CRay::CRay(const Vector3D& origin, const Vector3D& direction)
    : mOrigin(origin), mDirection(Normalize(direction))
{

}

Vector3D CRay::GetSpecifiedPoint(double distance)
{
    return mOrigin + mDirection * distance;
}

/////////////////// CTriangle ///////////////////
CTriangle::CTriangle()
{

}

CTriangle::CTriangle(Vector3D a, Vector3D b, Vector3D c)
{
    m_Points[0] = a;
    m_Points[1] = b;
    m_Points[2] = c;
}

bool CTriangle::Initialize(void* arg)
{
    if (arg == NULL) {
        return false;
    }
    string* input_arg = static_cast<string*>(arg);
    string tmp;
    istringstream iss0(input_arg[0]);
    iss0 >> tmp >> tmp >> m_Normal.x >> m_Normal.y >> m_Normal.z;
    for (int i = 0; i < 3; i++) {
        istringstream iss(input_arg[i + 2]);
        iss >> tmp;
        iss >> m_Points[i].x >> m_Points[i].y >> m_Points[i].z;
    }
    return true;
}

bool CTriangle::Update()
{
    return true;
}

bool CTriangle::Destroy()
{
    return true;
}

// Make sure that glBegin() is ON!!
bool CTriangle::Render()
{
    glNormal3f(-m_Normal.x, -m_Normal.y, -m_Normal.z);
    for (int i = 0; i < 3; i++) {
        glVertex3dv(m_Points[i].num);
    }
    return true;
}

void CTriangle::DebugOutput()
{
    cout << "Normal: " << m_Normal.x << ' ' << m_Normal.y << ' ' << m_Normal.z  << endl;
    for (int i = 0; i < 3; i++) {
        cout << "Point" << i << ": " << m_Points[i].x << ' ' << m_Points[i].y << ' ' << m_Points[i].z << endl;
    }
}

int CTriangle::Intersect(CRay ray, Vector3D& interPos)
{
//     int raySegmentRes;
//     raySegmentRes = IntersectRaySegment(ray.GetOrigin(), ray.GetDirection(), m_Points[0], m_Points[1], interPos);
//     if (raySegmentRes != 0) {
//         return raySegmentRes;
//     }
//     raySegmentRes = IntersectRaySegment(ray.GetOrigin(), ray.GetDirection(), m_Points[1], m_Points[2], interPos);
//     if (raySegmentRes != 0) {
//         return raySegmentRes;
//     }
//     raySegmentRes = IntersectRaySegment(ray.GetOrigin(), ray.GetDirection(), m_Points[2], m_Points[0], interPos);
//     if (raySegmentRes != 0) {
//         return raySegmentRes;
//     }

    Vector3D u, v, n;
    Vector3D w0, w;
    double r, a, b;
    static const double epsilon = 1e-4;

    u = m_Points[1] - m_Points[0];
    v = m_Points[2] - m_Points[0];
#ifdef __TEST_INTERSECT__
    n = Cross(u, v);
#else
    n = Normalize(m_Normal);
#endif ///> end of __TEST_INTERSECT__

    w0 = ray.GetOrigin() - m_Points[0];
    a = -Dot(n, w0);
    b = Dot(n, ray.GetDirection());

    if (b > -epsilon && b < epsilon) {
        return false;
    }

    r = a / b;
    if (r < 0.0) {
        return false;
    }

    // Vector3D hitPoint = ray.GetOrigin() + r * ray.GetDirection();
    Vector3D hitPoint = ray.GetSpecifiedPoint(r);

    double uu, uv, vv, wu, wv, D;
    uu = Dot(u, u);
    uv = Dot(u, v);
    vv = Dot(v, v);
    w = hitPoint - m_Points[0];
    wu = Dot(w, u);
    wv = Dot(w, v);
    D = 1.0 / (uv * uv - uu * vv);

    double s, t;
    s = (uv * wv - vv * wu) * D;
    if (s < 0.0 || s > 1.0) {
        return 0;
    }
    t = (uv * wu - uu * wv) * D;
    if (t < 0.0 || (s + t) > 1.0) {
        return 0;
    }

    interPos = hitPoint;
    return 3;
}

double CTriangle::CalcClosestToPoint2(CPoint tgtPoint, Vector3D& resPoint)
{
    double res = DBL_MAX;
    for (int i = 0; i < 3; i++) {
        double l2 = Length2(tgtPoint.GetPoint() - m_Points[i]);
        if (l2 < res) {
            resPoint = m_Points[i];
            res = l2;
        }
    }
    return res;
}

/////////////////// CMesh ///////////////////
bool CMesh::Initialize(void* arg)
{
    if (arg == NULL) {
        return false;
    }
    string* input_arg = static_cast<string*>(arg);
    m_Error = "";
    m_TriangleArray.clear();

    ifstream fin(input_arg[0]);
    if (!fin) {
        m_Error = "cannot open the file: " + input_arg[0];
        return false;
    }
    string buffer[7];
    string mark, tmp_str;
    while (getline(fin, buffer[0])) {
        istringstream iss(buffer[0]);
        iss >> mark;
        if (mark == "facet") {
            CTriangle* tmp = new CTriangle();
            fin >> tmp_str;
            for (int i = 1; i < 7; i++) {
                getline(fin, buffer[i]);
            }
            tmp->Initialize(buffer);
            m_TriangleArray.push_back(tmp);

            for (int i = 0; i < 3; i++) {
                m_Center += tmp->m_Points[i];
            }
        }
    }
    m_Center /= (m_TriangleArray.size() * 3.0);

    return true;
}

bool CMesh::Update()
{
    for (int i = 0; i < m_TriangleArray.size(); i++) {
        m_TriangleArray[i]->Update();
    }
    return true;
}

bool CMesh::Render()
{
    const static unsigned char meshColor[] = { 0x00, 0x7F, 0xFF };
    glBegin(GL_TRIANGLES);
    {
        for (int i = 0; i < m_TriangleArray.size(); i++) {
            glColor3ubv(meshColor);
//            glColor3f(0.5f, 0.5f, 0.5f);
            m_TriangleArray[i]->Render();
        }
    }
    glEnd();
    return true;
}

bool CMesh::Destroy()
{
    for (int i = 0; i < m_TriangleArray.size(); i++) {
        if (m_TriangleArray[i] != NULL) {
            m_TriangleArray[i]->Destroy();
            delete m_TriangleArray[i];
        }
    }
    return true;
}

void CMesh::DebugOutput()
{
//     for (int i = 0; i < 10/*m_TriangleArray.size()*/; i++) {
//         m_TriangleArray[i]->DebugOutput();
//     }
#ifdef __TEST_INTERSECT__
    TestOut();
    TestEdge();
    TestPoint();
    TestInside();
#endif ///> end of __TEST_INTERSECT__
}

bool CMesh::InteractWith(void* arg)
{
    return true;
}

void CMesh::OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam)
{

}
