#include "AnimationPlayer.h"

#include "AnimationClip.h"
#include "Rig.h"

AnimationPlayer::AnimationPlayer(Node *parent) : Node(parent)
{
    _rig = NULL;
    _anim = NULL;
    setType("Animation");
    setName("Animation");
    DOF* speed = new DOF(DOF::AnimSpeed, true);
    speed->setMin(-2);
    speed->setMax(2);
    speed->setValue(1);
    setDOF(DOF::AnimSpeed, speed);
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::update(float elapsed)
{
    if (!_anim || !_rig)
        return ;
    DOF* speed = getDOF(DOF::AnimSpeed);
    _elapsed += speed ? (elapsed * speed->value()) : elapsed;
    Pose newPose;
    _anim->evaluate(_elapsed, newPose);
    _rig->applyPose(newPose);
    Node::update(elapsed);
}


void AnimationPlayer::setRig(Rig *rig)
{
    _rig = rig;
}

void AnimationPlayer::setAnimation(AnimationClip *anim)
{
    _anim = anim;
}

Channel *AnimationPlayer::channel(DOF *dof) const
{
    if (!_rig || !_anim)
        return NULL;
    int idx = _rig->index(dof);
    if (idx == -1)
        return NULL;
    return _anim->channel(idx);
}

void AnimationPlayer::saveToFile(const QString &filename)
{
    if (_anim)
        _anim->saveToFile(filename);
}
