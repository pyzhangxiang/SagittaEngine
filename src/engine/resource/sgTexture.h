//  [10/11/2012  zhangxiang]

#ifndef __SGTEXTURE_H__
#define __SGTEXTURE_H__


#include "sgResource.h"


namespace Sagitta{
    
    class sgFrameBuffer;

	class _SG_KernelExport sgTexture : public sgResource
	{
		SG_META_DECLARE(sgTexture)
        
    private:
        sgFrameBuffer *mBuffer;
        Int32 mTextureId;

	public:
		sgTexture(void);
		virtual ~sgTexture(void);

        sgFrameBuffer *getBuffer(void) const{ return mBuffer; }
        
        Int32 getTextureId(void) const{ return mTextureId; }
        /// should only be called by renderer implementations
        void _setTexureId(UInt32 tid);
        
        bool isActive(void) const;
        bool hasData(void) const;
        
        // temp
        void _setTextureId(int ID){ mTextureId = ID; }

	}; 

} // namespace Sagitta


#endif // __SGTEXTURE_H__

