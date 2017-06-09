
#ifndef __OEFUNCTIONTASK_H__
#define __OEFUNCTIONTASK_H__

#include <functional>

#include "OE/EasyPool/Task.h"

class OEFunctionTask :
    public OETask
{
private:

    typedef std::function<int(void)> PvFunc;

public:
    OEFunctionTask(std::function<int(void)> f) : pf_(f) {}
    OEFunctionTask(void) :pf_ (nullptr) {}

    virtual ~OEFunctionTask(void) {}

    template <typename F, typename... Args>
    void asynBind(F(*f)(Args...), Args... args) {
        pf_ = std::bind(f, args...);
    }


    virtual int doWork(void) {
        if (pf_ == nullptr)
            return 86;
        return pf_();
    };

private:
    PvFunc pf_;

};

#endif // __OEFUNCTIONTASK_H__
