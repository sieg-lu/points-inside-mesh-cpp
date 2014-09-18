#include "CMesh.h"
#include "IntersectTest.h"

void TestOut()
{
    Vector3D interPos;
    CTriangle tri(
        Vector3D(-1, 0, -3),
        Vector3D(2, 0, -3),
        Vector3D(0, 3, -3));
    CRay* rays[6];
    rays[0] = new CRay(Vector3D(), Vector3D(-1, 2, -3));
    rays[1] = new CRay(Vector3D(), Vector3D(0, 4, -3));
    rays[2] = new CRay(Vector3D(), Vector3D(1.5, 2, -3));
    rays[3] = new CRay(Vector3D(), Vector3D(2.5, -0.5, -3));
    rays[4] = new CRay(Vector3D(), Vector3D(0.5, -1, -3));
    rays[5] = new CRay(Vector3D(), Vector3D(-1.5, -0.5, -3));
    for (int i = 0; i < 6; i++) {
        cout << tri.Intersect(*rays[i], interPos) << endl;
        delete rays[i];
    }
    cout << endl;
}

void TestEdge()
{
    Vector3D interPos;
    CTriangle tri(
        Vector3D(-1, 0, -3),
        Vector3D(2, 0, -3),
        Vector3D(0, 3, -3));
    CRay* rays[3];
    rays[0] = new CRay(Vector3D(), Vector3D(-0.5, 1.5, -3));
    rays[1] = new CRay(Vector3D(), Vector3D(1, 1.5, -3));
    rays[2] = new CRay(Vector3D(), Vector3D(0.5, 0, -3));
    for (int i = 0; i < 3; i++) {
        cout << tri.Intersect(*rays[i], interPos) << endl;
        delete rays[i];
    }
    cout << endl;
}

void TestPoint()
{
    Vector3D interPos;
    CTriangle tri(
        Vector3D(-1, 0, -3),
        Vector3D(2, 0, -3),
        Vector3D(0, 3, -3));
    CRay* rays[3];
    rays[0] = new CRay(Vector3D(), Vector3D(-1, 0, -3));
    rays[1] = new CRay(Vector3D(), Vector3D(2, 0, -3));
    rays[2] = new CRay(Vector3D(), Vector3D(0, 3, -3));
    for (int i = 0; i < 3; i++) {
        cout << tri.Intersect(*rays[i], interPos) << endl;
        delete rays[i];
    }
    cout << endl;
}

void TestInside()
{
    Vector3D interPos;
    CTriangle tri(
        Vector3D(-1, 0, -3),
        Vector3D(2, 0, -3),
        Vector3D(0, 3, -3));

    cout << tri.Intersect(CRay(Vector3D(), Vector3D(0.25, 1. -3)), interPos) << endl << endl;
}
