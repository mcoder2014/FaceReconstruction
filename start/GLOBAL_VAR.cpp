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

///
/// \brief GLOBAL_VAR::GLOBAL_VAR
///
GLOBAL_VAR::GLOBAL_VAR()
{
    this->landmarkdat = QString("./shape_predictor_68_face_landmarks.dat");
    this->morphableModel = QString("./sfm_shape_3448.bin");
    this->mappingsFile = QString("./ibug2did.txt");
}
