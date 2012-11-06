//  [8/1/2008 fabiozhang]

#include "sgRenderer.h"
#include "sgViewport.h"
#include "engine/component/sgLightComponent.h"
#include "engine/component/sgCameraComponent.h"
#include "engine/common/sgException.h"
#include "engine/component/sgMeshComponent.h"
#include "engine/component/sgRenderStateComponent.h"
#include "engine/resource/sgMesh.h"
#include "engine/scenegraph/sgScene.h"
#include "sgRenderQueue.h"
#include "sgSceneRenderEffect.h"
#include "sgRenderPass.h"
#include "sgGpuProgram.h"
#include "sgGLRenderer.h"
#include "SagiRenderer.h"


namespace Sagitta{
    
    sgRenderer *g_renderer = 0;
    
    SG_META_DEFINE_ABSTRACT(sgRenderer, sgObject)
    
    sgRenderer *sgCreateRenderer(const sgStrHandle &type/*, bool useshader*/)
    {
        if(g_renderer)
            return g_renderer;
        g_renderer = (sgRenderer*)sgObject::createObject(type);
//        if(g_renderer)
//            g_renderer->setUseShader(useshader);
        return g_renderer;
    }
    
    sgRenderer *sgGetRenderer(void)
    {
        if(!g_renderer)
            sgCreateRenderer(sgGLRenderer::GetClassName());
        return g_renderer;
    }
    
    void sgDestroyRenderer(void)
    {
        if(g_renderer)
        {
            sgObject::destroyObject(g_renderer);
            g_renderer = 0;
        }
    }

	//  [8/1/2008 zhangxiang]
	sgRenderer::sgRenderer()
	: m_bSwapBufferSelf(false)
	, m_iTargetWidth(800)
	, m_iTargetHeight(600)
	{
        
	}

	//  [8/1/2008 zhangxiang]
	sgRenderer::~sgRenderer(void)
	{
		removeAllViewport();
	}

	//  [1/10/2009 zhangxiang]
	void sgRenderer::doSthIfNoViewportActive(void) const{
		THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
			"No active viewport.", "sgRenderer::doSthIfNoViewpotActive");
	}

	//  [1/15/2009 zhangxiang]
	void sgRenderer::doSthWhenResized(void){
		// do nothing here.
	}

	//  [10/9/2008 zhangxiang]
	bool sgRenderer::swapBufferSelf(void) const{
		return m_bSwapBufferSelf;
	}

	//  [1/10/2009 zhangxiang]
	//  [11/6/2012 fabiozhang]
	void sgRenderer::collectLights() const
	{
		sgStrHandle strLightType = sgLightComponent::GetClassName();

		m_CurRenderParam.lightlist.clear();
		for(size_t i=0; i<m_CurRenderParam.objlist.size(); ++i)
		{
			sgSceneObject *obj = m_CurRenderParam.objlist[i];
			sgLightComponent *pcomp = (sgLightComponent*)(obj->getComponent(strLightType));
			if(pcomp)
				m_CurRenderParam.lightlist.push_back(pcomp);
		}
        
		
		/*if(m_CurRenderParam.lightlist.empty())
		{
		return 0;
		}*/


		//return setupLightsImpl(m_CurRenderParam.pscene->getAmbiantColor());
	}

	//  [1/15/2009 zhangxiang]
	void sgRenderer::render(sgViewport *aViewport) const{
		m_CurRenderParam.pviewport = aViewport;
		m_CurRenderParam.pcamera = aViewport->camera();
		m_CurRenderParam.pscene = aViewport->camera()->getParent()->getScene();
        m_CurRenderParam.last_gpu_program = m_CurRenderParam.current_gpu_program =
            m_CurRenderParam.scene_gpu_program = NULL;
        
        
        if(!m_CurRenderParam.pscene)
            return ;
		
		// collect scene objects
		m_CurRenderParam.objlist.clear();
		m_CurRenderParam.pscene->getRoot()->getAllObjects(m_CurRenderParam.objlist);

		collectLights();
		

		// set projection matrix
		setProjMatrix(m_CurRenderParam.pcamera->getProjectionMatrix());

		// set view matrix
		setViewMatrix(m_CurRenderParam.pcamera->getViewMatrix());

		sgSceneRenderEffect *sceneRenderEffect = m_CurRenderParam.pscene->getRenderEffect();
		if(!sceneRenderEffect)
		{
			// traditional pipeline

			// setup lights
			if(!(m_CurRenderParam.lightlist.empty())
				&& m_CurRenderParam.pscene->getRenderState().isLightEnable())
			{
				setupLightsImpl(m_CurRenderParam.pscene->getAmbiantColor());
			}

			// prepare render queue
			m_CurRenderParam.resetRenderQueue(m_CurRenderParam.mDefaultRenderQueue);
			m_CurRenderParam.renderqueue->clear();

			// cull objects and add visible object to the render queue and sort it
			m_CurRenderParam.cullObjects(m_CurRenderParam.pcamera);

			// render
			const sgRenderQueue::ObjectList &objects = m_CurRenderParam.renderqueue->getObjectList();
			for(size_t i=0; i<objects.size(); ++i)
			{
				render(m_CurRenderParam.pscene->getRenderState(), objects[i]);
			}
		}
		else
		{
			sceneRenderEffect->render(&m_CurRenderParam, NULL);
		}

		// draw to target
		postRenderImpl();

		// reset lights
//		resetLights(lightNum);
	}

	//  [1/3/2009 zhangxiang]
	void sgRenderer::render(void) const{
		ViewportList::const_reverse_iterator rit = m_ViewportList.rbegin();
		ViewportList::const_reverse_iterator reit = m_ViewportList.rend();
		int avnum = 0;	// active viewport num
		for(; rit!=reit; ++rit){
            sgViewport *viewport = rit->second;
            // set viewport
            setViewport(viewport);
            
            // clear frame buffers
            clearFrameBuffers(viewport->getClearBuffers(),
                              viewport->getBackColor(),
                              viewport->getBackDepth(),
                              viewport->getBackStencil());
           
			if(viewport->isActive()){
				render(viewport);
				
			}
            ++avnum;
		}
		if(avnum == 0){
			doSthIfNoViewportActive();
		}
	}

	//  [1/10/2009 zhangxiang]
	int sgRenderer::tgtWidth(void) const{
		return m_iTargetWidth;
	}

	//  [1/10/2009 zhangxiang]
	int sgRenderer::tgtHeight(void) const{
		return m_iTargetHeight;
	}

	//  [1/3/2009 zhangxiang]
	sgViewport *sgRenderer::createViewport(int aRTWidth, int aRTHeight,
										Real aLeft, Real aTop, 
										Real aWidth, Real aHeight, 
										int aZOrder, sgCameraComponent *aCamera /* = 0 */){
		sgViewport *ret = new sgViewport(aRTWidth, aRTHeight,
									aLeft, aTop,
									aWidth, aHeight,
									aZOrder, aCamera);
		
		m_ViewportList.insert(std::make_pair(ret->getZOrder(), ret));

		return ret;
	}

	//  [1/3/2009 zhangxiang]
	void sgRenderer::removeViewport(sgViewport *aViewport){
		if(!aViewport){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null viewport pointer.", "sgRenderer::removeViewport");
		}

		int zorder = aViewport->getZOrder();
		ViewportList::iterator it = m_ViewportList.lower_bound(zorder);
		ViewportList::iterator eit = m_ViewportList.upper_bound(zorder);
		for(; it!=eit; ++it){
			if(it->second == aViewport){
				delete it->second;
				m_ViewportList.erase(it);
				return ;
			}
		}
	}

	//  [1/3/2009 zhangxiang]
	void sgRenderer::removeViewport(uInt aIndex){
		if(aIndex >= m_ViewportList.size()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Index out of bound.",
							"sgRenderer::removeViewport");
		}

		ViewportList::iterator it = m_ViewportList.begin();
		while(aIndex--) ++it;
		delete it->second;
		m_ViewportList.erase(it);
	}

	//  [1/3/2009 zhangxiang]
	void sgRenderer::removeAllViewport(void){
		ViewportList::iterator it = m_ViewportList.begin();
		ViewportList::iterator eit = m_ViewportList.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ViewportList.clear();
	}

	//  [1/3/2009 zhangxiang]
	sgViewport *sgRenderer::topViewport(void) const{
		if(m_ViewportList.empty()){
			return 0;
		}else{
			return m_ViewportList.begin()->second;
		}
	}

	//  [1/3/2009 zhangxiang]
	sgViewport *sgRenderer::getViewport(uInt aIndex) const{
		if(aIndex >= m_ViewportList.size()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Index out of bound.",
				"sgRenderer::getViewport");
		}

		ViewportList::const_iterator it = m_ViewportList.begin();
		while(aIndex--) ++it;
		return it->second;
	}

	//  [1/3/2009 zhangxiang]
	uInt sgRenderer::getViewportNum(void) const{
		return static_cast<uInt>(m_ViewportList.size());
	}

	//  [1/3/2009 zhangxiang]
	void sgRenderer::setViewportZOrder(sgViewport *aViewport, int aZOrder){
		if(!aViewport){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null viewport pointer.", "sgRenderer::setViewportZOrder");
		}

		int zorder = aViewport->getZOrder();
		ViewportList::iterator it = m_ViewportList.lower_bound(zorder);
		ViewportList::iterator eit = m_ViewportList.upper_bound(zorder);
		for(; it!=eit; ++it){
			if(it->second == aViewport){
				m_ViewportList.erase(it);
			}
		}

		aViewport->_setZOrder(aZOrder);
		m_ViewportList.insert(std::make_pair(aZOrder, aViewport));
	}

	//  [1/3/2009 zhangxiang]
	void sgRenderer::resize(int aWidth, int aHeight){
		m_iTargetWidth = aWidth;
		m_iTargetHeight = aHeight;

		ViewportList::iterator it = m_ViewportList.begin();
		ViewportList::iterator eit = m_ViewportList.end();
		for(; it!=eit; ++it){
			it->second->_targetResized(aWidth, aHeight);
		}

		doSthWhenResized();
	}
    
    bool sgRenderer::setUniformForShader(int type, int location, int extra, sgBuffer* data)
    {
        UniformFuncMap::iterator it = mUniformFuncMap.find(type);
        if(it == mUniformFuncMap.end())
            return false;
        
        it->second(location, extra, data);
        return true;
    }


	sg_render::CurrentRenderParam::CurrentRenderParam( void )
	{
		mDefaultRenderQueue = (sgRenderQueue*)sgObject::createObject(sgRenderQueue::GetClassName());
	}

	sg_render::CurrentRenderParam::~CurrentRenderParam( void )
	{
		sgObject::destroyObject(mDefaultRenderQueue);
	}

	void sg_render::CurrentRenderParam::resetRenderQueue( sgRenderQueue *rq )
	{
		renderqueue = rq;
		if(renderqueue == NULL)
		{
			renderqueue = mDefaultRenderQueue;
		}
		renderqueue->clear();
	}

	//  [8/21/2008 zhangxiang]
	//  [11/6/2012 fabiozhang]
	void sg_render::CurrentRenderParam::cullObjects(sgCameraComponent *aCamera) const
	{
		renderqueue->clear();
		if(objlist.empty())
			return ;

		Matrix4 viewMatrix = aCamera->getViewMatrix();
		Real maxradius;
		Real dot;
		sgSceneObject *obj = 0;
		sgMeshComponent *meshComp = 0;
		sgMesh *mesh = 0;
		for(size_t i=0; i<objlist.size(); ++i)
		{
			obj = objlist[i];
			if(!obj->isActive())
				continue;

			meshComp = (sgMeshComponent*)obj->getComponent(sgMeshComponent::GetClassName());
			if(!meshComp)
				continue;

			mesh = meshComp->getMesh();

			maxradius = mesh->maxRadius();

			Vector3 spherePos = viewMatrix * (obj->position() + mesh->center());

			// near and far plane cull
			if((spherePos.z() + maxradius) < -aCamera->zFar() ||
				(spherePos.z() - maxradius) > -aCamera->zNear()){
					// This object is not in the forum,
					// don't add it into render queque. So do nothing.
					continue;

			}	//#### end z-plane

			// left and right plane cull
			if(spherePos.x() < 0){	// left plane
				dot = spherePos.dotProduct(aCamera->leftPlaneNormal());
				if(dot > 0){	// out of the plane
					if(dot > maxradius){	// total out of the plane, cull it
						continue;
					}
				}
			}else{	// right plane
				dot = spherePos.dotProduct(aCamera->rightPlaneNormal());
				if(dot > 0){	// out of the plane
					if(dot > maxradius){	// total out of the plane, cull it
						continue;
					}
				}
			}

			// up and down plane cull
			if(spherePos.y() > 0){	// up plane
				dot = spherePos.dotProduct(aCamera->upPlaneNormal());
				if(dot > 0){	// out of the plane
					if(dot > maxradius){	// total out of the plane, cull it
						continue;
					}
				}
			}else{	// down plane
				dot = spherePos.dotProduct(aCamera->downPlaneNormal());
				if(dot > 0){	// out of the plane
					if(dot > maxradius){	// total out of the plane, cull it
						continue;
					}
				}
			}

			// The object passed cull test, so add it in to render queque.
			renderqueue->push_back(obj);

		} //#### end for renderable

		renderqueue->sort();
	}
} // namespace Sagitta
