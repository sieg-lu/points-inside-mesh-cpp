#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "../Common/Global.h"

class ShaderManager {
private:
    GLuint                    m_Program;
    GLuint                    m_VertexProgram;
    GLuint                    m_FragmentProgram;

public:
    bool                      Initialize(string vertexFilename, string fragmentFilename);
    void                      TurnShader(bool isOn);

};

#endif ///> end of __SHADER_MANAGER_H__