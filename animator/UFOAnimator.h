#ifndef UFOANIMATOR_H
#define UFOANIMATOR_H
#include "animatorBase.h"

class UFOAnimator: public AnimatorBase
{
public:
    UFOAnimator() = default;
    void Animate(float deltaTime, Drawable &drawable) override;
};

#endif //UFOANIMATOR_H
