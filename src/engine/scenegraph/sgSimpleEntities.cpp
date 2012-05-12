//////////////////////////////////////////////////////
// file: SSimpleEntities.cpp @ 2008-8-10 by Zhang Xiang
// defines of some simple entities class
// ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgSimpleEntities.h"
#include "../buffer/sgVertexData.h"
#include "../buffer/sgVertexIndexBuffer.h"
#include "../buffer/sgVertexBufferElement.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// sgCube //////////////////////////////////////////////////////////////////////////

	//  [8/10/2008 zhangxiang]
	sgCube::sgCube(Real aEdgeLength)
	: sgEntity(3, 8, 12, false), m_fEdgeLength(aEdgeLength){
		StdString newname = this->name();
		newname.insert(0, "Cube");
		sgSceneObject::_setName(newname);
		m_RenderOption.setRenderElementType(sgRenderOption::RET_TRIANGLES);
		init();
	}

	sgCube::sgCube(const StdString &asName, Real aEdgeLength)
	: sgEntity(3, 8, 12, false), m_fEdgeLength(aEdgeLength){
		sgSceneObject::_setName(asName);
		m_RenderOption.setRenderElementType(sgRenderOption::RET_TRIANGLES);
		init();
	}

	//  [8/10/2008 zhangxiang]
	sgCube::~sgCube(void){

	}

	//  [1/5/2009 zhangxiang]
	void sgCube::init(void){
		if(m_fEdgeLength <= 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Negative edge length.",
							"sgCube::init");
		}

		Real halfLength = m_fEdgeLength * 0.5;

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::ET_VERTEX, 3, m_iVertexNum)->data());
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

		calCenterAndRadius();
		setupNormals();
		//	computeEdgeNormal(); for future ...
	}

	//  [1/5/2009 zhangxiang]
	Real sgCube::edgeLength(void) const{
		return m_fEdgeLength;
	}

	// sgCube //////////////////////////////////////////////////////////////////////////


	// sgSphere //////////////////////////////////////////////////////////////////////////

	//  [8/18/2008 zhangxiang]
	sgSphere::sgSphere(Real aRadius, int aSlices, int aStacks)
	: sgEntity(4, aSlices * aStacks + 2, aSlices * (aStacks + 1)),
	m_fRadius(aRadius), m_iSlices(aSlices), m_iStacks(aStacks){
		StdString newname = this->name();
		newname.insert(0, "Sphere");
		sgSceneObject::_setName(newname);
		m_RenderOption.setRenderElementType(sgRenderOption::RET_QUADS);
		// setup geometry
		init();
	}

	//  [8/18/2008 zhangxiang]
	sgSphere::sgSphere(const StdString &asName, Real aRadius, int aSlices, int aStacks)
	: sgEntity(4, aSlices * aStacks + 2, aSlices * (aStacks + 1)),
	m_fRadius(aRadius), m_iSlices(aSlices), m_iStacks(aStacks){
		sgSceneObject::_setName(asName);
		m_RenderOption.setRenderElementType(sgRenderOption::RET_QUADS);
		// setup geomety
		init();
	}

	//  [1/5/2009 zhangxiang]
	void sgSphere::init(void){
		if(m_fRadius <= 0 || m_iSlices < 2 || m_iStacks <= 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Invalid parameters for the sphere.",
							"sgSphere::init");
		}

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::ET_VERTEX, 3, m_iVertexNum)->data());
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
		m_bGeometryPrepared = true;

		// will calculate normals 
		trianglate();
	}

	//  [8/18/2008 zhangxiang]
	sgSphere::~sgSphere(void){

	}

	//  [8/28/2008 zhangxiang]
	Real sgSphere::radius(void) const{
		return m_fRadius;
	}

	//  [8/28/2008 zhangxiang]
	int sgSphere::slices(void) const{
		return m_iSlices;
	}

	//  [8/28/2008 zhangxiang]
	int sgSphere::stacks(void) const{
		return m_iStacks;
	}


	// sgCone //////////////////////////////////////////////////////////////////////////

	//  [8/18/2008 zhangxiang]
	sgCone::sgCone(Real aRadius, Real aHeight, int aSlices)
	: sgEntity(3, aSlices + 2, aSlices + aSlices),
	m_fRadius(aRadius), m_fHeight(aHeight), m_iSlices(aSlices){
		StdString newname = this->name();
		newname.insert(0, "Cone");
		sgSceneObject::_setName(newname);
		m_RenderOption.setRenderElementType(sgRenderOption::RET_TRIANGLES);
		init();
	}

	//  [8/18/2008 zhangxiang]
	sgCone::sgCone(const StdString &asName, Real aRadius, Real aHeight, int aSlices)
	: sgEntity(3, aSlices + 2, aSlices + aSlices),
	m_fRadius(aRadius), m_fHeight(aHeight), m_iSlices(aSlices){
		sgSceneObject::_setName(asName);
		m_RenderOption.setRenderElementType(sgRenderOption::RET_TRIANGLES);
		init();
	}

	//  [8/18/2008 zhangxiang]
	sgCone::~sgCone(void){

	}

	//  [1/5/2009 zhangxiang]
	void sgCone::init(void){
		if(m_fRadius <= 0 || m_fHeight <= 0 || m_iSlices < 2){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Invalid parameters for the cone",
							"sgCone::init");
		}

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(sgVertexBufferElement::ET_VERTEX, 3, m_iVertexNum)->data());
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

		calCenterAndRadius();
		setupNormals();
	//	setupEdgeNormal(); // for future ...
	}

	// sgCone //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta