#include "SceneManager.h"

#include "../Primitives/CPoints.h"
#include "../Primitives/CMesh.h"
#include "CCamera.h"

SceneManager* SceneManager::m_Instance = NULL;

bool SceneManager::Initialize(void* arg)
{
    bool flag = true;

    IObject* camera = new CCamera();
    if (!camera->Initialize(NULL)) {
        flag = false;
    }
    m_SceneObjects.push_back(camera);

    IObject* mesh = new CMesh();
    if (!mesh->Initialize(&string("concave_shape_ascii.stl"))) {
        flag = false;
    }
#ifdef __TEST_INTERSECT__
    mesh->DebugOutput();
#endif ///> end of __TEST_INTERSECT__
    m_SceneObjects.push_back(mesh);

    IObject* pointMgr = new CPointManager();
    if (!pointMgr->Initialize(&string("points.txt"))) {
        flag = false;
    }
    m_SceneObjects.push_back(pointMgr);

    pointMgr->InteractWith(mesh);
    camera->InteractWith(mesh);

    if (!flag) {
        this->Destroy();
        return false;
    }

    return true;
}

bool SceneManager::Update()
{
    for (int i = 0; i < m_SceneObjects.size(); i++) {
        m_SceneObjects[i]->Update();
    }
    return true;
}

bool SceneManager::Destroy()
{
    for (int i = 0; i < m_SceneObjects.size(); i++) {
        if (m_SceneObjects[i] != NULL) {
            m_SceneObjects[i]->Destroy();
            delete m_SceneObjects[i];
        }
    }
    return true;
}

void SceneManager::OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam)
{
    for (int i = 0; i < m_SceneObjects.size(); i++) {
        m_SceneObjects[i]->OnMouseEvent(evt, wParam, lParam);
    }
}

bool SceneManager::Render()
{
    for (int i = 0; i < m_SceneObjects.size(); i++) {
        m_SceneObjects[i]->Render();
    }
    return true;
}

SceneManager* SceneManager::GetInstance()
{
    if (m_Instance == NULL) {
        m_Instance = new SceneManager();
    }
    return m_Instance;
}

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
    if (m_Instance != NULL) {
        m_Instance->Destroy();
        delete m_Instance;
    }
}
