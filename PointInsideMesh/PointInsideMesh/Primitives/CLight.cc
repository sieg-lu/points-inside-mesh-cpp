#include "CLight.h"

void CLight::InitLight(string vertexFilename, string fragFilename)
{
    m_ShaderMgr.Initialize(vertexFilename, fragFilename);

    m_Cutoff = 30.0;
    GLfloat ambientColor[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat diffuseColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specularColor[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float matShine[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glEnable(GL_COLOR_MATERIAL); 
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void CLight::UpdateLight()
{
    if (m_Direction != Vector3D()) {
        glPushMatrix();
        float lightPos[] = { m_Position.x, m_Position.y, m_Position.z, 1.0 };
        float lightDirection[] = { m_Direction.x, m_Direction.y, m_Direction.z };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, m_Cutoff);
        m_ShaderMgr.TurnShader(true);
        glPopMatrix();
    }
}

void CLight::TurnOffLight()
{
    m_ShaderMgr.TurnShader(false);
    m_Direction = Vector3D();
}

void CLight::SetCutoff(float dist)
{
    m_Cutoff = 150.0 / dist;
}
