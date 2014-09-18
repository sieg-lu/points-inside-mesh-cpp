#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Global.h"

// The camera class
//  taking control of the view point and direction
//  as well as the Mouse Event
class CCamera : public IObject {
public:
    virtual ~CCamera() { }

    virtual bool                    Initialize(void* arg);
    virtual bool                    Update();
    virtual bool                    Destroy();
    virtual bool                    Render();

    virtual bool                    InteractWith(void* arg);
    virtual void                    DebugOutput();
    virtual void                    OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam);

    void                            OnMouseRightDown(int x, int y);
    void                            OnMouseMove(int x, int y);
    void                            OnMouseRightUp(int x, int y);
    void                            OnMouseLeftUp(int x, int y);

private:
    bool                            m_IsPressedC;
    int                             m_PolygonMode;

    double                          m_AngleX;
    double                          m_AngleY;
    double                          m_StoredX;
    double                          m_StoredY;
    double                          m_DistanceAway;

    double                          m_DistanceThreshold;
    Vector3D                        m_Center;
};

#endif ///> end of __C_CAMERA_H__