#include "DemoTriangle.h"
#include <engine/scenegraph/sgScene.h>
#include <engine/scenegraph/sgSceneObject.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgMeshComponent.h>
#include <engine/resource/sgResourceCenter.h>
#include <engine/resource/sgSimpleMeshes.h>
using namespace Sagitta;

DemoTriangle::DemoTriangle() 
: sgDemo("Sagitta Demo Triangle", "../resource/")
{

}

DemoTriangle::~DemoTriangle()
{

}

void DemoTriangle::prepare(void)
{
    if(mScene && mCamera)
	{
		mCamera->translate(Vector3(0.0f, 0.0f, 2.0f));

		// prepare resources
		sgMeshTriangle *meshTriangle = (sgMeshTriangle*)sgResourceCenter::instance()->createResource(sgMeshTriangle::GetClassName(), sgMeshTriangle::InternalFileName);
		meshTriangle->setVertecies(Vector3(-1.0f, 0.0, 0.0f), Color::RED, 
								   Vector3(1.0f, 0.0f, 0.0f), Color::GREEN, 
								   Vector3(0.0f, 1.0f, 0.0f), Color::BLUE);
    
		sgSceneObject *triangle = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		triangle->setParent(mScene->getRoot());
		triangle->yaw(Radian(Math::PI_DIV_4));
		//triangle->scale(Vector3(5, 5, 5));
    
		sgMeshComponent *meshComp = (sgMeshComponent*)triangle->createComponent(sgMeshComponent::GetClassName());
	//    sgRenderStateComponent *rsComp = (sgRenderStateComponent*)triangle->createComponent(sgRenderStateComponent::GetClassName());
		meshComp->setMeshFile(meshTriangle->getFilename());
	}
}
