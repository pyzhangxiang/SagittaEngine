//////////////////////////////////////////////////////
// file: SharePtr.h 
// created by zhangxiang on 09-10-3
// declares of the class SharePtr
// SharePtr is a class ...
//////////////////////////////////////////////////////
#ifndef __SHAREPTR_H__
#define __SHAREPTR_H__

// INCLUDES //////////////////////////////////////////
#include <cassert>

// DECLARES //////////////////////////////////////////

namespace Sagitta{
	
	/** class representation
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	template<typename T>
	class SharePtr{
	// member variables
	private:
		T *_m_pPointee;
		unsigned int *_m_pCount;
		
	// constructors & destructor
	public:
		SharePtr(void)
		: _m_pPointee(0), _m_pCount(0){
			
		}
		
		template<typename R>
		SharePtr(R *rawp)
		: _m_pPointee(rawp), _m_pCount(0){
			if(_m_pPointee)
				_m_pCount = new unsigned int(1);
		}
		
		// needed, or the default copy constructor will be generated
		// copy constructor cannot be template
		SharePtr(const SharePtr &v)
		: _m_pPointee(v._m_pPointee), _m_pCount(v._m_pCount){
			if(_m_pCount){
				++(*_m_pCount);
			}
		}
		
		template<typename R>
		SharePtr(const SharePtr<R> &v)
		: _m_pPointee(v.raw()), _m_pCount(v.pcount()){
			if(_m_pCount){
				++(*_m_pCount);
			}
		}
		
		~SharePtr(void){
			release();
		}
		
	// member functions
	public:
		template<typename R>
		SharePtr &operator=(R *rawp){
			if(_m_pPointee == rawp)
				return *this;
			
			release();
			
			_m_pPointee = rawp;
			if(_m_pPointee)
				_m_pCount = new unsigned int(1);
			else
				_m_pCount = 0;
			
			return *this;
		}
		
		// needed, or the default assignment operator will be generated
		// assignment operater cannot be template
		SharePtr &operator=(const SharePtr &v){
			if(_m_pPointee == v._m_pPointee)
				return *this;
			
			release();
			
			_m_pPointee = v._m_pPointee;
			_m_pCount = v._m_pCount;
			if(_m_pCount){
				++(*_m_pCount);
			}
			
			return *this;
		}
		
		template<typename R>
		SharePtr &operator=(const SharePtr<R> &v){
			if(_m_pPointee == v.raw())
				return *this;
			
			release();
			
			_m_pPointee = v.raw();
			_m_pCount = v.pcount();
			if(_m_pCount){
				++(*_m_pCount);
			}
			
			return *this;
		}
		
		inline T &operator*(void) const{assert(_m_pPointee); return *_m_pPointee; }
		inline T *operator->(void) const{ assert(_m_pPointee); return _m_pPointee; }
		inline T *raw(void) const{ return _m_pPointee; }
		inline unsigned int count(void) const{ assert(_m_pCount); return *_m_pCount; }
		inline unsigned int *pcount(void) const{ return _m_pCount; }
		
//		inline bool operator!(void) const{ return _m_pPointee == 0; }	// no used
		template<typename U>
		inline bool operator==(const SharePtr<U> &rhs) const{ return _m_pPointee == rhs.raw(); }
		template<typename U>
		inline bool operator!=(const SharePtr<U> &rhs) const{ return _m_pPointee != rhs.raw(); }
		
	protected:
		void release(void){
			if(_m_pCount){
				--(*_m_pCount);
				if((*_m_pCount) == 0){
					delete _m_pPointee;
					delete _m_pCount;
					
					_m_pPointee = 0;
					_m_pCount = 0;
				}
			}
		}
		
	}; //#### end class SharePtr
	
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SHAREPTR_H__

