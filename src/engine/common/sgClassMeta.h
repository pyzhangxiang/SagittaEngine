#ifndef __SGCLASSMETA_H__
#define __SGCLASSMETA_H__

#include "sgStrHandle.h"
#include "math/sgTypeDef.h"
#include "Singleton.h"
#include <iostream>
#include <map>

namespace Sagitta
{
    
#ifndef SG_META_DECLARE    
#define SG_META_DECLARE(classname) \
    public: \
        static sgObject *Constructor(void); \
		static sgClassMeta *GetMetaInfo(void); \
		static sgStrHandle GetClassName(void); \
		static sgStrHandle GetSuperName(void); \
		virtual sgClassMeta *GetMyMetaInfo(void) const; \
        virtual sgStrHandle GetMyClassName(void) const; \
    private: 
    
#endif
    
#ifndef SG_META_DEFINE
#define SG_META_DEFINE(classname, supername) \
    sgClassMeta classname##Meta(#classname, #supername, classname::Constructor);\
\
    sgObject *classname::Constructor(void) \
    { \
        sgObject *obj = new classname(); \
        obj->mMeta = &(classname##Meta); \
        return obj; \
    } \
	sgClassMeta *classname::GetMetaInfo(void){ return &(classname##Meta); } \
	sgStrHandle classname::GetClassName(void) { return classname##Meta.getName(); } \
	sgStrHandle classname::GetSuperName(void) { return classname##Meta.getSuperName(); } \
	sgClassMeta *classname::GetMyMetaInfo(void) const{ return classname::GetMetaInfo(); } \
    sgStrHandle classname::GetMyClassName(void) const{ return classname::GetClassName(); }
    
#endif
    
    
#ifndef SG_META_DECLARE_ABSTRACT
#define SG_META_DECLARE_ABSTRACT(classname) \
    public: \
        static sgClassMeta *GetMetaInfo(void); \
        static sgStrHandle GetClassName(void); \
        static sgStrHandle GetSuperName(void); \
        virtual sgClassMeta *GetMyMetaInfo(void) const; \
        virtual sgStrHandle GetMyClassName(void) const; \
    private:
    
#endif
    
#ifndef SG_META_DEFINE_ABSTRACT
#define SG_META_DEFINE_ABSTRACT(classname, supername) \
    sgClassMeta classname##Meta(#classname, #supername, 0);\
    \
    sgClassMeta *classname::GetMetaInfo(void){ return &(classname##Meta); } \
    sgStrHandle classname::GetClassName(void) { return classname##Meta.getName(); } \
    sgStrHandle classname::GetSuperName(void) { return classname##Meta.getSuperName(); } \
    sgClassMeta *classname::GetMyMetaInfo(void) const{ return classname::GetMetaInfo(); } \
    sgStrHandle classname::GetMyClassName(void) const{ return classname::GetClassName(); }
    
#endif
    
	class sgObject;
	
	class _SG_KernelExport sgClassMeta
	{
	public:
		typedef sgObject *(*Constructor)(void);

		sgClassMeta(const sgStrHandle &name, const sgStrHandle &superName, Constructor creator);
		~sgClassMeta(void);
        
    private:
		sgStrHandle mClassName;
		sgStrHandle mSuperName;
		Constructor	mCreator;
        
    public:
        sgStrHandle getName(void) const
        {
            return mClassName;
        }
        
        sgStrHandle getSuperName(void) const
        {
            return mSuperName;
        }
        
        sgClassMeta *getSuperMeta(void) const;
        
        sgObject *createObject(void);
        
        bool isClass(const sgStrHandle &classtype) const;
        
	};
    
    class _SG_KernelExport sgMetaCenter : public Singleton<sgMetaCenter>
    {
		friend class sgClassMeta;
    private:
        typedef std::map<sgStrHandle, sgClassMeta*> MetaMap;
        MetaMap mMetaMap;
        
		void addMeta(sgClassMeta *meta);
    public:
        sgMetaCenter(void){}
        ~sgMetaCenter(void){}
        
        sgClassMeta *findMeta(const sgStrHandle &name) const;
    };

}


#endif	// __SGCLASSMETA_H__

