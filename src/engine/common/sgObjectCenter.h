
#ifndef __SGOBJECTCENTER_H__
#define __SGOBJECTCENTER_H__

#include "sgObject.h"
#include "Singleton.h"
#include "sgStlAllocator.h"
#include <map>

// DECLARES //////////////////////////////////////////
namespace Sagitta{
    
	class _SG_KernelExport sgObjectCenter : public sgManuallySingleton<sgObjectCenter>
    {
        // let object could remove objects from db
        friend class sgObject;
        
	private:
		typedef sg_map(id_type, sgObject*) ObjMap;
        ObjMap mObjMap;
        
	private:
		// disable compiler version copying
		sgObjectCenter(const sgObjectCenter &);
		sgObjectCenter &operator=(const sgObjectCenter &);

	public:
		sgObjectCenter(void);
		virtual ~sgObjectCenter(void);

    public:
        sgObject *findObject(id_type Id) const;
    private:
        void addObject(sgObject *obj);
        void removeObject(id_type Id);

	};

} // namespace Sagitta

#endif // __SGOBJECTCENTER_H__

