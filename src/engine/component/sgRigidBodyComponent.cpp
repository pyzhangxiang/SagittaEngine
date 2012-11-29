
#include "sgRigidBodyComponent.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/scenegraph/sgScene.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/scenegraph/sgDynamicsWorld.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace Sagitta
{
	SG_META_DEFINE(sgRigidBodyComponent, sgComponent)

	sgRigidBodyComponent::sgRigidBodyComponent( void )
	: mRigidBody(0)
	{

	}

	sgRigidBodyComponent::~sgRigidBodyComponent( void )
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
		}
	}

	void sgRigidBodyComponent::setRigidBody( btRigidBody *body )
	{
		if(mRigidBody == body)
		{
			return ;
		}
		
		_removeFromSimWorld();
		mRigidBody = body;
		_addToSimWorld();

	}

	void sgRigidBodyComponent::_addToSimWorld( void )
	{
		sgScene *scene = this->getParent()->getScene();
		if(scene)
		{
			scene->getDynamicsWorld()->addRigidBody(mRigidBody);
		}
	}

	void sgRigidBodyComponent::_removeFromSimWorld( void )
	{
		sgScene *scene = this->getParent()->getScene();
		if(scene)
		{
			scene->getDynamicsWorld()->removeRigidBody(mRigidBody);
		}
	}
}
