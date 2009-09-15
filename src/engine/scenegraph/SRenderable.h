//////////////////////////////////////////////////////
// file: SRenderable.h @ 2008-1-31 by Zhang Xiang
// declares of the class SRenderable
// SRenderable is a class ...
//////////////////////////////////////////////////////
#ifndef __SRENDERABLE_H__
#define __SRENDERABLE_H__

// INCLUDES //////////////////////////////////////////
#include "SEsse.h"
#include "SMaterial.h"
#include "../renderer/SRenderOption.h"
#include "../../math/SMathHeader.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SBuffer;
	class SVertexData;
	class SIndexData;
	class SVertexIndexBuffer;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SRenderable : public SEsse{
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
		SRenderOption m_RenderOption;

		/// my material
		SMaterial m_Material;

		/// vertex data, include position, normal, color, texture ...
		SVertexData *m_pVertexData;
		/// vertex index data, include position, normal, color, texture indices
		SIndexData *m_pIndexData;

		/// face normal list
		SBuffer *m_pFaceNormalBuffer;

	// constructors & destructor
	public:
		SRenderable(void);
		SRenderable(const StdString &aName);
		~SRenderable(void);

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
		const SRenderOption &renderOption(void) const;
		/** Gets my render option to be l-value. */
		SRenderOption &renderOption(void);
		/** Sets my render option. */
		void setRenderOption(const SRenderOption &aROP);

		/** Gets my material. */
		const SMaterial &material(void) const;
		/** Sets my material. */
		void setMaterial(const SMaterial &aMaterial);

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
		virtual void getVertexBuffer(SVertexData *outVBuffer, SVertexIndexBuffer *outIBuffer) const = 0;

		/** Gets face normal buffer 
		*/
		virtual SBuffer *getFaceNormalBuffer(void) const = 0;

		/** Gets full model transform matrix for this renderable object. */
		Matrix4 getModelMatrix(void) const;

	}; //#### end class SRenderable

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRENDERABLE_H__