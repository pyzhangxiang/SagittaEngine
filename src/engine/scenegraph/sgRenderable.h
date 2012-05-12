//////////////////////////////////////////////////////
// file: sgRenderable.h @ 2008-1-31 by Zhang Xiang
// declares of the class sgRenderable
// sgRenderable is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRENDERABLE_H__
#define __SGRENDERABLE_H__

// INCLUDES //////////////////////////////////////////
#include "sgEsse.h"
#include "sgMaterial.h"
#include "../renderer/sgRenderOption.h"
#include "../../math/sgMathHeader.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgBuffer;
	class sgVertexData;
	class sgIndexData;
	class sgVertexIndexBuffer;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgRenderable : public sgEsse{
	// enum defines
	public:
		/** scene object type */
		enum SEsseType{ SET_RENDERABLE = 2 };
		
	// static memebers
		static uLong ms_iRenderableCount;

	// member variables
	protected:
		/** if true, then this renderable object will render itself using it's own render function;
		*	or it will be render by unified shader.
		*/
		bool m_bRenderSelf;

		/// if i'm visible.
		bool m_bVisible;

		/// average radius of this renderable object
		Real m_fAverageRadius;

		/// the max radius of this renderable object
		Real m_fMaxRadius;

		/// center position of this renderable object relative to my attached node
		Vector3 m_Center;

		/// special render option for this renderable object
		sgRenderOption m_RenderOption;

		/// my material
		sgMaterial m_Material;

		/// vertex data, include position, normal, color, texture ...
		sgVertexData *m_pVertexData;
		/// vertex index data, include position, normal, color, texture indices
		sgIndexData *m_pIndexData;

		/// face normal list
		sgBuffer *m_pFaceNormalBuffer;

	// constructors & destructor
	public:
		sgRenderable(void);
		sgRenderable(const StdString &aName);
		~sgRenderable(void);

	// member functions
	protected:
		/** Calculates center position of this entity
			and the maximum radius and the average radius.
		*/
		virtual void calCenterAndRadius(void) = 0;
	public:
		/** Render fucntion used to render this renderable object when
			that the varialbe m_bRenderSelf is true
			and need be overwrite by class user
		*/
		virtual void render(void);

		/** Gets my render option. */
		const sgRenderOption &renderOption(void) const;
		/** Gets my render option to be l-value. */
		sgRenderOption &renderOption(void);
		/** Sets my render option. */
		void setRenderOption(const sgRenderOption &aROP);

		/** Gets my material. */
		const sgMaterial &material(void) const;
		/** Sets my material. */
		void setMaterial(const sgMaterial &aMaterial);

		/** Checks if i'm visible. */
		bool isVisible(void) const;
		/** Sets my visible property. */
		void setVisible(bool abVisible);

		/** Gets my center position. */
		const Vector3 &center(void) const;
		
		/** Locates this object to it's center.
			@remarks
				After lacated, the center will be Vector3::ZERO.
		*/
		virtual void locateToCenter(void) = 0;

		/** Gets my average radius. */
		Real avgRadius(void) const;
		/** Gets my max radius. */
		Real maxRadius(void) const;

		/** Gets the vertex data.
			@param
				outVBuffer Where copied vertices data stored, should has been malloced.
			@param
				outIBuffer Where copied indices data stored, should has been malloced.
		*/
		virtual void getVertexBuffer(sgVertexData *outVBuffer, sgVertexIndexBuffer *outIBuffer) const = 0;

		/** Gets face normal buffer 
		*/
		virtual sgBuffer *getFaceNormalBuffer(void) const = 0;

		/** Gets full model transform matrix for this renderable object. */
		Matrix4 getModelMatrix(void) const;

	}; //#### end class sgRenderable

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRENDERABLE_H__

