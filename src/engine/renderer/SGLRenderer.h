//////////////////////////////////////////////////////
// file: SGLRenderer.h @ 2008-8-1 by Zhang Xiang
// declares of the class SGLRenderer
// SGLRenderer is a class ...
//////////////////////////////////////////////////////
#ifndef __SGLRENDERER_H__
#define __SGLRENDERER_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderer.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SGLRenderer : public SRenderer{
	// member variables

	// constructors & destructor
	public:
		SGLRenderer(int aTWidth, int aTHeight);
		virtual ~SGLRenderer(void);

	// member functions
	private:
		/** Sets viewport. Overrides from SRenderer. */
		void setViewport(SViewport *aViewport) const;

		/** clear frame buffers. Overrides from SRenderer. */
		void clearFrameBuffers(uInt aFlags,
							const Color &aBkColor,
							Real aBkDepth, int aBkStencil) const;

		/** Sets projection matrix. Overrides from SRenderer.
			@param
				aMatrix The raw matrix from camera.
		*/
		void setProjMatrix(const Matrix4 &aMatrix) const;

		/** Sets view matrix. Overrides from SRenderer.
			@param
				aMatrix The raw matrix from camera.
		*/
		void setViewMatrix(const Matrix4 &aMatrix) const;

		/** Internal hook. Calls 3D API to setup lights. Overrides from SRenderer. */
		int setupLightsImpl(const Color &aGlobalAmbiantColor) const;

		/** Renders a specific renderable object. Overrides from SRenderer. */
		void render(const SRenderOption &aGlobalRop, SRenderable *aRenderable) const;

		/** Draw frame buffer to target and do something after rendering. Overrides from SRenderer. */
		void postRenderImpl(void) const;

		/** Resets light state. Overrides from SRenderer. */
		void resetLights(int aLightNum) const;

	protected:
		/** Overridden from SRenderer. Does conversion between Sagitta element type to graphics api's element type (e.g. ET_POINTS to GL_POINTS). */
		int retMapping(int aRet) const;

	public:
		/** Overridden from SRenderer. Initialize the environment of graphics device. */
		void init(void);

		/** Overridden from SRenderer. Reset graphics attributes when the window's size changed. */
	//	void resize(int aWidth, int aHeight);

	}; //#### end class SGLRenderer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGLRENDERSYSTEM_H__