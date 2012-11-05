
#include "sgMeshComponent.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/resource/sgMesh.h"


namespace Sagitta
{
	SG_META_DEFINE(sgMeshComponent, sgComponent)

	sgMeshComponent::sgMeshComponent( void ) : mMeshFile(sgStrHandle::EmptyString)
	{

	}

	sgMeshComponent::~sgMeshComponent( void )
	{

	}

	sgStrHandle sgMeshComponent::getMeshFile( void ) const
	{
		return mMeshFile;
	}

	void sgMeshComponent::setMeshFile( sgStrHandle meshFile )
	{
		mMeshFile = meshFile;
	}

	sgMesh * sgMeshComponent::getMesh( void ) const
	{
		return dynamic_cast<sgMesh*>(sgResourceCenter::instance()->findResource(mMeshFile));
	}

	
}
