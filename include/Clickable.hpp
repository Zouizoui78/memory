#ifndef CLICKABLE
#define CLICKABLE

#include "IClickable.hpp"
#include "Logger.hpp"

template<class T>
class Clickable : public IClickable<T>
{
    public:

    Clickable() {}
    virtual ~Clickable() {}

    virtual void setCallback(std::function<bool (T* clicked)> callback) override {
        _callback = callback;
        this->setClickable(true);
    }

    virtual bool click() override {
        if(_callback && this->isClickable())
            return _callback((T*)this);
        return true;
    }

    virtual void setClickable(bool clickable) override {
        _clickable = clickable;
    }

    virtual bool isClickable() override {
        return _clickable && _callback;
    }

    protected:
    bool _clickable = false;

    private:
    std::function<bool (T*)> _callback;
};

#endif // CLICKABLE