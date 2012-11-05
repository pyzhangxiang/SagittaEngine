//////////////////////////////////////////////////////
// file: sgViewport.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgViewport
// sgViewport is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgViewport.h"
#include "engine/component/sgCameraComponent.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/scenegraph/sgScene.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	sgViewport::sgViewport(int aRTWidth, int aRTHeight,
						Real aLeft, Real aTop, 
						Real aWidth, Real aHeight,
						int aZOrder, sgCameraComponent *aCamera /* = 0 */) :
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

	sgViewport::~sgViewport(){
		if(m_pCamera){
			m_pCamera->_setViewport(0);
		}
	}

	//  [8/6/2008 zhangxiang]
	int sgViewport::actleft(void) const{
		return m_iLeft;
	}

	//  [1/3/2009 zhangxiang]
	Real sgViewport::left(void) const{
		return m_fLeft;
	}

	//  [8/6/2008 zhangxiang]
	void sgViewport::setLeft(Real aLeft){
		m_fLeft = aLeft;
		_updateActDimensions();
	}

	//  [8/6/2008 zhangxiang]
	int sgViewport::acttop(void) const{
		return m_iTop;
	}

	//  [1/3/2009 zhangxiang]
	Real sgViewport::top(void) const{
		return m_fTop;
	}

	//  [8/6/2008 zhangxiang]
	void sgViewport::setTop(Real aTop){
		m_fTop = aTop;
		_updateActDimensions();
	}

	//  [8/6/2008 zhangxiang]
	int sgViewport::actwidth(void) const{
		return m_iWidth;
	}

	//  [1/3/2009 zhangxiang]
	Real sgViewport::width(void) const{
		return m_fWidth;
	}

	//  [8/6/2008 zhangxiang]
	void sgViewport::setWidth(Real aWidth){
		m_fWidth = aWidth;
		_updateActDimensions();
	}

	//  [8/6/2008 zhangxiang]
	int sgViewport::actheight(void) const{
		return m_iHeight;
	}

	//  [1/3/2009 zhangxiang]
	Real sgViewport::height(void) const{
		return m_fHeight;
	}

	//  [8/6/2008 zhangxiang]
	void sgViewport::setHeight(Real aHeight){
		m_fHeight = aHeight;
		_updateActDimensions();
	}

	//  [1/3/2009 zhangxiang]
	void sgViewport::setDimensions(Real aLeft, Real aTop, Real aWidth, Real aHeight){
		m_fLeft = aLeft;
		m_fTop = aTop;
		m_fWidth = aWidth;
		m_fHeight = aHeight;

		_updateActDimensions();
	}

	//  [1/3/2009 zhangxiang]
	void sgViewport::_updateActDimensions(void){
		m_iLeft = static_cast<int>(m_fLeft * m_iRTWidth);
		m_iTop = static_cast<int>(m_fTop * m_iRTHeight);
		m_iWidth = static_cast<int>(m_fWidth * m_iRTWidth);
		m_iHeight = static_cast<int>(m_fHeight * m_iRTHeight);

		if(m_pCamera){
			m_pCamera->_setAspect((Real)m_iWidth / (Real)m_iHeight);
		}
	}

	//  [1/3/2009 zhangxiang]
	void sgViewport::_targetResized(int aWidth, int aHeight){
		m_iRTWidth = aWidth;
		m_iRTHeight = aHeight;
		_updateActDimensions();
	}

	//  [1/3/2009 zhangxiang]
	int sgViewport::getZOrder(void) const{
		return m_iZOrder;
	}

	//  [1/3/2009 zhangxiang]
	void sgViewport::_setZOrder(int aZOrder){
		m_iZOrder = aZOrder;
	}

	//  [1/3/2009 zhangxiang]
	sgCameraComponent *sgViewport::camera(void) const{
		return m_pCamera;
	}

	//  [1/3/2009 zhangxiang]
	void sgViewport::setCamera(sgCameraComponent *aCamera){
		m_pCamera = aCamera;
		if(m_pCamera){
			m_pCamera->_setViewport(this);
		}
	}

	//  [1/3/2009 zhangxiang]
	const Color &sgViewport::getBackColor(void) const{
		return m_BackColor;
	}

	//  [1/3/2009 zhangxiang]
	void sgViewport::setBackColor(const Color &aColor){
		m_BackColor = aColor;
	}

	//  [1/15/2009 zhangxiang]
	Real sgViewport::getBackDepth(void) const{
		return m_fBackDepth;
	}

	//  [1/15/2009 zhangxiang]
	void sgViewport::setBackDepth(Real aValue){
		m_fBackDepth = aValue;
	}

	//  [1/15/2009 zhangxiang]
	int sgViewport::getBackStencil(void) const{
		return m_iBackStencil;
	}

	//  [1/15/2009 zhangxiang]
	void sgViewport::setBackStencil(int aValue){
		m_iBackStencil = aValue;
	}

	//  [1/15/2009 zhangxiang]
	uInt sgViewport::getClearBuffers(void) const{
		return m_iClearBuffers;
	}

	//  [1/15/2009 zhangxiang]
	void sgViewport::setClearBuffers(uInt aBuffers){
		m_iClearBuffers;
	}

	//  [1/3/2009 zhangxiang]
	bool sgViewport::isActive(void) const{
		if(!m_pCamera)
			return false;
		if( ! (m_pCamera->getParent()->getScene()) )
			return false;

		return m_pCamera->getParent()->getScene()->getRoot()->isActive();
	}

} // namespace Sagitta