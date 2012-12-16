
#include "sgRagdoll.h"
#include "sgSceneObject.h"
#include "sgBoneObject.h"
#include "engine/component/sgAnimationComponent.h"
#include "engine/resource/sgAnimation.h"
#include "engine/resource/sgAnimationJoint.h"
#include "../resource/sgRagdollConfig.h"
#include "../resource/sgResourceCenter.h"
#include "../resource/sgSimpleMeshes.h"
#include "../component/sgMeshComponent.h"
#include "LinearMath/btMotionState.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include "sgScene.h"
#include "sgDynamicsWorld.h"

namespace Sagitta{
    
	
	SG_META_DEFINE(sgRagdoll, sgObject)
    
	//  [1/1/2009 zhangxiang]
	sgRagdoll::sgRagdoll(void)
	: sgObject(), mParentObject(0), mRagdollConfig(sgStrHandle::EmptyString)
    {
		
	}
    
    
	//  [1/1/2009 zhangxiang]
	sgRagdoll::~sgRagdoll(void)
    {
        
	}

	void sgRagdoll::setParent( sgSceneObject *parent )
	{
		if(mParentObject == parent)
			return ;
		bool sceneChanged = true;
		if(mParentObject && parent)
		{
			if(mParentObject->getScene() == parent->getScene())
			{
				sceneChanged = false;
			}
		}
		
		if(sceneChanged)
			removeFromScene();

		mParentObject = parent;
		resetTransform();

		if(sceneChanged)
			addToScene();
	}

	void sgRagdoll::setRagdollConfig( const sgStrHandle &config )
	{
		if(mRagdollConfig == config)
			return ;

		sgRagdollConfig *conf = (sgRagdollConfig*)sgResourceCenter::instance()->findResource(config);
		if(!conf)
			return ;
		const sgRagdollConfig::BodyInfoMap &bodyset = conf->getBodyInfoSet();

		mRagdollConfig = config;
		
		release();
		
		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassTypeName(), sgMeshCube::InternalFileName);
		// create object 
		sgRagdollConfig::BodyInfoMap::const_iterator it = bodyset.begin();
		for(; it!=bodyset.end(); ++it)
		{
			const sgRagdollConfig::BodyInfo &bi = it->second;
			
			BodyMap::iterator it = mBodyMap.find(bi.bodyName);
			if(it != mBodyMap.end())
			{
				continue;
			}

			// mesh object
			sgSceneObject *obj = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
			obj->setName(bi.bodyName);
			obj->scale(Vector3(bi.width, bi.height, bi.thick));
			obj->setIsDebugObj(true);

			sgMeshComponent *meshComp = (sgMeshComponent*)obj->createComponent(sgMeshComponent::GetClassTypeName());
			meshComp->setMeshFile(meshCube->getFilename());

			// rigidbody
			btCollisionShape* colShape = new btBoxShape(btVector3(bi.width, bi.height, bi.thick));
			
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass = bi.mass;

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0,0,0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass,localInertia);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);


			Body mybody;
			mybody.jointName = bi.jointName;
			mybody.body = body;
			mybody.object = obj;

			mBodyMap.insert(std::make_pair(bi.bodyName, mybody));
		}

		resetTransform();
		addToScene();
	}

	void sgRagdoll::resetTransform( void )
	{
		if(!mParentObject)
		{
			return ;
		}

		sgSkeleton *skeleton = mParentObject->getSkeleton();
		if(!skeleton)
			return ;

		sgRagdollConfig *conf = (sgRagdollConfig*)sgResourceCenter::instance()->findResource(mRagdollConfig);
		if(!conf)
			return ;
		const sgRagdollConfig::BodyInfoMap &bodyset = conf->getBodyInfoSet();

		BodyMap::iterator it = mBodyMap.begin();
		for(; it!=mBodyMap.end(); ++it)
		{
			Body &b = it->second;

			sgRagdollConfig::BodyInfoMap::const_iterator biIt = bodyset.find(b.object->getName());
			if(biIt == bodyset.end())
				continue;

			const sgRagdollConfig::BodyInfo &bi = biIt->second;

			// todo
			
			// constrain

		}
	}

	void sgRagdoll::addToScene( void )
	{
		if(!mParentObject || !(mParentObject->getScene()))
			return ;

		sgScene *scene = mParentObject->getScene();
		sgDynamicsWorld *dynamicsWorld = scene->getDynamicsWorld();

		BodyMap::iterator it = mBodyMap.begin();
		for(; it!=mBodyMap.end(); ++it)
		{
			Body &b = it->second;

			b.object->setParent(scene->getRoot());
			dynamicsWorld->addRigidBody(b.body);
			// constrain

		}
	}

	void sgRagdoll::removeFromScene( void )
	{
		if(!mParentObject || !(mParentObject->getScene()))
			return ;

		sgScene *scene = mParentObject->getScene();
		sgDynamicsWorld *dynamicsWorld = scene->getDynamicsWorld();

		BodyMap::iterator it = mBodyMap.begin();
		for(; it!=mBodyMap.end(); ++it)
		{
			Body &b = it->second;

			b.object->setParent(0);
			dynamicsWorld->removeRigidBody(b.body);
			// constrain
		}
	}

	void sgRagdoll::release( void )
	{
		removeFromScene();

		BodyMap::iterator it = mBodyMap.begin();
		for(; it!=mBodyMap.end(); ++it)
		{
			Body &b = it->second;

			btMotionState *ms = b.body->getMotionState();
			if(ms)
			{
				delete ms;
			}

			btCollisionShape *shape = b.body->getCollisionShape();
			if(shape)
			{
				delete shape;
			}

			delete b.body;
			sgObject::destroyObject(b.object);
		}

		mBodyMap.clear();
	}

	
} // namespace Sagitta
