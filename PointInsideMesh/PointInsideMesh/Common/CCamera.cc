#include "CCamera.h"
#include "../Primitives/CMesh.h"
#include <cmath>
#include "../Utility/Utility.h"
#include "SceneManager.h"
#include "../Primitives/CLight.h"

// param to change between radians and angles
const static double C = M_PI / 180.0;
extern int g_SelectedIndex;
extern CLight g_Light;
// extern ShaderManager g_ShaderMgr;
// extern Vector3D g_LightPostion;
// extern Vector3D g_LightDirection;

bool CCamera::Initialize(void* arg)
{
    m_PolygonMode = GL_FILL;
    m_StoredX = -1;
    m_StoredY = -1;
//    m_DistanceAway = 150;
    m_AngleX = 0;
    m_AngleY = 0;
    return true;
}

bool CCamera::Update()
{
    double radianX = m_AngleX * M_PI / 180.0;
    double radianY = m_AngleY * M_PI / 180.0;

    double btm = m_DistanceAway * cos(radianX);
    double vpY = m_DistanceAway * sin(radianX);
    double vpX = btm * sin(radianY);
    double vpZ = btm * cos(radianY);

    if (fabs(m_AngleX) < 90.0) {
        gluLookAt(vpX, vpY, vpZ, m_Center.x, m_Center.y, m_Center.z, 0, 1, 0);
    } else {
        if (fabs(m_AngleX) >= 270.0) {
            if (fabs(m_AngleX) >= 360.0) {
                m_AngleX = 0.0;
            }
            gluLookAt(vpX, vpY, vpZ, m_Center.x, m_Center.y, m_Center.z, 0, 1, 0);
        } else {
            gluLookAt(vpX, vpY, vpZ, m_Center.x, m_Center.y, m_Center.z, 0, -1, 0);
        }
    }
    if (fabs(m_AngleY) >= 360.0) {
        m_AngleY = 0.0;
    }
//     gluLookAt(
//         m_DistanceAway * cos(C * m_RotateAngle), m_Height, m_DistanceAway * sin(C * m_RotateAngle), 
//         m_Center.x, m_Center.y, m_Center.z, 
//         0, 1, 0);

    g_Light.UpdateLight();

    if (g_bKeys['C'] && !m_IsPressedC) {
        m_PolygonMode = (m_PolygonMode == GL_FILL ? GL_LINE : GL_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, m_PolygonMode);
        m_IsPressedC = true;
    } else if (!g_bKeys['C']) {
        m_IsPressedC = false;
    }

    return true;
}

bool CCamera::Destroy()
{
    return true;
}

void CCamera::DebugOutput()
{

}

bool CCamera::Render()
{
    return true;
}

bool CCamera::InteractWith(void* arg)
{
    CMesh* mesh = static_cast<CMesh*>(arg);
    m_Center = mesh->GetCenter();
    vector<CTriangle*>* tris = mesh->GetTriangleArrayPtr();
    double sqrDist = 0.0, tmp;
    for (vector<CTriangle*>::iterator it = tris->begin(); it != tris->end(); it++) {
        tmp = Length2((*it)->GetPointA() - m_Center);
        if (tmp > sqrDist) {
            sqrDist = tmp;
        }
        tmp = Length2((*it)->GetPointB() - m_Center);
        if (tmp > sqrDist) {
            sqrDist = tmp;
        }
        tmp = Length2((*it)->GetPointC() - m_Center);
        if (tmp > sqrDist) {
            sqrDist = tmp;
        }
    }
    m_DistanceThreshold = sqrt(sqrDist) * 2.0;
    m_DistanceAway = m_DistanceThreshold / 2.0;
    cout << m_DistanceThreshold << endl;
    return true;
}

void CCamera::OnMouseRightDown(int x, int y)
{
    m_StoredX = x;
    m_StoredY = y;
}

void CCamera::OnMouseMove(int x, int y)
{
    if (m_StoredX == -1 && m_StoredY == -1) {
        return;
    }
    static double xSpeed = 1;
    static double ySpeed = 1;
    
    if (x > m_StoredX) {
        m_AngleY -= ySpeed;
    }
    if (x < m_StoredX) {
        m_AngleY += ySpeed;
    }

    if (y > m_StoredY) {
        m_AngleX += xSpeed;
    }
    if (y < m_StoredY) {
        m_AngleX -= xSpeed;
    }

    m_StoredX = x;
    m_StoredY = y;
}

void CCamera::OnMouseRightUp(int x, int y)
{
    m_StoredX = -1;
    m_StoredY = -1;
}

void CCamera::OnMouseLeftUp(int x, int y)
{
    GLuint buffer[512];
    GLint hits;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(512, buffer);

    (void)glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0f, 5.0f, viewport);

    gluPerspective(60.0f, (GLfloat)(viewport[2]-viewport[0])/(GLfloat)(viewport[3]-viewport[1]), 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    SceneManager::GetInstance()->Render();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    hits = glRenderMode(GL_RENDER);
//    cout << hits << endl;

    if (hits > 0) {
        int choose = buffer[3];
        int depth = buffer[1];
        for (int i = 0; i < hits; i++) {
            if (buffer[i * 4 + 1] < GLuint(depth)) {
                choose = buffer[i * 4 + 3];
                depth = buffer[i * 4 + 1];
            }
        }
        g_SelectedIndex = choose;
    } else {
        g_SelectedIndex = -1;
        g_Light.TurnOffLight();
//        g_LightDirection = Vector3D();
    }
//    cout << g_SelectedIndex << endl;
}

void CCamera::OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam)
{
    switch (evt)
    {
    case eLeftButtonUp:
        {
            OnMouseLeftUp((int)LOWORD(lParam), (int)HIWORD(lParam));
        } break;
    case eRightButtonDown:
        {
            OnMouseRightDown((int)LOWORD(lParam), (int)HIWORD(lParam));
        } break;
    case eRightButtonUp:
        {
            OnMouseRightUp((int)LOWORD(lParam), (int)HIWORD(lParam));
        } break;
    case eMouseMove:
        {
            OnMouseMove((int)LOWORD(lParam), (int)HIWORD(lParam));
        } break;
    case eMouseWheelScrollUp:
        {
            m_DistanceAway -= 5.0;
            if (m_DistanceAway < 0.0) {
                m_DistanceAway = 0.0;
            }
        } break;
    case eMouseWheelScrollDown:
        {
            m_DistanceAway += 5.0;
            if (m_DistanceAway > m_DistanceThreshold) {
                m_DistanceAway = m_DistanceThreshold;
            }
        } break;
    }
}
