#include "DemoGLSL.h"
#include <engine/scenegraph/sgScene.h>
#include <engine/scenegraph/sgSceneObject.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgMeshComponent.h>
#include <engine/resource/sgResourceCenter.h>
#include <engine/resource/sgSimpleMeshes.h>
#include <engine/resource/sgMaterial.h>
#include <engine/renderer/sgRenderTechnique.h>
#include <engine/renderer/sgRenderEffect.h>
#include <engine/renderer/sgRenderPass.h>
#include <engine/renderer/sgGpuProgram.h>
#include <engine/renderer/sgGLGpuProgram.h>
#include <engine/renderer/sgRenderer.h>
#include <engine/renderer/sgRenderQueue.h>
#include <engine/component/sgLightComponent.h>
#include <engine/component/sgRenderStateComponent.h>
#include <engine/common/sgLogSystem.h>
using namespace Sagitta;

DemoGLSL::DemoGLSL() 
: sgDemo("Sagitta Demo GLSL", "../resource/")
{

}

DemoGLSL::~DemoGLSL()
{

}

void DemoGLSL::prepare(void)
{
    if(mScene && mCamera)
	{
		if( ! (sgGetRenderer()->initShaderEnvironment()) )
		{
			sgLogSystem::instance()->warning("Init shader environment failed");
		}

		// prepare shaders
		sgShader *vsColor = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassTypeName(), "shaders/VertexColor.vs");
		sgShader *fsColor = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassTypeName(), "shaders/VertexColor.fs");
		sgGpuProgram *programColor = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassTypeName());
		if(!programColor->setShader(vsColor->getFilename(), fsColor->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'Color' is invalid");
		}

		sgShader *vsStandard = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassTypeName(), "shaders/StandardShading.vs");
		sgShader *fsStandard = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassTypeName(), "shaders/StandardShading.fs");
		sgGpuProgram *programStandard = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassTypeName());
		if(!programStandard->setShader(vsStandard->getFilename(), fsStandard->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'Standard Shading' is invalid");
		}

		// create scene effect
		sgRenderTechnique *renderTech = sgGetRenderer()->getRenderTechnique();
		sgRenderPass *sceneRp = renderTech->getRenderPass(0);
		sgRenderEffect *sceneRe = sceneRp->createRenderEffect(sgRenderEffect::GetClassTypeName());
		sceneRe->setGpuProgram(programStandard);	
		


		// prepare materials
		sgMaterial *mat1 = (sgMaterial*)sgResourceCenter::instance()->createResource(sgMaterial::GetClassTypeName(), "material_test_1");
		mat1->setDiffuseColor(Color(255, 255, 0));
		

		// prepare camera
		mCamera->translate(Vector3(0.0f, 2.5f, 2.0f));
		mCamera->pitch(Radian(-Math::PI / 6.0f));

		// set lights
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		light1->setParent(mScene->getRoot());
		light1->translate(Vector3(3.0f, 3.0f, 5.0f));
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassTypeName());
		lightComp1->setDiffuseColor(Color(0, 125, 11));
		lightComp1->setIntensity(3.0f);
		

		// prepare resources
		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassTypeName(), sgMeshCube::InternalFileName);
		sgMeshSphere *meshSphere = (sgMeshSphere*)sgResourceCenter::instance()->createResource(sgMeshSphere::GetClassTypeName(), sgMeshSphere::InternalFileName);
		sgMeshCone *meshCone = (sgMeshCone*)sgResourceCenter::instance()->createResource(sgMeshCone::GetClassTypeName(), sgMeshCone::InternalFileName);
		sgMeshTriangle *meshTriangle = (sgMeshTriangle*)sgResourceCenter::instance()->createResource(sgMeshTriangle::GetClassTypeName(), sgMeshTriangle::InternalFileName);
		meshTriangle->setVertecies(Vector3(-1.0f, 0.0, 0.0f), Color::RED, 
			Vector3(1.0f, 0.0f, 0.0f), Color::GREEN, 
			Vector3(0.0f, 1.0f, 0.0f), Color::BLUE);

		// place cube 
		sgSceneObject *objCube = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		objCube->setParent(mScene->getRoot());
		objCube->translate(Vector3(-2.5f, 0.0f, -2.5f));
		objCube->yaw(Radian(Math::PI_DIV_4));
		objCube->pitch(Radian(-Math::PI_DIV_3));

		sgMeshComponent *cubeMeshComp = (sgMeshComponent*)objCube->createComponent(sgMeshComponent::GetClassTypeName());
		cubeMeshComp->setMeshFile(meshCube->getFilename());

		sgRenderStateComponent *cubeRsComp = (sgRenderStateComponent*)objCube->createComponent(sgRenderStateComponent::GetClassTypeName());
		cubeRsComp->setMaterialFile(mat1->getFilename());
		
		// triangle    
		sgSceneObject *triangle = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		triangle->setParent(mScene->getRoot());
		triangle->yaw(Radian(Math::PI_DIV_4));
		//triangle->scale(Vector3(5, 5, 5));
    
		sgMeshComponent *triMeshComp = (sgMeshComponent*)triangle->createComponent(sgMeshComponent::GetClassTypeName());
		triMeshComp->setMeshFile(meshTriangle->getFilename());
	    
		sgRenderStateComponent *triRsComp = (sgRenderStateComponent*)triangle->createComponent(sgRenderStateComponent::GetClassTypeName());
		sgRenderEffect *triRe = triRsComp->createRenderEffect(sgRenderEffect::GetClassTypeName());
		triRe->setGpuProgram(programColor);      
        
	}
}
