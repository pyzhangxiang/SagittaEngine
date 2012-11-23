#include "DemoShadowMap.h"
#include <engine/scenegraph/sgScene.h>
#include <engine/scenegraph/sgSceneObject.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgMeshComponent.h>
#include <engine/resource/sgResourceCenter.h>
#include <engine/resource/sgSimpleMeshes.h>
#include <engine/resource/sgMaterial.h>
#include <engine/resource/sgTexture.h>
#include <engine/resource/sgLoader.h>
#include <engine/renderer/sgRenderTechniqueShadowMap.h>
#include <engine/renderer/sgRenderEffect.h>
#include <engine/renderer/sgRenderPass.h>
#include <engine/renderer/sgViewport.h>
#include <engine/renderer/sgRenderTarget.h>
#include <engine/renderer/sgGpuProgram.h>
#include <engine/renderer/sgGLGpuProgram.h>
#include <engine/renderer/sgRenderer.h>
#include <engine/renderer/sgRenderQueue.h>
#include <engine/component/sgLightComponent.h>
#include <engine/component/sgRenderStateComponent.h>
#include <engine/common/sgLogSystem.h>
using namespace Sagitta;

DemoShadowMap::DemoShadowMap() 
: sgDemo("Sagitta Demo ShadowMap", "../resource/")
{

}

DemoShadowMap::~DemoShadowMap()
{

}

void DemoShadowMap::prepare(void)
{
    if(mScene && mCamera)
	{
		if( ! (sgGetRenderer()->initShaderEnvironment()) )
		{
			sgLogSystem::instance()->warning("Init shader environment failed");
		}

		// prepare shaders
		sgShader *vsColor = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassName(), "shaders/VertexColor.vs");
		sgShader *fsColor = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassName(), "shaders/VertexColor.fs");
		sgGpuProgram *programColor = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassName());
		if(!programColor->setShader(vsColor->getFilename(), fsColor->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'Color' is invalid");
		}
        
        sgShader *vsDepthRT = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassName(), "shaders/DepthRT.vs");
		sgShader *fsDepthRT = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassName(), "shaders/DepthRT.fs");
		sgGpuProgram *programDepthRT = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassName());
		if(!programDepthRT->setShader(vsDepthRT->getFilename(), fsDepthRT->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'DepthRT' is invalid");
		}

		sgShader *vsShadow = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassName(), "shaders/SimpleShadowMap.vs");
		sgShader *fsShadow = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassName(), "shaders/SimpleShadowMap.fs");
		sgGpuProgram *programShadow = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassName());
		if(!programShadow->setShader(vsShadow->getFilename(), fsShadow->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'SimpleShadowMap' is invalid");
		}

		// create scene effect
		sgRenderTechnique *renderTech = sgGetRenderer()->useRenderTechnique(sgRenderTechniqueShadowMap::GetClassName());
		sgRenderPass *depthRTPass = renderTech->getRenderPass(0);
		sgRenderEffect *depthRTEffect = depthRTPass->createRenderEffect(sgRenderEffect::GetClassName());
		depthRTEffect->setGpuProgram(programDepthRT);
        
        /*sgRenderPass *shadowPass = renderTech->getRenderPass(1);
		sgRenderEffect *shadowEffect = shadowPass->createRenderEffect(sgRenderEffect::GetClassName());
		shadowEffect->setGpuProgram(programShadow);*/


		// prepare materials
		sgMaterial *mat1 = (sgMaterial*)sgResourceCenter::instance()->createResource(sgMaterial::GetClassName(), "material_test_1");
		mat1->setDiffuseColor(Color(255, 125, 75));

		// prepare resources
		sgMeshTriangle *meshTriangle = (sgMeshTriangle*)sgResourceCenter::instance()->createResource(sgMeshTriangle::GetClassName(), sgMeshTriangle::InternalFileName);
		meshTriangle->setVertecies(Vector3(-1.0f, 0.0, 0.0f), Color::RED, 
			Vector3(1.0f, 0.0f, 0.0f), Color::GREEN, 
			Vector3(0.0f, 1.0f, 0.0f), Color::BLUE);
		sgMeshPlane *meshPlane = (sgMeshPlane*)sgResourceCenter::instance()->createResource(sgMeshPlane::GetClassName(), sgMeshPlane::InternalFileName);
		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassName(), sgMeshCube::InternalFileName);

		// prepare camera
		//mCamera->translate(Vector3(0.0f, 32.5f, 202.0f));
		mCamera->translate(Vector3(0.0f, 13.5f, 22.0f));
		mCamera->pitch(Radian(-Math::PI / 6.0f));
        
        // set shadow pass camera
        /*sgCameraComponent *cameraComp = (sgCameraComponent*)mCamera->getComponent(sgCameraComponent::GetClassName());
        shadowPass->getRenderTarget()->getViewport()->setCamera(cameraComp);
		shadowPass->getRenderTarget()->getViewport()->setBackColor(Color::DARKGRAY);*/

		// set lights
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		light1->setParent(mScene->getRoot());
		light1->translate(Vector3(3.0f, 10.0f, 5.0f));
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassName());
		//lightComp1->setDiffuseColor(Color(0, 125, 11));
		lightComp1->setIntensity(8.0f);
		mLight = light1;

		/*sgSceneObject *light1Debug = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		light1Debug->setIsDebugObj(true);
		light1Debug->scale(Vector3(0.2f));
		sgMeshComponent *light1DebugMeshComp = (sgMeshComponent*)light1Debug->createComponent(sgMeshComponent::GetClassName());
		light1DebugMeshComp->setMeshFile(meshCube->getFilename());
		light1->setDebugObjectToShow(light1Debug);
		light1->setShowDebug(true);*/
        
        // set depth pass camera
        sgSceneObject *lightCamera = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
        lightCamera->setParent(light1);
        sgCameraComponent *lightcameraComp = (sgCameraComponent*)lightCamera->createComponent(sgCameraComponent::GetClassName());
        lightcameraComp->setUpDirection(Vector3::UNIT_Y);
        lightcameraComp->setShootDirection(Vector3(0.0f, 0.0f, -1.0f));
		lightcameraComp->setPerspective(Math::PI / 1.2f, 0.1f, 10000.0f);
		lightCamera->pitch(Radian(-Math::PI / 2.0f));
        depthRTPass->getRenderTarget()->getViewport()->setCamera(lightcameraComp);
		depthRTPass->getRenderTarget()->getViewport()->setBackColor(Color::DARKGRAY);

		// plane
		sgSceneObject *plane = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
		plane->setParent(mScene->getRoot());
		sgMeshComponent *planeComp = (sgMeshComponent*)plane->createComponent(sgMeshComponent::GetClassName());
		planeComp->setMeshFile(meshPlane->getFilename());
		sgRenderStateComponent *planeRsComp = (sgRenderStateComponent*)plane->createComponent(sgRenderStateComponent::GetClassName());
		planeRsComp->setMaterialFile(mat1->getFilename());

		// place cube 
		sgSceneObject *objRoot = /*sgLoader::load_pod("scene/podscene1/scene.pod");*/sgLoader::load_obj("models/cube.obj");
        objRoot->setParent(mScene->getRoot());
		objRoot->translate(Vector3(-1.0f, 1.0f, -2.5f));
		objRoot->yaw(Radian(Math::PI_DIV_4));
		//objRoot->pitch(Radian(-Math::PI_DIV_3));
		
        sgSceneObject *objCube = (sgSceneObject*)objRoot->getFirstChild();
		sgRenderStateComponent *cubeRsComp = (sgRenderStateComponent*)objCube->createComponent(sgRenderStateComponent::GetClassName());
		cubeRsComp->setMaterialFile(mat1->getFilename());
        // load textures
        sgTexture *texture = (sgTexture*)sgResourceCenter::instance()->createResource(sgTexture::GetClassName(), "images/cube.png");
        if(texture)
        {
            cubeRsComp->addTexture(texture->getFilename());
        }

	}
}
