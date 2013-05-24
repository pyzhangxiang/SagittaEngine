
#include "sgMemChoc.h"
#include <cassert>

namespace Sagitta{
    
	
    

	sgMemChoc::sgMemChoc( size_t aBlockSize, UByte aBlockNum /*= 0xffff*/ )
	{
		_data.m_uiFirstAvailable = 0;
		_data.m_pStorage = 0;
		_data.m_uiBlockNum = aBlockNum;
		_data.m_uiblocksAvailable = aBlockNum;
		_data.m_uiBlockSize = aBlockSize;//sizeof(T);

		_data.m_uiByteSize = _data.m_uiBlockSize * _data.m_uiBlockNum;

		assert(_data.m_uiBlockSize > 0);  
		assert(_data.m_uiBlockNum > 0);  
		// Overflow check  
		assert(_data.m_uiByteSize / _data.m_uiBlockSize == _data.m_uiBlockNum);

		_data.m_pStorage = (UByte*)__internal::__sgMalloc(_data.m_uiByteSize);//new char[m_uiByteSize];



#ifdef _DEBUG
		_data.mBlockIdleInfo.resize(_data.m_uiBlockNum);
#endif
		UByte *pdata = _data.m_pStorage;            
		for(UByte i=0; i<_data.m_uiBlockNum; ){
			// first byte of every available block point to next available block
#ifdef _DEBUG
			_data.mBlockIdleInfo[i] = true;
#endif
			*pdata = ++i;
			pdata += _data.m_uiBlockSize;

		}
	}

	sgMemChoc::~sgMemChoc( void )
	{
		__internal::__sgFree(_data.m_pStorage);
	}

	bool sgMemChoc::isFull( void ) const
	{
		return (_data.m_uiblocksAvailable == 0);
	}

	bool sgMemChoc::isEmpty( void ) const
	{
		return (_data.m_uiblocksAvailable == _data.m_uiBlockNum);
	}

	void *sgMemChoc::allocate(void)
	{
		assert(!isFull());

#ifdef _DEBUG
		_data.mBlockIdleInfo[_data.m_uiFirstAvailable] = false;
#endif
		UByte *ptr = _data.m_pStorage + _data.m_uiFirstAvailable * _data.m_uiBlockSize;
		_data.m_uiFirstAvailable = *ptr;
		--_data.m_uiblocksAvailable; 
		return ptr;
	}

	void sgMemChoc::deallocate( void *aPointer )
	{
		assert(aPointer >= _data.m_pStorage);
		UByte *toRelease = static_cast<UByte*>(aPointer);  

		// Overflow check
		assert(toRelease - _data.m_pStorage <= _data.m_uiByteSize - _data.m_uiBlockSize);
		// Alignment check  
		assert((toRelease - _data.m_pStorage) % _data.m_uiBlockSize == 0); 

		*toRelease = _data.m_uiFirstAvailable;
		_data.m_uiFirstAvailable = 
			static_cast<UByte>((toRelease - _data.m_pStorage) / _data.m_uiBlockSize); 

		// Truncation check  
		assert(_data.m_uiFirstAvailable == (toRelease - _data.m_pStorage) / _data.m_uiBlockSize);

		++_data.m_uiblocksAvailable; 

#ifdef _DEBUG
		_data.mBlockIdleInfo[_data.m_uiFirstAvailable] = true;
#endif
	}

	bool sgMemChoc::hasBlock( void *aPointer )
	{
		if(aPointer < _data.m_pStorage)
			return false;

		UByte *toRelease = static_cast<UByte*>(aPointer);  
		if((toRelease - _data.m_pStorage) > (_data.m_uiByteSize - _data.m_uiBlockSize))
			return false;

		return true;
	}

#ifdef _DEBUG
	void sgMemChoc::dump( void )
	{
		std::cout << "-----------------Storage Graph: \n";

		std::cout << "storage address: " << (void*)_data.m_pStorage << "\n";
		std::cout << "available blocks: " << (size_t)_data.m_uiblocksAvailable << "\n";
		std::cout << "first available: " << (size_t)_data.m_uiFirstAvailable << "\n";
		for(UByte i=0; i<_data.m_uiBlockNum; ++i){

			std::cout << (size_t)i;
			if(_data.mBlockIdleInfo[i])
				std::cout << "(i) ";
			else
				std::cout << "(b)";
		}


		std::cout << "\n-----------------End Storage Graph.\n\n";
	}
#endif

	//////////////////////////////////////////////////////////////////////////





	sgMemChocBox::sgMemChocBox( size_t aBlockSize, UByte aBlockNum /*= 0xffff*/ ) : m_uiBlockNumPerChoc(aBlockNum),
		m_uiBlockSizePerChoc(aBlockSize/*sizeof(T)*/),
		mLatestAllocChoc(0),
		mLatestDeallocChoc(0),
		mEmptyChoc(0)
	{
		mLatestAllocChoc = produceNewChoc();
		mLatestDeallocChoc = 0;
	}

	sgMemChocBox::~sgMemChocBox( void )
	{
		for(size_t i=0; i<m_listChoc.size(); ++i)
		{
			delete m_listChoc[i];
		}
	}

	sgMemChoc * sgMemChocBox::produceNewChoc( void )
	{
		sgMemChoc *pMc = new sgMemChoc(m_uiBlockSizePerChoc, m_uiBlockNumPerChoc);
		m_listChoc.push_back(pMc);
		mEmptyChoc = pMc;
		return mEmptyChoc;
	}

	sgMemChoc * sgMemChocBox::findChoc( void *ptr )
	{
		if(mLatestDeallocChoc && mLatestDeallocChoc->hasBlock(ptr))
			return mLatestDeallocChoc;

		sgMemChoc *choc = 0;
		for(size_t i=0; i<m_listChoc.size(); ++i)
		{
			choc = m_listChoc[i];
			if(choc->hasBlock(ptr))
				return choc;
		}
		return 0;
	}

	size_t sgMemChocBox::blockSize( void )
	{
		return m_uiBlockSizePerChoc;
	}

	void *sgMemChocBox::allocate(void){
		sgMemChoc *pChunk = 0;

		if(!mLatestAllocChoc->isFull())
		{
			pChunk = mLatestAllocChoc;
		}
		else
		{
			for(size_t i=0; i<m_listChoc.size(); ++i)
			{
				if( ! (m_listChoc[i]->isFull()))
				{
					pChunk = m_listChoc[i];
					break;
				}
			}
		}
		
		if(!pChunk)
		{
			pChunk = produceNewChoc();
		}
		if(pChunk == mEmptyChoc)
			mEmptyChoc = 0;

		mLatestAllocChoc = pChunk;

		return pChunk->allocate();
	}

	void sgMemChocBox::deallocate( void *aPointer )
	{
		sgMemChoc *choc = findChoc(aPointer);
		assert(choc);

		choc->deallocate(aPointer);
		mLatestDeallocChoc = choc;

		if(mLatestDeallocChoc->isEmpty())
		{
			if(mEmptyChoc && (mEmptyChoc != mLatestDeallocChoc))
			{
				sgMemChoc *lastChoc = m_listChoc[m_listChoc.size() - 1];
				// If last Chunk is empty, just change what deallocChunk_  
				// points to, and release the last.  Otherwise, swap an empty  
				// Chunk with the last, and then release it. 
				if(lastChoc == mLatestDeallocChoc)
					mLatestDeallocChoc = mEmptyChoc;
				else if(lastChoc != mEmptyChoc)
					std::swap(mEmptyChoc->_data, lastChoc->_data);
				assert(lastChoc->isEmpty());
				delete lastChoc;
				m_listChoc.pop_back();

				if((mLatestAllocChoc == lastChoc) || (mLatestAllocChoc->isFull()))
					mLatestAllocChoc = mLatestDeallocChoc;
			}

			mEmptyChoc = mLatestDeallocChoc;
		}

		// prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.  
		assert((0 == mEmptyChoc) || (mEmptyChoc->isEmpty()));
	}

#ifdef _DEBUG
	void sgMemChocBox::dump( void )
	{
		std::cout << "Choc Box Storage Graph ------------\n";

		for(size_t i=0; i<m_listChoc.size(); ++i)
		{
			m_listChoc[i]->dump();
		}

		std::cout << "End Choc Box Storage Graph --------\n\n";
	}
#endif
} // namespace Sagitta

