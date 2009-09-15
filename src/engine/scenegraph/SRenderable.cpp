//////////////////////////////////////////////////////
// file: SRenderable.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class SRenderable
// SRenderable is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SRenderable.h"
#include "SSceneNode.h"
#include "../buffer/SVertexData.h"
#include "../buffer/SVertexIndexBuffer.h"
#include "../buffer/SVertexBufferElement.h"
#include "../../common/utils/SString.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong SRenderable::ms_iRenderableCount = 1;

	SRenderable::SRenderable(void) :
	SEsse(SET_RENDERABLE), m_bRenderSelf(false),
	m_bVisible(true), m_fAverageRadius(0.0),
	m_fMaxRadius(0.0), m_Center(Vector3::ZERO),
	m_pVertexData(0), m_pIndexData(0), m_pFaceNormalBuffer(0){
		StdString name = "RenderableObject" + SString::to_string(SRenderable::ms_iRenderableCount++);
		SSceneObject::_setName(name);
	}

	//  [1/1/2009 zhangxiang]
	SRenderable::SRenderable(const StdString &aName)
	: SEsse(aName, SET_RENDERABLE), m_bRenderSelf(false),
	m_bVisible(true), m_fAverageRadius(0.0),
	m_fMaxRadius(0.0), m_Center(Vector3::ZERO),
	m_pVertexData(0), m_pIndexData(0), m_pFaceNormalBuffer(0){
		ms_iRenderableCount++;
	}

	SRenderable::~SRenderable(){
		SRenderable::ms_iRenderableCount--;
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

	void SRenderable::render(){
		/// overwrite code ...
	}

	//  [8/1/2008 zhangxiang]
	const SRenderOption &SRenderable::renderOption(void) const{
		return m_RenderOption;
	}

	//  [1/5/2009 zhangxiang]
	SRenderOption &SRenderable::renderOption(void){
		return m_RenderOption;
	}

	void SRenderable::setRenderOption(const SRenderOption &arROP){
		m_RenderOption = arROP;
	}

	//  [8/11/2008 zhangxiang]
	const SMaterial &SRenderable::material(void) const{
		return m_Material;
	}

	//  [8/11/2008 zhangxiang]
	void SRenderable::setMaterial(const SMaterial &aMaterial){
		m_Material = aMaterial;
	}

	//  [8/8/2008 zhangxiang]
	bool SRenderable::isVisible(void) const{
		return m_bVisible && isActive();
	}

	//  [8/8/2008 zhangxiang]
	void SRenderable::setVisible(bool abVisible){
		m_bVisible = abVisible;
	}

	//  [8/19/2008 zhangxiang]
	const Vector3 &SRenderable::center(void) const{
		return m_Center;
	}

	//  [8/21/2008 zhangxiang]
	Real SRenderable::avgRadius(void) const{
		return m_fAverageRadius;
	}

	//  [8/21/2008 zhangxiang]
	Real SRenderable::maxRadius(void) const{
		return m_fMaxRadius;
	}

	//  [1/5/2009 zhangxiang]
	Matrix4 SRenderable::getModelMatrix(void) const{
		if(isAttached())
			return attachedNode()->_getFullTransform();
		else
			return Matrix4::IDENTITY;
	}

} // namespace Sagitta