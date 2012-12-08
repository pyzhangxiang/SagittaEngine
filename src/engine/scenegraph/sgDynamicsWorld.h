#ifndef __SGDYNAMICSWORLD_H__
#define __SGDYNAMICSWORLD_H__


#include "engine/common/sgObject.h"
#include "math/sgTypeDef.h"
#include "engine/common/sgStlAllocator.h"
#include <set>

class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btRigidBody;

namespace Sagitta{

	class _SG_KernelExport sgDynamicsWorld : public sgObject
    {
		SG_META_DECLARE(sgDynamicsWorld)

	protected:
		btDynamicsWorld *m_dynamicsWorld;
		btBroadphaseInterface *m_broadphase;
		btCollisionDispatcher *m_dispatcher;
		btConstraintSolver *m_solver;
		btDefaultCollisionConfiguration *m_collisionConfiguration;

		typedef sg_set(btRigidBody*) RigidBodySet;
		RigidBodySet mRigidBodySet;

	public:
		sgDynamicsWorld(void);
		virtual ~sgDynamicsWorld(void);
        
	public:
		btDynamicsWorld *getBulletWorld(void) const{ return m_dynamicsWorld; }
		int	stepSimulation( Float32 timeStep, int maxSubSteps=1, Float32 fixedTimeStep=1.0f/60.0f);

		void addRigidBody(btRigidBody *body);
		void removeRigidBody(btRigidBody *body);
		
	}; 

} // namespace Sagitta


#endif // __SGDYNAMICSWORLD_H__

