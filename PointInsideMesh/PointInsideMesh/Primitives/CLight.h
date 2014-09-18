#ifndef __C_LIGHT_H__
#define __C_LIGHT_H__

#include "../Common/Global.h"
#include "../Utility/ShaderManager.h"

// The light class
//  There's only one light in this program
//  the light changed its position and direction according to mouse event
class CLight {
private:
    ShaderManager                  m_ShaderMgr;
    Vector3D                       m_Position;
    Vector3D                       m_Direction;
    float                          m_Cutoff;

public:
    void                           InitLight(string vertexFilename, string fragFilename);
    void                           UpdateLight();
    void                           TurnOffLight();

    void                           SetCutoff(float dist);

    Vector3D                       GetPosition() const                         { return m_Position; }
    void                           SetPosition(Vector3D val)                   { m_Position = val; }
    // Set the direction not to be (0, 0, 0) to turn off the light
    Vector3D                       GetDirection() const                        { return m_Direction; }
    void                           SetDirection(Vector3D val)                  { m_Direction = val; }
};

#endif ///> end of __C_LIGHT_H__