//////////////////////////////////////////////////////
// file: SGraphicsView.cpp @ 2008-12-13 by Zhang Xiang
// defines of the class SGraphicsView
// SGraphicsView is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SGraphicsView.h"
#include "SWindowDelegate.h"
#include "SRenderContext.h"
#include "SGLContext.h"
#include "../common/utils/SException.h"
#include "../engine/scenegraph/SSceneManager.h"
#include "../engine/renderer/SRenderer.h"
#include "../engine/renderer/SGLRenderer.h"
#include "../engine/renderer/SagiRanderer.h"
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
#	include "SD3D9Context.h"
#	include "../engine/renderer/SD3D9Renderer.h"
#endif

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/13/2008 zhangxiang]
	SGraphicsView::SGraphicsView(SGraphicsViewDelegate *aDelegate, SMainFrame *aParent,
								RendererType aRendererType /* = RT_UNKNOWN */,
								const std::string &aTitle /* = "New Render View" */,
								SRect aRect /* = SRect(0, 0, 170, 170) */)
		: SView(aDelegate, aParent, aTitle, aRect), m_RendererType(aRendererType),
		m_pRenderer(0), m_pSceneManager(0), m_pRenderContext(0){
		init();
	}

	//  [12/13/2008 zhangxiang]
	SGraphicsView::SGraphicsView(SMainFrame *aParent,
								RendererType aRendererType /* = RT_UNKNOWN */,
								const std::string &aTitle /* = "New Render View" */,
								SRect aRect /* = SRect(0, 0, 170, 170) */)
		: SView(new SGraphicsViewDelegate(), aParent, aTitle, aRect), m_RendererType(aRendererType),
		m_pRenderer(0), m_pSceneManager(0), m_pRenderContext(0){
		init();
	}

	//  [12/13/2008 zhangxiang]
	SGraphicsView::~SGraphicsView(void){
		if(m_pRenderContext)
			delete m_pRenderContext;
		if(m_pRenderer)
			delete m_pRenderer;
	}

	//  [12/13/2008 zhangxiang]
	void SGraphicsView::init(void){
		if(m_RendererType == RT_UNKNOWN){
			m_RendererType = RT_OPENGL;
		}

		switch(m_RendererType){
			case RT_UNKNOWN:
			case RT_OPENGL:
			{
				m_pRenderContext = new SGLContext(this);
				try{
					m_pRenderer = new SGLRenderer(width(), height());
				}catch(SException&){
					delete m_pRenderContext;
					throw;
				}
				break;
			}

			case RT_SAGITTA:
			{
				m_pRenderContext = new SGLContext(this);
				try{
					m_pRenderer = new SagiRenderer(width(), height(), false);
				}catch(SException&){
					delete m_pRenderContext;
					throw;
				}
				break;
			}

#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
			case RT_D3D9:
			{
				m_pRenderContext = new SD3D9Context(this);
				try{
					m_pRenderer = new SD3D9Renderer(handle(), width(), height());
				}catch(SException&){
					delete m_pRenderContext;
					throw;
				}
				break;
			}
#endif

			default:
				THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "No such renderer.", "SGraphicsView::init");
		}

		m_pRenderer->createViewport(width(), height(), 0.0, 0.0, 1.0, 1.0, 0, 0);
		m_pRenderer->init();
	}

	//  [12/14/2008 zhangxiang]
	bool SGraphicsView::onEraseBkgnd(const SContext &aContext){
		return true;
	}

	//  [12/14/2008 zhangxiang]
	bool SGraphicsView::onResize(uLong aWidth, uLong aHeight){
		if(m_pSceneManager && m_pRenderContext){
			m_pRenderer->resize(aWidth, aHeight);
			m_pRenderContext->update();
			return true;
		}
		return false;
	}

	//  [12/14/2008 zhangxiang]
	bool SGraphicsView::onPaint(void){
		if(m_pRenderer && m_pRenderContext){
			m_pRenderer->render();
	//		if(!m_pRenderer->swapBufferSelf())
			m_pRenderContext->swapBuffers();

			validateRect(0);
			return true;
		}
		
		return false;
	}

	//  [12/14/2008 zhangxiang]
	bool SGraphicsView::onClose(void){
		if(m_pRenderContext){
			delete m_pRenderContext;
			m_pRenderContext = 0;
		}
		// let system to dispose windows
	//	dispose();
		return false;
	}

	//  [1/3/2009 zhangxiang]
	SRenderer *SGraphicsView::getRenderer(void) const{
		return m_pRenderer;
	}

	//  [12/14/2008 zhangxiang]
	SSceneManager *SGraphicsView::getSceneManager(void) const{
		return m_pSceneManager;
	}

	//  [12/14/2008 zhangxiang]
	void SGraphicsView::setSceneManager(SSceneManager *aSceneManager){
		if(m_pSceneManager != aSceneManager){
			// a new scenemanager, set mine to be it and update
			m_pSceneManager = aSceneManager;
	//		_update();
		}
		// else, do nothing
	}

} // namespace Sagitta