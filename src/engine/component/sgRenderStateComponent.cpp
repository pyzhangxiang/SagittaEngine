
#include "sgRenderStateComponent.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/resource/sgMaterial.h"
#include "engine/renderer/sgRenderEffect.h"

namespace Sagitta
{
	SG_META_DEFINE(sgRenderStateComponent, sgComponent)

	sgRenderStateComponent::sgRenderStateComponent( void )
    : mMaterialFile(sgStrHandle::EmptyString)
    , mRenderEffect(NULL)
	{

	}

	sgRenderStateComponent::~sgRenderStateComponent( void )
	{

	}

	sgStrHandle sgRenderStateComponent::getMaterialFile( void ) const
	{
		return mMaterialFile;
	}

	void sgRenderStateComponent::setMaterialFile( sgStrHandle materialFile )
	{
		mMaterialFile = materialFile;
	}

	sgMaterial * sgRenderStateComponent::getMaterial( void ) const
	{
		return dynamic_cast<sgMaterial*>(sgResourceCenter::instance()->findResource(mMaterialFile));
	}

	Sagitta::sgRenderState sgRenderStateComponent::getRenderState( void ) const
	{
		return mRenderState;
	}

	void sgRenderStateComponent::setRenderOption( const sgRenderState &ro )
	{
		mRenderState = ro;
	}
    
    sgRenderEffect *sgRenderStateComponent::createRenderEffect(const sgStrHandle &effectType)
    {
        sgClassMeta *meta = sgMetaCenter::instance().findMeta(effectType);
        if(!meta)
            return mRenderEffect;
        if(!meta->isClass(sgRenderEffect::GetClassName()))
            return mRenderEffect;
        
        if(mRenderEffect)
        {
            // warning: the original one will be destroyed
            destroyRenderEffect();
        }
        mRenderEffect = (sgRenderEffect*)sgObject::createObject(effectType);
        
        return mRenderEffect;
    }
    
    void sgRenderStateComponent::destroyRenderEffect(void)
    {
        if(mRenderEffect)
        {
            sgObject::destroyObject(mRenderEffect);
            mRenderEffect = NULL;
        }
    }
    
    void sgRenderStateComponent::addTexture(const sgStrHandle &filename)
    {
        mTextureList.push_back(filename);
    }
    
    void sgRenderStateComponent::clearTexture(void)
    {
        mTextureList.clear();
    }
    
    sgTexture *sgRenderStateComponent::getTexture(size_t index) const
    {
        if(mTextureList.empty() || index >= mTextureList.size())
            return NULL;
        
        return (sgTexture*)sgResourceCenter::instance()->findResource(mTextureList[index]);
    }
}
