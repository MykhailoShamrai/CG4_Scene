#ifndef BINDEDLIGHT_H
#define BINDEDLIGHT_H
#include <memory>

#include "drawable.h"

struct BindLight
{
public:
    virtual ~BindLight();
    virtual void ChangePositionToObject();
    virtual void BindToObject(const std::shared_ptr<Drawable> &ptr);
    virtual void UseInShader(const Shader &shader);
};

#endif //BINDEDLIGHT_H
