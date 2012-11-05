
#include "sgRenderStateComponent.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/resource/sgMaterial.h"


namespace Sagitta
{
	SG_META_DEFINE(sgRenderStateComponent, sgComponent)

	sgRenderStateComponent::sgRenderStateComponent( void ) : mMaterialFile(sgStrHandle::EmptyString)
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
}
