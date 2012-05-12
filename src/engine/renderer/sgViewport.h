//////////////////////////////////////////////////////
// file: sgViewport.h @ 2008-1-31 by Zhang Xiang
// declares of the class sgViewport
// sgViewport is a class ...
//////////////////////////////////////////////////////
#ifndef __SGVIEWPORT_H__
#define __SGVIEWPORT_H__

// INCLUDES //////////////////////////////////////////
#include "sgRenderObject.h"
#include "../../math/sgColor.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgCamera;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgViewport : public sgRenderObject{
	// enum declares
	public:
		enum ClearBuffers{
			CB_NON = 0x00,
			CB_COLOR = 0x01,	// 0001
			CB_DEPTH = 0x02,	// 0010
			CB_STENCIL = 0x04	// 0100
		};

	// member variables
	private:
		/// render target's size
		int m_iRTWidth, m_iRTHeight;

		/// relative dimensions to the render target(0..1)
		Real m_fLeft, m_fTop, m_fWidth, m_fHeight;

		/// actual dimensions in the render target
		int m_iLeft, m_iTop, m_iWidth, m_iHeight;

		/// z-order, higher = further to the front
		int m_iZOrder;

		/// camera this viewport used, could be changed
		sgCamera *m_pCamera;

		/// background color
		Color m_BackColor;

		/// depth buffer clear value
		Real m_fBackDepth;

		/// stencil buffer clear value
		int m_iBackStencil;

		/// which buffer to clear
		uInt m_iClearBuffers;

	// constructors & destructor
	private:
		/** constructor
			@remarks Only called by sgRenderer::createViewport
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
		sgViewport(int aRTWidth, int aRTHeight,
				Real aLeft, Real aTop, 
				Real aWidth, Real aHeight, 
				int aZOrder, sgCamera *aCamera = 0);
	public:
		virtual ~sgViewport(void);

	// member functions
	protected:
		/** Update the actual dimensions of me. Internal method. */
		void _updateActDimensions(void);

		/** Responds for the render target resized.
			@remarks
				Internal method. Only called by sgRenderer::resize
			@param
				aWidth
			@param
				aHeight
				The render target's new size.
		*/
		void _targetResized(int aWidth, int aHeight);

		/** Sets my z-order value.
			@remarks Internal method. Only called by sgRenderer::setViewportZOrder.
		*/
		void _setZOrder(int aZOrder);

	public:
		/** Gets one of the actual dimension. */
		int actleft(void) const;
		/** Gets one of the relative dimension. */
		Real left(void) const;
		/** Sets one of the relative dimension. */
		void setLeft(Real aLeft);

		/** Gets one of the actual dimension. */
		int acttop(void) const;
		/** Gets one of the relative dimension. */
		Real top(void) const;
		/** Sets one of the relative dimension. */
		void setTop(Real aTop);

		/** Gets one of the actual dimension. */
		int actwidth(void) const;
		/** Gets one of the relative dimension. */
		Real width(void) const;
		/** Sets one of the relative dimension. */
		void setWidth(Real aWidth);

		/** Gets one of the actual dimension. */
		int actheight(void) const;
		/** Gets one of the relative dimension. */
		Real height(void) const;
		/** Sets one of the relative dimension. */
		void setHeight(Real aHeight);

		/** Sets the relative dimensions to the render target.
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
		*/
		void setDimensions(Real aLeft, Real aTop, Real aWidth, Real aHeight);

		/** Gets my z-order value. */
		int getZOrder(void) const;

		/** Gets my combined camera. */
		sgCamera *camera(void) const;
		/** Sets a camera to combine to me. */
		void setCamera(sgCamera *aCamera);

		/** Gets my background color. */
		const Color &getBackColor(void) const;
		/** Sets my background color. */
		void setBackColor(const Color &aColor);

		/** Gets background depth. */
		Real getBackDepth(void) const;
		/** Sets my background depth. */
		void setBackDepth(Real aValue);

		/** Gets background stencil value. */
		int getBackStencil(void) const;
		/** Sets my background stencil value. */
		void setBackStencil(int aValue);

		/** Gets which buffers are to be cleared. */
		uInt getClearBuffers(void) const;
		/** Sets which buffers are to be cleared. */
		void setClearBuffers(uInt aBuffers);

		/** Check if I'm active. */
		bool isActive(void) const;

	// friend declare
	public:
		friend class sgRenderer;

	}; //#### end class sgViewport

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGVIEWPORT_H__

