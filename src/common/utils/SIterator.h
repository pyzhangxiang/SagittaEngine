//////////////////////////////////////////////////////
// file: SIterator.h @ 2008-8-21 by Zhang Xiang
// declares of the class SMenu
// SMenu is a class ...
//////////////////////////////////////////////////////
#ifndef __SITERATOR_H__
#define __SITERATOR_H__

// INCLUDES //////////////////////////////////////////

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class SVectorIterator{
		// member variables
	protected:
		typename T::iterator m_Current;
		typename T::iterator m_End;

		// constructors & destructor
	private:
		SVectorIterator();

	public:
		SVectorIterator(typename T::iterator aStart, typename T::iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit SVectorIterator(T &aVect) : m_Current(aVect.begin()), m_End(aVect.end()){

		}
// ??? for future ...
		SVectorIterator(const SVectorIterator<T> &aVectIt)
			: m_Current(aVectIt.m_Current), m_End(aVectIt.m_End){

		}

		// member functions
		/** Returns true if there are more items in the collection. */
		bool hasMoreElements(void) const{
			return m_Current != m_End;
		}

		/** Returns the next value element in the collection, and advances to the next. */
		typename T::value_type value(void){
			return m_Current->second;
		}

		/** Required to overcome intermittent bug */
		SVectorIterator<T> &operator = (const SVectorIterator<T> &aVectIt){
			m_Current = aVectIt.m_Current;
			m_End = aVectIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const SVectorIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		SVectorIterator<T> operator ++ (int){
			SVectorIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class SVectorIterator


	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class SConstVectorIterator{
	// member variables
	protected:
		typename T::const_iterator m_Current;
		typename T::const_iterator m_End;

	// constructors & destructor
	public:
		SConstVectorIterator(typename T::const_iterator aStart, typename T::const_iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit SConstVectorIterator(const T &aVect) : m_Current(aVect.begin()), m_End(aVect.end()){

		}
// ??? for future ...
		SConstVectorIterator(const SConstVectorIterator<T> &aVectIt) :
		m_Current(aVectIt.m_Current), m_End(aVectIt.m_End){

		}

		// member functions
		/** Returns true if there are more items in the collection. */
		bool hasMoreElements(void) const{
			return m_Current != m_End;
		}

		/** Returns the next value element in the collection, and advances to the next. */
		typename T::value_type value(void){
			return *m_Current;
		}

		/** Required to overcome intermittent bug */
		SConstVectorIterator<T> &operator = (const SConstVectorIterator<T> &aVectIt){
			m_Current = aVectIt.m_Current;
			m_End = aVectIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const SConstVectorIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		SConstVectorIterator<T> operator ++ (int){
			SConstVectorIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class SConstVectorIterator


	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class SMapIterator{
		// member variables
	protected:
		typename T::iterator m_Current;
		typename T::iterator m_End;

		// constructors & destructor
	private:
		SMapIterator();

	public:
		SMapIterator(typename T::iterator aStart, typename T::iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit SMapIterator(T &aMap) : m_Current(aMap.begin()), m_End(aMap.end()){

		}
// ??? for future ... why can not have a copy constructor?
		SMapIterator(const SMapIterator<T> &aMapIt) :
		m_Current(aMapIt.m_Current), m_End(aMapIt.m_End){

		}

		// member functions
		/** Returns true if there are more items in the collection. */
		bool hasMoreElements(void) const{
			return m_Current != m_End;
		}

		/** Returns the next value element in the collection, and advances to the next. */
		typename T::mapped_type value(void){
			return m_Current->second;
		}
		
		/** Returns the next key element in the collection, without advancing to the next. */
		typename T::key_type key(void){
			return m_Current->first;
		}

		/** Required to overcome intermittent bug */
		SMapIterator<T> &operator = (const SMapIterator<T> &aMapIt){
			m_Current = aMapIt.m_Current;
			m_End = aMapIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const SMapIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		SMapIterator<T> operator ++ (int){
			SMapIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class SMapIterator


	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class SConstMapIterator{
		// member variables
	protected:
		typename T::const_iterator m_Current;
		typename T::const_iterator m_End;

		// constructors & destructor
	private:
		SConstMapIterator();

	public:
		SConstMapIterator(typename T::const_iterator aStart, typename T::const_iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit SConstMapIterator(const T &aMap) : m_Current(aMap.begin()), m_End(aMap.end()){

		}
// ??? for future ...		
		SConstMapIterator(const SConstMapIterator<T> &aMapIt) :
		m_Current(aMapIt.m_Current), m_End(aMapIt.m_End){

		}

		// member functions
	public:
		/** Returns true if there are more items in the collection. */
		bool hasMoreElements(void) const{
			return m_Current != m_End;
		}

		/** Returns the next value element in the collection, and advances to the next. */
		typename T::mapped_type value(void){
			return m_Current->second;
		}

		/** Returns the next key element in the collection, without advancing to the next. */
		typename T::key_type key(void){
			return m_Current->first;
		}

		/** Required to overcome intermittent bug */
		SConstMapIterator<T> &operator = (const SConstMapIterator<T> &aMapIt){
			m_Current = aMapIt.m_Current;
			m_End = aMapIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const SConstMapIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		SConstMapIterator<T> operator ++ (int){
			SConstMapIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class SConstMapIterator


	/** SCMPMapIterator
		@param
			Inner std::map<inner_key_type, value_type>
		@param
			Outer std::map<outer_key_type, Inner*>
	*/
	template<class Outer, class Inner>
	class SCMPMapIterator{
	// type defines
	protected:
		typedef typename Outer::iterator OuterIt;
		typedef typename Inner::iterator InnerIt;

	// member variables
	protected:
		OuterIt m_oCurrent;
		OuterIt m_oEnd;

		InnerIt m_iCurrent;
		InnerIt *m_iEnds;

		size_t m_iInnerIndex;
		size_t m_iInnerNum;

	// constructors & destructor
	public:
		explicit SCMPMapIterator(Outer &oMap) :
		m_oCurrent(oMap.begin()), m_oEnd(oMap.end()),
		m_iInnerIndex(0), m_iInnerNum(oMap.size()){
			if(m_iInnerNum > 0){
				m_iCurrent = m_oCurrent->second->begin();
				m_iEnds = new InnerIt[m_iInnerNum];
				OuterIt bgn = m_oCurrent;
				for(size_t i=0; m_oCurrent!=m_oEnd; ++m_oCurrent, ++i){
					m_iEnds[i] = m_oCurrent->second->end();
				}
				m_oCurrent = bgn;

				// process the case m_iCurrent == m_iEnds[0] at first
				while(m_iCurrent == m_iEnds[m_iInnerIndex]){
					++m_oCurrent;
					++m_iInnerIndex;
					if(m_oCurrent != m_oEnd){
						m_iCurrent = m_oCurrent->second->begin();
					}else{
						break;
					}
				}

			}else{
				m_iEnds = 0;
			}
		}

		SCMPMapIterator(const SCMPMapIterator<Outer, Inner> &cmpMap) :
		m_oCurrent(cmpMap.m_oCurrent), m_oEnd(cmpMap.m_oEnd),
		m_iCurrent(cmpMap.m_iCurrent), m_iInnerIndex(cmpMap.m_iInnerIndex),
		m_iInnerNum(cmpMap.m_iInnerNum){
			if(m_iInnerNum > 0){
				m_iEnds = new InnerIt[m_iInnerNum];
				for(size_t i=0; i<m_iInnerNum; ++i){
					m_iEnds[i] = cmpMap.m_iEnds[i];
				}
			}
		}

		~SCMPMapIterator(void){
			if(m_iEnds)
				delete []m_iEnds;
		}

	// member functions
	public:
		/** Returns true if there are more items in the collection. */
		bool hasMoreElements(void) const{
			return m_oCurrent != m_oEnd;
		}

		/** Returns the next value element in the collection. */
		typename Inner::mapped_type value(void){
			return m_iCurrent->second;
		}

		/** Returns the key element in the inner collection. */
		typename Inner::key_type innerKey(void){
			return m_iCurrent->first;
		}

		/** Returns the key element in the outer collection. */
		typename Outer::key_type outerKey(void){
			return m_oCurrent->first;
		}

		/** Required to overcome intermittent bug */
		SCMPMapIterator<Outer, Inner> &operator = (const SCMPMapIterator<Outer, Inner> &cmpMap){
			m_oCurrent = cmpMap.m_oCurrent;
			m_oEnd = cmpMap.m_oEnd;
			m_iCurrent = cmpMap.m_iCurrent;
			m_iInnerIndex = cmpMap.m_iInnerIndex;
			m_iInnerNum = cmpMap.m_iInnerNum;

			if(m_iInnerNum > 0){
				m_iEnds = new InnerIt[m_iInnerNum];
				for(size_t i=0; i<m_iInnerNum; ++i){
					m_iEnds[i] = cmpMap.m_iEnds[i];
				}
			}else{
				m_iEnds = 0;
			}

			return *this;
		}

		/** Moves the iterator on one element. */
		const SCMPMapIterator<Outer, Inner> &operator ++ (void){
			++m_iCurrent;
			if(m_iCurrent == m_iEnds[m_iInnerIndex]){
				++m_oCurrent;
				++m_iInnerIndex;
				if(m_oCurrent != m_oEnd){
					m_iCurrent = m_oCurrent->second->begin();
				}
			}
			return *this;
		}

		SCMPMapIterator<Outer, Inner> operator ++ (int){
			SCMPMapIterator<Outer, Inner> ret(*this);
			this->operator ++();
			return ret;
		}

	}; //#### end class SCMPMapIterator


	/** SConstCMPMapIterator
		@param
			Inner std::map<inner_key_type, value_type>
		@param
			Outer std::map<outer_key_type, Inner*>
	*/
	template<class Outer, class Inner>
	class SConstCMPMapIterator{
		// type defines
	protected:
		typedef typename Outer::const_iterator OuterIt;
		typedef typename Inner::const_iterator InnerIt;

		// member variables
	protected:
		OuterIt m_oCurrent;
		OuterIt m_oEnd;

		InnerIt m_iCurrent;
		InnerIt *m_iEnds;

		size_t m_iInnerIndex;
		size_t m_iInnerNum;

		// constructors & destructor
	public:
		explicit SConstCMPMapIterator(const Outer &oMap) :
		m_oCurrent(oMap.begin()), m_oEnd(oMap.end()),
		m_iInnerIndex(0), m_iInnerNum(oMap.size()){
			if(m_iInnerNum > 0){
				m_iCurrent = m_oCurrent->second->begin();
				m_iEnds = new InnerIt[m_iInnerNum];
				OuterIt bgn = m_oCurrent;
				for(size_t i=0; m_oCurrent!=m_oEnd; ++m_oCurrent, ++i){
					m_iEnds[i] = m_oCurrent->second->end();
				}
				m_oCurrent = bgn;
				
				// process the case m_iCurrent == m_iEnds[0] at first
				while(m_iCurrent == m_iEnds[m_iInnerIndex]){
					++m_oCurrent;
					++m_iInnerIndex;
					if(m_oCurrent != m_oEnd){
						m_iCurrent = m_oCurrent->second->begin();
					}else{
						break;
					}
				}

			}else{
				m_iEnds = 0;
			}
		}

		SConstCMPMapIterator(const SConstCMPMapIterator<Outer, Inner> &cmpMap) :
		m_oCurrent(cmpMap.m_oCurrent), m_oEnd(cmpMap.m_oEnd),
		m_iCurrent(cmpMap.m_iCurrent), m_iInnerIndex(cmpMap.m_iInnerIndex),
		m_iInnerNum(cmpMap.m_iInnerNum){
			if(m_iInnerNum > 0){
				m_iEnds = new InnerIt[m_iInnerNum];
				for(size_t i=0; i<m_iInnerNum; ++i){
					m_iEnds[i] = cmpMap.m_iEnds[i];
				}
			}
		}

		~SConstCMPMapIterator(void){
			if(!m_iEnds)
				delete []m_iEnds;
		}

		// member functions
	public:
		/** Returns true if there are more items in the collection. */
		bool hasMoreElements(void) const{
			return m_oCurrent != m_oEnd;
		}

		/** Returns the next value element in the collection. */
		typename Inner::mapped_type value(void){
			return m_iCurrent->second;
		}

		/** Returns the key element in the inner collection. */
		typename Inner::key_type innerKey(void){
			return m_iCurrent->first;
		}

		/** Returns the key element in the outer collection. */
		typename Outer::key_type outerKey(void){
			return m_oCurrent->first;
		}

		/** Required to overcome intermittent bug */
		SConstCMPMapIterator<Outer, Inner> &operator = (const SConstCMPMapIterator<Outer, Inner> &cmpMap){
			m_oCurrent = cmpMap.m_oCurrent;
			m_oEnd = cmpMap.m_oEnd;
			m_iCurrent = cmpMap.m_iCurrent;
			m_iInnerIndex = cmpMap.m_iInnerIndex;
			m_iInnerNum = cmpMap.m_iInnerNum;

			if(m_iInnerNum > 0){
				m_iEnds = new InnerIt[m_iInnerNum];
				for(size_t i=0; i<m_iInnerNum; ++i){
					m_iEnds[i] = cmpMap.m_iEnds[i];
				}
			}

			return *this;
		}

		/** Moves the iterator on one element. */
		const SConstCMPMapIterator<Outer, Inner> &operator ++ (void){
			++m_iCurrent;
			if(m_iCurrent == m_iEnds[m_iInnerIndex]){
				++m_oCurrent;
				++m_iInnerIndex;
				if(m_oCurrent != m_oEnd){
					m_iCurrent = m_oCurrent->second->begin();
				}
			}
			return *this;
		}

		SConstCMPMapIterator<Outer, Inner> operator ++ (int){
			SCMPMapIterator<Outer, Inner> ret(*this);
			this->operator ++();
			return ret;
		}

	}; //#### end class SConstCMPMapIterator

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SITERATOR_H__