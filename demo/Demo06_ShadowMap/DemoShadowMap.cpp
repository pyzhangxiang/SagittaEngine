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
		sgShader *vsColor = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassTypeName(), "shaders/VertexColor.vs");
		sgShader *fsColor = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassTypeName(), "shaders/VertexColor.fs");
		sgGpuProgram *programColor = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassTypeName());
		if(!programColor->setShader(vsColor->getFilename(), fsColor->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'Color' is invalid");
		}
        
        sgShader *vsDepthRT = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassTypeName(), "shaders/DepthRT.vs");
		sgShader *fsDepthRT = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassTypeName(), "shaders/DepthRT.fs");
		sgGpuProgram *programDepthRT = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassTypeName());
		if(!programDepthRT->setShader(vsDepthRT->getFilename(), fsDepthRT->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'DepthRT' is invalid");
		}

		sgShader *vsShadow = (sgShader*)sgResourceCenter::instance()->createResource(sgGLVertexShader::GetClassTypeName(), "shaders/SimpleShadowMap.vs");
		sgShader *fsShadow = (sgShader*)sgResourceCenter::instance()->createResource(sgGLFragmentShader::GetClassTypeName(), "shaders/SimpleShadowMap.fs");
		sgGpuProgram *programShadow = (sgGpuProgram*)sgObject::createObject(sgGLGpuProgram::GetClassTypeName());
		if(!programShadow->setShader(vsShadow->getFilename(), fsShadow->getFilename()))
		{
			sgLogSystem::instance()->warning("Program 'SimpleShadowMap' is invalid");
		}

		// create scene effect
		sgRenderTechnique *renderTech = sgGetRenderer()->useRenderTechnique(sgRenderTechniqueShadowMap::GetClassTypeName());
		sgRenderPass *depthRTPass = renderTech->getRenderPass(0);
		sgRenderEffect *depthRTEffect = depthRTPass->createRenderEffect(sgRenderEffect::GetClassTypeName());
		depthRTEffect->setGpuProgram(programDepthRT);
        
        sgRenderPass *shadowPass = renderTech->getRenderPass(1);
		sgRenderEffect *shadowEffect = shadowPass->createRenderEffect(sgRenderEffect::GetClassTypeName());
		shadowEffect->setGpuProgram(programShadow);


		// prepare materials
		sgMaterial *mat1 = (sgMaterial*)sgResourceCenter::instance()->createResource(sgMaterial::GetClassTypeName(), "material_test_1");
		mat1->setDiffuseColor(Color(255, 125, 75));

		// prepare resources
		sgMeshTriangle *meshTriangle = (sgMeshTriangle*)sgResourceCenter::instance()->createResource(sgMeshTriangle::GetClassTypeName(), sgMeshTriangle::InternalFileName);
		meshTriangle->setVertecies(Vector3(-1.0f, 0.0, 0.0f), Color::RED, 
			Vector3(1.0f, 0.0f, 0.0f), Color::GREEN, 
			Vector3(0.0f, 1.0f, 0.0f), Color::BLUE);
		sgMeshPlane *meshPlane = (sgMeshPlane*)sgResourceCenter::instance()->createResource(sgMeshPlane::GetClassTypeName(), sgMeshPlane::InternalFileName);
		sgMeshCube *meshCube = (sgMeshCube*)sgResourceCenter::instance()->createResource(sgMeshCube::GetClassTypeName(), sgMeshCube::InternalFileName);

		// prepare camera
		//mCamera->translate(Vector3(0.0f, 32.5f, 202.0f));
		mCamera->translate(Vector3(4.0f, 3.5f, -12.0f));
        mCamera->yaw(Radian(Math::PI / 1.2f));
		mCamera->pitch(Radian(-Math::PI / 9.0f));
        
        // set shadow pass camera
        sgCameraComponent *cameraComp = (sgCameraComponent*)mCamera->getComponent(sgCameraComponent::GetClassTypeName());
        shadowPass->getRenderTarget()->getViewport()->setCamera(cameraComp);
		shadowPass->getRenderTarget()->getViewport()->setBackColor(Color::DARKGRAY);

		// set lights
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		light1->setParent(mScene->getRoot());
		//light1->translate(Vector3(3.0f, 4.0f, 0.0f));
        light1->translate(Vector3(4.0f, 4.0f, 0.0f));
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassTypeName());
		//lightComp1->setDiffuseColor(Color(0, 125, 11));
		lightComp1->setIntensity(8.0f);
		mLight = light1;

		sgSceneObject *light1Debug = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		light1Debug->setIsDebugObj(true);
		light1Debug->scale(Vector3(0.2f));
		sgMeshComponent *light1DebugMeshComp = (sgMeshComponent*)light1Debug->createComponent(sgMeshComponent::GetClassTypeName());
		light1DebugMeshComp->setMeshFile(meshCube->getFilename());
		light1Debug->setIsDebugObj(true);
		light1Debug->setParent(light1);
       
        // set depth pass camera
        sgSceneObject *lightCamera = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
        lightCamera->setParent(light1);
        sgCameraComponent *lightcameraComp = (sgCameraComponent*)lightCamera->createComponent(sgCameraComponent::GetClassTypeName());
        lightcameraComp->setUpDirection(Vector3::UNIT_Y);
        lightcameraComp->setShootDirection(Vector3(0.0f, 0.0f, -1.0f));
		lightcameraComp->setPerspective(Math::PI / 1.2f, 0.1f, 10000.0f);
		lightCamera->pitch(Radian(-Math::PI / 2.0f));
        depthRTPass->getRenderTarget()->getViewport()->setCamera(lightcameraComp);
		depthRTPass->getRenderTarget()->getViewport()->setBackColor(Color::WHITE);

		// plane
		sgSceneObject *plane = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		plane->setParent(mScene->getRoot());
		sgMeshComponent *planeComp = (sgMeshComponent*)plane->createComponent(sgMeshComponent::GetClassTypeName());
		planeComp->setMeshFile(meshPlane->getFilename());
		sgRenderStateComponent *planeRsComp = (sgRenderStateComponent*)plane->createComponent(sgRenderStateComponent::GetClassTypeName());
		planeRsComp->setMaterialFile(mat1->getFilename());
        plane->setCastShadow(false);
        
        sgTexture *textureChess = (sgTexture*)sgResourceCenter::instance()->createResource(sgTexture::GetClassTypeName(), "images/chess.png");
        if(textureChess)
        {
            planeRsComp->addTexture(textureChess->getFilename());
        }

		// place cube 
		sgSceneObject *objRoot = //sgLoader::load_pod("scene/podscene1/scene.pod");
                        sgLoader::load_obj("models/cube.obj");
        objRoot->setParent(mTargetRoot);
		mTargetRoot->translate(Vector3(-1.0f, 1.0f, -2.5f));
		objRoot->yaw(Radian(Math::PI_DIV_4));
		//objRoot->pitch(Radian(-Math::PI_DIV_3));
		
        sgSceneObject *objCube = (sgSceneObject*)objRoot->getFirstChild();
		sgRenderStateComponent *cubeRsComp = (sgRenderStateComponent*)objCube->createComponent(sgRenderStateComponent::GetClassTypeName());
		cubeRsComp->setMaterialFile(mat1->getFilename());
        // load textures
        sgTexture *texture = (sgTexture*)sgResourceCenter::instance()->createResource(sgTexture::GetClassTypeName(), "images/cube.png");
        if(texture)
        {
            cubeRsComp->addTexture(texture->getFilename());
        }

	}
}
