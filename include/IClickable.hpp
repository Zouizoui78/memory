#ifndef ICLICKABLE
#define ICLICKABLE

#include <functional>

template <class T>
class IClickable
{
    public:
    virtual void setCallback(std::function<bool (T* clicked)>) = 0;

    virtual bool click() = 0;

    virtual void setClickable(bool clickable) = 0;

    virtual bool isClickable() = 0;
};

#endif // ICLICKABLE