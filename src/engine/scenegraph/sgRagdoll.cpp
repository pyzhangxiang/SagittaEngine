
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
#include "sgSkeleton.h"
#include "sgNode.h"

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
        release();
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
			btCollisionShape* colShape = new btBoxShape(btVector3(bi.width*0.5f, bi.height*0.5f, bi.thick*0.5f));
			
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
			mBodyMapByJointName.insert(std::make_pair(bi.jointName, mybody));
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

			sgBoneObject *boneNode = skeleton->getBoneNode(bi.jointName);
			if(!boneNode)
				continue;

			btTransform bodyTrans = b.body->getWorldTransform();

			if(bi.posOnJoint)
			{
				const Vector3 &pos = boneNode->absolutePosition();
				b.object->setAbsolutePosition(pos);
				bodyTrans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
				
				
			}
			else
			{
				sgBoneObject *boneNode2 = 0;
				if( ! (bi.joint2Name.empty()) )
					boneNode2 = skeleton->getBoneNode(bi.joint2Name);
				if(!boneNode2)
				{
					sgNode::ConstChildIterator cit = boneNode->getConstChildIterator();
					while(cit.hasMoreElements())
					{
						boneNode2 = dynamic_cast<sgBoneObject*>(cit.value());
						if(boneNode2)
							break;
						++cit;
					}
				}
				if(!boneNode2)
				{
					const Vector3 &pos = boneNode->absolutePosition();
					b.object->setAbsolutePosition(pos);
					bodyTrans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
				}
				else
				{
					Vector3 pos = (boneNode->absolutePosition() + boneNode2->absolutePosition()) * 0.5f;
					b.object->setAbsolutePosition(pos);
					bodyTrans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
				}
			}

			const Quaternion &q = boneNode->absoluteOrientation();
			b.object->setAbsoluteOrientation(boneNode->absoluteOrientation());
			bodyTrans.setRotation(btQuaternion(q.x(), q.y(), q.z(), q.w()));
			b.body->setWorldTransform(bodyTrans);
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
		mBodyMapByJointName.clear();
	}

	void sgRagdoll::update( Float32 deltaTime )
	{
		sgScene *scene = 0;
		if(mParentObject)
		{
			scene = mParentObject->getScene();
		}
		if(!scene)
			return ;

		if(scene->isPhysicsEnabled())
		{
			syncTransformToSkeleton();
		}
		else
		{
			resetTransform();
		}

	}

	void sgRagdoll::syncTransformToSkeleton( void )
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

		StringHandleSet boneFilter;
		boneFilter.insert(sgBoneObject::GetClassTypeName());
		sgNode::NodeList boneList;
		skeleton->getRoot()->getInheritsNodes(boneList, skeleton->getBoneNum(), boneFilter);
		for(size_t i=0; i<boneList.size(); ++i)
		{
			sgBoneObject *boneNode = dynamic_cast<sgBoneObject*>(boneList[i]);
			if(!boneNode)
				continue;

			BodyMap::iterator it = mBodyMapByJointName.find(boneNode->getName());
			if(it == mBodyMapByJointName.end())
				continue;
			Body &b = it->second;

			sgRagdollConfig::BodyInfoMap::const_iterator biIt = bodyset.find(b.object->getName());
			if(biIt == bodyset.end())
				continue;
			const sgRagdollConfig::BodyInfo &bi = biIt->second;

			btTransform bodyTrans = b.body->getWorldTransform();

			btQuaternion q = bodyTrans.getRotation();
			Quaternion myq(q.w(), q.x(), q.y(), q.z());
			b.object->setAbsoluteOrientation(myq);
			boneNode->setAbsoluteOrientation(myq);

			btVector3 pos = bodyTrans.getOrigin();
			Vector3 mypos(pos.x(), pos.y(), pos.z());
			b.object->setAbsolutePosition(mypos);
			if(bi.ifSyncPos)
			{
				boneNode->setAbsolutePosition(mypos);
			}
			
			// constrain
		}
	}

} // namespace Sagitta
