#ifndef __DEMOLOADEROBJ_H__
#define __DEMOLOADEROBJ_H__

#include "../DemoCore/sgDemo.h"

class DemoLoaderObj : public sgDemo
{
    
public:
	DemoLoaderObj();
	~DemoLoaderObj();

protected:
    virtual void prepare(void);
};

#endif // __DEMOLOADEROBJ_H__