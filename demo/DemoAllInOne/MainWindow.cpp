#include "MainWindow.h"
#include <gui/SGraphicsView.h>
#include <gui/SEvent.h>
#include <engine/scenegraph/sgSceneManager.h>
#include <engine/scenegraph/sgEntity.h>
#include <engine/scenegraph/sgSceneNode.h>
#include <engine/scenegraph/sgGrid.h>
#include <engine/scenegraph/sgCamera.h>
#include <engine/scenegraph/sgLight.h>
#include <engine/scenegraph/sgSimpleEntities.h>
#include <engine/scenegraph/sgSkeleton.h>
#include <engine/renderer/sgRenderOption.h>
#include <engine/renderer/sgRenderer.h>
#include <engine/renderer/sgViewport.h>
#include <engine/io/sgFile.h>
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
		
		sgSceneManager *m_pUniqueSM = m_pSceneManager;

		SVertex v0(Vector3(0.0f, 2.0f, -20.0f));
		v0.setColor(Color(Vector3(1, 0, 0)));
		SVertex v1(Vector3(-2.0f, -2.0f, -20.0f));
		v1.setColor(Color(Vector3(0, 1, 0)));
		SVertex v2(Vector3(2.0f, -2.0f, -20.0f));
		v2.setColor(Color(Vector3(0, 0, 1)));
		//STriangle *triangle = new STriangle(v0, v1, v2);

//		sgGrid *grid = new sgGrid(20, 10, 10); 

	//	sgCube *cube = new sgCube(5);
	//	sgSphere *cube = new sgSphere(5, 20, 20);
	//	sgCone *tank = new sgCone(5, 20, 10);
	/*	tank->setMaterial(sgMaterial(Color(Color::GLColor(0.5, 0.3, 0.1, 1.0)),
									Color(Color::GLColor(0.5, 0.3, 0.1, 1.0)),
									Color(Color::GLColor(0.7, 0.7, 0.7, 1.0)),
									Color(Color::GLColor(0.0, 0.0, 0.0, 1.0)),
									0.0, 0.5, 0.0));
*/
		sgSceneNode *gridnode = m_pUniqueSM->createSceneNode("gridnode");
		sgSceneNode *rolenode = m_pUniqueSM->createSceneNode("rolenode");
		sgSceneNode *tanknode = m_pUniqueSM->createSceneNode("tanknode");
		sgSceneNode *cameranode = m_pUniqueSM->createSceneNode("cameranode");
		sgSceneNode *lightnode = m_pUniqueSM->createSceneNode("lightnode");
		sgSceneNode *trianglenode = m_pUniqueSM->createSceneNode("trianglenode");
		tanknode->stepToParent(rolenode);
		cameranode->stepToParent(rolenode); 


		lightnode->translate(Vector3(0, 20, -30));
		rolenode->translate(Vector3(0, 0, 6/*-10*/));
		tanknode->pitch(Radian(-Math::PI_DIV_2));
		tanknode->translate(Vector3(-5, -17, -40));


		//trianglenode->attachEsse(triangle);

	//	grid->attachToNode(gridnode);

	//	tank->attachToNode(tanknode);
		
		sgCamera *camera = new sgCamera(Vector3(0, 1, 0), Vector3(0, 0, -1), sgCamera::CM_FIXER);
		rolenode->attachEsse(camera);
        camera->rotate(Quaternion(-Math::PI/10.0f, Vector3(1.0f, 0.0f, 0.0f)));
        camera->translate(Vector3(0.0f, 30.0f, 160.0f));
//        camera->roll(Radian(Math::PI/4.0f));
		static_cast<SGraphicsView*>(getChild(0))->getRenderer()->getViewport(0)->setCamera(camera);
		
		sgLight *light0 = new sgLight;
		sgCube *lightCube = new sgCube("LightCube", 1);
		light0->attachToNode(lightnode);
		lightCube->attachToNode(lightnode);

		//	m_pUniqueSM->loadSceneObj("F:\\Game Design\\models\\obj\\jiedao.obj");
		m_pUniqueSM->load("../../resource/models/jiedao.obj"/*, sgScene::LOP_REFRESH*/);

		// test motion data
		sgSkeleton *skeleton = new sgSkeleton("skeleton");
		skeleton->load("../../resource/animations/GHBW_0001.bvh");
		skeleton->setLoopFrame(true);
		m_pUniqueSM->addSubScene(skeleton);

		//sgLight *light1 = new sgLight;
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
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->translate(Vector3(0, 0, -2));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_S:
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->translate(Vector3(0, 0, 2));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_A:
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->translate(Vector3(-2, 0, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_D:
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->translate(Vector3(2, 0, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Up:
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->rotationX(Radian(-Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Down:
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->rotationX(Radian(Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Left:
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->rotationY(Radian(Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Right:
			static_cast<sgCamera*>(m_pSceneManager->getTypedEsse(sgCamera::SET_CAMERA, 0))->rotationY(Radian(-Math::PI / 16));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_L:
			{
				sgRenderOption &rop = m_pSceneManager->globalRenderOption();
				rop.setLightEnable(!rop.isLightEnable());
				invalidateRect(0, false);
			}
			break;

		case Sagitta::Key_U:
			m_pSceneManager->getTypedEsse(sgLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, 0, -10));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_J:
			m_pSceneManager->getTypedEsse(sgLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, 0, 10));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_Y:
			m_pSceneManager->getTypedEsse(sgLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, 10, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_I:
			m_pSceneManager->getTypedEsse(sgLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(0, -10, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_H:
			m_pSceneManager->getTypedEsse(sgLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(-10, 0, 0));
			invalidateRect(0, false);
			break;

		case Sagitta::Key_K:
			m_pSceneManager->getTypedEsse(sgLight::SET_LIGHT, 0)->attachedNode()->translate(Vector3(10, 0, 0));
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