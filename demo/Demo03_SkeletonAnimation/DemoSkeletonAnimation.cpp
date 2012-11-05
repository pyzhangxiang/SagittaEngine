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
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassName());
		light1->setParent(mScene->getRoot());
		light1->translate(Vector3(3.0f, 100.0f, 105.0f));
		mLight = light1;

		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassName(), sgMeshCube::InternalFileName);
		sgSceneObject *light1Debug = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		light1Debug->setIsDebugObj(true);
		sgMeshComponent *light1DebugMeshComp = (sgMeshComponent*)light1Debug->createComponent(sgMeshComponent::GetClassName());
		light1DebugMeshComp->setMeshFile(meshCube->getFilename());

		light1->setDebugObjectToShow(light1Debug);
		light1->setShowDebug(true);


		// grid
		sgSceneObject *grid = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		grid->setParent(mScene->getRoot());
		sgMeshGrid *meshGrid = (sgMeshGrid*)sgResourceCenter::instance()->createResource(sgMeshGrid::GetClassName(), sgMeshGrid::InternalFileName);
		sgMeshComponent *gridComp = (sgMeshComponent*)grid->createComponent(sgMeshComponent::GetClassName());
		gridComp->setMeshFile(meshGrid->getFilename());


		sgSceneObject *player = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		player->setParent(mScene->getRoot());
		// load .bvh
		sgSkeleton *pSkeleton = sgLoader::load_bvh_skeleton("animations/GHBW_0001.bvh");
		pSkeleton->setShowDebug(true);
		player->setSkeleton(pSkeleton);
		//pSkeleton->setParent(mScene->getRoot());

		sgAnimation *pAnimation = sgLoader::load_bvh_animation("animations/GHBW_0001.bvh");
		sgAnimationComponent *animComp = (sgAnimationComponent*)player->createComponent(sgAnimationComponent::GetClassName());
		animComp->setAnimationFile(pAnimation->getFilename());
		animComp->setPlayMode(sgAnimationComponent::PM_LOOP);
		animComp->play();
	}
	
}


