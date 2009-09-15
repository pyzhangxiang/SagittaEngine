//////////////////////////////////////////////////////
// file: SGraphicsView.h @ 2008-12-13 by Zhang Xiang
// declares of the class SGraphicsView
// SGraphicsView is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRAPHICSVIEW_H__
#define __SGRAPHICSVIEW_H__

// INCLUDES //////////////////////////////////////////
#include "SView.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SGraphicsViewDelegate;
	class SMainFrame;
	class SRenderContext;
	class SRenderer;
	class SSceneManager;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SGraphicsView : public SView{
	// type defines
	public:
		enum RendererType{
			RT_UNKNOWN = 0,		// will choose gl
			RT_SAGITTA,
			RT_OPENGL,
			RT_D3D9,
			RT_D3D10
		};

	// member variables
	private:
		/** Stores the renderer type. */
		RendererType m_RendererType;
		/** Stores the pointer to a renderer, and holds the ownership. */
		SRenderer *m_pRenderer;
		/** Stores the pointer to a SceneManager, and does not hold the ownership. */
		SSceneManager *m_pSceneManager;

		/** Stores owner context. */
		SRenderContext *m_pRenderContext;

	// constructors & destructor
	public:
		/** Constructor.
		*	@param
		*		aDelegate A pointer to SGraphicsViewDelegate. Should not be zero. If don't want to
		*				create it, then use the other constructor. That will create a default delegate
		*				by itself.
		*	@param
		*		aParent The parent window for this view. Cannot be zero, or will throw an exception. 
		*/
		SGraphicsView(SGraphicsViewDelegate *aDelegate, SMainFrame *aParent, RendererType aRendererType = RT_UNKNOWN, const std::string &aTitle = "New Render View", SRect aRect = SRect(0, 0, 170, 170));
		/** Constructor
		*	@remarks
		*		Will create a default SGraphicsViewDelegate by itself.
		*	@param
		*		aParent The parent window for this view. Cannot be zero, or will throw an exception. 
		*/
		SGraphicsView(SMainFrame *aParent, RendererType aRendererType = RT_UNKNOWN, const std::string &aTitle = "New Render View", SRect aRect = SRect(0, 0, 170, 170));
		virtual ~SGraphicsView(void);

	// member functions
	private:
		void init(void);

	protected:
		/** Take charge the erase background event, so that the os could not erase it. Overrides from SWindow. */
		bool onEraseBkgnd(const SContext &aContext);

		/** Handles the resize event to update the projection matrix. Overrides from SWindow. */
		bool onResize(uLong aWidth, uLong aHeight);

		/** Handles the paint event. Overrides from SWindow. */
		bool onPaint();

		/** Does some purge work before destroy the window. */
		bool onClose();

	public:
		SRenderer *getRenderer(void) const;
		SSceneManager *getSceneManager(void) const;
		void setSceneManager(SSceneManager *aSceneManager);

	}; //#### end class SGraphicsView

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRAPHICSVIEW_H__