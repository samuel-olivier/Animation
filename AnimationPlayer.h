#ifndef ANIMATIONPLAYER_H
#define ANIMATIONPLAYER_H

#include "Node.h"

class Rig;
class AnimationClip;
class Channel;

class AnimationPlayer : public Node
{
public:
    AnimationPlayer(Node* parent);
    virtual ~AnimationPlayer();

    virtual void    update(float elapsed);

    void    setRig(Rig* rig);
    void    setAnimation(AnimationClip* anim);

    Channel*    channel(DOF* dof) const;

    void    saveToFile(QString const& filename);

private:
    Rig*            _rig;
    AnimationClip*  _anim;
    float           _elapsed;
};

#endif // ANIMATIONPLAYER_H
