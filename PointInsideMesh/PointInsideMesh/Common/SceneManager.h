#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Global.h"

// The scene manager, taking control of the Objects in the scene
class SceneManager {
private:
    SceneManager();
    ~SceneManager();
    static SceneManager*            m_Instance;
    vector<IObject*>                m_SceneObjects;

public:
    static SceneManager*            GetInstance();

    virtual bool                    Initialize(void* arg);
    virtual bool                    Update();
    virtual bool                    Destroy();
    virtual bool                    Render();

    virtual void                    OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam);
};


#endif ///> end of __SCENE_MANAGER_H__