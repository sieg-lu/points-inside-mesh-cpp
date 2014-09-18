#ifndef __C_POINT_H__
#define __C_POINT_H__

#include "../Common/Global.h"

/*
 * Class Point
 *   pass the buffer (e.g "0.52 6.3 9.41") to Initialize function to build a point
 */
class CPoint {
public:
    CPoint();
    CPoint(double X, double Y, double Z);

    virtual bool                    Initialize(void* arg);
    virtual bool                    Update();
    virtual bool                    Destroy();
    virtual bool                    Render();

    virtual void                    DebugOutput();

    Vector3D                        GetPoint() const                               { return m_Point; }
    void                            SetIsInside(bool val)                          { m_IsInside = val; }
    bool                            GetIsInside() const                            { return m_IsInside; }
    bool                            GetIsSelected() const                          { return m_IsSelected; }
    void                            SetIsSelected(bool val)                        { m_IsSelected = val; }
    int                             GetId() const                                  { return m_Id; }
    void                            SetId(int val)                                 { m_Id = val; }
    Vector3D                        GetClosestPoint() const                        { return m_ClosestPoint; }
    void                            SetClosestPoint(Vector3D val)                  { m_ClosestPoint = val; m_DistanceToClosestPoint = Length(m_Point - m_ClosestPoint); }
    double                          GetDistanceToClosestPoint() const              { return m_DistanceToClosestPoint; }

    void                            CreateSphere()                                 { m_Sphere = gluNewQuadric(); }

private:
    int                             m_Id;
    Vector3D                        m_Point;
    bool                            m_IsInside;
    bool                            m_IsSelected;
    GLUquadricObj*                  m_Sphere;

    Vector3D                        m_ClosestPoint;
    double                          m_DistanceToClosestPoint;
};

class CPointManager : public IObject {
private:
    vector<CPoint*>                 m_PointArray;
    string                          m_Error;

public:
    virtual ~CPointManager() { }

    virtual bool                    Initialize(void* arg);
    virtual bool                    Update();
    virtual bool                    Destroy();
    virtual bool                    Render();

    virtual bool                    InteractWith(void* arg);
    virtual void                    DebugOutput();

    virtual void                    OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam);

    string                          GetError() const                            { return m_Error; }

};

#endif ///> end of __C_POINT_H__