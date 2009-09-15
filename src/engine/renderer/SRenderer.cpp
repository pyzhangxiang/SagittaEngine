//////////////////////////////////////////////////////
// file: SRenderer.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class SRenderSystem
// SRenderSystem is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SRenderer.h"
#include "SViewport.h"
#include "../scenegraph/SSubScene.h"
#include "../scenegraph/SSceneManager.h"
#include "../scenegraph/SRenderable.h"
#include "../scenegraph/SCamera.h"
#include "../scenegraph/SLight.h"
#include "../scenegraph/SRenderQueue.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/1/2008 zhangxiang]
	SRenderer::SRenderer(int aTWidth, int aTHeight, bool abSwapBufferSelf /* = false */)
	: m_bSwapBufferSelf(abSwapBufferSelf),
	m_iTargetWidth(aTWidth),
	m_iTargetHeight(aTHeight){
		if(aTWidth < 0 || aTHeight < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Negative target width or height", "SRenderer::SRenderer");
		}
		m_pRenderQueue = new SRenderQueue;
	}

	//  [8/1/2008 zhangxiang]
	SRenderer::~SRenderer(void){
		removeAllViewport();
		if(m_pRenderQueue){
			delete m_pRenderQueue;
		}
	}

	//  [1/10/2009 zhangxiang]
	void SRenderer::doSthIfNoViewportActive(void) const{
		THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
			"No active viewport.", "SRenderer::doSthIfNoViewpotActive");
	}

	//  [1/15/2009 zhangxiang]
	void SRenderer::doSthWhenResized(void){
		// do nothing here.
	}

	//  [10/9/2008 zhangxiang]
	bool SRenderer::swapBufferSelf(void) const{
		return m_bSwapBufferSelf;
	}

	//  [8/21/2008 zhangxiang]
	void SRenderer::cullObjects(SCamera *aCamera) const{
		if(!aCamera->isInScene() || !aCamera->getScene()->getSceneManager()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"This camera is not in any SceneManager.",
							"SRenderer::cullObjects");
		}

		SSceneManager *scene = aCamera->getScene()->getSceneManager();

		std::list<SRenderable*> rlist;
		// collect scene manager's renderable objects
		SScene::ConstTypedEsseIterator rit = scene->getConstEsseIterator(SRenderable::SET_RENDERABLE);
		for(; rit.hasMoreElements(); ++rit){
			rlist.push_back(static_cast<SRenderable*>(rit.value()));
		}
		// collect sub-scenes' renderable objects
		SSubScene *subscene;
		SSceneManager::ConstTotalSubSceneIterator tssit = scene->getConstSubSceneIterator();
		for(; tssit.hasMoreElements(); ++tssit){
			subscene = tssit.value();
			SScene::ConstTypedEsseIterator teit = subscene->getConstEsseIterator(SRenderable::SET_RENDERABLE);
			for(; teit.hasMoreElements(); ++teit){
				rlist.push_back(static_cast<SRenderable*>(teit.value()));
			}
		}

		// clear render queue
		m_pRenderQueue->clear();

		if(rlist.empty()){
			// no renderable object
			return ;
		}

		Matrix4 viewMatrix = aCamera->getViewMatrix();
		Real maxradius;
		Real dot;

		SRenderable *object;
		std::list<SRenderable*>::const_iterator newrit = rlist.begin();
		std::list<SRenderable*>::const_iterator newreit = rlist.end();
		for(; newrit!=newreit; ++newrit){
			object = static_cast<SRenderable*>(*newrit);
			maxradius = object->maxRadius();

			if(!object->isVisible()){
				continue;
			}

			Vector3 spherePos = viewMatrix * (object->position() + object->center());

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
			m_pRenderQueue->addRenderable(object);

		} //#### end for renderable
	}

	//  [1/10/2009 zhangxiang]
	int SRenderer::setupLights(SSceneManager *aScene) const{
		m_LightList.clear();

		// scene manager's lights
		SScene::ConstTypedEsseIterator smteit = aScene->getConstEsseIterator(SLight::SET_LIGHT);
		for(; smteit.hasMoreElements(); ++smteit){
			m_LightList.push_back(static_cast<SLight*>(smteit.value()));
		}

		// sub-scenes' lights
		SSubScene *subscene;
		SSceneManager::ConstTotalSubSceneIterator tssit = aScene->getConstSubSceneIterator();
		for(; tssit.hasMoreElements(); ++tssit){
			subscene = tssit.value();
			SScene::ConstTypedEsseIterator teit = subscene->getConstEsseIterator(SLight::SET_LIGHT);
			for(; teit.hasMoreElements(); ++teit){
				m_LightList.push_back(static_cast<SLight*>(teit.value()));
			}
		}

		return setupLightsImpl(aScene->globalAmbientColor());
	}

	//  [1/15/2009 zhangxiang]
	void SRenderer::render(SViewport *aViewport) const{
		m_CurRenderParam.pviewport = aViewport;
		m_CurRenderParam.pcamera = aViewport->camera();
		m_CurRenderParam.pscenemanager = m_CurRenderParam.pcamera->getScene()->getSceneManager();
		m_CurRenderParam.prenderoption = &(m_CurRenderParam.pscenemanager->globalRenderOption());

		// update scene graph
		// update scene geometry by current frame from it's animation data
		// will update visible sub-scenes too
		m_CurRenderParam.pscenemanager->_updateFromAnimation();

		// set viewport
		setViewport(aViewport);

		// clear frame buffers
		clearFrameBuffers(aViewport->getClearBuffers(),
						aViewport->getBackColor(),
						aViewport->getBackDepth(),
						aViewport->getBackStencil());

		// set projection matrix
		setProjMatrix(m_CurRenderParam.pcamera->getProjectionMatrix());

		// set view matrix
		setViewMatrix(m_CurRenderParam.pcamera->getViewMatrix());

		// setup lights
		int lightNum = 0;
		if(m_CurRenderParam.prenderoption->isLightEnable()){
			lightNum = setupLights(m_CurRenderParam.pscenemanager);
		}

		// cull objects
		cullObjects(m_CurRenderParam.pcamera);

		// render
		SRenderQueue::ConstIterator it = m_pRenderQueue->getConstIterator();
		SRenderable *renderable;
		for(; it.hasMoreElements(); it++){
			renderable = it.value();
			render(*(m_CurRenderParam.prenderoption), renderable);
		}

		// draw to target
		postRenderImpl();

		// reset lights
		resetLights(lightNum);
	}

	//  [1/3/2009 zhangxiang]
	void SRenderer::render(void) const{
		ViewportList::const_reverse_iterator rit = m_ViewportList.rbegin();
		ViewportList::const_reverse_iterator reit = m_ViewportList.rend();
		int avnum = 0;	// active viewport num
		for(; rit!=reit; ++rit){
			if(rit->second->isActive()){
				render(rit->second);
				++avnum;
			}
		}
		if(avnum == 0){
			doSthIfNoViewportActive();
		}
	}

	//  [1/10/2009 zhangxiang]
	int SRenderer::tgtWidth(void) const{
		return m_iTargetWidth;
	}

	//  [1/10/2009 zhangxiang]
	int SRenderer::tgtHeight(void) const{
		return m_iTargetHeight;
	}

	//  [1/3/2009 zhangxiang]
	SViewport *SRenderer::createViewport(int aRTWidth, int aRTHeight,
										Real aLeft, Real aTop, 
										Real aWidth, Real aHeight, 
										int aZOrder, SCamera *aCamera /* = 0 */){
		SViewport *ret = new SViewport(aRTWidth, aRTHeight,
									aLeft, aTop,
									aWidth, aHeight,
									aZOrder, aCamera);
		
		m_ViewportList.insert(std::make_pair(ret->getZOrder(), ret));

		return ret;
	}

	//  [1/3/2009 zhangxiang]
	void SRenderer::removeViewport(SViewport *aViewport){
		if(!aViewport){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null viewport pointer.", "SRenderer::removeViewport");
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
	void SRenderer::removeViewport(uInt aIndex){
		if(aIndex >= m_ViewportList.size()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Index out of bound.",
							"SRenderer::removeViewport");
		}

		ViewportList::iterator it = m_ViewportList.begin();
		while(aIndex--) ++it;
		delete it->second;
		m_ViewportList.erase(it);
	}

	//  [1/3/2009 zhangxiang]
	void SRenderer::removeAllViewport(void){
		ViewportList::iterator it = m_ViewportList.begin();
		ViewportList::iterator eit = m_ViewportList.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ViewportList.clear();
	}

	//  [1/3/2009 zhangxiang]
	SViewport *SRenderer::topViewport(void) const{
		if(m_ViewportList.empty()){
			return 0;
		}else{
			return m_ViewportList.begin()->second;
		}
	}

	//  [1/3/2009 zhangxiang]
	SViewport *SRenderer::getViewport(uInt aIndex) const{
		if(aIndex >= m_ViewportList.size()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Index out of bound.",
				"SRenderer::getViewport");
		}

		ViewportList::const_iterator it = m_ViewportList.begin();
		while(aIndex--) ++it;
		return it->second;
	}

	//  [1/3/2009 zhangxiang]
	uInt SRenderer::getViewportNum(void) const{
		return static_cast<uInt>(m_ViewportList.size());
	}

	//  [1/3/2009 zhangxiang]
	void SRenderer::setViewportZOrder(SViewport *aViewport, int aZOrder){
		if(!aViewport){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null viewport pointer.", "SRenderer::setViewportZOrder");
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
	void SRenderer::resize(int aWidth, int aHeight){
		m_iTargetWidth = aWidth;
		m_iTargetHeight = aHeight;

		ViewportList::iterator it = m_ViewportList.begin();
		ViewportList::iterator eit = m_ViewportList.end();
		for(; it!=eit; ++it){
			it->second->_targetResized(aWidth, aHeight);
		}

		doSthWhenResized();
	}

} // namespace Sagitta