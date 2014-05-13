#include "SkeletonRig.h"

#include "Skeleton.h"
#include "Joint.h"

SkeletonRig::SkeletonRig()
{
}

SkeletonRig::~SkeletonRig()
{

}

void SkeletonRig::setSkeleton(Skeleton *skel)
{
    _skel = skel;
    QList<Joint*> const& joints = _skel->joints();
    QList<DOF*> DOFList;
    foreach(Joint* joint, joints) {
        foreach(DOF* DOF, joint->DOFs()) {
            DOFList << DOF;
        }
    }
    setDOFs(DOFList.toVector());
}
