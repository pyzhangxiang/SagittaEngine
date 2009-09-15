//////////////////////////////////////////////////////
// file: SGrid.cpp @ 2008-8-2 by Zhang Xiang
// defines of the class SGrid
// SGrid is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SGrid.h"
#include "../buffer/SVertexData.h"
#include "../buffer/SVertexIndexBuffer.h"
#include "../buffer/SVertexBufferElement.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/2/2008 zhangxiang]
	SGrid::SGrid(uInt aiLengthPerUnit, uInt aiHUnitNum, uInt aiVUnitNum, const Color &arColor/* = Color::WHITE*/)
	: SEntity(2, 2 * (aiHUnitNum + aiVUnitNum), aiHUnitNum + aiVUnitNum + 2, true){
		StdString newname = this->name();
		newname.insert(0, "Grid");
		SSceneObject::_setName(newname);
		
		m_RenderOption.setRenderElementType(SRenderOption::RET_LINES);

		int hTotalLength = aiLengthPerUnit * aiHUnitNum;
		int vTotalLength = aiLengthPerUnit * aiVUnitNum;
		int hHalfTotalLength = hTotalLength * 0.5;
		int vHalfTotalLength = vTotalLength * 0.5;

		Vector3 *pPosData = static_cast<Vector3*>(m_pVertexData->createElement(SVertexBufferElement::ET_VERTEX, 3, m_iVertexNum)->data());
		Color *pColorData = static_cast<Color*>(m_pVertexData->createElement(SVertexBufferElement::ET_COLOR, 4, m_iVertexNum)->data());
		size_t *pIndex = static_cast<size_t*>(m_pIndexData->createElement(SVertexBufferElement::ET_VERTEX)->data());

		size_t vi = 0;
		size_t i = 0;
		size_t ii = 0;
	//	for(int x=-hHalfTotalLength; x<=hHalfTotalLength; x+=aiLengthPerUnit){
		int x = -hHalfTotalLength;
		for(uInt vl=0; vl<=aiVUnitNum; ++vl){
			pPosData[vi] = Vector3(x, 0, -vHalfTotalLength);
			pColorData[vi] = arColor;
			pIndex[ii] = i;

			++vi; ++i; ++ii;
			
			pPosData[vi] = Vector3(x, 0, vHalfTotalLength);
			pColorData[vi] = arColor;
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
			pColorData[vi] = arColor;
			pIndex[ii] = i;

			++vi; ++i; ++ii;

			pPosData[vi] = Vector3(hHalfTotalLength, 0, z);
			pColorData[vi] = arColor;
			pIndex[ii] = i;

			++vi; ++i; ++ii;

			z += aiLengthPerUnit;
		}

		m_bNormalOuter = false;

		calCenterAndRadius();
	}

	//  [8/2/2008 zhangxiang]
	SGrid::~SGrid(void){

	}

} // namespace Sagitta
