//////////////////////////////////////////////////////
// file: sgObject.h @ 2007-11-17 by Zhang Xiang
// declares of the class sgObject
// sgObject is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgObject.h"
#include "sgClassMeta.h"
#include "sgObjectCenter.h"
#include "strtk/strtk.hpp"

// DECLARATION ///////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
    
    SG_META_DEFINE_ABSTRACT(sgObject, 0)

	id_type sgObject::ms_oid = ID_NULL;

	sgObject::sgObject(void) 
    : m_iOId(++ms_oid)
    , mMeta(0){
        mName = "Object" + strtk::type_to_string(m_iOId);
        sgObjectCenter::instance()->addObject(this);
	}

	sgObject::~sgObject(void){
        sgObjectCenter::instance()->removeObject(this->getId());
	}

	//  [1/9/2009 zhangxiang]
	id_type sgObject::getId(void) const{
		return m_iOId;
	}

    sgClassMeta *sgObject::getMeta(void) const
    {
        return mMeta;
    }
    

    const std::string &sgObject::getName(void) const
    {
        return mName;
    }

    void sgObject::setName(const std::string &aName)
    {
        mName = aName;
    }
    
    sgObject *sgObject::createObject(const sgStrHandle &classname)
    {
        sgClassMeta *meta = sgMetaCenter::instance().findMeta(classname);
        if(!meta)
            return 0;
        sgObject *obj = meta->createObject();
        return obj;
    }
    
    void sgObject::destroyObject(sgObject *obj)
    {
        if(!obj)
            return ;
        delete obj;
    }
    
    sgObject *sgObject::getObject(id_type oid)
    {
        return sgObjectCenter::instance()->findObject(oid);
    }
    
    bool sgObject::ISA(const sgStrHandle &classtype) const
    {
        sgClassMeta *meta = this->GetMyMetaInfo();
        return meta->isClass(classtype);
    } 
    
    void sgObject::load(SERIALIZE_LOAD_ARCHIVE &archive)
    {
        archive & SG_MAKE_NVP(this->GetMyClassName().getStr(), *this);
    }
    
    void sgObject::save(SERIALIZE_SAVE_ARCHIVE &archive)
    {
        archive & SG_MAKE_NVP(this->GetMyClassName().getStr(), *this);
    }

} // namespace Sagitta
