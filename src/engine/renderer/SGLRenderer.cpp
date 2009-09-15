//////////////////////////////////////////////////////
// file: SGLRenderer.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class SGLRenderer
// SGLRenderer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SGLRenderer.h"
#include "SViewport.h"
#include "../buffer/SVertexData.h"
#include "../buffer/SVertexIndexBuffer.h"
#include "../buffer/SVertexBufferElement.h"
#include "../scenegraph/SSceneManager.h"
#include "../scenegraph/SRenderQueue.h"
#include "../scenegraph/SCamera.h"
#include "../scenegraph/SRenderable.h"
#include "../scenegraph/SLight.h"
#include "../scenegraph/SMaterial.h"
#include "../../common/utils/SException.h"
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
#	include <gl/glew.h>
#	include <gl/glut.h>
#elif SAGITTA_PLATFORM == SAGITTA_PLATFORM_APPLE
#	include <OpenGL/glu.h>
#	include <OpenGL/glext.h>
#else
#endif

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/1/2008 zhangxiang]
	SGLRenderer::SGLRenderer(int aTWidth, int aTHeight) :
	SRenderer(aTWidth, aTHeight, false){
		
	}

	//  [8/1/2008 zhangxiang]
	SGLRenderer::~SGLRenderer(void){
		
	}

	//  [8/1/2008 zhangxiang]
	void SGLRenderer::init(void){
		glShadeModel(GL_SMOOTH);
		glClearDepth(1.0f);

		// set enable
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);                            
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}

	//  [8/1/2008 zhangxiang]
/*	void SGLRenderer::resize(int aWidth, int aHeight){
		SRenderer::resize(aWidth, aHeight);

		ViewportList::const_reverse_iterator rit = m_ViewportList.rbegin();
		ViewportList::const_reverse_iterator reit = m_ViewportList.rend();
		for(; rit!=reit; ++rit){
			SViewport *res = rit->second;
			glViewport(res->actleft(), res->acttop(), res->actwidth(), res->actheight());
		}

	//	glMatrixMode(GL_PROJECTION);                        
	//	glLoadIdentity();
	////	glMultMatrixf(projectionMatrix.arr());
	//	gluPerspective(90.0f,(GLfloat)width/(GLfloat)height,0.1f,10000.0f);
	//	glMatrixMode(GL_MODELVIEW);                            
	//	glLoadIdentity();
	}
*/
	//  [1/15/2009 zhangxiang]
	void SGLRenderer::setViewport(SViewport *aViewport) const{
		glViewport(aViewport->actleft(), aViewport->acttop(), aViewport->actwidth(), aViewport->actheight());
	}

	//  [1/15/2009 zhangxiang]
	void SGLRenderer::clearFrameBuffers(uInt aFlags, const Color &aBkColor,
									Real aBkDepth, int aBkStencil) const{
		GLbitfield gl_flags = 0;
		if(aFlags & SViewport::CB_COLOR){
			gl_flags |= GL_COLOR_BUFFER_BIT;
			glClearColor(aBkColor.redF(), aBkColor.greenF(), aBkColor.blueF(), aBkColor.alphaF());
		}
		if(aFlags & SViewport::CB_DEPTH){
			gl_flags |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(aBkDepth);
		}
		if(aFlags & SViewport::CB_STENCIL){
			gl_flags |= GL_STENCIL_BUFFER_BIT;
			glClearStencil(aBkStencil);
		}
		glClear(gl_flags);
	}

	//  [1/15/2009 zhangxiang]
	void SGLRenderer::setProjMatrix(const Matrix4 &aMatrix) const{
		glMatrixMode(GL_PROJECTION);                        
		glLoadIdentity();
		glMultMatrixf(aMatrix.arr());
		
		glMatrixMode(GL_MODELVIEW);
	}

	//  [1/15/2009 zhangxiang]
	void SGLRenderer::setViewMatrix(const Matrix4 &aMatrix) const{
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		Matrix4 mat(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, -1, 0,
					0, 0, 0, 1);
		glMultMatrixf((aMatrix.transpose().arr()));//.transpose() * mat).arr());
	}

	//  [1/10/2009 zhangxiang]
	int SGLRenderer::setupLightsImpl(const Color &aGlobalAmbiantColor) const{
		uInt LIGHT_INDEX = GL_LIGHT0;
		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGlobalAmbiantColor.toGLColor().data());

		SLight *light;
		LightList::const_iterator lit = m_LightList.begin();
		LightList::const_iterator leit = m_LightList.end();
		// OpenGL only support 8 light at most
		int lightNum = 0;
		for(; lightNum<9 && leit!=lit; ++lightNum, ++lit){
			light = *lit;

			glLightfv(LIGHT_INDEX, GL_POSITION, Vector4(light->position()).data());
			glLightfv(LIGHT_INDEX, GL_AMBIENT, light->ambientColor().toGLColor().data());
			glLightfv(LIGHT_INDEX, GL_DIFFUSE, light->diffuseColor().toGLColor().data());
			glLightfv(LIGHT_INDEX, GL_SPECULAR, light->specularColor().toGLColor().data());

			glEnable(LIGHT_INDEX++);
		}

		return lightNum;
	}

	//  [1/15/2009 zhangxiang]
	void SGLRenderer::render(const SRenderOption &aGlobalRop, SRenderable *aRenderable) const{
		// setup material
		if(aGlobalRop.isLightEnable()){
			SMaterial material = aRenderable->material();
			glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambientColor().toGLColor().data());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuseColor().toGLColor().data());
			glMaterialfv(GL_FRONT, GL_SPECULAR, material.specularColor().toGLColor().data());
			glMaterialf(GL_FRONT, GL_SHININESS, material.shininess());
			glMaterialfv(GL_FRONT, GL_EMISSION, material.emissionColor().toGLColor().data());
		}


		SVertexData *pvb = new SVertexData();
		SVertexIndexBuffer *pvib = new SVertexIndexBuffer(SVertexBufferElement::ET_VERTEX);
		aRenderable->getVertexBuffer(pvb, pvib);

		// render
		SVertexData::ConstIterator elemIt = pvb->getConstIterator();
		for(; elemIt.hasMoreElements(); elemIt++){
			SVertexBufferElement *element = elemIt.value();
			switch(element->type()){
					case SVertexBufferElement::ET_VERTEX:
						{
							glVertexPointer(element->coordNum(), GL_FLOAT, 0, element->data());
							glEnableClientState(GL_VERTEX_ARRAY);
							break;
						}

					case SVertexBufferElement::ET_COLOR:
						{
							element->_colorConversion();
							glColorPointer(element->coordNum(), GL_UNSIGNED_BYTE, 0, element->data());
							glEnableClientState(GL_COLOR_ARRAY);
							break;
						}

					case SVertexBufferElement::ET_NORMAL:
						{
							glNormalPointer(GL_FLOAT, 0, element->data());
							glEnableClientState(GL_NORMAL_ARRAY);
							break;
						}

					case SVertexBufferElement::ET_TEXTURE_COORD:
						{
							glTexCoordPointer(element->coordNum(), GL_FLOAT, 0, element->data());
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							break;
						}

					case SVertexBufferElement::ET_FOG_COORDINATE:
						{
							glFogCoordPointer(GL_FLOAT, 0, element->data());
							glEnableClientState(GL_FOG_COORDINATE_ARRAY);
							break;
						}

					default:
						{
							THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "Invalid render state.", "SGLRenderSystem::render");
							break;
						}
			}
		}

		// model transform
		Matrix4 modelMatrix = aRenderable->getModelMatrix();
		glPushMatrix();
		glMultMatrixf(modelMatrix.transpose().arr());

		glDrawElements(retMapping(aRenderable->renderOption().renderElementType()), pvib->dataNum(), GL_UNSIGNED_INT, pvib->data());
		
		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_FOG_COORDINATE_ARRAY);

		delete pvb;
		delete pvib;
	}

	//  [1/15/2009 zhangxiang]
	void SGLRenderer::postRenderImpl(void) const{
		
	}

	//  [1/10/2009 zhangxiang]
	void SGLRenderer::resetLights(int aLightNum) const{
		int LIGHT_INDEX = GL_LIGHT0;
		for(int i=0; i<aLightNum; ++i){
			glDisable(LIGHT_INDEX++);
		}
		glDisable(GL_LIGHTING);
	}

	//  [8/1/2008 zhangxiang]
	int SGLRenderer::retMapping(int aRet) const{
		switch(aRet){
			case SRenderOption::RET_POINTS:
				return GL_POINTS;

			case SRenderOption::RET_LINES:
				return GL_LINES;

			case SRenderOption::RET_LINE_STRIP:
				return GL_LINE_STRIP;

			case SRenderOption::RET_LINE_LOOP:
				return GL_LINE_LOOP;

			case SRenderOption::RET_TRIANGLES:
				return GL_TRIANGLES;

			case SRenderOption::RET_TRIANGLE_STRIP:
				return GL_TRIANGLE_STRIP;

			case SRenderOption::RET_TRIANGLE_FAN:
				return GL_TRIANGLE_FAN;

			case SRenderOption::RET_QUADS:
				return GL_QUADS;

			default:
				THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
								"Unknown render element type.",
								"SGLRender::retMapping");

		}
	}

} // namespace Sagitta