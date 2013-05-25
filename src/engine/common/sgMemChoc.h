#ifndef __SGMEMCHOC_H__
#define __SGMEMCHOC_H__

#include "sgMalloc.h"
#include <iostream>
#include <list>
#include <vector>
#include <exception>


namespace Sagitta{
    
	//template<typename T>
	class sgMemChocBox;
    
	//template<typename T>
	class _SG_KernelExport sgMemChoc{
        
		friend class sgMemChocBox;//<T>;
        
        
	private:
		typedef unsigned char UByte;
        
	private:
        struct MemData
        {
            UByte m_uiFirstAvailable;
            UByte m_uiBlockNum;
            UByte m_uiblocksAvailable;
            size_t m_uiBlockSize; 
            size_t m_uiByteSize;
            UByte *m_pStorage;
            
#ifdef _DEBUG
            std::vector<bool> mBlockIdleInfo;
#endif
        };
        MemData _data;
        
		sgMemChoc(const sgMemChoc&);
		sgMemChoc &operator=(const sgMemChoc&);
	public:
		sgMemChoc(size_t aBlockSize, UByte aBlockNum = 0xff);
        
		~sgMemChoc(void);
        
		bool isFull(void) const;
        
		bool isEmpty(void) const;
        
		void *allocate(void);
        
		void deallocate(void *aPointer);
        
		bool hasBlock(void *aPointer);
        
#ifdef _DEBUG
		void dump(void);
#endif
        
	}; // class sgMemChoc
    
    
	//template<typename T>
	class _SG_KernelExport sgMemChocBox{
	protected:
		// typedef sgMemChoc/*<T>*/ ChunkType;
		typedef std::vector<sgMemChoc*> ChocList;
		typedef /*typename */sgMemChoc::UByte UByte;
        
		ChocList m_listChoc;
        
		UByte m_uiBlockNumPerChoc;
		size_t m_uiBlockSizePerChoc;  // real choc size
		sgMemChoc *mLatestAllocChoc;
		sgMemChoc *mLatestDeallocChoc;
		sgMemChoc *mEmptyChoc;
        
		sgMemChocBox(const sgMemChocBox&);
		sgMemChocBox &operator=(const sgMemChocBox&);
	public:
		sgMemChocBox(size_t aBlockSize, UByte aBlockNum = 0xff);
        
		virtual ~sgMemChocBox(void);
        
	private:
		sgMemChoc *produceNewChoc(void);
        
		sgMemChoc *findChoc(void *ptr);
        
	public:
		size_t blockSize(void);
        
        
        
		void *allocate(void);
        
		void deallocate(void *aPointer);
        
#ifdef _DEBUG
		void dump(void);
#endif
        
	}; // class sgMemChocBox

	template<typename T>
	class sgMemTypeChocBox : public sgMemChocBox
	{
	public:
		sgMemTypeChocBox(UByte aBlockNum = 0xffff)
			: sgMemChocBox(sizeof(T), aBlockNum)
		{

		}
	};

    
} // namespace Sagitta

#endif // __SGMEMCHOC_H__
