#include <map>
#include <fstream>
#include <sstream>

#include "CMesh.h"
#include "CPoints.h"
#include "../Utility/Utility.h"
#include "CLight.h"

extern int g_SelectedIndex;
extern CLight g_Light;
// extern ShaderManager g_ShaderMgr;
// extern Vector3D g_LightPostion;
// extern Vector3D g_LightDirection;

/////////////////// Point ///////////////////
CPoint::CPoint()
{

}

CPoint::CPoint(double X, double Y, double Z)
    : m_Point(X, Y, Z)
{

}

bool CPoint::Initialize(void* arg)
{
    if (arg == NULL) {
        return true;
    }
    m_IsInside = false;
    m_IsSelected = false;
    string* input_arg = static_cast<string*>(arg);
    istringstream iss(input_arg[0]);
    iss >> m_Point.x >> m_Point.y >> m_Point.z;

    return true;
}

bool CPoint::Update()
{
    return true;
}

bool CPoint::Destroy()
{
    return true;
}

bool CPoint::Render()
{
    glLoadName(m_Id + 1);
    if (m_IsInside) {
        glColor3f(0.1f, 1.0f, 0.3f);
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPolygonMode(GL_FRONT, GL_FILL);
        glTranslatef(m_Point.x, m_Point.y, m_Point.z);
        gluSphere(m_Sphere, 0.5, 16, 16);
        if (m_Id + 1 == g_SelectedIndex) {
            g_Light.SetDirection(Normalize(m_ClosestPoint - m_Point));
            g_Light.SetPosition(m_Point - g_Light.GetDirection());
            g_Light.SetCutoff(m_DistanceToClosestPoint);
            glColor3f(1.0f, 1.0f, 1.0f);
            auxWireCube(1.2);
            glLineWidth(2.5);
//            glTranslatef(-m_Point.x, -m_Point.y, -m_Point.z);
            glPopMatrix();
            glPushMatrix();
            glBegin(GL_LINES);
            {
                glVertex3d(m_Point.x, m_Point.y, m_Point.z);
                glVertex3d(m_ClosestPoint.x, m_ClosestPoint.y, m_ClosestPoint.z);
            }
            glEnd();
            glLineWidth(1.0);
        }
        glPopAttrib();
        glPopMatrix();
    } else {
        glColor3f(1.0f, 1.0f, 0.1f);
        glPointSize(2.5f);
        glBegin(GL_POINTS);
        {
            glVertex3dv(m_Point.num);
        }
        glEnd();
        glPointSize(1.0f);
        const static unsigned char meshColor[] = { 0x00, 0x7F, 0xFF };
        glColor3ubv(meshColor);
    }
    return true;
}

void CPoint::DebugOutput()
{
    cout << m_Point.x << ' ' << m_Point.y << ' ' << m_Point.z << endl;
}

/////////////////// PointManager ///////////////////
bool CPointManager::Initialize(void* arg)
{
    if (arg == NULL) {
        return false;
    }
    string* input_arg = static_cast<string*>(arg);
    m_Error = "";
    m_PointArray.clear();

    ifstream fin(input_arg[0]);
    if (!fin) {
        m_Error = "cannot open the file: " + input_arg[0];
        return false;
    }
    string buffer;
    while (getline(fin, buffer)) {
        CPoint* tmp = new CPoint();
        tmp->Initialize(&buffer);
        tmp->SetId(m_PointArray.size());
        m_PointArray.push_back(tmp);
    }
    fin.close();
    return true;
}

bool CPointManager::Update()
{
    for (int i = 0; i < m_PointArray.size(); i++) {
        m_PointArray[i]->Update();
    }
    return true;
}

bool CPointManager::Destroy()
{
    for (int i = 0; i < m_PointArray.size(); i++) {
        if (m_PointArray[i] != NULL) {
            m_PointArray[i]->Destroy();
            delete m_PointArray[i];
        }
    }
    return true;
}

bool CPointManager::Render()
{
//    glColor3f(1.0f, 0.1f, 0.1f);
    for (int i = 0; i < m_PointArray.size(); i++) {
        m_PointArray[i]->Render();
    }
    return true;
}

bool CPointManager::InteractWith(void* arg)
{
    CMesh* input_arg = static_cast<CMesh*>(arg);
    vector<CTriangle*>* tris = input_arg->GetTriangleArrayPtr();

    int insideCount = 0;
    for (int i = 0; i < m_PointArray.size(); i++) {
        if (i % 10 == 0) {
            cout << i * 100 / m_PointArray.size() << "%\n";
        }

        CRay ray(m_PointArray[i]->GetPoint(), Vector3D(1, 1, 1));
        int interCount = 0;
        int noCount = 0;
        map<Vector3D, bool, Vector3D_Cmp> mm;
        Vector3D interPos;
        for (vector<CTriangle*>::iterator it = tris->begin(); it != tris->end(); it++) {
            int interRes = (*it)->Intersect(ray, interPos);
            if (interRes != 0) {
                if (!mm[interPos]) {
                    mm[interPos] = true;
                    interCount++;
                } else {
                    noCount++;
                }
            }
        }

//        cout << interCount << ' ' << noCount << endl;
        if ((interCount & 1) == 1) {
            m_PointArray[i]->SetIsInside(true);
            m_PointArray[i]->CreateSphere();
            insideCount++;

            double dist2 = DBL_MAX, tmpDist2;
            Vector3D closestPoint, tmpClosestPoint;
            for (vector<CTriangle*>::iterator it = tris->begin(); it != tris->end(); it++) {
                tmpDist2 = (*it)->CalcClosestToPoint2(*(m_PointArray[i]), tmpClosestPoint);
                if (DoubleCompare(tmpDist2, dist2) < 0) {
                    dist2 = tmpDist2;
                    closestPoint = tmpClosestPoint;
                }
            }
//            cout << i << ", Dist:" << sqrt(dist2) << ", (" << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << ")\n";
            m_PointArray[i]->SetClosestPoint(closestPoint);
        }

    }
    cout << "100%\n";
    cout << insideCount << " points are inside the mesh (Total: " << m_PointArray.size() << ")\n";

    return true;
}

void CPointManager::DebugOutput()
{
    for (int i = 0; i < m_PointArray.size(); i++) {
        m_PointArray[i]->DebugOutput();
    }
}

void CPointManager::OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam)
{

}
