//////////////////////////////////////////////////////
// file: SSimpleEntities.cpp @ 2008-8-10 by Zhang Xiang
// defines of some simple entities class
// ...
//////////////////////////////////////////////////////

//  [10/10/2012 zhangxiang]
// INCLUDES //////////////////////////////////////////
#include "sgSimpleMeshes.h"
#include "sgResourceCenter.h"
#include "engine/common/sgException.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgUtil.h"
#include "math/sgVector2.h"
#include "math/sgVector3.h"
#include "math/sgPlane.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
    
    sgStrHandle sgMeshLine::InternalFileName = "__sagitta.internal.sgmeshline.mesh";
    sgStrHandle sgMeshTriangle::InternalFileName = "__sagitta.internal.sgmeshtriangle.mesh";
    sgStrHandle sgMeshCube::InternalFileName = "__sagitta.internal.sgmeshcube.mesh";
    sgStrHandle sgMeshSphere::InternalFileName = "__sagitta.internal.sgmeshsphere.mesh";
    sgStrHandle sgMeshCone::InternalFileName = "__sagitta.internal.sgmeshcone.mesh";
	sgStrHandle sgMeshGrid::InternalFileName = "__sagitta.internal.sgmeshgrid.mesh";
	sgStrHandle sgMeshPlane::InternalFileName = "__sagitta.internal.sgmeshplane.mesh";
    
    void sgLoadInternalMeshes(void)
    {
        sgResourceCenter *rm = sgResourceCenter::instance();
        if(rm)
        {
            rm->createResource(sgMeshLine::GetClassName(), sgMeshLine::InternalFileName);
            rm->createResource(sgMeshTriangle::GetClassName(), sgMeshTriangle::InternalFileName);
            rm->createResource(sgMeshCube::GetClassName(), sgMeshCube::InternalFileName);
            rm->createResource(sgMeshSphere::GetClassName(), sgMeshSphere::InternalFileName);
            rm->createResource(sgMeshCone::GetClassName(), sgMeshCone::InternalFileName);
        }
    }
    
    
    // sgVertex //////////////////////////////////////////////////////////////////////////
    
    SG_META_DEFINE(sgMeshVertex, sgMesh)
    
	sgMeshVertex::sgMeshVertex() : sgMesh()
    {
        reset(1, 1, 1);
        
		m_pPosition = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
        
		m_pColor = static_cast<Color*>(m_pVertexData->createElement(sgVertexBufferElement::ColorAttributeName, RDT_UBYTE, 4, m_iVertexNum)->data());
        
		size_t *pIndex = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());
		// juse is this variable
		*pIndex = 0;

		prepareGeometry();
	}
    
	//  [1/5/2009 zhangxiang]
	Vector3 sgMeshVertex::position(void) const{
		return *m_pPosition;
	}
    
	//  [1/5/2009 zhangxiang]
	void sgMeshVertex::setPos(const Vector3 &aPos){
		*m_pPosition = aPos;
	}
    
	//  [7/31/2008 zhangxiang]
	const Color &sgMeshVertex::color(void) const{
		return *m_pColor;
	}
    
	//  [7/31/2008 zhangxiang]
	void sgMeshVertex::setColor(const Color &aColor){
		*m_pColor = aColor;
	}
    
	// sgVertex //////////////////////////////////////////////////////////////////////////
    
    
    // sgLine //////////////////////////////////////////////////////////////////////////
    
    SG_META_DEFINE(sgMeshLine, sgMesh)
    
	//  [1/5/2009 zhangxiang]
	sgMeshLine::sgMeshLine() : sgMesh()
    {
        reset(2, 2, 1);
        
        m_pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
        m_pPosData[0] = Vector3(0.0f, 0.0f, 0.0f);
		m_pPosData[1] = Vector3(1.0f, 1.0f, 1.0f);
        
        
		m_pColorData = static_cast<Color*>(m_pVertexData->createElement(sgVertexBufferElement::ColorAttributeName, RDT_UBYTE, 4, m_iVertexNum)->data());
        m_pColorData[0] = Color::WHITE;
		m_pColorData[1] = Color::WHITE;
        
		size_t *pIndex = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());
		pIndex[0] = 0;
		pIndex[1] = 1;
        
		prepareGeometry();
	}
    
	//  [1/5/2009 zhangxiang]
	const Vector3 &sgMeshLine::p1() const{
		return m_pPosData[0];
	}
    
	//  [1/5/2009 zhangxiang]
	const Vector3 &sgMeshLine::p2() const{
		return m_pPosData[1];
	}
    
	//  [1/5/2009 zhangxiang]
	Real sgMeshLine::length() const{
		return m_pPosData[0].distance(m_pPosData[1]);
	}
    
    void sgMeshLine::setVertecies(const Vector3 &p1, const Color &c1,
                                  const Vector3 &p2, const Color &c2)
    {
		_setDirty();

        m_pPosData[0] = p1;
		m_pPosData[1] = p2;
        
        m_pColorData[0] = c1;
		m_pColorData[1] = c2;
        
        prepareGeometry();

    }
    
	// sgLine //////////////////////////////////////////////////////////////////////////
    
    // STriangle //////////////////////////////////////////////////////////////////////////
    
    SG_META_DEFINE(sgMeshTriangle, sgMesh)
    
	sgMeshTriangle::sgMeshTriangle(void) : sgMesh()
    {
        reset(3, 3, 1);
        
        m_bCounterClockWise = true;
        
        m_pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
        m_pPosData[0] = Vector3(-1.0f, 0.0f, 0.0f);
		m_pPosData[1] = Vector3(1.0f, 0.0f, 0.0f);
        m_pPosData[2] = Vector3(0.0f, 1.0f, 0.0f);
        
		m_pColorData = static_cast<Color*>(m_pVertexData->createElement(sgVertexBufferElement::ColorAttributeName, RDT_UBYTE, 4, m_iVertexNum)->data());
        m_pColorData[0] = Color::WHITE;
        m_pColorData[1] = Color::WHITE;
        m_pColorData[2] = Color::WHITE;
        
		size_t *pIndex = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());
		pIndex[0] = 0;
		pIndex[1] = 1;
		pIndex[2] = 2;
        
		//setupNormals();
        prepareGeometry();
		m_pFaceNormal = static_cast<Vector3*>(m_pFaceNormalBuffer->data());
	}
    
	//  [1/5/2009 zhangxiang]
	const Vector3 &sgMeshTriangle::faceNormal(void) const{
		return *m_pFaceNormal;
	}
    
	//  [1/5/2009 zhangxiang]
	Real sgMeshTriangle::perimeter(void) const{
		const Vector3 &a = m_pPosData[0];
		const Vector3 &b = m_pPosData[1];
		const Vector3 &c = m_pPosData[2];
		
		return a.distance(b) + b.distance(c) + c.distance(a);
	}
    
	//  [1/5/2009 zhangxiang]
	Real sgMeshTriangle::area(void) const{
		const Vector3 &a = m_pPosData[0];
		const Vector3 &b = m_pPosData[1];
		const Vector3 &c = m_pPosData[2];
        
		Real fDistanceAB = a.distance(b);
		Real fDistanceBC = b.distance(c);
		Real fDistanceCA = c.distance(a);
        
		// Heron's Formula
		Real s = (fDistanceAB + fDistanceBC + fDistanceCA) / 2;
		return Math::Sqrt(s * (s - fDistanceAB) * (s - fDistanceBC) * (s - fDistanceCA));
	}
    
	//  [1/5/2009 zhangxiang]
	Plane sgMeshTriangle::plane(void) const{
		return Plane(m_pPosData[0], m_pPosData[1], m_pPosData[2]);
	}
    
    void sgMeshTriangle::setVertecies(const Vector3 &p1, const Color &c1,
                                      const Vector3 &p2, const Color &c2,
                                      const Vector3 &p3, const Color &c3)
    {

		_setDirty();

        m_pPosData[0] = p1;
		m_pPosData[1] = p2;
        m_pPosData[2] = p3;
        
        m_pColorData[0] = c1;
		m_pColorData[1] = c2;
        m_pColorData[2] = c3;
        
		//setupNormals();
		prepareGeometry();
        
    }
    
	// STriangle //////////////////////////////////////////////////////////////////////////

	// sgMeshCube //////////////////////////////////////////////////////////////////////////
    
    SG_META_DEFINE(sgMeshCube, sgMesh)

	//  [8/10/2008 zhangxiang]
	sgMeshCube::sgMeshCube()
	: m_fEdgeLength(1.0f)
    {
        // smooth false
        //this->setSmooth(false);
        // should use 24 vertecies
        reset(3, 8, 12);
		init();
	}

	//  [8/10/2008 zhangxiang]
	sgMeshCube::~sgMeshCube(void){

	}

	//  [1/5/2009 zhangxiang]
	void sgMeshCube::init(void){

		Real halfLength = m_fEdgeLength * 0.5f;

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
		size_t *pIndexData = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());

		// setup vertecies
		pPosData[0] = Vector3(-halfLength, -halfLength, halfLength);
		pPosData[1] = Vector3(halfLength, -halfLength, halfLength);
		pPosData[2] = Vector3(halfLength, halfLength, halfLength);
		pPosData[3] = Vector3(-halfLength, halfLength, halfLength);

		pPosData[4] = Vector3(-halfLength, halfLength, -halfLength);
		pPosData[5] = Vector3(halfLength, halfLength, -halfLength);
		pPosData[6] = Vector3(halfLength, -halfLength, -halfLength);
		pPosData[7] = Vector3(-halfLength, -halfLength, -halfLength);

		// setup faces
		pIndexData[0] = 0;
		pIndexData[1] = 1;
		pIndexData[2] = 2;

		pIndexData[3] = 2;
		pIndexData[4] = 3;
		pIndexData[5] = 0;

		pIndexData[6] = 0;
		pIndexData[7] = 7;
		pIndexData[8] = 6;

		pIndexData[9] = 6;
		pIndexData[10] = 1;
		pIndexData[11] = 0;

		pIndexData[12] = 4;
		pIndexData[13] = 5;
		pIndexData[14] = 6;

		pIndexData[15] = 6;
		pIndexData[16] = 7;
		pIndexData[17] = 4;

		pIndexData[18] = 4;
		pIndexData[19] = 7;
		pIndexData[20] = 0;

		pIndexData[21] = 0;
		pIndexData[22] = 3;
		pIndexData[23] = 4;

		pIndexData[24] = 5;
		pIndexData[25] = 4;
		pIndexData[26] = 3;

		pIndexData[27] = 3;
		pIndexData[28] = 2;
		pIndexData[29] = 5;

		pIndexData[30] = 2;
		pIndexData[31] = 1;
		pIndexData[32] = 6;

		pIndexData[33] = 6;
		pIndexData[34] = 5;
		pIndexData[35] = 2;

        setSmooth(false);
		//setupNormals();
		//	computeEdgeNormal(); for future ...
		prepareGeometry();
	}

	// sgMeshCube //////////////////////////////////////////////////////////////////////////


	// sgMeshSphere //////////////////////////////////////////////////////////////////////////

    SG_META_DEFINE(sgMeshSphere, sgMesh)

	//  [8/18/2008 zhangxiang]
	sgMeshSphere::sgMeshSphere()
	: sgMesh(),	m_fRadius(1.0f), m_iSlices(30), m_iStacks(30)
    {
        // smooth true
        reset(4, m_iSlices * m_iStacks + 2, m_iSlices * (m_iStacks + 1));
		// setup geometry
		init();
	}

	//  [1/5/2009 zhangxiang]
	void sgMeshSphere::init(void){
        /*
		if(m_fRadius <= 0 || m_iSlices < 2 || m_iStacks <= 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Invalid parameters for the sphere.",
							"sgMeshSphere::init");
		}
         */

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
		size_t *pIndexData = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());

		Real RX2 = m_fRadius + m_fRadius;
		Real RUp2 = m_fRadius * m_fRadius;

		Real Dy = RX2 / (m_iStacks + 1);
		Quaternion Dq(Math::PI_X_2 / m_iSlices, Vector3(0.0, 1.0, 0.0));

		size_t index = 0;
		std::vector< std::vector<size_t> > indexedStacks;
		std::vector<size_t> indexedStack;

		// vertices
		pPosData[0] = Vector3(0.0, -m_fRadius, 0.0);
		indexedStack.clear();
		for(int i=0; i<m_iSlices; i++){
			indexedStack.push_back(index);
		}
		indexedStacks.push_back(indexedStack);
		index++;

		Real y = -m_fRadius + Dy;
		for(int k=1; k<=m_iStacks; ++k, y+=Dy){
			indexedStack.clear();

			Real SectionRUp2 = RUp2 - y * y;

			Vector3 o(0.0, y, Math::Sqrt(SectionRUp2));
			for(int i=0; i<m_iSlices; ++i, ++index){
				indexedStack.push_back(index);

				pPosData[index] = o;
				o = Dq * o;
			}
			indexedStacks.push_back(indexedStack);
		}

		pPosData[index] = Vector3(0.0, m_fRadius, 0.0);
		indexedStack.clear();
		for(int i=0; i<m_iSlices; ++i){
			indexedStack.push_back(index);
		}
		indexedStacks.push_back(indexedStack);

		// faces
		int faceNum = (m_iStacks + 1) * m_iSlices;
		uInt ii = 0;
		for(int i=0; i<=m_iStacks; ++i){
			int stack = i;
			int nextStack = i+1;

			for(int j=0; j<m_iSlices; ++j){
				int slice = j;
				int nextSlice = j + 1 < m_iSlices ? j + 1: 0;

				pIndexData[ii++] = indexedStacks[stack][slice];
				pIndexData[ii++] = indexedStacks[stack][nextSlice];
				pIndexData[ii++] = indexedStacks[nextStack][nextSlice];
				pIndexData[ii++] = indexedStacks[nextStack][slice];
			}
		}

		m_Center = Vector3::ZERO;
		m_fAverageRadius = m_fMaxRadius = m_fRadius;

		// will calculate normals 
		//trianglate();
		prepareGeometry();
	}

	//  [8/18/2008 zhangxiang]
	sgMeshSphere::~sgMeshSphere(void){

	}
/*
	//  [8/28/2008 zhangxiang]
	Real sgMeshSphere::radius(void) const{
		return m_fRadius;
	}

	//  [8/28/2008 zhangxiang]
	int sgMeshSphere::slices(void) const{
		return m_iSlices;
	}

	//  [8/28/2008 zhangxiang]
	int sgMeshSphere::stacks(void) const{
		return m_iStacks;
	}
*/

	// sgMeshCone //////////////////////////////////////////////////////////////////////////
    
    SG_META_DEFINE(sgMeshCone, sgMesh)

	//  [8/18/2008 zhangxiang]
	sgMeshCone::sgMeshCone()
	: sgMesh(),	m_fRadius(1.0f), m_fHeight(1.0f), m_iSlices(30)
    {
        // smooth true
		reset(3, m_iSlices + 2, m_iSlices + m_iSlices);
		init();
	}

	//  [8/18/2008 zhangxiang]
	sgMeshCone::~sgMeshCone(void){

	}

	//  [1/5/2009 zhangxiang]
	void sgMeshCone::init(void){
	/*	if(m_fRadius <= 0 || m_fHeight <= 0 || m_iSlices < 2){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Invalid parameters for the cone",
							"sgMeshCone::init");
		}
*/
		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
		size_t *pIndexData = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());

		// setup vertices
		pPosData[0] = Vector3::ZERO;

		Quaternion Dq(Math::PI_X_2 / m_iSlices, Vector3(0.0, 1.0, 0.0));

		Vector3 o(0.0, 0.0, m_fRadius);
		uInt index = 1;
		for(int i=0; i<m_iSlices; i++){
			pPosData[index++] = o;
		//	m_pVertexList->push_back(o);
			o = Dq * o;
		}
		pPosData[index] = Vector3(0.0, m_fHeight, 0.0);

		// setup faces
		size_t faceNum = m_iSlices + m_iSlices;

		size_t lowCenter = 0;
		size_t highCenter = index; //static_cast<size_t>(m_pVertexList->size() - 1);

		int slice = 1;
		size_t ii = 0;
		for(size_t i=0; i<faceNum; ++i){
			int nextSlice = slice + 1 <= m_iSlices ? slice + 1 : 1;

			pIndexData[ii++] = lowCenter;
			pIndexData[ii++] = nextSlice;
			pIndexData[ii++] = slice;

			++i;

			pIndexData[ii++] = highCenter;
			pIndexData[ii++] = slice;
			pIndexData[ii++] = nextSlice;

			++slice;
		}

		//setupNormals();
	//	setupEdgeNormal(); // for future ...
		prepareGeometry();
	}

	// sgMeshCone //////////////////////////////////////////////////////////////////////////



	// sgMeshGrid //////////////////////////////////////////////////////////////////////////
    
    SG_META_DEFINE(sgMeshGrid, sgMesh)

	//  [8/18/2008 zhangxiang]
	sgMeshGrid::sgMeshGrid()
	: sgMesh()
    {
		UInt32 aiLengthPerUnit = 50;
		UInt32 aiHUnitNum = 50;
		UInt32 aiVUnitNum = 50;
		//Color arColor = Color::YELLOW;
        // smooth true
		reset(2, 2 * (aiHUnitNum + aiVUnitNum), aiHUnitNum + aiVUnitNum + 2);

		int hTotalLength = aiLengthPerUnit * aiHUnitNum;
		int vTotalLength = aiLengthPerUnit * aiVUnitNum;
		int hHalfTotalLength = hTotalLength * 0.5;
		int vHalfTotalLength = vTotalLength * 0.5;

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
		//Color *pColorData = static_cast<Color*>(m_pVertexData->createElement(sgVertexBufferElement::ET_COLOR, 4, m_iVertexNum)->data());
		size_t *pIndex = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());

		size_t vi = 0;
		size_t i = 0;
		size_t ii = 0;
		//	for(int x=-hHalfTotalLength; x<=hHalfTotalLength; x+=aiLengthPerUnit){
		int x = -hHalfTotalLength;
		for(uInt vl=0; vl<=aiVUnitNum; ++vl){
			pPosData[vi] = Vector3(x, 0, -vHalfTotalLength);
			//pColorData[vi] = arColor;
			pIndex[ii] = i;

			++vi; ++i; ++ii;

			pPosData[vi] = Vector3(x, 0, vHalfTotalLength);
			//pColorData[vi] = arColor;
			pIndex[ii] = i;

			++vi; ++i; ++ii;

			x += aiLengthPerUnit;
		}

		pIndex[ii++] = 0;
		pIndex[ii++] = vi - 2;

		pIndex[ii++] = 1;
		pIndex[ii++] = vi - 1;

		//	for(int z=-vHalfTotalLength+aiLengthPerUnit; z<vHalfTotalLength; z+=aiLengthPerUnit){
		int z = -vHalfTotalLength + aiLengthPerUnit;
		for(uInt vh=2; vh<=aiHUnitNum; ++vh){
			pPosData[vi] = Vector3(-hHalfTotalLength, 0, z);
			//pColorData[vi] = arColor;
			pIndex[ii] = i;

			++vi; ++i; ++ii;

			pPosData[vi] = Vector3(hHalfTotalLength, 0, z);
			//pColorData[vi] = arColor;
			pIndex[ii] = i;

			++vi; ++i; ++ii;

			z += aiLengthPerUnit;
		}

		m_bNormalOuter = true;

		prepareGeometry();
	}

	//  [8/18/2008 zhangxiang]
	sgMeshGrid::~sgMeshGrid(void){

	}


	// sgMeshPlane //////////////////////////////////////////////////////////////////////////

	SG_META_DEFINE(sgMeshPlane, sgMesh)

		//  [8/18/2008 zhangxiang]
		sgMeshPlane::sgMeshPlane()
		: sgMesh()
	{
		UInt32 aiLengthPerUnit = 10;
		UInt32 aiHUnitNum = 10;
		UInt32 aiVUnitNum = 10;
		//reset(2, 2 * (aiHUnitNum + aiVUnitNum), aiHUnitNum + aiVUnitNum + 2);
		reset(4, 4, 1);

		int hTotalLength = aiLengthPerUnit * aiHUnitNum;
		int vTotalLength = aiLengthPerUnit * aiVUnitNum;
		int hHalfTotalLength = hTotalLength * 0.5;
		int vHalfTotalLength = vTotalLength * 0.5;

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, m_iVertexNum)->data());
		Vector2 *pUV0Data = static_cast<Vector2*>(m_pVertexData->createElement(sgVertexBufferElement::UV0AttributeName, RDT_F, 2, m_iVertexNum)->data());
		//Color *pColorData = static_cast<Color*>(m_pVertexData->createElement(sgVertexBufferElement::ET_COLOR, 4, m_iVertexNum)->data());
		size_t *pIndex = static_cast<size_t*>(m_pIndexData->createElement(sgVertexBufferElement::ET_VERTEX)->data());
		
		pPosData[0] = Vector3(-hHalfTotalLength, 0.0f, -vHalfTotalLength);
		pPosData[1] = Vector3(-hHalfTotalLength, 0.0f, vHalfTotalLength);
		pPosData[2] = Vector3(hHalfTotalLength, 0.0f, vHalfTotalLength);
		pPosData[3] = Vector3(hHalfTotalLength, 0.0f, -vHalfTotalLength);

		pUV0Data[0] = Vector2(0.0f, 0.0f);
		pUV0Data[1] = Vector2(0.0f, 1.0f);
		pUV0Data[2] = Vector2(1.0f, 1.0f);
		pUV0Data[3] = Vector2(0.0f, 0.0f);

		pIndex[0] = 0;
		pIndex[1] = 1;
		pIndex[2] = 2;
		pIndex[3] = 3;

		m_bNormalOuter = true;

		prepareGeometry();
	}

	//  [8/18/2008 zhangxiang]
	sgMeshPlane::~sgMeshPlane(void){

	}

	

	// sgMeshGrid //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta