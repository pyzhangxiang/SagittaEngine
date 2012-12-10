
#include "sgRigidBodyComponent.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/scenegraph/sgScene.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/scenegraph/sgDynamicsWorld.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace Sagitta
{
	SG_META_DEFINE(sgRigidBodyComponent, sgComponent)

	sgRigidBodyComponent::sgRigidBodyComponent( void )
	: mRigidBody(0), mCurrentDynamicsWorld(ID_NULL)
	{

	}

	sgRigidBodyComponent::~sgRigidBodyComponent( void )
	{
		release();
	}

	void sgRigidBodyComponent::setRigidBody( btRigidBody *body )
	{
		if(mRigidBody == body)
		{
			return ;
		}
		
		_removeFromDynamicsWorld();
		release();
		mRigidBody = body;
		_addToDynamicsWorld();

	}

	void sgRigidBodyComponent::_addToDynamicsWorld( void )
	{
		if(!mRigidBody)
			return ;

		sgDynamicsWorld *dworld = 0;
		sgScene *scene = this->getParent()->getScene();
		if(scene)
		{
			dworld = scene->getDynamicsWorld();
		}

		if(dworld && mCurrentDynamicsWorld == dworld->getId())
			return ;
			
		if(mCurrentDynamicsWorld != ID_NULL)
		{
			_removeFromDynamicsWorld();
		}

		if(dworld)
		{
			dworld->addRigidBody(mRigidBody);
			mCurrentDynamicsWorld = dworld->getId();
		}
		

	}

	void sgRigidBodyComponent::_removeFromDynamicsWorld( void )
	{
		if(!mRigidBody)
			return ;

		sgDynamicsWorld *dworld = dynamic_cast<sgDynamicsWorld*>(sgObject::getObject(mCurrentDynamicsWorld));
		if(!dworld)
		{
			mCurrentDynamicsWorld = ID_NULL;
			return ;
		}

		dworld->removeRigidBody(mRigidBody);
		mCurrentDynamicsWorld = ID_NULL;
	}

	void sgRigidBodyComponent::release( void )
	{
		if(mRigidBody)
		{
			btMotionState *ms = mRigidBody->getMotionState();
			if(ms)
			{
				delete ms;
			}

			btCollisionShape *shape = mRigidBody->getCollisionShape();
			if(shape)
			{
				delete shape;
			}

			delete mRigidBody;
			mRigidBody = 0;
		}
	}
}
