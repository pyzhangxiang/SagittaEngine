//////////////////////////////////////////////////////
// file: SVertexData.cpp @ 2008-7-31 by Zhang Xiang
// defines of the class SVertexData
// SVertexData is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SVertexData.h"
#include "SVertexBufferElement.h"
#include "SVertexIndexBuffer.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// SVertexData //////////////////////////////////////////////////////////////////////////

	//  [10/8/2008 zhangxiang]
	SVertexData::SVertexData(size_t aiVertexNum /* = 0 */)
	: m_iVertexNum(aiVertexNum), m_iSourceCount(0){

	}

	//  [10/8/2008 zhangxiang]
	SVertexData::~SVertexData(void){
		clear();
	}

	//  [10/8/2008 zhangxiang]
	size_t SVertexData::vertexNum(void) const{
		return m_iVertexNum;
	}

	//  [10/8/2008 zhangxiang]
	void SVertexData::setVertexNum(size_t aVertexNum){
		if(!m_ElementList.empty()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
				"Element list is not empty, cannot reset vertex num",
				"SIndexData::setVertexNum");
		}
		
		m_iVertexNum = aVertexNum;
	}

	//  [10/8/2008 zhangxiang]
	size_t SVertexData::elementNum(void) const{
		return m_ElementList.size();
	}

	//  [10/8/2008 zhangxiang]
	void SVertexData::clear(void){
		ElementList::iterator it = m_ElementList.begin();
		ElementList::iterator eit = m_ElementList.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ElementList.clear();

		m_iSourceCount = 0;
	}

	//  [10/8/2008 zhangxiang]
	void SVertexData::addElement(SVertexBufferElement *aElement){
		if(!aElement){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null element pointer.", "SVertexData::addElement");
		}

		// for vertex position element, check 
		if(aElement->type() == SVertexBufferElement::ET_VERTEX && aElement->vertexNum() != m_iVertexNum){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Different vertex num.", "SVertexData::addElement");
		}

		ElementList::iterator it = m_ElementList.find(aElement->type());
		if(it != m_ElementList.end()){
			THROW_SAGI_EXCEPT(SException::ERR_DUPLICATE_ITEM,
							"Such type element is existed.",
							"SVertexData::addElement");
		}

		m_ElementList.insert(std::make_pair(aElement->type(), aElement));
	}

	//  [1/3/2009 zhangxiang]
	SVertexBufferElement *SVertexData::createElement(int aType, uShort aCoordNum, size_t aVertexNum){
		SVertexBufferElement *newelm = new SVertexBufferElement(aType, aCoordNum, m_iVertexNum, m_iSourceCount++, 0);
		addElement(newelm);
		return newelm;
	}

	//  [1/3/2009 zhangxiang]
	SVertexBufferElement *SVertexData::getElement(int aType) const{
		ElementList::const_iterator it = m_ElementList.find(aType);
		if(it == m_ElementList.end()){
			return 0;
		}
		return it->second;
	}

	//  [10/8/2008 zhangxiang]
	SVertexData::Iterator SVertexData::getIterator(void){
		return Iterator(m_ElementList);
	}

	//  [10/8/2008 zhangxiang]
	SVertexData::ConstIterator SVertexData::getConstIterator(void) const{
		return ConstIterator(m_ElementList);
	}

	// SVertexData //////////////////////////////////////////////////////////////////////////


	// SIndexData //////////////////////////////////////////////////////////////////////////

	//  [1/4/2009 zhangxiang]
	SIndexData::SIndexData(uInt aPolyType, size_t aPolyNum)
	: m_iPolyType(aPolyType), m_iPolyNum(aPolyNum){

	}

	//  [10/8/2008 zhangxiang]
	SIndexData::~SIndexData(void){
		clear();
	}

	//  [10/8/2008 zhangxiang]
	size_t SIndexData::polyNum(void) const{
		return m_iPolyNum;
	}

	//  [10/8/2008 zhangxiang]
	void SIndexData::setPolyNum(size_t aPolyNum){
		if(!m_ElementList.empty()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
							"Element list is not empty, cannot reset polygon num",
							"SIndexData::setPolyNum");
		}
		
		m_iPolyNum = aPolyNum;
	}

	//  [1/4/2009 zhangxiang]
	uInt SIndexData::polyType(void) const{
		return m_iPolyType;
	}

	//  [1/4/2009 zhangxiang]
	void SIndexData::setPolyType(uInt aPolyType){
		if(!m_ElementList.empty()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
				"Element list is not empty, cannot reset polygon type",
				"SIndexData::setPolyType");
		}

		m_iPolyType = aPolyType;
	}

	//  [10/8/2008 zhangxiang]
	size_t SIndexData::elementNum(void) const{
		return m_ElementList.size();
	}

	//  [10/8/2008 zhangxiang]
	void SIndexData::clear(void){
		ElementList::iterator it = m_ElementList.begin();
		ElementList::iterator eit = m_ElementList.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ElementList.clear();
	}

	//  [10/8/2008 zhangxiang]
	void SIndexData::addElement(SVertexIndexBuffer *aElement){
		if(!aElement){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null element pointer.", "SIndexData::addElement");
		}

		if(aElement->polyType() != m_iPolyType){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Different polygon type.", "SIndexData::addElement");
		}

		if(aElement->polyNum() != m_iPolyNum){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Different polygon num.", "SIndexData::addElement");
		}

		ElementList::iterator it = m_ElementList.find(aElement->type());
		if(it != m_ElementList.end()){
			THROW_SAGI_EXCEPT(SException::ERR_DUPLICATE_ITEM,
				"Such type element is existed.",
				"SIndexData::addElement");
		}

		m_ElementList.insert(std::make_pair(aElement->type(), aElement));
	}

	//  [1/3/2009 zhangxiang]
	SVertexIndexBuffer *SIndexData::createElement(int aType){
		SVertexIndexBuffer *newelm = new SVertexIndexBuffer(aType, m_iPolyType, m_iPolyNum);
		addElement(newelm);
		return newelm;
	}

	//  [1/3/2009 zhangxiang]
	SVertexIndexBuffer *SIndexData::getElement(int aType) const{
		ElementList::const_iterator it = m_ElementList.find(aType);
		if(it == m_ElementList.end()){
			return 0;
		}
		return it->second;
	}

	//  [10/8/2008 zhangxiang]
	SIndexData::Iterator SIndexData::getIterator(void){
		return Iterator(m_ElementList);
	}

	//  [10/8/2008 zhangxiang]
	SIndexData::ConstIterator SIndexData::getConstIterator(void) const{
		return ConstIterator(m_ElementList);
	}

	// SIndexData //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta