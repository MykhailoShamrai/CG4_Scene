#ifndef BINDEDLIGHT_H
#define BINDEDLIGHT_H

struct BindedLight
{
public:
    virtual ~BindedLight() = default;
    virtual void ChangePositionToObject();
};

#endif //BINDEDLIGHT_H
