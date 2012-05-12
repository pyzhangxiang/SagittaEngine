//////////////////////////////////////////////////////
// file: sgRenderQueue.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgRenderQueue
// sgRenderQueue is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgRenderQueue.h"
#include "sgRenderable.h"
#include "../../common/utils/sgException.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	sgRenderQueue::sgRenderQueue(){

	}

	sgRenderQueue::~sgRenderQueue(){

	}

	//  [8/21/2008 zhangxiang]
	void sgRenderQueue::clear(void){
		m_RenderableList.clear();
	}

	void sgRenderQueue::addRenderable(sgRenderable *aObject){
		if(!aObject){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null renderable object pointer.",
							"sgRenderQueue::addRenderable");
		}

		/*RenderableList::iterator it = m_RenderableList.find(aObject);
		if(it != m_RenderableList.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"This renderable object has been in the render queue.",
							"sgRenderQueue::addRenderable");
		}*/

		m_RenderableList.push_back(aObject);
	}

	//  [8/21/2008 zhangxiang]
	sgRenderQueue::Iterator sgRenderQueue::getIterator(void){
		return Iterator(m_RenderableList);
	}

	//  [8/21/2008 zhangxiang]
	sgRenderQueue::ConstIterator sgRenderQueue::getConstIterator(void) const{
		return ConstIterator(m_RenderableList);
	}

} // namespace Sagitta