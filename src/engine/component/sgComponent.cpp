
#include "sgComponent.h"

namespace Sagitta
{
	SG_META_DEFINE(sgComponent, sgObject)

	sgComponent::sgComponent( void )
	{

	}

	sgComponent::~sgComponent( void )
	{
        mpParent = 0;
	}

	void sgComponent::setParent( sgSceneObject *obj )
	{
		mpParent = obj;
	}

	sgSceneObject * sgComponent::getParent( void ) const
	{
		return mpParent;
	}
}
