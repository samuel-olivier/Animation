#include "Rig.h"

#include "DOF.h"

Rig::Rig()
{
}

Rig::~Rig()
{

}

void Rig::applyPose(const Pose &pose)
{
    for (int i = 0; i < pose.size() && i < _DOFs.size(); ++i) {
        DOF* current = _DOFs[i];
        if (current)
            current->setValue(pose[i]);
    }
}

const QVector<DOF *> &Rig::DOFs() const
{
    return _DOFs;
}

void Rig::setDOFs(const QVector<DOF *> &DOFs)
{
    _DOFs = DOFs;
}

int Rig::index(DOF *dof) const
{
    return _DOFs.indexOf(dof);
}
