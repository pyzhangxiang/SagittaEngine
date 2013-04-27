//////////////////////////////////////////////////////
// file: SSimpleEntities.h @ 2008-8-10 by Zhang Xiang
// declares of some simple entities class
// ...
//////////////////////////////////////////////////////
#ifndef __SGSIMPLEMESHES_H__
#define __SGSIMPLEMESHES_H__

// INCLUDES //////////////////////////////////////////
#include "sgMesh.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{
    
    void _SG_KernelExport sgLoadInternalMeshes(void);
    
    /** class representation
     @remarks
     
     @note
     
     @par
     
     */
	class _SG_KernelExport sgMeshVertex : public sgMesh
    {
        SG_META_DECLARE(sgMeshVertex)
        
        // member variables
	private:
		Vector3 *m_pPosition;
		Color *m_pColor;
        
        // constructors & destructor
	public:
		sgMeshVertex(void);
		virtual ~sgMeshVertex(void){}
        
        // member functions
	public:
		/** Gets position of this vertex. Overrides from sgSceneObject. */
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
		default: (0,0,0) -> (1, 1, 1)
     @note
     
     @par
     
     */
	class _SG_KernelExport sgMeshLine : public sgMesh
    {
        SG_META_DECLARE(sgMeshLine)
        
    public:
        static sgStrHandle InternalFileName;
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
		sgMeshLine(void);
		~sgMeshLine(void){}
        
        // member functions
	public:
		/** Gets the first vertex. */
		const Vector3 &p1(void) const;
		/** Gets the second vertex. */
		const Vector3 &p2(void) const;
        
        void setVertecies(const Vector3 &p1, const Color &c1,
                          const Vector3 &p2, const Color &c2);
        
		/** Calculates this line's length and returns. */
		Real length(void) const;
        
	}; //#### end class sgLine
    
    
    /** polygon indexed by vertics
     @remarks
     
     @note
     
     @par
     
     */
	class _SG_KernelExport sgMeshTriangle : public sgMesh{
        
        SG_META_DECLARE(sgMeshTriangle)
        
    public:
        static sgStrHandle InternalFileName;
        
        // member variables
	private:
		Vector3 *m_pPosData;
		Color *m_pColorData;
		Vector3 *m_pFaceNormal;
        
        // constructors & destructor
	public:
		sgMeshTriangle(void);
		~sgMeshTriangle(void){}
        
        // member functions
        
	public:
		/** Gets face normal. */
		const Vector3 &faceNormal(void) const;
        
		/** Calculate perimeter. */
		Real perimeter(void) const;
        
		/** Calculate area. */
		Real area(void) const;
        
		/** Gets the plane of this triangle. */
		Plane plane(void) const;
        
        void setVertecies(const Vector3 &p1, const Color &c1,
                          const Vector3 &p2, const Color &c2,
                          const Vector3 &p3, const Color &c3);
		
        
	}; //#### end class sgTriangle
    

	/** class representation
	@remarks
		4-----5
	   /|	 /|
	  / |	/ |
	 /  7--/--6
	3-----2  /
	| /   | /
	|/    |/
	0-----1
	@note

	@par

	*/
	class _SG_KernelExport sgMeshCube : public sgMesh
    {
        SG_META_DECLARE(sgMeshCube)
        
    public:
        static sgStrHandle InternalFileName;
        
	// member variables
	private:
		Real m_fEdgeLength;

	// constructors & destructor
	public:
		sgMeshCube(void);
		virtual ~sgMeshCube(void);

	// member functions
	private:
		void init(void);

	}; //#### end class sgCube


	/** class representation
	@remarks
		counterclockwise
			444
		   / | \
		  /  |  \
		 3 --1-- 2
		  \  |  /
		   \ | /
			000
		face: 0021, 0032, 0013, 1244, 2344, 3144
	@note

	@par

	*/
	class _SG_KernelExport sgMeshSphere : public sgMesh
    {
        SG_META_DECLARE(sgMeshSphere)
        
    public:
        static sgStrHandle InternalFileName;
        
	// member variables
	private:
		Real m_fRadius; // 1.0
		int m_iSlices;  // 30
		int m_iStacks;  // 30

	// constructors & destructor
	public:
		/** Constructor with auto-generated name.
			@param
				aRadius the radius of this sphere
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude).
			@param
				aStacks the number of subdivisions along the Z axis(similar to lines of latitude).
		*/
		sgMeshSphere(void);
		virtual ~sgMeshSphere(void);

	// member functions
	private:
		void init(void);
	public:
        /*
		int slices(void) const;
		int stacks(void) const;
         */
	}; //#### end class sgSphere


	/** class representation
	@remarks
		counterclockwise
		    4	
		  / | \
	     /  |  \
        /   |   \
	   1---0 3---2
		face: 021, 412, 032, 423, 013, 431

	@note

	@par

	*/
	class _SG_KernelExport sgMeshCone : public sgMesh
    {
        SG_META_DECLARE(sgMeshCone)
        
    public:
        static sgStrHandle InternalFileName;
        
	// member variables
	private:
		Real m_fRadius; // 1.0
		Real m_fHeight; // 1.0
		int m_iSlices;  // 30

	// constructors & destructor
	public:
		/** Constructor with auto-generated name.
			@param
				aRadius the radius of the base of the cone
			@param
				aHeight the height of the cone
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude)
		*/
		sgMeshCone(void);
		virtual ~sgMeshCone(void);

	// member functions
	private:
		void init(void);

	}; //#### end class sgCone


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgMeshGrid : public sgMesh
	{
		SG_META_DECLARE(sgMeshGrid)

	public:
		static sgStrHandle InternalFileName;

	public:
		sgMeshGrid(void);
		virtual ~sgMeshGrid(void);


	}; //#### end class sgGrid


	class _SG_KernelExport sgMeshPlane : public sgMesh
	{
		SG_META_DECLARE(sgMeshPlane)

	public:
		static sgStrHandle InternalFileName;

	public:
		sgMeshPlane(void);
		virtual ~sgMeshPlane(void);


	}; //#### end class sgGrid

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSIMPLEMESHES_H__
