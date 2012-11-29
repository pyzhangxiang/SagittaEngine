
#include "sgScene.h"
#include "sgSceneObject.h"
#include "sgDynamicsWorld.h"

namespace Sagitta{

	
	SG_META_DEFINE(sgScene, sgObject)

	//  [1/1/2009 zhangxiang]
	sgScene::sgScene(void) 
	: sgObject(), mpRoot(NULL)
	, mPhysicsEnabled(false), mDynamicsWorld(NULL)
    {
        mAmbiantColor = Color(Color::GLColor(0.1, 0.1, 0.1, 1.0));
        
		mpRoot = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
        mpRoot->setScene(this);

		mDynamicsWorld = new sgDynamicsWorld();

	}


	//  [1/1/2009 zhangxiang]
	sgScene::~sgScene(void)
    {
        // destroy all scene objects belong to me
		sgObject::destroyObject(mpRoot);
		delete mDynamicsWorld;
	}

	sgSceneObject * sgScene::getRoot( void ) const
	{
		return mpRoot;
	}

	void sgScene::update( Float32 deltaTime )
	{
		if(mPhysicsEnabled)
		{
			// physics step
			mDynamicsWorld->stepSimulation(deltaTime);
		}
		
		mpRoot->update(deltaTime);
	}
    
    const Color &sgScene::getAmbiantColor(void) const
    {
        return mAmbiantColor;
    }
    
    void sgScene::setAmbiantColor(const Color &color)
    {
        mAmbiantColor = color;
    }

	void sgScene::setPhysicsEnabled( bool enable )
	{
		mPhysicsEnabled = enable;
	}
	/*
    const sgRenderState &sgScene::getRenderState(void) const
    {
        return mRenderState;
    }
    
    void sgScene::setRenderState(const sgRenderState &state)
    {
        mRenderState = state;
    }
*/
	
} // namespace Sagitta
