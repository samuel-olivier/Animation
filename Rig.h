#ifndef RIG_H
#define RIG_H

#include <QVector>

#include "Pose.h"

class DOF;

class Rig
{
public:
    Rig();
    virtual ~Rig();

    void    applyPose(Pose const& pose);

    QVector<DOF*> const&    DOFs() const;
    void                    setDOFs(QVector<DOF*> const& DOFs);
    int                     index(DOF* dof) const;

private:
    QVector<DOF*>   _DOFs;
};

#endif // RIG_H
