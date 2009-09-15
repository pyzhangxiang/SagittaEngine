//////////////////////////////////////////////////////
// file: SRenderOption.h @ 2008-8-1 by Zhang Xiang
// declare of the class SRenderOption
// SMenu is a class ...
//////////////////////////////////////////////////////
#ifndef __SRENDEROPTION_H__
#define __SRENDEROPTION_H__

// INCLUDES //////////////////////////////////////////
//#include "SObject.h"
#include "../../common/configs/SagittaPrerequisites.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SRenderOption{// : public SObject{
	// enum declares
	public:
		// smallest render element type
		enum RenderElementType{
			RET_UNKNOWN = -1,
			RET_POINTS,
			RET_LINES,
			RET_LINE_STRIP,
			RET_LINE_LOOP,
			RET_TRIANGLES,
			RET_TRIANGLE_STRIP,
			RET_TRIANGLE_FAN,
			RET_QUADS
		};

		// witch face to cull
		enum FaceToCull{
			FTC_BACK,
			FTC_FRONT
		};

		// render mode
		enum RenderMode{
			RM_WIRE,
			RM_FILL
		};

		// texture mode
		enum TextureMode{
			TM_1D,
			TM_2D,
			TM_3D,
			TM_CUBE
		};

	// member variables
	private:
		/// render element type
		int m_RenderElementType;
		
		/// is light enable
		bool m_bLight;

		/// is fog enable
		bool m_bFog;
		
		/// is blend enable
		bool m_bBlend;
		
		/// is depth test enable
		bool m_bDepth;

		/// is stencil enable
		bool m_bStencil;

		/// is face culling enable
		bool m_bFaceCulling;

		/// is texture enable
		bool m_bTexture;

		/// is render object's individual render option enable
		bool m_bIndividualOption;

		/// witch face to cull
		int m_iFaceToCull;

		/// render mode
		int m_iRenderMode;

		/// texture mode
		int m_iTextureMode;

	// constructors & destructor
	public:
		SRenderOption(void);
		~SRenderOption(void);

	// member functions
	public:
		/** Gets render element type. */
		int renderElementType(void) const;
		/** Sets render element type. */
		void setRenderElementType(int aRet);

		/** Checks if light is enable. */
		bool isLightEnable(void) const;
		/** Sets the property of light. */
		void setLightEnable(bool aEnable);

		/** Checks if fog is enable. */
		bool isFogEnable(void) const;
		/** Sets the property of fog. */
		void setFogEnable(bool aEnable);

		/** Checks if blend is enable. */
		bool isBlendEnable(void) const;
		/** Sets the property of blend. */
		void setBlendEnable(bool aEnable);

		/** Checks if depth test is enable. */
		bool isDepthTestEnable(void) const;
		/** Sets the property of depth tes. */
		void setDepthTestEnable(bool aEnable);

		/** Checks if stencil test is enable. */
		bool isStencilTestEnable(void) const;
		/** Sets the property of stencil test. */
		void setStencilTestEnable(bool aEnable);

		/** Checks if face culling is enable. */
		bool isFaceCullingEnable(void) const;
		/** Sets the property of face culling. */
		void setFaceCullingEnable(bool aEnable);

		/** Checks if texture enabel. */
		bool isTextureEnable(void) const;
		/** Sets the property of texture. */
		void setTextureEnable(bool aEnable);

		/** Checks if individual render option enable. */
		bool isIndivdualOptionEnable(void) const;
		/** Sets if enable individual render option. */
		void setIndivdualOptionEnable(bool aEnable);

		/** Gets witch face to cull. */
		int faceToCull(void) const;
		/** Sets witch face to cull. */
		void setFaceToCull(int aFaceToCull);

		/** Gets render mode. */
		int renderMode(void) const;
		/** Sets render mode. */
		void setRenderMode(int aRenderMode);

		/** Gets texture mode. */
		int textureMode(void) const;
		/** Sets texture mode. */
		void setTextureMode(int aTextureMode);

	}; //#### end class SRenderOption

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRENDEROPTION_H__