//////////////////////////////////////////////////////
// file: SRenderer.h @ 2008-8-1 by Zhang Xiang
// declares of the class SRenderer
// SRenderer is a class ...
//////////////////////////////////////////////////////
#ifndef __SRENDERER_H__
#define __SRENDERER_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderObject.h"
#include "SRenderOption.h"
#include "../../math/SColor.h"
#include <list>
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SViewport;
	class SRenderQueue;
	class SCamera;
	class SSceneManager;
	class SLight;
	class SRenderable;
	class SSceneManager;
	class SVertexData;
	class SVertexIndexBuffer;
	class Matrix4;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SRenderer : public SRenderObject{
	// inner class defines

		struct CurrentRenderParam{
			SViewport *pviewport;
			SCamera *pcamera;
			SSceneManager *pscenemanager;
			SRenderOption *prenderoption;
		};

	// type defines
	protected:
		// key_value is z-order value
		typedef std::multimap<int, SViewport*> ViewportList;
		typedef std::list<SLight*> LightList;

	// member variables
	protected:
		/// viewport list ordered by z-order, higher = further to the front
		ViewportList m_ViewportList;

		/// render queue
		SRenderQueue *m_pRenderQueue;

		/// light list
		mutable LightList m_LightList;

		/// current render param for very viewport
		mutable CurrentRenderParam m_CurRenderParam;

		/** Whether swap buffer by renderer self, if false, let os do it. */
		bool m_bSwapBufferSelf;

		/// whole render target size
		int m_iTargetWidth, m_iTargetHeight;

		/** stores whether the target's size is changed.
			@remarks
				Sets to be true when SRender::resize() is called.
				And will be reseted after a render process.
		*/
	//	bool m_bResized;

	// constructors & destructor
	public:
		SRenderer(int aTWidth, int aTHeight, bool abSwapBufferSelf = false);
		virtual ~SRenderer(void);

	// member functions
	private:
		/** Internal hook. Does something if no viewport is acitve. */
		virtual void doSthIfNoViewportActive(void) const;

		/** Internal hook. Does something when target resized.
			@remarks SagiRenderer needs it to resize it's frame buffers.
		*/
		virtual void doSthWhenResized(void);

		/** Sets viewport. */
		virtual void setViewport(SViewport *aViewport) const = 0;

		/** clear frame buffers. */
		virtual void clearFrameBuffers(uInt aFlags,
									const Color &aBkColor,
									Real aBkDepth, int aBkStencil) const = 0;

		/** Sets projection matrix.
			@param
				aMatrix The raw matrix from camera.
		*/
		virtual void setProjMatrix(const Matrix4 &aMatrix) const = 0;

		/** Sets view matrix.
			@param
				aMatrix The raw matrix from camera.
		*/
		virtual void setViewMatrix(const Matrix4 &aMatrix) const = 0;

		/** Collects and setups lights.
			@return Actually enabled lights num.
		*/
		int setupLights(SSceneManager *aScene) const;

		/** Internal hook. Calls 3D API to setup lights. */
		virtual int setupLightsImpl(const Color &aGlobalAmbiantColor) const = 0;

		/** Checks if the specified renderable object is in the forum of camera, if true, add it to the render queque of scene manager. */
		void cullObjects(SCamera *aCamera) const;

		/** Renders a specific renderable object. */
		virtual void render(const SRenderOption &aGlobalRop, SRenderable *aRenderable) const = 0;

		/** Draw frame buffer to target and do something after rendering. */
		virtual void postRenderImpl(void) const = 0;

		/** Resets light state. */
		virtual void resetLights(int aLightNum) const = 0;

		/** Renders specified viewport. */
		void render(SViewport *aViewport) const;

	protected:
		/** Does conversion between Sagitta element type to graphics api's element type (e.g. ET_POINTS to GL_POINTS). */
		virtual int retMapping(int aRet) const = 0;

		/** Checks if target was resized.
			@remarks Internal method.
		*/
//		bool isResized(void) const;

	public:
		/** Gets target width. */
		int tgtWidth(void) const;

		/** Gets target height. */
		int tgtHeight(void) const;

		/** Creates a viewport.
			@remarks Add the new viewport to list.
			@param
				aRTWidth Render target's total width.
			@param
				aRTHeight Render target's total height.
			@param
				aLeft
			@param
				aTop
			@param
				aWidth
			@param
				aHeight
				Dimensions of the viewport expressed as a value between
				0 and 1.This allows the dimensions to apply irrespective of
				changes in the target's size: e.g. to fill the whole area,
				values of 0,0,1,1 are appropriate.
			@param
				aZOrder Relative z-order in the render target. Height = further to the front.
			@param
				aCamera Camera this viewport to combine, could be 0.
		*/
		SViewport *createViewport(int aRTWidth, int aRTHeight,
								Real aLeft, Real aTop, 
								Real aWidth, Real aHeight, 
								int aZOrder, SCamera *aCamera = 0);

		/** Remove a viewport from this renderer.
			@remarks Destroy the specified viewport too.
		*/
		void removeViewport(SViewport *aViewport);

		/** Remove a viewport from this renderer by index.
			@remarks Destroy the specified viewport too.
		*/
		void removeViewport(uInt aIndex);

		/** Remove all viewports.
			@remarks Destroy all viewports too.
		*/
		void removeAllViewport(void);

		/** Gets the top viewport. */
		SViewport *topViewport(void) const;

		/** Gets a viewport by index. */
		SViewport *getViewport(uInt aIndex) const;

		/** Gets viewports num. */
		uInt getViewportNum(void) const;

		/** Sets the specified viewport's z-order value. */
		void setViewportZOrder(SViewport *aViewport, int aZOrder);

		/** Gets Whether swap buffer by renderer self. */
		bool swapBufferSelf(void) const;

		/** Initialize the environment of graphics device. */
		virtual void init(void) = 0;
		
		/** Reset graphics attributes when the window's size changed. */
		void resize(int aWidth, int aHeight);

		/** Renders viewports */
		void render(void) const;

	}; //#### end class SRenderer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRENDERER_H__