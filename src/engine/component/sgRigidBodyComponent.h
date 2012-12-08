/// @2012.11.27 by zhangxiang

#ifndef __SGMESHCOMPONENT_H__
#define __SGMESHCOMPONENT_H__


#include "sgComponent.h"
#include "engine/common/sgStrHandle.h"

class btRigidBody;

namespace Sagitta{

	class sgMesh;

	/// is btRigidBody, btCollisionShape, btMotionState' owner
	class _SG_KernelExport sgRigidBodyComponent : public sgComponent
	{
		SG_META_DECLARE(sgRigidBodyComponent)

	protected:
		btRigidBody *mRigidBody;
		id_type mCurrentDynamicsWorld;

	public:
		sgRigidBodyComponent(void);
		virtual ~sgRigidBodyComponent(void);

		btRigidBody *getRigidBody(void) const{ return mRigidBody; }
		void setRigidBody(btRigidBody *body);

		/// callback
		void _addToDynamicsWorld(void);
		void _removeFromDynamicsWorld(void);

	private:
		void release(void);
	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGMESHCOMPONENT_H__
