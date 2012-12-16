//  [10/11/2012  zhangxiang]

#include "sgRagdollConfig.h"
#include "sgResourceCenter.h"
#include "engine/common/sgStringUtil.h"
#include "../serialization/sgInXmlArchive.h"

namespace Sagitta{

	SG_META_DEFINE(sgRagdollConfig, sgResource)

	sgRagdollConfig::sgRagdollConfig()
    : sgResource()
    {
	}

	sgRagdollConfig::~sgRagdollConfig(void)
    {
        
	}

	void sgRagdollConfig::onSetFilename( void )
	{
		std::string filename = getFilename().getStr();
		std::string fullpath = sgResourceCenter::instance()->getResourcePath(filename);
		serialization::sgInXmlArchive arc(fullpath);

		BodyInfoSet bis;
		arc & (bis);

		mBodyInfoSet.clear();

		for(size_t i=0; i<bis.mBodyInfo.size(); ++i)
		{
			BodyInfo &bi = bis.mBodyInfo[i];

			mBodyInfoSet.insert(std::make_pair(bi.bodyName, bi));
		}
	}


} // namespace Sagitta
