//////////////////////////////////////////////////////
// file: sgObject.h @ 2007-11-17 by Zhang Xiang
// declares of the class sgObject
// sgObject is a class ...
//////////////////////////////////////////////////////
#ifndef __SGOBJECT_H__
#define __SGOBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "math/SagittaPlatform.h"
#include "math/sgMath.h"
#include "engine/serialization/sg_serialization_nvp.h"
#include "engine/serialization/sgInXmlArchive.h"
#include "engine/serialization/sgOutXmlArchive.h"
#include "sgMemObject.h"
#include "sgStrHandle.h"
#include "sgClassMeta.h"
#include "sgStlAllocator.h"

// DECLARES //////////////////////////////////////////
namespace Sagitta{
    
	class _SG_KernelExport sgObject : public sgMemObject{
        SG_META_DECLARE_ABSTRACT(sgObject)
	// static members
		static id_type ms_oid;

	// member variables
	private:
		/// my id
		const id_type m_iOId;
        
        SERIALIZE_BEGIN(sgObject)
        std::string mName;
        SERIALIZE_END
    public:
        sgClassMeta *mMeta;
	
	// constructors & destructor
	private:
		// disable compiler version copying
		sgObject(const sgObject &);
		sgObject &operator=(const sgObject &);

	public:
		sgObject(void);
		virtual ~sgObject(void);
        
        bool ISA(const sgStrHandle &classtype) const;

	// member functions

	public:

		/** Gets this object id. */
		id_type getId(void) const;
        
        sgClassMeta *getMeta(void) const;
        
        /** Gets my name. */
		const std::string &getName(void) const;
		/** Sets my name. */
		void setName(const std::string &aName);
        
        virtual void load(SERIALIZE_LOAD_ARCHIVE &archive);
        virtual void save(SERIALIZE_SAVE_ARCHIVE &archive);
        
        static sgObject *createObject(const sgStrHandle &classname);
        static void destroyObject(sgObject *obj);
        static sgObject *getObject(id_type oid);
        
        virtual void setShowDebug(bool show){}

	};
    
    template<class Archive>
    Archive &operator & (Archive &archive, sgObject &obj)
    {
        archive & SG_MAKE_NVP("mName", obj.mName);
        return archive;
    }

} // namespace Sagitta

#endif // __SGOBJECT_H__

