//////////////////////////////////////////////////////
// file: sgRenderable.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgRenderable
// sgRenderable is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgRenderable.h"
#include "sgSceneNode.h"
#include "../buffer/sgVertexData.h"
#include "../buffer/sgVertexIndexBuffer.h"
#include "../buffer/sgVertexBufferElement.h"
#include "../../common/utils/sgStringUtil.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong sgRenderable::ms_iRenderableCount = 1;

	sgRenderable::sgRenderable(void) :
	sgEsse(SET_RENDERABLE), m_bRenderSelf(false),
	m_bVisible(true), m_fAverageRadius(0.0),
	m_fMaxRadius(0.0), m_Center(Vector3::ZERO),
	m_pVertexData(0), m_pIndexData(0), m_pFaceNormalBuffer(0){
		StdString name = "RenderableObject" + sgStringUtil::to_string(sgRenderable::ms_iRenderableCount++);
		sgSceneObject::_setName(name);
	}

	//  [1/1/2009 zhangxiang]
	sgRenderable::sgRenderable(const StdString &aName)
	: sgEsse(aName, SET_RENDERABLE), m_bRenderSelf(false),
	m_bVisible(true), m_fAverageRadius(0.0),
	m_fMaxRadius(0.0), m_Center(Vector3::ZERO),
	m_pVertexData(0), m_pIndexData(0), m_pFaceNormalBuffer(0){
		ms_iRenderableCount++;
	}

	sgRenderable::~sgRenderable(){
		sgRenderable::ms_iRenderableCount--;
		if(m_pVertexData){
			delete m_pVertexData;
		}
		if(m_pIndexData){
			delete m_pIndexData;
		}
		if(m_pFaceNormalBuffer){
			delete m_pFaceNormalBuffer;
		}
	}

	void sgRenderable::render(){
		/// overwrite code ...
	}

	//  [8/1/2008 zhangxiang]
	const sgRenderOption &sgRenderable::renderOption(void) const{
		return m_RenderOption;
	}

	//  [1/5/2009 zhangxiang]
	sgRenderOption &sgRenderable::renderOption(void){
		return m_RenderOption;
	}

	void sgRenderable::setRenderOption(const sgRenderOption &arROP){
		m_RenderOption = arROP;
	}

	//  [8/11/2008 zhangxiang]
	const sgMaterial &sgRenderable::material(void) const{
		return m_Material;
	}

	//  [8/11/2008 zhangxiang]
	void sgRenderable::setMaterial(const sgMaterial &aMaterial){
		m_Material = aMaterial;
	}

	//  [8/8/2008 zhangxiang]
	bool sgRenderable::isVisible(void) const{
		return m_bVisible && isActive();
	}

	//  [8/8/2008 zhangxiang]
	void sgRenderable::setVisible(bool abVisible){
		m_bVisible = abVisible;
	}

	//  [8/19/2008 zhangxiang]
	const Vector3 &sgRenderable::center(void) const{
		return m_Center;
	}

	//  [8/21/2008 zhangxiang]
	Real sgRenderable::avgRadius(void) const{
		return m_fAverageRadius;
	}

	//  [8/21/2008 zhangxiang]
	Real sgRenderable::maxRadius(void) const{
		return m_fMaxRadius;
	}

	//  [1/5/2009 zhangxiang]
	Matrix4 sgRenderable::getModelMatrix(void) const{
		if(isAttached())
			return attachedNode()->_getFullTransform();
		else
			return Matrix4::IDENTITY;
	}

} // namespace Sagitta