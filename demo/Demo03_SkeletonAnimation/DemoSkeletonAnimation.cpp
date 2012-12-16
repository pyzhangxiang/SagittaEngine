#include "DemoSkeletonAnimation.h"
#include <engine/scenegraph/sgScene.h>
#include <engine/scenegraph/sgSceneObject.h>
#include <engine/scenegraph/sgSkeleton.h>
#include <engine/resource/sgAnimation.h>
#include <engine/resource/sgSimpleMeshes.h>
#include <engine/resource/sgResourceCenter.h>
#include <engine/component/sgMeshComponent.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgLightComponent.h>
#include <engine/component/sgAnimationComponent.h>
#include <engine/resource/sgLoader.h>
#include <iostream>
using namespace Sagitta;

DemoSkeletonAnimation::DemoSkeletonAnimation() 
: sgDemo("Sagitta Demo Skeleton Animation", "../resource")
{

}

DemoSkeletonAnimation::~DemoSkeletonAnimation()
{

}

void DemoSkeletonAnimation::prepare(void)
{
	if(mScene && mCamera)
	{
		mCamera->translate(Vector3(0.0f, 100.5f, 152.0f));
		//mCamera->pitch(Radian(-Math::PI / 6.0f));

		// set lights
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassTypeName());
		light1->setParent(mScene->getRoot());
		light1->translate(Vector3(3.0f, 100.0f, 105.0f));
		mLight = light1;

		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassTypeName(), sgMeshCube::InternalFileName);
		sgSceneObject *light1Debug = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		sgMeshComponent *light1DebugMeshComp = (sgMeshComponent*)light1Debug->createComponent(sgMeshComponent::GetClassTypeName());
		light1DebugMeshComp->setMeshFile(meshCube->getFilename());

		light1Debug->setIsDebugObj(true);
		light1Debug->setParent(light1);


		// grid
		sgSceneObject *grid = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		grid->setParent(mScene->getRoot());
		sgMeshGrid *meshGrid = (sgMeshGrid*)sgResourceCenter::instance()->createResource(sgMeshGrid::GetClassTypeName(), sgMeshGrid::InternalFileName);
		sgMeshComponent *gridComp = (sgMeshComponent*)grid->createComponent(sgMeshComponent::GetClassTypeName());
		gridComp->setMeshFile(meshGrid->getFilename());


		sgSceneObject *player = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		player->setParent(mScene->getRoot());
		// load .bvh
		sgSkeleton *pSkeleton = sgLoader::load_bvh_skeleton("animations/GHBW_0001.bvh");
		player->setSkeleton(pSkeleton);
		//pSkeleton->setParent(mScene->getRoot());

		sgAnimation *pAnimation = sgLoader::load_bvh_animation("animations/GHBW_0001.bvh");
		sgAnimationComponent *animComp = (sgAnimationComponent*)player->createComponent(sgAnimationComponent::GetClassTypeName());
		animComp->setAnimationFile(pAnimation->getFilename());
		animComp->setPlayMode(sgAnimationComponent::PM_LOOP);
		animComp->play();
	}
	
}


