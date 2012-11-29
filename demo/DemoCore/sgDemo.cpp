//
//  sgDemo.cpp
//  Sagitta
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "sgDemo.h"
#include <engine/sgEngine.h>
#include <engine/scenegraph/sgScene.h>
#include <engine/renderer/sgGLRenderer.h>
#include <engine/renderer/SagiRenderer.h>
#include <engine/renderer/sgViewport.h>
#include <engine/renderer/sgRenderTechnique.h>
#include <engine/renderer/sgRenderPass.h>
#include <engine/renderer/sgRenderTarget.h>
#include <engine/component/sgCameraComponent.h>
#include <engine/component/sgLightComponent.h>
#include <engine/common/sgLogSystem.h>
#include <math/sgMathHeader.h>
#include <iostream>

using namespace Sagitta;

void demoLogHandler(int type, const std::string &category, const std::string &content)
{
	std::string strType = "";
	if(type == sgLogSystem::LT_INFO)
		strType = "Info";
	else if(type == sgLogSystem::LT_WARNING)
		strType = "Warning";
	else if(type == sgLogSystem::LT_ERROR)
		strType = "Error";

	std::cout << "<" << category << "> [" << strType << "] -- " << content << "\n";
}

sgDemo *gApp = 0;

sgDemo *GetApp(void)
{
    return gApp;
}

sgDemo::sgDemo(const std::string &winTitle,
               const std::string &assetRootDir)
: mFrameRate(1.0f/30.0f)
, mRenderWindowX(-1)
, mRenderWindowY(-1)
, mRenderWindowW(400)
, mRenderWindowH(300)
, mRenderWindowTitle(winTitle)
, mDestroying(false)
, mAssetRootDir(assetRootDir)
, mScene(NULL), mTargetRoot(NULL)
, mCamera(NULL), mLight(NULL)
{
    if(gApp)
    {
        delete gApp;
        gApp = 0;
    }
    gApp = this;
}

sgDemo::~sgDemo(void)
{
    gApp = 0;
    
    // destroy the scene
    sgObject::destroyObject(mLight);
    mLight = 0;
    
    sgObject::destroyObject(mCamera);
    mCamera = 0;
    
	sgObject::destroyObject(mTargetRoot);
	mTargetRoot = 0;

    sgObject::destroyObject(mScene);
    mScene = 0;
}

float sgDemo::getFrameRate(void) const
{
    return mFrameRate;
}

void sgDemo::setFrameRate( float fr )
{
	mFrameRate = fr;
	if(mFrameRate < 0)
		mFrameRate = 1.0f / 30.0f;
}

void sgDemo::setWindowTitle(const std::string title)
{
	mRenderWindowTitle = title;
}

void sgDemo::setWindowSize( int width, int height )
{
	mRenderWindowW = width;
	mRenderWindowH = height;
}

void sgDemo::setWindowPos( int x, int y )
{
	mRenderWindowX = x;
	mRenderWindowY = y;
}

void sgDemo::OnCreate( void )
{
	Sagitta::sgEngineInit(sgGLRenderer::GetClassName(), mAssetRootDir);
	sgLogSystem::instance()->addLogHandler(demoLogHandler);
	createEvent();
}

void sgDemo::OnDestroy( void )
{
	mDestroying = true;
    destroyEvent();
	Sagitta::sgEngineShutdown();
}

void sgDemo::initialize( void )
{
	std::cout << "sgDemo::initialize, initialize render and scene\n";

	sgGetRenderer()->init();
	/*sgViewport *viewport = sgGetRenderer()->createViewport(
		getRenderWindowWidth()
		, getRenderWindowHeight()
		, 0.0, 0.0, 1.0, 1.0, 0, 0);*/
	sgRenderTechnique *technique = sgGetRenderer()->getRenderTechnique();
	sgRenderTarget *rt = technique->getRenderPass(0)->getRenderTarget();
	sgViewport *viewport = rt->getViewport();
	rt->resize(getRenderWindowWidth(), getRenderWindowHeight());
	viewport->setBackColor(Color::DARKGRAY);

	sgCameraComponent *cameraComp = (sgCameraComponent*)mCamera->getComponent(sgCameraComponent::GetClassName());
	viewport->setCamera(cameraComp);

	this->prepare();
}

void sgDemo::update( float dt )
{
	if(mScene)
	{
		mScene->update(dt);
		sgLogSystem::instance()->update(dt);
	}
}

void sgDemo::render( void )
{
	sgGetRenderer()->render();
}

void sgDemo::resize( int width, int height )
{
	std::stringstream ss;
	ss << "sgDemo::resize, window resize(" << width << ", " << height << ")";
	sgLogSystem::instance()->info(ss.str());

	mRotationSphereRadius = (Math::Sqrt(width*width + height*height) / 2) / Math::Sin(Math::PI_DIV_3);
	mRotationSphereCenter = Vector3(width/2.0f, height / 2.0f, mRotationSphereRadius / 2);

	sgGetRenderer()->resize(width, height);
}

void sgDemo::createEvent( void )
{
	std::cout << "sgDemo::createEvent, create the scene\n";

	mScene = (sgScene*)sgObject::createObject(sgScene::GetClassName());
	mTargetRoot = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
	mTargetRoot->setParent(mScene->getRoot());
	mCamera = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
	mCamera->setParent(mScene->getRoot());
	sgCameraComponent *cameraComp = (sgCameraComponent*)mCamera->createComponent(sgCameraComponent::GetClassName());

	cameraComp->setUpDirection(Vector3::UNIT_Y);
	cameraComp->setShootDirection(Vector3(0.0f, 0.0f, -1.0f));
}

void sgDemo::destroyEvent( void )
{
	std::cout << "sgDemo::createEvent, destroy the scene\n";

	sgObject::destroyObject(mLight);
	mLight = 0;
	sgObject::destroyObject(mCamera);
	mCamera = 0;
	sgObject::destroyObject(mTargetRoot);
	mTargetRoot = 0;
	sgObject::destroyObject(mScene);
	mScene = 0;
}

void sgDemo::keyPressEvent( sgKeyEvent &event )
{
	if(event.key == Sagitta::Key_Esc)
	{
		//sendEvent(SEvent(SEvent::ET_DESTROY, 0, 0, this), this);
		;
	}
	if(mScene && mCamera)
	{
		switch(event.key)
		{
		case 'W':
		case 'w':
            mCamera->translate(Vector3(0, 0, -2), sgNode::TS_LOCAL);
			break;

		case 'S':
		case 's':
			mCamera->translate(Vector3(0, 0, 2), sgNode::TS_LOCAL);
			break;

		case 'A':
		case 'a':
			mCamera->translate(Vector3(-2, 0, 0), sgNode::TS_LOCAL);
			break;

		case 'D':
		case 'd':
			mCamera->translate(Vector3(2, 0, 0), sgNode::TS_LOCAL);
			break;

		case Sagitta::Key_Up:
			mCamera->rotationX(Radian(-Math::PI / 16));
			break;

		case Sagitta::Key_Down:
			mCamera->rotationX(Radian(Math::PI / 16));
			break;

		case Sagitta::Key_Left:
			mCamera->rotationY(Radian(Math::PI / 16));
			break;

		case Sagitta::Key_Right:
			mCamera->rotationY(Radian(-Math::PI / 16));
			break;

		default:
			break;
		}
	}

	if(mScene && mLight)
	{
		switch(event.key)
		{
		case 'U':
        case 'u':
			mLight->translate(Vector3(0, 0, -10));
			break;

		case 'J':
        case 'j':
			mLight->translate(Vector3(0, 0, 10));
			break;

		case 'Y':
        case 'y':
			mLight->translate(Vector3(0, 10, 0));
			break;

		case 'T':
        case 't':
			mLight->translate(Vector3(0, -10, 0));
			break;

		case 'H':
        case 'h':
			mLight->translate(Vector3(-10, 0, 0));
			break;

		case 'K':
        case 'k':
			mLight->translate(Vector3(10, 0, 0));
			break;

		default:
			break;
		}
	}
	
}

void sgDemo::mousePressEvent( sgMouseEvent &event )
{
	if(mScene && mCamera && mTargetRoot)
	{
		if(event.buttons == Sagitta::MBT_Right)
		{
			mStartRotRayX = Vector3(event.x, mRotationSphereCenter.y(), 0.0f) - mRotationSphereCenter;
			mStartRotRayY = Vector3(mRotationSphereCenter.x(), event.y, 0.0f) - mRotationSphereCenter;
			mStartRotOrientation = mCamera->relativeOrentation();
		}
		else if(event.buttons == Sagitta::MBT_Left)
		{
			mStartRotRayTarget = Vector3(event.x, event.y, 0.0f) - mRotationSphereCenter;
			mStartRotOrientationTarget = mTargetRoot->relativeOrentation();
		}
	}
	
}

void sgDemo::mouseMoveEvent( sgMouseEvent &event )
{
	if(mScene && mCamera && mTargetRoot)
	{
		if(event.buttons == Sagitta::MBT_Right)
		{
			Vector3 currentRayX = Vector3(event.x, mRotationSphereCenter.y(), 0.0f) - mRotationSphereCenter;
			Vector3 currentRayY = Vector3(mRotationSphereCenter.x(), event.y, 0.0f) - mRotationSphereCenter;
			Quaternion q1 = mStartRotRayX.getRotationTo(currentRayX);
			Quaternion q2 = mStartRotRayY.getRotationTo(currentRayY);
			// Y axes is in world coordination, and X axes is in local coordination
			mCamera->setRelativeOrientation(q1 * mStartRotOrientation * q2);

			//mCamera->rotationX(Radian(-event.deltaY / 100.0f));
			//mCamera->rotationY(Radian(-event.deltaX / 100.0f));
		}
		else if(event.buttons == Sagitta::MBT_Left)
		{
			Vector3 currentRay = Vector3(event.x, event.y, 0.0f) - mRotationSphereCenter;
			Quaternion q = currentRay.getRotationTo(mStartRotRayTarget);
			mTargetRoot->setRelativeOrientation(q * q * mStartRotOrientationTarget);
		}
	}
}