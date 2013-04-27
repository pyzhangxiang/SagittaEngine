#ifndef __DEMOTEXTURE_H__
#define __DEMOTEXTURE_H__

#include "../DemoCore/sgDemo.h"

class DemoTexture : public sgDemo
{
public:
	DemoTexture();
	~DemoTexture();

protected:
    virtual void prepare(void);
};

#endif // __DEMOTEXTURE_H__

