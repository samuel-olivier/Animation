#ifndef SKELETONRIG_H
#define SKELETONRIG_H

#include "Rig.h"

class Skeleton;

class SkeletonRig : public Rig
{
public:
    SkeletonRig();
    virtual ~SkeletonRig();

    void    setSkeleton(Skeleton* skel);

private:
    Skeleton*   _skel;
};

#endif // SKELETONRIG_H
