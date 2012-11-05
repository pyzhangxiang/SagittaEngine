//  [8/1/2008 fabiozhang]

#ifndef __SGRENDERER_H__
#define __SGRENDERER_H__

#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "sgRenderState.h"
#include "math/sgColor.h"
#include <list>
#include <map>


namespace Sagitta{
    
    class sgRenderer;
    sgRenderer *sgCreateRenderer(const sgStrHandle &type/*, bool useshader*/);
    void sgDestroyRenderer(void);
    _SG_KernelExport sgRenderer *sgGetRenderer(void);
    
    class sgBuffer;
    // params, location, extra(matrix: if transpose; fv: count), data
    typedef void(*SetShaderUniformFunc)(int, int, sgBuffer*);

	class sgCameraComponent;
	class sgLightComponent;
	class sgViewport;
	class sgScene;
	class sgVertexData;
	class sgVertexIndexBuffer;
	class Matrix4;
    class sgRenderQueue;
    class sgGpuProgram;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgRenderer : public sgObject
    {
        SG_META_DECLARE_ABSTRACT(sgRenderer)
        
//        friend sgRenderer *sgCreateRenderer(const sgStrHandle &type, bool useshader);
        
    public:
        enum DataType
        {
            DT_F,
            DT_FV2,
            DT_FV3,
            DT_FV4,
            DT_I,
            DT_IV2,
            DT_IV3,
            DT_IV4,
            DT_UI,
            DT_UIV2,
            DT_UIV3,
            DT_UIV4,
            DT_B,
            DT_BV2,
            DT_BV3,
            DT_BV4,
            DT_FM22,
            DT_FM33,
            DT_FM44,
            DT_TEXTURE,
            DT_NIL,
        };

	protected:
		// key_value is z-order value
		typedef sg_multimap(int, sgViewport*) ViewportList;
		typedef sg_vector(sgLightComponent*) LightList;
		typedef sgSceneObject::SceneObjectVec ObjectList;
        
        typedef sg_map(int, SetShaderUniformFunc) UniformFuncMap;
        UniformFuncMap mUniformFuncMap;

		struct CurrentRenderParam{
			sgViewport *pviewport;
			sgCameraComponent *pcamera;
			sgScene *pscene;
			ObjectList objlist;
			//ObjectList renderqueue;
            sgRenderQueue *renderqueue;
            sgGpuProgram *scene_gpu_program;
            sgGpuProgram *current_gpu_program;
			LightList lightlist;
		};

	// type defines
	

	// member variables
	protected:

		/// viewport list ordered by z-order, higher = further to the front
		ViewportList m_ViewportList;

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
        
        sgRenderQueue *mDefaultRenderQueue;
        
    private:
        // if the scene has an effect, then we use
        // its shader to render
        // otherwise, use the traditional pipe line
//        bool mUseShader;
//        void setUseShader(bool use){ mUseShader = use; }
    public:
        //bool isUseShader(void) const{ return mUseShader; }

	// constructors & destructor
	public:
		sgRenderer(/*int aTWidth, int aTHeight, bool abSwapBufferSelf = false*/);
		virtual ~sgRenderer(void);

	// member functions
	private:
		/** Internal hook. Does something if no viewport is acitve. */
		virtual void doSthIfNoViewportActive(void) const;

		/** Internal hook. Does something when target resized.
			@remarks SagiRenderer needs it to resize it's frame buffers.
		*/
		virtual void doSthWhenResized(void);

		/** Sets viewport. */
		virtual void setViewport(sgViewport *aViewport) const = 0;

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
		int setupLights() const;

		/** Internal hook. Calls 3D API to setup lights. */
		virtual int setupLightsImpl(const Color &aGlobalAmbiantColor) const = 0;

		/** Checks if the specified renderable object is in the forum of camera, if true, add it to the render queque of scene manager. */
		void cullObjects(sgCameraComponent *aCamera) const;

		/** Renders a specific renderable object. */
		virtual void render(const sgRenderState &aGlobalRop, sgSceneObject *aRenderable) const = 0;

		/** Draw frame buffer to target and do something after rendering. */
		virtual void postRenderImpl(void) const = 0;

		/** Resets light state. */
		virtual void resetLights(int aLightNum) const = 0;

		/** Renders specified viewport. */
		void render(sgViewport *aViewport) const;

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
		sgViewport *createViewport(int aRTWidth, int aRTHeight,
								Real aLeft, Real aTop, 
								Real aWidth, Real aHeight, 
								int aZOrder, sgCameraComponent *aCamera = 0);

		/** Remove a viewport from this renderer.
			@remarks Destroy the specified viewport too.
		*/
		void removeViewport(sgViewport *aViewport);

		/** Remove a viewport from this renderer by index.
			@remarks Destroy the specified viewport too.
		*/
		void removeViewport(uInt aIndex);

		/** Remove all viewports.
			@remarks Destroy all viewports too.
		*/
		void removeAllViewport(void);

		/** Gets the top viewport. */
		sgViewport *topViewport(void) const;

		/** Gets a viewport by index. */
		sgViewport *getViewport(uInt aIndex) const;

		/** Gets viewports num. */
		uInt getViewportNum(void) const;

		/** Sets the specified viewport's z-order value. */
		void setViewportZOrder(sgViewport *aViewport, int aZOrder);

		/** Gets Whether swap buffer by renderer self. */
		bool swapBufferSelf(void) const;

		/** Initialize the environment of graphics device. */
		virtual void init(void) = 0;
		
		/** Reset graphics attributes when the window's size changed. */
		void resize(int aWidth, int aHeight);

		/** Renders viewports */
		void render(void) const;
        
        virtual bool initShaderEnvironment(void){ return false; };
        
        bool setUniformForShader(int type, int location, int extra, sgBuffer* data);

	}; //#### end class sgRenderer

} // namespace Sagitta


#endif // __SGRENDERER_H__

