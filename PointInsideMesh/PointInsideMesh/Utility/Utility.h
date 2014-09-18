#ifndef __UTILITY_H__
#define __UTILITY_H__

/*
 * This file contains some utility functions
 */

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <iostream>
using namespace std;

#include "VMath.h"

GLuint                     LoadGLTexture(const char *filename);
bool                       IsInLegalRange(int num, int low, int high);
int                        LimitRange(int num, int low, int high);

char*                      ReadFileText(char* fn);

bool                       IntersectRays(Vector3D linePoint1, Vector3D lineVec1, Vector3D linePoint2, Vector3D lineVec2, Vector3D& inter);

/*
 * IntersectRaySegment
 *    Test if a ray and a line segment is intersected
 *    return:
 *        0 -- no intersection
 *        1 -- intersect at point
 *        2 -- intersect inside the segment
 */
int                        IntersectRaySegment(Vector3D linePoint1, Vector3D lineVec1, Vector3D linePoint21, Vector3D linePoint22, Vector3D& inter);

struct Vector3D_Cmp {
    bool operator() (const Vector3D& k1, const Vector3D& k2) const;
};

#endif ///> end of __UTILITY_H__