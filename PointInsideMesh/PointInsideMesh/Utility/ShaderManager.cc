#include "ShaderManager.h"
#include <gl/glut.h>
#include "Utility.h"

bool ShaderManager::Initialize(string vertexFilename, string fragmentFilename)
{
    char *vs = NULL, *fs = NULL;

    m_VertexProgram = glCreateShader(GL_VERTEX_SHADER);
    m_FragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);

    vs = ReadFileText("spotlight.vert");
    fs = ReadFileText("spotlight.frag");
    const char* vv = vs;
    const char* ff = fs;

    glShaderSource(m_VertexProgram, 1, &vv, NULL);
    glShaderSource(m_FragmentProgram, 1, &ff, NULL);

    free(vs);
    free(fs);

    glCompileShader(m_VertexProgram);
    glCompileShader(m_FragmentProgram);

    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertexProgram);
    glAttachShader(m_Program, m_FragmentProgram);

    glLinkProgram(m_Program);

    return true;
}

void ShaderManager::TurnShader(bool isOn)
{
    if (isOn) {
        glUseProgram(m_Program);
    } else {
        glUseProgram(NULL);
    }
}

