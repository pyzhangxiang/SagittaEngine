
#include "sgDynamicsWorld.h"
#include <btBulletDynamicsCommon.h>

namespace Sagitta{

	SG_META_DEFINE(sgDynamicsWorld, sgObject)

	sgDynamicsWorld::sgDynamicsWorld( void )
	{
		///collision configuration contains default setup for memory, collision setup
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		//m_collisionConfiguration->setConvexConvexMultipointIterations();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

		m_broadphase = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
		m_solver = sol;

		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);

		m_dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	}

	sgDynamicsWorld::~sgDynamicsWorld( void )
	{
		delete m_dynamicsWorld;

		delete m_solver;

		delete m_broadphase;

		delete m_dispatcher;

		delete m_collisionConfiguration;
	}

	void sgDynamicsWorld::addRigidBody( btRigidBody *body )
	{
		if(!body)
			return ;

		RigidBodySet::iterator it = mRigidBodySet.find(body);
		if(it != mRigidBodySet.end())
			return ;

		mRigidBodySet.insert(body);
		m_dynamicsWorld->addRigidBody(body);
	}

	void sgDynamicsWorld::removeRigidBody( btRigidBody *body )
	{
		if(!body)
			return ;

		RigidBodySet::iterator it = mRigidBodySet.find(body);
		if(it == mRigidBodySet.end())
			return ;

		mRigidBodySet.erase(body);
		m_dynamicsWorld->removeRigidBody(body);
	}

	int sgDynamicsWorld::stepSimulation( Float32 timeStep, int maxSubSteps/*=1*/, Float32 fixedTimeStep/*=1.0f/60.0f*/ )
	{
		return m_dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
	}

} // namespace Sagitta
