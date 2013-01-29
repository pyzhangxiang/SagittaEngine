#ifndef __DEMORAGDOLL_H__
#define __DEMORAGDOLL_H__

#include "../DemoCore/sgDemo.h"

class DemoRagdoll : public sgDemo
{
public:
	DemoRagdoll();
	~DemoRagdoll();

protected:
    virtual void prepare(void);

public:
	virtual void keyPressEvent(sgKeyEvent &event);
};

#endif // __DEMOSHADOWMAP_H__

