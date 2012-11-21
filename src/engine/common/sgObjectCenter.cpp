

#include "sgObjectCenter.h"

namespace Sagitta{
    
	sgObjectCenter::sgObjectCenter(void) 
    {

	}

	sgObjectCenter::~sgObjectCenter(void)
    {
        ObjMap todel = mObjMap;
		int count = todel.size();
		while( count > 0)
		{
			ObjMap::iterator it = todel.begin();
			for(; it!=todel.end(); ++it)
			{
				delete it->second;
				--count;
				if(count != mObjMap.size())
				{
					todel = mObjMap;
					count = todel.size();
					break;
				}
			}
		}
		mObjMap.clear();        
	}

	void sgObjectCenter::addObject(sgObject *obj)
    {
        assert(obj);
        sgObject *exist = findObject(obj->getId());
        if(exist)
            return ;
        mObjMap.insert(std::make_pair(obj->getId(), obj));
    }
    
    sgObject *sgObjectCenter::findObject(id_type Id) const
    {
        ObjMap::const_iterator it = mObjMap.find(Id);
        if(it != mObjMap.end())
            return it->second;
        return 0;
    }

    void sgObjectCenter::removeObject(id_type Id)
    {
        ObjMap::iterator it = mObjMap.find(Id);
        if(it == mObjMap.end())
            return ;
        mObjMap.erase(it);
    }


} // namespace Sagitta
