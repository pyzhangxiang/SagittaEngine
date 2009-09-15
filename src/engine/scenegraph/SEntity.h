//////////////////////////////////////////////////////
// file: SEntity.h @ 2008-1-31 by Zhang Xiang
// declares of the class SEntity
// SEntity is a class ...
//////////////////////////////////////////////////////
#ifndef __SENTITY_H__
#define __SENTITY_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderable.h"
#include "../../math/SMathHeader.h"
#include <vector>
#include <string>
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	// Indexed classes declares //////////////////////////////////////////////////////////////////////////

	/** Indexed triangle, use 3 size_t to express.
	*/
	class _SagittaExport IndexedTriangle{
		// member variables
	public:
		union{
			size_t v[3];

			struct{
				size_t a, b, c;
			}; //#### end struct
		}; //#### end union

		// constructors & destructor
	public:
		IndexedTriangle(void){

		}

		IndexedTriangle(size_t aA, size_t aB, size_t aC)
			: a(aA), b(aB), c(aC){

		}

		IndexedTriangle(const IndexedTriangle &rhs)
			: a(rhs.a), b(rhs.b), c(rhs.c){

		}

		// member functions
	public:
		IndexedTriangle &operator=(const IndexedTriangle &rhs){
			a = rhs.a;
			b = rhs.b;
			c = rhs.c;

			return *this;
		}

		void setValues(size_t aA, size_t aB, size_t aC){
			a = aA;
			b = aB;
			c = aC;
		}

		size_t operator[](size_t i) const{
			assert(i < 3);
			return v[i];
		}

		size_t &operator[](size_t i){
			assert(i < 3);
			return v[i];
		}

	}; //#### end class IndexedTriangle


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport IndexedEdge{
	private:
		int m_iP1, m_iP2;

	public:
		inline IndexedEdge(int p1, int p2) : m_iP1(p1), m_iP2(p2){}
		inline int operator [] (const size_t viIndex) const{
#ifdef _DEBUG
			assert(viIndex < 2);
			// add error code...
#endif
			return *(&m_iP1 + viIndex);
		}

	}; //#### end class IndexedEdge


	/** Indexed polygon, template class
	*/
	template<size_t VN>
	class IndexedPolygon{
		// member variables
	public:
		size_t v[VN];

		// constructors & destructor
	public:
		IndexedPolygon(void){

		}

		IndexedPolygon(size_t av[VN]){
			memcpy(v, av, sizeof(size_t) * VN);
		}

		IndexedPolygon(const IndexedPolygon<VN> &rhs){
			memcpy(v, rhs.v, sizeof(size_t) * VN);
		}

		// member functions
	public:
		IndexedPolygon<VN> &operator=(const IndexedPolygon<VN> &rhs){
			memcpy(v, rhs.v, sizeof(size_t) * VN);
		}

		void setValues(size_t av[VN]){
			memcpy(v, av, sizeof(size_t) * VN);
		}

		size_t operator[](size_t i) const{
			assert(i < VN);
			return v[i];
		}

		size_t &operator[](size_t i){
			assert(i < VN);
			return v[i];
		}

	}; //#### end class IndexedPolygon

	// Indexed classes declares //////////////////////////////////////////////////////////////////////////

	class SNode;
	class SBuffer;
	class SVertexData;
	class SIndexData;
	class SVertexIndexBuffer;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SEntity : public SRenderable{
	// static members
	protected:
		static uLong ms_iEntityCount;	// count for entities

	// member variables
	protected:
		/// if center and radius are computed
		bool m_bGeometryPrepared;	

		/// polygon type
		uInt m_iPolyType;
		/// number of polygons
		size_t m_iPolyNum;
		/// number of vertices
		size_t m_iVertexNum;
		/// number of edges, not Euler edges, duplicate edges
	//	size_t m_iEdgeNum;

		/** true -- a smooth entity, the number it's vertex normals equals to the number of vertecis; 
		*	false -- a flat entity, the number it's vertex normals may be more than the number of vertecis.
		*		On every face, each vertex normal equals to the face normal. So we need not store vertex normal
		*	default false
		*/
		bool m_bSmooth;

		/** for normal compute
		*	true -- the normals of faces radiate
		*	false -- the normal of each face points to the center of the entity
		*	default true
		*/
		bool m_bNormalOuter;

		/** What's the vertices of a polygon form.
		*	true -- counter-clockwise
		*	false -- clockwise
		*/
		bool m_bCounterClockWise;
		
		
		/// edge normal list, the size is equal to the number index data
	//	SBuffer *m_pEdgeNormalList;	// no need now, for ray tracing

	// constructors & destructor
	public:
		SEntity(void);

		SEntity(uInt aPolyType,
				size_t aVertexNum,
				size_t aPolyNum,
				bool aSmooth = true,
				bool aCounterClockWise = true);

		SEntity(const StdString& aName, 
				uInt aPolyType,
				size_t aVertexNum,
				size_t aPolyNum,
				bool aSmooth = true,
				bool aCounterClockWise = true);

		virtual ~SEntity(void);

	// member functions
	protected:
		/** Overrides from SRenderable.
			@remarks
				Calculates center position of this entity
				and the maximum radius and the average radius.
			
		*/
		void calCenterAndRadius(void);
	public:
		/** Clones this object. Overrides from SSceneObject. */
		SSceneObject *clone() const;
		
		/** Gets if geometry prepared. */
		bool geometryPrepared(void) const;
		/** Prepare center and radius. */
		void prepareGeometry(void);

		/** Locates this object to it's center. Overrides from SRenderable. */
		void locateToCenter(void);

		/** Gets polygon type. */
		uInt polyType(void) const;

		/** Gets polygon num. */
		size_t polyNum(void) const;

		/** If I'm smooth. */
		bool isSmooth(void) const;
		/** Sets smooth property. */
		void setSmooth(bool aSmooth);

		/** If my normals radiate out. */
		bool normalOuter(void) const;
		/** Sets normal direction property. */ 
		void setNormalOuter(bool aNormalOuter);

		/** compute face, vertex normals for this entity, but only for triangle based entity. */
		void setupNormals(void);
		/** compute face, vertex normals for this entity, but only for triangle based entity.
			@param aNormalOuter
				Sets normal direction property first.
		*/
		void setupNormals(bool aNormalOuter);
	//	void setupEdgeNormals(void);

		/** Trianglates the entity 
			@remark
				4---3
			   /     \
			  5       2
			   \     /
			    0---1
			the result will be 015, 125, 235, 345
		*/
		void trianglate(void);

		/** Returns vertex data. */
		SVertexData *getVertexData(void) const;

		/** Returns vertex index data. */
		SIndexData *getIndexData(void) const;

		/** Gets the vertex data for rendering. Overrides from SRenderable.
			@remarks
				Copy the original vertex data.
				If this entity is not smooth, that mean
				it has different number of normals, texture coordinates and vertex,
				we should store dupicating vertex data and
				have to adjust index data.
			@param
				outVBuffer Where copied vertices data stored, should has been malloced.
			@param
				outIBuffer Where copied indices data stored, should has been malloced.
		*/
		void getVertexBuffer(SVertexData *outVBuffer, SVertexIndexBuffer *outIBuffer) const;

		/** Gets face normal buffer 
		*/
		SBuffer *getFaceNormalBuffer(void) const;

	}; //#### end class SEntity


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SVertex : public SEntity{
	// member variables
	private:
		Vector3 *m_pPosition;
		Color *m_pColor;

	// constructors & destructor
	public:
		SVertex(const Vector3& aPos, const Color &aColor = Color::WHITE);
		~SVertex(void){}

	// member functions
	public:
		/** Gets position of this vertex. Overrides from SSceneObject. */
		Vector3 position(void) const;
		/** Sets position of this vertex. */
		void setPos(const Vector3 &aPos);

		/** Gets color of this vertex. */
		const Color &color(void) const;
		/** Sets color of this vertex. */
		void setColor(const Color &arColor);

	}; //#### end class SVertex


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SLine : public SEntity{
	// enum declares
	public:
		/* for future...
		enum LineMode{

		}; */

	// member variables
	private:
		Vector3 *m_pPosData;
		Color *m_pColorData;

	// constructors & destructor
	public:
		SLine(const SVertex &aP0, const SVertex &aP1);
		SLine(const Vector3 &aP0, const Vector3 &aP1);
		~SLine(void){}

	// member functions
	private:
		void init(const SVertex &aV0, const SVertex &aV1);
	public:
		/** Gets the first vertex. */
		const Vector3 &fstVertex(void) const;
		/** Gets the second vertex. */
		const Vector3 &scdVertex(void) const;

		/** Calculates this line's length and returns. */
		Real length(void) const;

	}; //#### end class SLine


	/** polygon indexed by vertics
	@remarks

	@note

	@par

	*/
	class _SagittaExport STriangle : public SEntity{
	// member variables
	private:
		Vector3 *m_pPosData;
		Color *m_pColorData;
		Vector3 *m_pFaceNormal;

	// constructors & destructor
	public:
		STriangle(const SVertex &av0,
				const SVertex &av1,
				const SVertex &av2,
				bool abCounterClockWise = true);
		STriangle(const Vector3 &av0,
				const Vector3 &av1,
				const Vector3 &av2,
				bool abCounterClockWise = true);
		~STriangle(void){}

	// member functions
	private:
		void init(const SVertex &ap0,
				const SVertex &ap1,
				const SVertex &ap2,
				bool abCounterClockWise);

	public:
		/** Gets face normal. */
		const Vector3 &faceNormal(void) const;

		/** Calculate perimeter. */
		Real perimeter(void) const;

		/** Calculate area. */
		Real area(void) const;

		/** Gets the plane of this triangle. */
		Plane plane(void) const;
		

	}; //#### end class STriangle

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SENTITY_H__