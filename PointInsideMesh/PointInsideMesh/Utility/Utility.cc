#include <cmath>
#include "VMath.h"
#include "Utility.h"

GLuint LoadGLTexture(const char *filename)
{
    GLuint texid;
    HBITMAP hBMP;
    BITMAP BMP;
    glGenTextures(1, &texid);
    hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    if (!hBMP) {
        return false;
    }
    GetObject(hBMP, sizeof(BMP), &BMP);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
    DeleteObject(hBMP);

    return texid;
}

bool IsInLegalRange(int num, int low, int high)
{
    return (num >= low && num < high);
}

bool IntersectRays(Vector3D linePoint1, Vector3D lineVec1, Vector3D linePoint2, Vector3D lineVec2, Vector3D& inter)
{
    Vector3D lineVec3 = linePoint2 - linePoint1;
    Vector3D crossVec12 = Cross(lineVec1, lineVec2);
    Vector3D crossVec32 = Cross(lineVec3, lineVec2);

    double planarFactor = Dot(lineVec3, crossVec12);
    if (!DoubleEquals(planarFactor, 0.0)) {
        return false;
    }
    double s = Dot(crossVec32, crossVec12) / Length2(crossVec12);
    if (s >= 0.0/* && s <= 1.0*/) {
        inter = linePoint1 + lineVec1 * s;
        return true;
    }
    return false;
}

int IntersectRaySegment(Vector3D linePoint1, Vector3D lineVec1, Vector3D linePoint21, Vector3D linePoint22, Vector3D& inter)
{
    if (IntersectRays(linePoint1, lineVec1, linePoint21, linePoint22 - linePoint21, inter)) {
        if (inter == linePoint21 || inter == linePoint22) {
            return 2; // intersect at point
        }
        if ((DoubleCompare(inter.x, min(linePoint21.x, linePoint22.x)) >= 0 && DoubleCompare(inter.x, max(linePoint21.x, linePoint22.x)) <= 0) && 
            (DoubleCompare(inter.y, min(linePoint21.y, linePoint22.y)) >= 0 && DoubleCompare(inter.y, max(linePoint21.y, linePoint22.y)) <= 0) &&
            (DoubleCompare(inter.z, min(linePoint21.z, linePoint22.z)) >= 0 && DoubleCompare(inter.z, max(linePoint21.z, linePoint22.z)) <= 0)) {
                return 1; // intersect
        }
    }
    return 0;
}

int LimitRange(int num, int low, int high)
{
    if (num > high) {
        return high;
    } else if (num < low) {
        return low;
    }
    return num;
}

char* ReadFileText(char *fn)
{
    FILE* fp;
    char* content = NULL;
    int count = 0;
    if (fn != NULL) {
        fp = fopen(fn,"rt");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            if (count > 0) {
                content = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(content,sizeof(char), count, fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

bool Vector3D_Cmp::operator()(const Vector3D& k1, const Vector3D& k2) const
{
    if (!DoubleEquals(k1.x, k2.x)) {
        return DoubleCompare(k1.x, k2.x) < 0;
    }
    if (!DoubleEquals(k1.y, k2.y)) {
        return DoubleCompare(k1.y, k2.y) < 0;
    }
    if (!DoubleEquals(k1.z, k2.z)) {
        return DoubleCompare(k1.z, k2.z) < 0;
    }
    return false;
}
