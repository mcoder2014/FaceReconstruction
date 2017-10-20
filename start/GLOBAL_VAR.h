#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <QString>

class GLOBAL_VAR
{


public:
    static GLOBAL_VAR *getInstance();  // 获得实例

    QString getLandmarkDat(){return this->landmarkdat;}
    QString getMorphableModel(){return this->morphableModel;}
    QString getMappingsFile(){return this->mappingsFile;}

private:
    static GLOBAL_VAR *m_instance;
    GLOBAL_VAR();

    QString landmarkdat;
    QString morphableModel;
    QString mappingsFile;
};

#endif // GLOBAL_VAR_H
