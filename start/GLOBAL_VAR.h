#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <QString>

class GLOBAL_VAR
{


public:
    GLOBAL_VAR *getInstance();  // 获得实例

    QString getLandmarkDat(){return this->landmarkdat;}

private:
    static GLOBAL_VAR *m_instance;
    GLOBAL_VAR();

    QString landmarkdat;
};

#endif // GLOBAL_VAR_H
