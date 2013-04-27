#ifndef __DEMOSHADOWMAP_H__
#define __DEMOSHADOWMAP_H__

#include "../DemoCore/sgDemo.h"

class DemoShadowMap : public sgDemo
{
public:
	DemoShadowMap();
	~DemoShadowMap();

protected:
    virtual void prepare(void);
};

#endif // __DEMOSHADOWMAP_H__

