//////////////////////////////////////////////////////
// file: SViewport.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class SViewport
// SViewport is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SViewport.h"
#include "../scenegraph/SCamera.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SViewport::SViewport(int aRTWidth, int aRTHeight,
						Real aLeft, Real aTop, 
						Real aWidth, Real aHeight,
						int aZOrder, SCamera *aCamera /* = 0 */) :
	m_iRTWidth(aRTWidth), m_iRTHeight(aRTHeight),
	m_fLeft(aLeft), m_fTop(aTop),
	m_fWidth(aWidth), m_fHeight(aHeight),
	m_iZOrder(aZOrder), m_pCamera(aCamera),
	m_BackColor(Color::BLACK), m_fBackDepth(1.0),
	m_iBackStencil(0), m_iClearBuffers(CB_COLOR | CB_DEPTH){
		if(m_pCamera){
			m_pCamera->_setViewport(this);
		}

		_updateActDimensions();
	}

	SViewport::~SViewport(){
		if(m_pCamera){
			m_pCamera->_setViewport(0);
		}
	}

	//  [8/6/2008 zhangxiang]
	int SViewport::actleft(void) const{
		return m_iLeft;
	}

	//  [1/3/2009 zhangxiang]
	Real SViewport::left(void) const{
		return m_fLeft;
	}

	//  [8/6/2008 zhangxiang]
	void SViewport::setLeft(Real aLeft){
		m_fLeft = aLeft;
		_updateActDimensions();
	}

	//  [8/6/2008 zhangxiang]
	int SViewport::acttop(void) const{
		return m_iTop;
	}

	//  [1/3/2009 zhangxiang]
	Real SViewport::top(void) const{
		return m_fTop;
	}

	//  [8/6/2008 zhangxiang]
	void SViewport::setTop(Real aTop){
		m_fTop = aTop;
		_updateActDimensions();
	}

	//  [8/6/2008 zhangxiang]
	int SViewport::actwidth(void) const{
		return m_iWidth;
	}

	//  [1/3/2009 zhangxiang]
	Real SViewport::width(void) const{
		return m_fWidth;
	}

	//  [8/6/2008 zhangxiang]
	void SViewport::setWidth(Real aWidth){
		m_fWidth = aWidth;
		_updateActDimensions();
	}

	//  [8/6/2008 zhangxiang]
	int SViewport::actheight(void) const{
		return m_iHeight;
	}

	//  [1/3/2009 zhangxiang]
	Real SViewport::height(void) const{
		return m_fHeight;
	}

	//  [8/6/2008 zhangxiang]
	void SViewport::setHeight(Real aHeight){
		m_fHeight = aHeight;
		_updateActDimensions();
	}

	//  [1/3/2009 zhangxiang]
	void SViewport::setDimensions(Real aLeft, Real aTop, Real aWidth, Real aHeight){
		m_fLeft = aLeft;
		m_fTop = aTop;
		m_fWidth = aWidth;
		m_fHeight = aHeight;

		_updateActDimensions();
	}

	//  [1/3/2009 zhangxiang]
	void SViewport::_updateActDimensions(void){
		m_iLeft = static_cast<int>(m_fLeft * m_iRTWidth);
		m_iTop = static_cast<int>(m_fTop * m_iRTHeight);
		m_iWidth = static_cast<int>(m_fWidth * m_iRTWidth);
		m_iHeight = static_cast<int>(m_fHeight * m_iRTHeight);

		if(m_pCamera){
			m_pCamera->_setAspect((Real)m_iWidth / (Real)m_iHeight);
		}
	}

	//  [1/3/2009 zhangxiang]
	void SViewport::_targetResized(int aWidth, int aHeight){
		m_iRTWidth = aWidth;
		m_iRTHeight = aHeight;
		_updateActDimensions();
	}

	//  [1/3/2009 zhangxiang]
	int SViewport::getZOrder(void) const{
		return m_iZOrder;
	}

	//  [1/3/2009 zhangxiang]
	void SViewport::_setZOrder(int aZOrder){
		m_iZOrder = aZOrder;
	}

	//  [1/3/2009 zhangxiang]
	SCamera *SViewport::camera(void) const{
		return m_pCamera;
	}

	//  [1/3/2009 zhangxiang]
	void SViewport::setCamera(SCamera *aCamera){
		m_pCamera = aCamera;
		if(m_pCamera){
			m_pCamera->_setViewport(this);
		}
	}

	//  [1/3/2009 zhangxiang]
	const Color &SViewport::getBackColor(void) const{
		return m_BackColor;
	}

	//  [1/3/2009 zhangxiang]
	void SViewport::setBackColor(const Color &aColor){
		m_BackColor = aColor;
	}

	//  [1/15/2009 zhangxiang]
	Real SViewport::getBackDepth(void) const{
		return m_fBackDepth;
	}

	//  [1/15/2009 zhangxiang]
	void SViewport::setBackDepth(Real aValue){
		m_fBackDepth = aValue;
	}

	//  [1/15/2009 zhangxiang]
	int SViewport::getBackStencil(void) const{
		return m_iBackStencil;
	}

	//  [1/15/2009 zhangxiang]
	void SViewport::setBackStencil(int aValue){
		m_iBackStencil = aValue;
	}

	//  [1/15/2009 zhangxiang]
	uInt SViewport::getClearBuffers(void) const{
		return m_iClearBuffers;
	}

	//  [1/15/2009 zhangxiang]
	void SViewport::setClearBuffers(uInt aBuffers){
		m_iClearBuffers;
	}

	//  [1/3/2009 zhangxiang]
	bool SViewport::isActive(void) const{
		return m_pCamera && m_pCamera->isActive();
	}

} // namespace Sagitta