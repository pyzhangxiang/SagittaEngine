
#include "sgRenderTechnique.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderTarget.h"
#include "sgRenderTargetWindow.h"
#include "sgRenderEffect.h"

namespace Sagitta
{

    SG_META_DEFINE_ABSTRACT(sgRenderTechnique, sgObject)
	
	sgRenderTechnique::sgRenderTechnique( void )
	: sgObject()
	{

	}

	sgRenderTechnique::~sgRenderTechnique( void )
	{
		for(size_t i=0; i<mPassList.size(); ++i)
		{
			delete mPassList[i];
		}
		mPassList.clear();
	}

	sgRenderPass *sgRenderTechnique::addPass(const sgStrHandle &queueType)
	{
		sgRenderPass *rp = new sgRenderPass(queueType);
		mPassList.push_back(rp);

		return rp;
	}

	sgRenderPass *sgRenderTechnique::getRenderPass(size_t index) const
	{
		if(index >= mPassList.size())
			return 0;

		return mPassList[index];
	}

	void sgRenderTechnique::render( void )
	{
		if(mPassList.empty())
			return ;

		for(size_t i=0; i<mPassList.size(); ++i)
		{
			this->setCurrentPass(i);
			this->preRenderPass();
			sgGetRenderer()->render(mPassList[i]);
			this->postRenderPass();
		}
	}

	void sgRenderTechnique::resize( UInt32 width, UInt32 height )
	{
		for(size_t i=0; i<mPassList.size(); ++i)
		{
			sgRenderTarget *rt = mPassList[i]->getRenderTarget();
			if(rt && dynamic_cast<sgRenderTargetWindow*>(rt))
			{
				rt->resize(width, height);
			}
		}
	}

	void sgRenderTechnique::update( Float32 deltaTime )
	{
		for(size_t i=0; i<mPassList.size(); ++i)
		{
			sgRenderEffect *re = mPassList[i]->getRenderEffect();
			if(re)
			{
				re->update(deltaTime);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SG_META_DEFINE(sgRenderTechniqueBase, sgRenderTechnique)

	sgRenderTechniqueBase::sgRenderTechniqueBase( void )
	: mRTWindow(0)
	{
		sgRenderPass *rp = addPass();
		mRTWindow = new sgRenderTargetWindow(1, 1);
		rp->setRenderTarget(mRTWindow);
		
	}

	sgRenderTechniqueBase::~sgRenderTechniqueBase( void )
	{
		delete mRTWindow;
	}

} // namespace Sagitta
