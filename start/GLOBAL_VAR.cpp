#include "GLOBAL_VAR.h"

GLOBAL_VAR *GLOBAL_VAR::m_instance = nullptr;

GLOBAL_VAR *GLOBAL_VAR::getInstance()
{
    if(m_instance == nullptr)
    {
        m_instance = new GLOBAL_VAR();
    }

    return m_instance;
}

GLOBAL_VAR::GLOBAL_VAR()
{
    this->landmarkdat = QString("shape_predictor_68_face_landmarks.dat");
}
