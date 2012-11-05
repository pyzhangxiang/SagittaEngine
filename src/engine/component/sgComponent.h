/// @2012.9.27 by zhangxiang

#ifndef __SGCOMPONENT_H__
#define __SGCOMPONENT_H__


#include "engine/common/sgObject.h"

namespace Sagitta{

	class sgSceneObject;

	class _SG_KernelExport sgComponent : public sgObject
	{
		SG_META_DECLARE(sgComponent)

		friend class sgSceneObject;

	private:
		sgSceneObject *mpParent;
		void setParent(sgSceneObject *obj);

	public:
		sgSceneObject *getParent(void) const;

	public:
		sgComponent(void);
		virtual ~sgComponent(void);
        
        virtual void update(Float32 deltaTime){}

	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGCOMPONENT_H__