#include "DemoLighting.h"
#include <engine/scenegraph/sgScene.h>
#include <engine/scenegraph/sgSceneObject.h>
#include <engine/common/sgMemChoc.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgMeshComponent.h>
#include <engine/resource/sgResourceCenter.h>
#include <engine/resource/sgMesh.h>
#include <engine/resource/sgSimpleMeshes.h>
#include <engine/component/sgLightComponent.h>
#include <engine/component/sgRenderStateComponent.h>
using namespace Sagitta;

DemoLighting::DemoLighting() 
: sgDemo("Sagitta Demo Lighting", "../resource/")
{

}

DemoLighting::~DemoLighting()
{

}

void DemoLighting::prepare(void)
{
	if(mScene && mCamera)
	{
		mCamera->translate(Vector3(0.0f, 2.5f, 2.0f));
		mCamera->pitch(Radian(-Math::PI / 6.0f));
    
		// set lights
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassName());
		light1->setParent(mScene->getRoot());
		light1->translate(Vector3(3.0f, 3.0f, 5.0f));
    
    
		// prepare resources
		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassName(), sgMeshCube::InternalFileName);
		sgMeshSphere *meshSphere = (sgMeshSphere*)sgResourceCenter::instance()->createResource(sgMeshSphere::GetClassName(), sgMeshSphere::InternalFileName);
		sgMeshCone *meshCone = (sgMeshCone*)sgResourceCenter::instance()->createResource(sgMeshCone::GetClassName(), sgMeshCone::InternalFileName);
    
		// place cube 
		sgSceneObject *objCube = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		objCube->setParent(mScene->getRoot());
		objCube->translate(Vector3(-2.5f, 0.0f, -2.5f));
		objCube->yaw(Radian(Math::PI_DIV_4));
		objCube->pitch(Radian(-Math::PI_DIV_3));
		//triangle->scale(Vector3(5, 5, 5));
    
		sgMeshComponent *cubeMeshComp = (sgMeshComponent*)objCube->createComponent(sgMeshComponent::GetClassName());
		cubeMeshComp->setMeshFile(meshCube->getFilename());
    
		// place sphere
		sgSceneObject *objSphere = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		objSphere->setParent(mScene->getRoot());
		objSphere->translate(Vector3(2.0f, 0.0f, -2.5f));
		objSphere->scale(Vector3(0.5f, 0.5f, 0.5f));
    
		sgMeshComponent *sphereMeshComp = (sgMeshComponent*)objSphere->createComponent(sgMeshComponent::GetClassName());
		sphereMeshComp->setMeshFile(meshSphere->getFilename());
    
		// place cone
		sgSceneObject *objCone = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		objCone->setParent(mScene->getRoot());
		objCone->translate(Vector3(0.0f, -0.5f, -2.5f));
		objCone->scale(Vector3(0.5f, 2.0f, 0.5f));
    
		sgMeshComponent *coneMeshComp = (sgMeshComponent*)objCone->createComponent(sgMeshComponent::GetClassName());
		coneMeshComp->setMeshFile(meshCone->getFilename());
	}
}

