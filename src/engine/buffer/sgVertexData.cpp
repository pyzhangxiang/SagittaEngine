//////////////////////////////////////////////////////
// file: sgVertexData.cpp @ 2008-7-31 by Zhang Xiang
// defines of the class sgVertexData
// sgVertexData is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgVertexData.h"
#include "sgVertexBufferElement.h"
#include "sgVertexIndexBuffer.h"
#include "engine/common/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// sgVertexData //////////////////////////////////////////////////////////////////////////

	//  [10/8/2008 zhangxiang]
	sgVertexData::sgVertexData(size_t aiVertexNum /* = 0 */)
	: m_iVertexNum(aiVertexNum), m_iSourceCount(0){

	}

	//  [10/8/2008 zhangxiang]
	sgVertexData::~sgVertexData(void){
		clear();
	}

	//  [10/8/2008 zhangxiang]
	size_t sgVertexData::vertexNum(void) const{
		return m_iVertexNum;
	}

	//  [10/8/2008 zhangxiang]
	void sgVertexData::setVertexNum(size_t aVertexNum){
		if(!m_ElementList.empty()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
				"Element list is not empty, cannot reset vertex num",
				"sgIndexData::setVertexNum");
		}
		
		m_iVertexNum = aVertexNum;
	}

	//  [10/8/2008 zhangxiang]
	size_t sgVertexData::elementNum(void) const{
		return m_ElementList.size();
	}

	//  [10/8/2008 zhangxiang]
	void sgVertexData::clear(void){
		ElementList::iterator it = m_ElementList.begin();
		ElementList::iterator eit = m_ElementList.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ElementList.clear();

		m_iSourceCount = 0;
	}

	//  [10/8/2008 zhangxiang]
	void sgVertexData::addElement(sgVertexBufferElement *aElement){
		if(!aElement){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null element pointer.", "sgVertexData::addElement");
		}

		// for vertex position element, check 
		if(aElement->type() == sgVertexBufferElement::ET_VERTEX && aElement->vertexNum() != m_iVertexNum){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Different vertex num.", "sgVertexData::addElement");
		}

		ElementList::iterator it = m_ElementList.find(aElement->type());
		if(it != m_ElementList.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_DUPLICATE_ITEM,
							"Such type element is existed.",
							"sgVertexData::addElement");
		}

		m_ElementList.insert(std::make_pair(aElement->type(), aElement));
	}

	//  [1/3/2009 zhangxiang]
	sgVertexBufferElement *sgVertexData::createElement(int aType, uShort aCoordNum, size_t aVertexNum){
		sgVertexBufferElement *newelm = new sgVertexBufferElement(aType, aCoordNum, m_iVertexNum, m_iSourceCount++, 0);
		addElement(newelm);
		return newelm;
	}

	//  [1/3/2009 zhangxiang]
	sgVertexBufferElement *sgVertexData::getElement(int aType) const{
		ElementList::const_iterator it = m_ElementList.find(aType);
		if(it == m_ElementList.end()){
			return 0;
		}
		return it->second;
	}

	//  [10/8/2008 zhangxiang]
	sgVertexData::Iterator sgVertexData::getIterator(void){
		return Iterator(m_ElementList);
	}

	//  [10/8/2008 zhangxiang]
	sgVertexData::ConstIterator sgVertexData::getConstIterator(void) const{
		return ConstIterator(m_ElementList);
	}

	// sgVertexData //////////////////////////////////////////////////////////////////////////


	// sgIndexData //////////////////////////////////////////////////////////////////////////

	//  [1/4/2009 zhangxiang]
	sgIndexData::sgIndexData(uInt aPolyType, size_t aPolyNum)
	: m_iPolyType(aPolyType), m_iPolyNum(aPolyNum){

	}

	//  [10/8/2008 zhangxiang]
	sgIndexData::~sgIndexData(void){
		clear();
	}

	//  [10/8/2008 zhangxiang]
	size_t sgIndexData::polyNum(void) const{
		return m_iPolyNum;
	}

	//  [10/8/2008 zhangxiang]
	void sgIndexData::setPolyNum(size_t aPolyNum){
		if(!m_ElementList.empty()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Element list is not empty, cannot reset polygon num",
							"sgIndexData::setPolyNum");
		}
		
		m_iPolyNum = aPolyNum;
	}

	//  [1/4/2009 zhangxiang]
	uInt sgIndexData::polyType(void) const{
		return m_iPolyType;
	}

	//  [1/4/2009 zhangxiang]
	void sgIndexData::setPolyType(uInt aPolyType){
		if(!m_ElementList.empty()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
				"Element list is not empty, cannot reset polygon type",
				"sgIndexData::setPolyType");
		}

		m_iPolyType = aPolyType;
	}

	//  [10/8/2008 zhangxiang]
	size_t sgIndexData::elementNum(void) const{
		return m_ElementList.size();
	}

	//  [10/8/2008 zhangxiang]
	void sgIndexData::clear(void){
		ElementList::iterator it = m_ElementList.begin();
		ElementList::iterator eit = m_ElementList.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ElementList.clear();
	}

	//  [10/8/2008 zhangxiang]
	void sgIndexData::addElement(sgVertexIndexBuffer *aElement){
		if(!aElement){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null element pointer.", "sgIndexData::addElement");
		}

		if(aElement->polyType() != m_iPolyType){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Different polygon type.", "sgIndexData::addElement");
		}

		if(aElement->polyNum() != m_iPolyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Different polygon num.", "sgIndexData::addElement");
		}

		ElementList::iterator it = m_ElementList.find(aElement->type());
		if(it != m_ElementList.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_DUPLICATE_ITEM,
				"Such type element is existed.",
				"sgIndexData::addElement");
		}

		m_ElementList.insert(std::make_pair(aElement->type(), aElement));
	}

	//  [1/3/2009 zhangxiang]
	sgVertexIndexBuffer *sgIndexData::createElement(int aType){
		sgVertexIndexBuffer *newelm = new sgVertexIndexBuffer(aType, m_iPolyType, m_iPolyNum);
		addElement(newelm);
		return newelm;
	}

	//  [1/3/2009 zhangxiang]
	sgVertexIndexBuffer *sgIndexData::getElement(int aType) const{
		ElementList::const_iterator it = m_ElementList.find(aType);
		if(it == m_ElementList.end()){
			return 0;
		}
		return it->second;
	}

	//  [10/8/2008 zhangxiang]
	sgIndexData::Iterator sgIndexData::getIterator(void){
		return Iterator(m_ElementList);
	}

	//  [10/8/2008 zhangxiang]
	sgIndexData::ConstIterator sgIndexData::getConstIterator(void) const{
		return ConstIterator(m_ElementList);
	}

	// sgIndexData //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta