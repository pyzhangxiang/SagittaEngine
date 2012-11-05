#ifndef __DEMOGLSL_H__
#define __DEMOGLSL_H__

#include "../DemoCore/sgDemo.h"

class DemoGLSL : public sgDemo
{
public:
	DemoGLSL();
	~DemoGLSL();

protected:
    virtual void prepare(void);
};

#endif // __DEMOGLSL_H__
