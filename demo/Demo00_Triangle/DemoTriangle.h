#ifndef __DEMOTRIANGLE_H__
#define __DEMOTRIANGLE_H__

#include "../DemoCore/sgDemo.h"

class DemoTriangle : public sgDemo
{
public:
	DemoTriangle();
	~DemoTriangle();

protected:
    virtual void prepare(void);
};

#endif // __DEMOTRIANGLE_H__