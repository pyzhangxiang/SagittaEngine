#include "MainWindow.h"
#include <gui/SGraphicsView.h>
#include <gui/SEvent.h>
#include <engine/scenegraph/SSceneManager.h>
#include <engine/scenegraph/SEntity.h>
#include <engine/scenegraph/SSceneNode.h>
#include <engine/scenegraph/SGrid.h>
#include <engine/scenegraph/SCamera.h>
#include <engine/scenegraph/SLight.h>
#include <engine/scenegraph/SSimpleEntities.h>
#include <engine/scenegraph/SSkeleton.h>
#include <engine/renderer/SRenderOption.h>
#include <engine/renderer/SRenderer.h>
#include <engine/renderer/SViewport.h>
#include <engine/io/SFile.h>
#include <vector>
using namespace Sagitta;

MainWindow::MainWindow() : SGraphicsFrame("SagiEditor", SRect(200, 200, 400, 400)){
	/*
									root
								/	   |				\			\
						gridnode    rolenode		trianglenode	lightnode
									  /		
								tanknode	

	*/

//	HIViewSetText(handle(), CFSTR("aaaaaa"));
	if(m_pSceneManager){
		
		SSceneManager *m_pUniqueSM = m_pSceneManager;

		SVertex v0(Vector3(0.0f, 2.0f, -20.0f));
		v0.setColor(Color(Vector3(1, 0, 0)));
		SVertex v1(Vector3(-2.0f, -2.0f, -20.0f));
		v1.setColor(Color(Vector3(0, 1, 0)));
		SVertex v2(Vector3(2.0f, -2.0f, -20.0f));
		v2.setColor(Color(Vector3(0, 0, 1)));
		STriangle *triangle = new STriangle(v0, v1, v2);

//		SGrid *grid = new SGrid(20, 10, 10); 

	//	SCube *cube = new SCube(5);
	//	SSphere *cube = new SSphere(5, 20, 20);
		SCone *tank = new SCone(5, 20, 10);
		tank->setMaterial(SMaterial(Color(Color::GLColor(0.5, 0.3, 0.1, 1.0)),
									Color(Color::GLColor(0.5, 0.3, 0.1, 1.0)),
									Color(Color::GLColor(0.7, 0.7, 0.7, 1.0)),
									Color(Color::GLColor(0.0, 0.0, 0.0, 1.0)),
									0.0, 0.5, 0.0));

		SSceneNode *gridnode = m_pUniqueSM->createSceneNode("gridnode");
		SSceneNode *rolenode = m_pUniqueSM->createSceneNode("rolenode");
		SSceneNode *tanknode = m_pUniqueSM->createSceneNode("tanknode");
		SSceneNode *cameranode = m_pUniqueSM->createSceneNode("cameranode");
		SSceneNode *lightnode = m_pUniqueSM->createSceneNode("lightnode");
		SSceneNode *trianglenode = m_pUniqueSM->createSceneNode("trianglenode");
		tanknode->stepToParent(rolenode);
		cameranode->stepToParent(rolenode); 


		lightnode->translate(Vector3(0, 20, -30));
		rolenode->translate(Vector3(0, 0, 6/*-10*/));
		tanknode->pitch(Radian(-Math::PI_DIV_2));
		tanknode->translate(Vector3(-5, -17, -40));


		trianglenode->attachEsse(triangle);

	//	grid->attachToNode(gridnode);

		tank->attachToNode(tanknode);
		
		SCamera *camera = new SCamera(Vector3(0, 1, 0), Vector3(0, 0, -1), SCamera::CM_FIXER);
		rolenode->attachEsse(camera);
		static_cast<SGraphicsView*>(getChild(0))->getRenderer()->getViewport(0)->setCamera(camera);
		
		SLight *light0 = new SLight;
		SCube *lightCube = new SCube("LightCube", 1);
		light0->attachToNode(lightnode);
		lightCube->attachToNode(lightnode);

		//	m_pUniqueSM->loadSceneObj("F:\\Game Design\\models\\obj\\jiedao.obj");
		m_pUniqueSM->load("../../resource/models/jiedao.obj"/*, SScene::LOP_REFRESH*/);

		// test motion data
		SSkeleton *skeleton = new SSkeleton("skeleton");
		skeleton->load("../../resource/animations/GHBW_0001.bvh");
		skeleton->setLoopFrame(true);
		m_pUniqueSM->addSubScene(skeleton);

		int aa = 0;
		//SLight *light1 = new SLight;
		//light1->attachToNode(lightNode1);
	}
}

MainWindow::~MainWindow(){

}

bool MainWindow::onKeyDown(uLong aKey, uLong aRepeatCount, uLong aModifiers){
	bool res = true;

	switch(aKey){
		case Sagitta::Key_Esc:
			sendEvent(SEvent(SEvent::ET_DESTROY, 0, 0, this), this);
			break;

		case Sagitta::Key_W:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->translate(Vector3(0, 0, -2));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_S:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->translate(Vector3(0, 0, 2));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_A:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->translate(Vector3(-2, 0, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_D:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->translate(Vector3(2, 0, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Up:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->rotationX(Radian(-Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Down:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->rotationX(Radian(Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Left:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->rotationY(Radian(Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Right:
			static_cast<SCamera*>(m_pSceneManager->getTypedEsse(SCamera::SET_CAMERA, 0))->rotationY(Radian(-Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_L:
			{
				SRenderOption &rop = m_pSceneManager->globalRenderOption();
				rop.setLightEnable(!rop.isLightEnable());
				invalidateRect(0, false);
			}
			break;

		case Sagitta::Key_U:
			m_pSceneManager->getTypedEsse(SLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, 0, -10));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_J:
			m_pSceneManager->getTypedEsse(SLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, 0, 10));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Y:
			m_pSceneManager->getTypedEsse(SLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, 10, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_I:
			m_pSceneManager->getTypedEsse(SLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, -10, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_H:
			m_pSceneManager->getTypedEsse(SLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(-10, 0, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_K:
			m_pSceneManager->getTypedEsse(SLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(10, 0, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_N:
			m_pSceneManager->stepFrame(1);
			invalidateRect(0, false);
			break;

		case Sagitta::Key_P:
			m_pSceneManager->stepFrame(-1);
			invalidateRect(0, false);
			break;

		default:
			res = false;
			break;
	}

	return res;

}