//////////////////////////////////////////////////////
// file: sgGLRenderer.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class sgGLRenderer
// sgGLRenderer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgGLRenderer.h"
#include "sgViewport.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/common/sgException.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/component/sgLightComponent.h"
#include "engine/component/sgRenderStateComponent.h"
#include "engine/component/sgMeshComponent.h"
#include "engine/resource/sgMaterial.h"
#include "engine/resource/sgMesh.h"
#include "sgGLInclude.h"
#include "sgGLUtil.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
    
    SG_META_DEFINE(sgGLRenderer, sgRenderer)

	//  [8/1/2008 zhangxiang]
	sgGLRenderer::sgGLRenderer()
	: sgRenderer()
    {
		// init uniform func map
        mUniformFuncMap[DT_F] = setUniform1f;
        mUniformFuncMap[DT_FV2] = setUniform2fv;
        mUniformFuncMap[DT_FV3] = setUniform3fv;
        mUniformFuncMap[DT_FV4] = setUniform4fv;
        
        mUniformFuncMap[DT_I] = setUniform1i;
        mUniformFuncMap[DT_IV2] = setUniform2iv;
        mUniformFuncMap[DT_IV3] = setUniform3iv;
        mUniformFuncMap[DT_IV4] = setUniform4iv;
        /*
        mUniformFuncMap[DT_UI] = setUniform1ui;
        mUniformFuncMap[DT_UIV2] = setUniform2uiv;
        mUniformFuncMap[DT_UIV3] = setUniform3uiv;
        mUniformFuncMap[DT_UIV4] = setUniform4uiv;
        */
        /*
        mUniformFuncMap[DT_B] = setUniform1b;
        mUniformFuncMap[DT_BV2] = setUniform2bv;
        mUniformFuncMap[DT_BV3] = setUniform3bv;
        mUniformFuncMap[DT_BV4] = setUniform4bv;
        */
        
        mUniformFuncMap[DT_FM22] = setUniformMatrixf22;
        mUniformFuncMap[DT_FM33] = setUniformMatrixf33;
        mUniformFuncMap[DT_FM44] = setUniformMatrixf44;
	}

	//  [8/1/2008 zhangxiang]
	sgGLRenderer::~sgGLRenderer(void){
		
	}

	//  [8/1/2008 zhangxiang]
	void sgGLRenderer::init(void){
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
/*	void sgGLRenderer::resize(int aWidth, int aHeight){
		sgRenderer::resize(aWidth, aHeight);

		ViewportList::const_reverse_iterator rit = m_ViewportList.rbegin();
		ViewportList::const_reverse_iterator reit = m_ViewportList.rend();
		for(; rit!=reit; ++rit){
			sgViewport *res = rit->second;
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
	void sgGLRenderer::setViewport(sgViewport *aViewport) const{
		glViewport(aViewport->actleft(), aViewport->acttop(), aViewport->actwidth(), aViewport->actheight());
	}

	//  [1/15/2009 zhangxiang]
	void sgGLRenderer::clearFrameBuffers(uInt aFlags, const Color &aBkColor,
									Real aBkDepth, int aBkStencil) const{
		GLbitfield gl_flags = 0;
		if(aFlags & sgViewport::CB_COLOR){
			gl_flags |= GL_COLOR_BUFFER_BIT;
			glClearColor(aBkColor.redF(), aBkColor.greenF(), aBkColor.blueF(), aBkColor.alphaF());
		}
		if(aFlags & sgViewport::CB_DEPTH){
			gl_flags |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(aBkDepth);
		}
		if(aFlags & sgViewport::CB_STENCIL){
			gl_flags |= GL_STENCIL_BUFFER_BIT;
			glClearStencil(aBkStencil);
		}
		glClear(gl_flags);
	}

	//  [1/15/2009 zhangxiang]
	void sgGLRenderer::setProjMatrix(const Matrix4 &aMatrix) const{
		glMatrixMode(GL_PROJECTION);                        
		glLoadIdentity();
		glMultMatrixf(aMatrix.arr());
        
//        gluPerspective(60,800/600.0,1,10000);
        
        
		
		glMatrixMode(GL_MODELVIEW);
	}

	//  [1/15/2009 zhangxiang]
	void sgGLRenderer::setViewMatrix(const Matrix4 &aMatrix) const{
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		/*Matrix4 mat(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, -1, 0,
					0, 0, 0, 1);*/
		glMultMatrixf((aMatrix.transpose().arr()));//.transpose() * mat).arr());
    //    gluLookAt(0,0,8,0,0,-1,0,1,0);
	}

	//  [1/10/2009 zhangxiang]
	int sgGLRenderer::setupLightsImpl(const Color &aGlobalAmbiantColor) const{
		uInt LIGHT_INDEX = GL_LIGHT0;
		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGlobalAmbiantColor.toGLColor().data());

		sgLightComponent *light;
		sg_render::LightList::const_iterator lit = m_CurRenderParam.lightlist.begin();
		sg_render::LightList::const_iterator leit = m_CurRenderParam.lightlist.end();
		// OpenGL only support 8 light at most
		int lightNum = 0;
		for(; lightNum<9 && leit!=lit; ++lightNum, ++lit){
			light = *lit;

			glLightfv(LIGHT_INDEX, GL_POSITION, Vector4(light->getParent()->position()).data());
			glLightfv(LIGHT_INDEX, GL_AMBIENT, light->ambientColor().toGLColor().data());
			glLightfv(LIGHT_INDEX, GL_DIFFUSE, light->diffuseColor().toGLColor().data());
			glLightfv(LIGHT_INDEX, GL_SPECULAR, light->specularColor().toGLColor().data());

			glEnable(LIGHT_INDEX++);
		}

		return lightNum;
	}

	//  [1/15/2009 zhangxiang]
	void sgGLRenderer::render(const sgRenderState &aGlobalRop, sgSceneObject *aRenderable) const{
        sgMeshComponent *meshComp = (sgMeshComponent*)(aRenderable->getComponent(sgMeshComponent::GetClassName()));
        if(!meshComp)
            return ;
        sgMesh *mesh = meshComp->getMesh();
        if(!mesh)
            return ;
        
        sgRenderStateComponent *renderState = (sgRenderStateComponent*)(aRenderable->getComponent(sgRenderStateComponent::GetClassName()));
        sgMaterial *material = 0;
        if(renderState)
            material = (sgMaterial*)(renderState->getMaterial());
        
		// setup material
		if( (renderState && material) && (aGlobalRop.isLightEnable() || 
           (renderState->getRenderState().isLightEnable())) )
        {
			glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambientColor().toGLColor().data());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuseColor().toGLColor().data());
			glMaterialfv(GL_FRONT, GL_SPECULAR, material->specularColor().toGLColor().data());
			glMaterialf(GL_FRONT, GL_SHININESS, material->shininess());
			glMaterialfv(GL_FRONT, GL_EMISSION, material->emissionColor().toGLColor().data());
		}

        sgVertexData *pvb = mesh->getVertexData(); //new sgVertexData();
		sgVertexIndexBuffer *pvib = mesh->getVertexIndexBuffer(); //new sgVertexIndexBuffer(sgVertexBufferElement::ET_VERTEX);
        //	mesh->getVertexBuffer(pvb, pvib);
        Matrix4 modelMatrix = aRenderable->getFullTransform();
        int polyType = retMapping(mesh->polyType());
		
		renderTraditionalPipeline(pvb, pvib, modelMatrix, polyType);

	//	delete pvb;
	//	delete pvib;
	}

	//  [1/15/2009 zhangxiang]
	void sgGLRenderer::postRenderImpl(void) const{
		
	}

	//  [1/10/2009 zhangxiang]
	void sgGLRenderer::resetLights(int aLightNum) const{
		int LIGHT_INDEX = GL_LIGHT0;
		for(int i=0; i<aLightNum; ++i){
			glDisable(LIGHT_INDEX++);
		}
		glDisable(GL_LIGHTING);
	}

	//  [8/1/2008 zhangxiang]
	int sgGLRenderer::retMapping(int aRet) const{
		switch(aRet){
			case sgMesh::RET_POINTS:
				return GL_POINTS;

			case sgMesh::RET_LINES:
				return GL_LINES;

		//	case sgRenderState::RET_LINE_STRIP:
		//		return GL_LINE_STRIP;

		//	case sgRenderState::RET_LINE_LOOP:
		//		return GL_LINE_LOOP;

			case sgMesh::RET_TRIANGLES:
				return GL_TRIANGLES;

		//	case sgRenderState::RET_TRIANGLE_STRIP:
		//		return GL_TRIANGLE_STRIP;

		//	case sgRenderState::RET_TRIANGLE_FAN:
		//		return GL_TRIANGLE_FAN;

			case sgMesh::RET_QUADS:
				return GL_QUADS;

			default:
				THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
								"Unknown render element type.",
								"SGLRender::retMapping");

		}
	}
    
    bool sgGLRenderer::initShaderEnvironment(void)
    {
        glewInit();
        bool gl2_0 = glewIsSupported("GL_VERSION_2_0") != 0;
        if(!gl2_0)
        {
            std::string log = "OpenGL 2.0 not supported\n";
        }
        return gl2_0;
    }
    
    void sgGLRenderer::renderTraditionalPipeline(sgVertexData *pvb, sgVertexIndexBuffer *pvib
                                                 , const Matrix4 &modelMatrix, int polyType) const
    {
        if(!pvb || !pvib)
            return ;
        
		// render
		sgVertexData::ConstIterator elemIt = pvb->getConstIterator();
		for(; elemIt.hasMoreElements(); elemIt++){
			sgVertexBufferElement *element = elemIt.value();
			switch(element->type()){
                case sgVertexBufferElement::ET_VERTEX:
                {
                    glVertexPointer(element->coordNum(), GL_FLOAT, 0, element->data());
                    glEnableClientState(GL_VERTEX_ARRAY);
                    break;
                }
                    
                case sgVertexBufferElement::ET_COLOR:
                {
                    //	element->_colorConversion();
                    glColorPointer(element->coordNum(), GL_UNSIGNED_BYTE, 0, element->data());
                    glEnableClientState(GL_COLOR_ARRAY);
                    break;
                }
                    
                case sgVertexBufferElement::ET_NORMAL:
                {
                    glNormalPointer(GL_FLOAT, 0, element->data());
                    glEnableClientState(GL_NORMAL_ARRAY);
                    break;
                }
                    
                case sgVertexBufferElement::ET_TEXTURE_COORD:
                {
                    glTexCoordPointer(element->coordNum(), GL_FLOAT, 0, element->data());
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    break;
                }
                    
                case sgVertexBufferElement::ET_FOG_COORDINATE:
                {
                    glFogCoordPointer(GL_FLOAT, 0, element->data());
                    glEnableClientState(GL_FOG_COORDINATE_ARRAY);
                    break;
                }
                    
                default:
                {
                    THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE, "Invalid render state.", "SGLRenderSystem::render");
                    break;
                }
			}
		}
        
		// model transform
		glPushMatrix();
		glMultMatrixf(modelMatrix.transpose().arr());
        
		glDrawElements(polyType, pvib->dataNum(), GL_UNSIGNED_INT, pvib->data());
		
		glPopMatrix();
        
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_FOG_COORDINATE_ARRAY);
    }
    
    void sgGLRenderer::renderProgramPipeline(sgVertexData *pvb, sgVertexIndexBuffer *pvib
                                             , const Matrix4 &modelMatrix, int polyType) const
    {
        if(!pvb || !pvib)
            return ;
    }

} // namespace Sagitta