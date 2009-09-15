//////////////////////////////////////////////////////
// file: SRenderQueue.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class SRenderQueue
// SRenderQueue is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SRenderQueue.h"
#include "SRenderable.h"
#include "../../common/utils/SException.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SRenderQueue::SRenderQueue(){

	}

	SRenderQueue::~SRenderQueue(){

	}

	//  [8/21/2008 zhangxiang]
	void SRenderQueue::clear(void){
		m_RenderableList.clear();
	}

	void SRenderQueue::addRenderable(SRenderable *aObject){
		if(!aObject){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null renderable object pointer.",
							"SRenderQueue::addRenderable");
		}

		/*RenderableList::iterator it = m_RenderableList.find(aObject);
		if(it != m_RenderableList.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"This renderable object has been in the render queue.",
							"SRenderQueue::addRenderable");
		}*/

		m_RenderableList.push_back(aObject);
	}

	//  [8/21/2008 zhangxiang]
	SRenderQueue::Iterator SRenderQueue::getIterator(void){
		return Iterator(m_RenderableList);
	}

	//  [8/21/2008 zhangxiang]
	SRenderQueue::ConstIterator SRenderQueue::getConstIterator(void) const{
		return ConstIterator(m_RenderableList);
	}

} // namespace Sagitta