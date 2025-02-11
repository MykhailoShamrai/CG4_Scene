
#ifndef ANIMATORBASE_H
#define ANIMATORBASE_H

struct Drawable;

class AnimatorBase
{
public:
    virtual ~AnimatorBase() {};
    virtual void Animate(float deltaTime, Drawable &drawable) {};
};


#endif //ANIMATORBASE_H
