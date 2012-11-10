//////////////////////////////////////////////////////
// file: sgMesh.h 
// created by zhangxiang on 09-10-4
// declares of the class sgMesh
// sgMesh is a class ...
//////////////////////////////////////////////////////
#ifndef __SGMESH_H__
#define __SGMESH_H__

// INCLUDES //////////////////////////////////////////
#include "sgResource.h"
#include "math/sgVector3.h"
#include "math/sgColor.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{
    
    // Indexed classes declares //////////////////////////////////////////////////////////////////////////
    
	/** Indexed triangle, use 3 size_t to express.
     */
	class _SG_KernelExport IndexedTriangle{
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
	class _SG_KernelExport IndexedEdge{
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
	
	class sgBuffer;
	class sgVertexData;
	class sgIndexData;
	class sgVertexIndexBuffer;
    
	/** class representation
     @remarks
     
     @note
     
     @par
     
     */
	class _SG_KernelExport sgMesh : public sgResource
	{
        SG_META_DECLARE(sgMesh)
        
    public:
        // smallest render element type
		enum RenderElementType{
			RET_UNKNOWN = -1,
			RET_POINTS = 1,
			RET_LINES = 2,
		//	RET_LINE_STRIP,
		//	RET_LINE_LOOP,
			RET_TRIANGLES = 3,
		//	RET_TRIANGLE_STRIP,
		//	RET_TRIANGLE_FAN,
			RET_QUADS = 4
		};
        
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
        
        
        /** if true, then this renderable object will render itself using it's own render function;
         *	or it will be render by unified shader.
         */
		//bool m_bRenderSelf;
        
		/// if i'm visible.
		//bool m_bVisible;
        
		/// average radius of this renderable object
		Real m_fAverageRadius;
        
		/// the max radius of this renderable object
		Real m_fMaxRadius;
        
		/// center position of this renderable object relative to my attached node
		Vector3 m_Center;
        
		/// special render option for this renderable object
		//sgRenderState m_RenderOption;
        
		/// my material
		//sgMaterial m_Material;
        
		/// vertex data, include position, normal, color, texture ...
		sgVertexData *m_pVertexData;
		/// vertex index data, include position, normal, color, texture indices
		sgIndexData *m_pIndexData;
        
		/// face normal list
		sgBuffer *m_pFaceNormalBuffer;
		
		// if the number of vertics and normals and texture coordinates are defferent
		// we will prepare a flat vertion data
		sgVertexData *mpVertexDataFlat;
		sgVertexIndexBuffer *mpIndexDataFlat;
		//sgBuffer *mpFaceNormalFlat;
		
		/// edge normal list, the size is equal to the number index data
        //	sgBuffer *m_pEdgeNormalList;	// no need now, for ray tracing
        
        // constructors & destructor
	public:
		sgMesh(void);
        
		/*sgMesh(uInt aPolyType,
                 size_t aVertexNum,
                 size_t aPolyNum,
                 bool aSmooth = true,
                 bool aCounterClockWise = true);
        
		sgMesh(const StdString& aName, 
                 uInt aPolyType,
                 size_t aVertexNum,
                 size_t aPolyNum,
                 bool aSmooth = true,
                 bool aCounterClockWise = true);*/
        
		virtual ~sgMesh(void);
        
        // member functions
	protected:
		/** Overrides from sgRenderable.
         @remarks
         Calculates center position of this entity
         and the maximum radius and the average radius.
         
         */
		void calCenterAndRadius(void);
        bool prepareFlatData(void);

        void release(void);

		void _setDirty(void){ m_bGeometryPrepared = false; }
	public:

		void reset(uInt aPolyType,
			size_t aVertexNum,
			size_t aPolyNum);
		
		/** Gets if geometry prepared. */
		bool geometryPrepared(void) const;
		/** Prepare center and radius. */
		bool prepareGeometry(void);
        
		/** Locates this object to it's center. */
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
		sgVertexData *getVertexData(void) const;
        
		/** Returns vertex index data. */
		sgIndexData *getIndexData(void) const;
        
        sgVertexIndexBuffer *getVertexIndexBuffer(void) const;
        
		/** Gets the vertex data for rendering. Overrides from sgRenderable.
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
		//void getVertexBuffer(sgVertexData *outVBuffer, sgVertexIndexBuffer *outIBuffer) const;

		bool getVertexBuffer(sgVertexData **outVBuffer, sgVertexIndexBuffer **outIBuffer);
        
		/** Gets face normal buffer 
         */
		sgBuffer *getFaceNormalBuffer(void) const;
        

	public:
		/** Render fucntion used to render this renderable object when
         that the varialbe m_bRenderSelf is true
         and need be overwrite by class user
         */
		//virtual void render(void);
        
		/** Gets my render option. */
		//const sgRenderState &renderOption(void) const;
		/** Gets my render option to be l-value. */
		//sgRenderState &renderOption(void);
		/** Sets my render option. */
		//void setRenderOption(const sgRenderState &aROP);
        
		/** Gets my material. */
		//const sgMaterial &material(void) const;
		/** Sets my material. */
		//void setMaterial(const sgMaterial &aMaterial);
        
		/** Checks if i'm visible. */
		//bool isVisible(void) const;
		/** Sets my visible property. */
		//void setVisible(bool abVisible);
        
		/** Gets my center position. */
		const Vector3 &center(void) const;
		
        
		/** Gets my average radius. */
		Real avgRadius(void) const;
		/** Gets my max radius. */
		Real maxRadius(void) const;
        
        virtual void load(SERIALIZE_LOAD_ARCHIVE &archive);
        virtual void save(SERIALIZE_SAVE_ARCHIVE &archive);
        
		/** Gets full model transform matrix for this renderable object. */
		//Matrix4 getModelMatrix(void) const;
        
	}; //#### end class sgEntity
    
    
    
	
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGMESH_H__

