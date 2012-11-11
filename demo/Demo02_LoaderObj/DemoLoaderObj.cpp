#include "DemoLoaderObj.h"
#include <engine/scenegraph/sgSceneObject.h>
#include <engine/scenegraph/sgScene.h>
#include <engine/resource/sgSimpleMeshes.h>
#include <engine/resource/sgResourceCenter.h>
#include <engine/component/sgMeshComponent.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgLightComponent.h>
#include <engine/resource/sgLoader.h>
using namespace Sagitta;

DemoLoaderObj::DemoLoaderObj()
: sgDemo("Sagitta Demo Loader .obj", "../resource/")
{

}

DemoLoaderObj::~DemoLoaderObj()
{

}

void DemoLoaderObj::prepare(void)
{
	if(mScene && mCamera)
	{
		mCamera->translate(Vector3(-15.0f, 72.5f, 132.0f));
		mCamera->pitch(Radian(-Math::PI / 6.0f));

		// set lights
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassName());
		light1->setParent(mScene->getRoot());
		light1->translate(Vector3(-40.0f, 53.0f, 75.0f));
		mLight = light1;

		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassName(), sgMeshCube::InternalFileName);
		sgSceneObject *light1Debug = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		light1Debug->setIsDebugObj(true);
		sgMeshComponent *light1DebugMeshComp = (sgMeshComponent*)light1Debug->createComponent(sgMeshComponent::GetClassName());
		light1DebugMeshComp->setMeshFile(meshCube->getFilename());

		light1->setDebugObjectToShow(light1Debug);
		light1->setShowDebug(true);

		// load .obj
		sgSceneObject *objRoot = sgLoader::load_obj("models/jiedao.obj");
		objRoot->setParent(mScene->getRoot());
	}
}
