//////////////////////////////////////////////////////
// file: SagiRenderer.h @ 2008-10-5 by Zhang Xiang
// declares of the class SagiRenderer
// SagiRenderer is a class ...
//////////////////////////////////////////////////////
#ifndef __SAGIRENDERER_H__
#define __SAGIRENDERER_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderer.h"
#include "../buffer/SFrameBufferManager.h"
#include "../../math/SMathHeader.h"
#include <list>
#include <vector>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SRenderable;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SagiRenderer : public SRenderer{
	// inner class defines
	protected:
		/** class representation
		@remarks

		@note

		@par

		*/
		struct SagiLight{
			Vector3 position;	// should be in world space
			Color::GLColor ambient;
			Color::GLColor diffuse;
			Color::GLColor specular;

			SagiLight(const Vector3 &pos,
				const Color::GLColor &amb,
				const Color::GLColor &diff,
				const Color::GLColor spec) :
			position(pos), ambient(amb), diffuse(diff), specular(spec){

			}

		}; //#### end struct SagiLight
		
	// type defines
	protected:
		typedef std::vector<Vector3> V3List;
		typedef std::vector<Vector2> V2List;
		typedef std::vector<Color::GLColor> ColorList;
		typedef std::vector<Real> RealList;
		typedef std::list<size_t> IndexList;
		typedef std::vector<uShort> CountList;

		typedef std::vector<SagiLight> SagiLightList;

	// inner class defines
	protected:

		/** Stores one object's vertex buffers and index buffer. */
		struct VertexBuffers{
			V3List pvertices;
			ColorList pcolors;
			V3List pnormals;
			V2List ptexcoords;
			RealList pflogcoords;
			IndexList pindices;
			CountList padjacentcount;	// vertices adjacent faces count
			int elementtype;
			size_t vertexnum;
			size_t facenum;

			SRenderable *renderable;

			VertexBuffers(void) :
			/*pvertices(0),
			pcolors(0),
			pnormals(0),
			ptexcoords(0),
			pflogcoords(0),
			pindices(0),*/
			elementtype(-1),
			vertexnum(0),
			facenum(0),
			renderable(0){

			}

			void release(void){
				pvertices.clear();
				pcolors.clear();
				pnormals.clear();
				ptexcoords.clear();
				pflogcoords.clear();
				pindices.clear();
				padjacentcount.clear();
				elementtype = -1;
				vertexnum = 0;
				facenum = 0;
				renderable = 0;
			}
		};//#### end class VertexBuffers

	// enum defines
	protected:
		/// Clip code
		enum ClipCode{
			CC_IN = 0x00,			// in the frustum

			CC_OUT_NEAR = 0x01,		// z > near
			CC_OUT_FAR = 0x02,		// z > far
			
			CC_OUT_LEFT = 0x04,		// x < x_min
			CC_OUT_RIGHT = 0x08,	// x > x_max

			CC_OUT_UP = 0x10,		// y > y_max
			CC_OUT_DOWN = 0x20		// y < y_min
		};

		/// Line Clip Code in screen space
		enum LineClipCode{
			LCC_IN = 0x00,			// total in region
			LCC_N = 0x08,			// north
			LCC_S = 0x04,			// south
			LCC_E = 0x02,			// east
			LCC_W = 0x01,			// west

			LCC_NE = 0x0a,			// north ease
			LCC_SE = 0x06,			// south east
			LCC_NW = 0x09,			// north west
			LCC_SW = 0x05			// south west
		};

	// member variables
	private:
		/// frame buffers
		SFrameBufferManager m_FrameBuffers;

		/// current viewport frame buffers
		mutable SFrameBufferManager m_CurFrameBuffers;

		/// projection matrix
		mutable Matrix4 m_CurProjMatrix;

		/// view matrix
		mutable Matrix4 m_CurViewMatrix;

		/// model matrix
		mutable Matrix4 m_CurModelMatrix;

		/// screen matrix
		mutable Matrix4 m_CurScrMatrix;

		/// vertex buffers for render
		mutable VertexBuffers m_CurVertexBuffers;

		/// light list for rendering
		mutable SagiLightList m_CurSagiLightList;

	// constructors & destructor
	public:
		SagiRenderer(int aTWidth, int aTHeight, bool aEnableSetencil);
		~SagiRenderer(void);

	// member functions
	private:
		/** Internal hook. Does something when target resized.
			@remarks SagiRenderer needs it to resize it's frame buffers.
		*/
		void doSthWhenResized(void);

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

		/** Culls faces.
			@remarks Witch face to cull according to render option.
		*/
		void cullFaces(void) const;

		/** Culls back faces. */
		void cullBackFacesImpl(void) const;

		/** Culls front faces. */
		void cullFrontFacesImpl(void) const;

		/** Transform vertices from wold space to view space. */
		void viewTransform(void) const;

		/** Clips polygons.
			@remarks
				Assumes in view space.
				Uses near-clip-plane to clip polygons.
				And at the same time, culls polygons that
				not in the frustum totally.
				May change the number of vertex and face.
		*/
		void clipFaces(void) const;

		/** Clips points in view space. Called by clipFaces. */
		void clipPointsImpl(void) const;

		/** Clips lines in view space. Called by clipFaces. */
		void clipLinesImpl(void) const;

		/** Clips triangles in view space. Called by clipFaces. */
		void clipTrianglesImpl(void) const;

		/** Vertex shader. */
		void vertexShader(void) const;

		/** Gouraund shader. */
		void gourandShaderImpl(void) const;

		/** Transform vertices from view space to screen space.
			@remarks First to perspective space and then to screen space.
		*/
		void screenTransform(void) const;

		/** Renders to frame buffer. */
		void renderToFrameBuffer(void) const;

		/** Renders points. */
		void renderPointsImpl(void) const;

		/** Renders lines. */
		void renderLinesImpl(void) const;

		/** Clips lines in screen space. Calles by lineColorInterpolation. 
			@remarks Cohen-Sutherland.
		*/
		bool clipLinesInScreen(int &aX1, int &aY1, Color::GLColor &aColor1,
							int &aX2, int &aY2, Color::GLColor &aColor2) const;

		/** Clips lines in screen space without color (white for default). Calles by lineInterpolation. */
		bool clipLinesInScreen(int &aX1, int &aY1, int &aX2, int &aY2) const;

		/** Clips lines in screen space. Calles by lineColorInterpolation.
			@remarks This is a version with z-interpolation
		*/
		bool clipLinesInScreen(int &aX1, int &aY1, Real &aZ1, Color::GLColor &aColor1,
							int &aX2, int &aY2, Real &aZ2, Color::GLColor &aColor2) const;

		/** Color interpolation on line. */
		void lineColorInterpolation(int aX1, int aY1, const Color::GLColor &aColor1,
								int aX2, int aY2, const Color::GLColor &aColor2) const;

		/** Color interplation on line with depth test. */
		void lineColorInterpolation(const Vector3 &aV1, const Color::GLColor &aColor1,
									const Vector3 &aV2, const Color::GLColor &aColor2) const;

		/** Draws line without color interpolation. */
		void lineInterpolation(int aX1, int aY1, int aX2, int aY2) const;

		/** Color interpolation on horizontal line. */
		void hlineColorInterpolation(int aX1, int aX2, int aY,
									const Color::GLColor &aColor1,
									const Color::GLColor &aColor2) const;

		/** Color interpolation on horizontal line with depth test. */
		void hlineColorInterpolation(int aX1, Real aZ1, int aX2, Real aZ2, int aY,
									const Color::GLColor &aColor1,
									const Color::GLColor &aColor2) const;

		/** Draws horizontal line without color interpolation. */
		void hlineInterpolation(int aX1, int aX2, int aY) const;

		/** Renders triangles. */
		void renderTrianglesImpl(void) const;

		/** Color interpolation on triangle. */
		void triangleColorInterpolation(int aX1, int aY1, const Color::GLColor &aColor1,
									int aX2, int aY2, const Color::GLColor &aColor2,
									int aX3, int aY3, const Color::GLColor &aColor3) const;

		/** Color interpolation on triangle with depth test. */
		void triangleColorInterpolation(const Vector3 &aV1, const Color::GLColor &aColor1,
									const Vector3 &aV2, const Color::GLColor &aColor2,
									const Vector3 &aV3, const Color::GLColor &aColor3) const;

		/** Draws triagle without color interpolation (default white). */
		void triangleInterpolation(int aX1, int aY1,
								int aX2, int aY2,
								int aX3, int aY3) const;

	protected:
		/** Overridden from SRenderer. Does conversion between Sagitta element type to graphics api's element type (e.g. ET_POINTS to GL_POINTS). */
		int retMapping(int aRet) const{ return 0; }

	public:
		/** Overridden from SRenderer. Initialize the environment of graphics device. */
		void init(void);

	}; //#### end class SagiRenderer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SAGIRENDERER_H__