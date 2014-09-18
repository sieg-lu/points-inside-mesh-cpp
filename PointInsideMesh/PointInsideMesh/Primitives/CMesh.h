#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "../Common/Global.h"
#include "CPoints.h"

// Ray, from position shooting a ray with direction
class CRay {
private:
    Vector3D                        mOrigin;
    Vector3D                        mDirection;

public:
    CRay(const Vector3D& origin, const Vector3D& direction);

    Vector3D                        GetSpecifiedPoint(double distance);
    Vector3D                        GetOrigin() const                            { return mOrigin; }
    Vector3D                        GetDirection() const                         { return mDirection; }

};

// triangle class
//  three points
class CTriangle {
public:
    CTriangle();
    CTriangle(Vector3D a, Vector3D b, Vector3D c);

    virtual bool                    Initialize(void* arg);
    virtual bool                    Update();
    virtual bool                    Destroy();
    virtual bool                    Render();

    virtual void                    DebugOutput();
    /*
     * Intersect:
     *    return:
     *        0 -- no intersection
     *    (disposed) 1 -- intersect at vertex
     *    (disposed) 2 -- intersect at edge
     *        3 -- intersect inside the triangle
     */
    virtual int                     Intersect(CRay ray, Vector3D& interPos);

    Vector3D                        GetPointA() const                            { return m_Points[0]; }
    Vector3D                        GetPointB() const                            { return m_Points[1]; }
    Vector3D                        GetPointC() const                            { return m_Points[2]; }

    double                          CalcClosestToPoint2(CPoint tgtPoint, Vector3D& resPoint);

    friend class CMesh;

private:
    Vector3D                        m_Points[3];
    Vector3D                        m_Normal;

};

// Mesh class
class CMesh : public IObject {
public:
    virtual ~CMesh() { }

    virtual bool                    Initialize(void* arg);
    virtual bool                    Update();
    virtual bool                    Destroy();
    virtual bool                    Render();

    virtual void                    DebugOutput();
    virtual bool                    InteractWith(void* arg);

    virtual void                    OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam);

    string                          GetError() const                             { return m_Error; }
    Vector3D                        GetCenter() const                            { return m_Center; }
    vector<CTriangle*>*             GetTriangleArrayPtr()                        { return &m_TriangleArray; }

private:
    string                           m_Error;
    vector<CTriangle*>               m_TriangleArray;
    Vector3D                         m_Center;
    double                           m_Radius;

};

#endif ///> end of __C_MESH_H__