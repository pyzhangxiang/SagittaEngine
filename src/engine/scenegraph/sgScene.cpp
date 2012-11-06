
#include "sgScene.h"
#include "sgSceneObject.h"
#include "engine/renderer/sgSceneRenderEffect.h"

namespace Sagitta{

	
	SG_META_DEFINE(sgScene, sgObject)

	//  [1/1/2009 zhangxiang]
	sgScene::sgScene(void) 
	: sgObject(), mpRoot(NULL)
    , mRenderEffect(NULL)
    {
		mpRoot = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
        mpRoot->setScene(this);
	}


	//  [1/1/2009 zhangxiang]
	sgScene::~sgScene(void)
    {
        if(mRenderEffect)
        {
            destroyRenderEffect();
        }
        
        // destroy all scene objects belong to me
		sgObject::destroyObject(mpRoot);
	}

	sgSceneObject * sgScene::getRoot( void ) const
	{
		return mpRoot;
	}

	void sgScene::update( Float32 deltaTime )
	{
		// physics step

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
    
    const sgRenderState &sgScene::getRenderState(void) const
    {
        return mRenderState;
    }
    
    void sgScene::setRenderState(const sgRenderState &state)
    {
        mRenderState = state;
    }
    
    sgSceneRenderEffect *sgScene::createRenderEffect(const sgStrHandle &effectType)
    {
        sgClassMeta *meta = sgMetaCenter::instance().findMeta(effectType);
        if(!meta)
            return mRenderEffect;
        if(!meta->isClass(sgSceneRenderEffect::GetClassName()))
            return mRenderEffect;
        
        if(mRenderEffect)
        {
            // warning: the original one will be destroyed
            destroyRenderEffect();
        }
        mRenderEffect = (sgSceneRenderEffect*)sgObject::createObject(effectType);
        
        return mRenderEffect;
    }
    
    void sgScene::destroyRenderEffect(void)
    {
        if(mRenderEffect)
        {
            sgObject::destroyObject(mRenderEffect);
            mRenderEffect = NULL;
        }
    }

	
} // namespace Sagitta
