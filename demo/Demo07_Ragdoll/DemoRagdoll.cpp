#include "DemoRagdoll.h"
#include <engine/scenegraph/sgScene.h>
#include <engine/scenegraph/sgSceneObject.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgMeshComponent.h>
#include <engine/resource/sgResourceCenter.h>
#include <engine/resource/sgSimpleMeshes.h>
#include <engine/resource/sgMaterial.h>
#include <engine/resource/sgTexture.h>
#include <engine/resource/sgRagdollConfig.h>
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
#include <engine/serialization/sgOutXmlArchive.h>
#include <engine/math_serialization.h>
#include <engine/scenegraph/sgSkeleton.h>
#include "engine/component/sgAnimationComponent.h"
#include "engine/resource/sgAnimation.h"
#include "engine/scenegraph/sgRagdoll.h"
#include "engine/component/sgRigidBodyComponent.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "LinearMath/btDefaultMotionState.h"
using namespace Sagitta;

DemoRagdoll::DemoRagdoll() 
: sgDemo("Sagitta Demo Ragdoll", "../resource/")
, ragdoll(0)
{

}

DemoRagdoll::~DemoRagdoll()
{

}


void AA()
{
	int i = 10;
	i++;
	int c = 20;
	c++;
}

void DemoRagdoll::prepare(void)
{
	/*sgRagdollConfig::BodyInfoSet bis;
	sgRagdollConfig::BodyInfo bi;
	bi.haveInertia = false;
	bi.width = 1.11;
	bis.mBodyInfo.push_back(bi);
	bis.mBodyInfo.push_back(bi);
	
	serialization::sgOutXmlArchive xml("bodyinfo.xml");
	xml & bis;
	xml.write();*/

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
		mCamera->translate(Vector3(0.0f, 100.5f, 152.0f));
        
        // set shadow pass camera
        sgCameraComponent *cameraComp = (sgCameraComponent*)mCamera->getComponent(sgCameraComponent::GetClassTypeName());
        shadowPass->getRenderTarget()->getViewport()->setCamera(cameraComp);
		shadowPass->getRenderTarget()->getViewport()->setBackColor(Color::DARKGRAY);

		// set lights
		sgSceneObject *light1 = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		light1->setParent(mScene->getRoot());
		//light1->translate(Vector3(3.0f, 4.0f, 0.0f));
        light1->translate(Vector3(4.0f, 120.0f, 10.0f));
		sgLightComponent *lightComp1 = (sgLightComponent*)light1->createComponent(sgLightComponent::GetClassTypeName());
		//lightComp1->setDiffuseColor(Color(0, 125, 11));
		lightComp1->setIntensity(1008.0f);
		mLight = light1;

		sgSceneObject *light1Debug = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
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
		//plane->scale(Vector3(2500.0f, 20.0f, 2500.0f));
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
		
		{
			sgRigidBodyComponent *planeRigidBody = (sgRigidBodyComponent*)plane->createComponent(sgRigidBodyComponent::GetClassTypeName());
			btCollisionShape* planeShape = new btBoxShape(btVector3(1250.0f, 10.0f, 1250.0f));
			btTransform startTransform;
			startTransform.setIdentity();
	//		startTransform.setOrigin(btVector3(0,-10,0));
			btVector3 localInertia(0,0,0);
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* planeMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, planeMotionState, planeShape, localInertia);
			btRigidBody* planeBody = new btRigidBody(rbInfo);

			planeRigidBody->setOffset(Vector3(0.0f, -10.0f, 0.0f));
			planeRigidBody->setRigidBody(planeBody);
		}
		

		// place cube 
		sgSceneObject *objRoot = //sgLoader::load_pod("scene/podscene1/scene.pod");
                        sgLoader::load_obj("models/cube.obj");
        objRoot->setParent(mTargetRoot);
		mTargetRoot->translate(Vector3(20.0f, 100.0f, -2.5f));
		mTargetRoot->scale(Vector3(10.0f));
		mTargetRoot->yaw(Radian(Math::PI_DIV_4));
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
		
		{
			sgRigidBodyComponent *planeRigidBody = (sgRigidBodyComponent*)mTargetRoot->createComponent(sgRigidBodyComponent::GetClassTypeName());
			btCollisionShape* planeShape = new btBoxShape(btVector3(5.0f, 5.0f, 5.0f));
			btTransform startTransform;
			startTransform.setIdentity();
			//startTransform.setOrigin(btVector3(0,-10,0));
			btVector3 localInertia(0,0,0);
			planeShape->calculateLocalInertia(1.0f, localInertia);
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* planeMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0, planeMotionState, planeShape, localInertia);
			btRigidBody* planeBody = new btRigidBody(rbInfo);

			planeRigidBody->setRigidBody(planeBody);
		}
		
		sgSceneObject *player = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
		player->setParent(mScene->getRoot());
	//	player->translate(Vector3(0.0f, 10.0f, 0.0f));

		// load .bvh
		sgSkeleton *pSkeleton = sgLoader::load_bvh_skeleton("animations/GHBW_0001.bvh");
		player->setSkeleton(pSkeleton);

		sgRagdollConfig *ragdollConfig = (sgRagdollConfig*)sgResourceCenter::instance()->createResource(sgRagdollConfig::GetClassTypeName(), "models/ragdoll.xml");
		ragdoll = (sgRagdoll*)sgObject::createObject(sgRagdoll::GetClassTypeName());
		ragdoll->setRagdollConfig(ragdollConfig->getFilename());
		player->setRagdoll(ragdoll);
		

		sgAnimation *pAnimation = sgLoader::load_bvh_animation("animations/GHBW_0001.bvh");
		sgAnimationComponent *animComp = (sgAnimationComponent*)player->createComponent(sgAnimationComponent::GetClassTypeName());
		animComp->setAnimationFile(pAnimation->getFilename());
		animComp->setPlayMode(sgAnimationComponent::PM_LOOP);
		//animComp->play();
		

		mScene->setPhysicsEnabled(true);
		mScene->setPhysicsContinuous(false);

/*		std::vector<std::string> aa;
		
		for(int i=0; i<100; ++i)
		{
			aa.push_back("nice");

		}

		for(int i=0; i<100; ++i)
		{
			std::cout << i;
			std::cout << aa[i];
			
			aa[i] = "xx";
		}

		AA();

		int a = 11;
		int b = 12;
		int *val = new int[5];
		for(int i=0; i<5; ++i)
		{
			val[i] = i;
		}


		//_asm INT 3; //调用断点中断
		//std::cout << "Hello world!";
		_asm INT 3; //调用断点中断

		int i = 0;

		_asm INT 3; //调用断点中断
		*/
	}
}

void DemoRagdoll::keyPressEvent( sgKeyEvent &event )
{
	if(mScene && mCamera)
	{
		switch(event.key)
		{
		case 'n':
		case 'N':
			mScene->stepPhysics(0.103);
			return ;
		case 'v':
		case 'V':
			if(ragdoll)
			{
				ragdoll->setVisible(!(ragdoll->isVisible()));
			}
			return ;
		}
	}

	sgDemo::keyPressEvent(event);
}