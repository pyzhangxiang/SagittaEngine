//////////////////////////////////////////////////////
// file: SIterator.h @ 2008-8-21 by Zhang Xiang
// declares of the class SMenu
// SMenu is a class ...
//////////////////////////////////////////////////////
#ifndef __SGITERATOR_H__
#define __SGITERATOR_H__

// INCLUDES //////////////////////////////////////////

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class sgVectorIterator{
		// member variables
	protected:
		typename T::iterator m_Current;
		typename T::iterator m_End;

		// constructors & destructor
	private:
		sgVectorIterator();

	public:
		sgVectorIterator(typename T::iterator aStart, typename T::iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit sgVectorIterator(T &aVect) : m_Current(aVect.begin()), m_End(aVect.end()){

		}
// ??? for future ...
		sgVectorIterator(const sgVectorIterator<T> &aVectIt)
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
		sgVectorIterator<T> &operator = (const sgVectorIterator<T> &aVectIt){
			m_Current = aVectIt.m_Current;
			m_End = aVectIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const sgVectorIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		sgVectorIterator<T> operator ++ (int){
			sgVectorIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class sgVectorIterator


	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class sgConstVectorIterator{
	// member variables
	protected:
		typename T::const_iterator m_Current;
		typename T::const_iterator m_End;

	// constructors & destructor
	public:
		sgConstVectorIterator(typename T::const_iterator aStart, typename T::const_iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit sgConstVectorIterator(const T &aVect) : m_Current(aVect.begin()), m_End(aVect.end()){

		}
// ??? for future ...
		sgConstVectorIterator(const sgConstVectorIterator<T> &aVectIt) :
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
		sgConstVectorIterator<T> &operator = (const sgConstVectorIterator<T> &aVectIt){
			m_Current = aVectIt.m_Current;
			m_End = aVectIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const sgConstVectorIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		sgConstVectorIterator<T> operator ++ (int){
			sgConstVectorIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class sgConstVectorIterator


	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class sgMapIterator{
		// member variables
	protected:
		typename T::iterator m_Current;
		typename T::iterator m_End;

		// constructors & destructor
	private:
		sgMapIterator();

	public:
		sgMapIterator(typename T::iterator aStart, typename T::iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit sgMapIterator(T &aMap) : m_Current(aMap.begin()), m_End(aMap.end()){

		}
// ??? for future ... why can not have a copy constructor?
		sgMapIterator(const sgMapIterator<T> &aMapIt) :
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
		sgMapIterator<T> &operator = (const sgMapIterator<T> &aMapIt){
			m_Current = aMapIt.m_Current;
			m_End = aMapIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const sgMapIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		sgMapIterator<T> operator ++ (int){
			sgMapIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class sgMapIterator


	/** class representation
	@remarks

	@note

	@par

	*/
	template <class T>
	class sgConstMapIterator{
		// member variables
	protected:
		typename T::const_iterator m_Current;
		typename T::const_iterator m_End;

		// constructors & destructor
	private:
		sgConstMapIterator();

	public:
		sgConstMapIterator(typename T::const_iterator aStart, typename T::const_iterator aEnd) : m_Current(aStart), m_End(aEnd){

		}

		explicit sgConstMapIterator(const T &aMap) : m_Current(aMap.begin()), m_End(aMap.end()){

		}
// ??? for future ...		
		sgConstMapIterator(const sgConstMapIterator<T> &aMapIt) :
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
		sgConstMapIterator<T> &operator = (const sgConstMapIterator<T> &aMapIt){
			m_Current = aMapIt.m_Current;
			m_End = aMapIt.m_End;
			return *this;
		}

		/** Moves the iterator on one element. */
		const sgConstMapIterator<T> &operator ++ (void){
			++m_Current;
			return *this;
		}

		sgConstMapIterator<T> operator ++ (int){
			sgConstMapIterator<T> ret(m_Current, m_End);
			++m_Current;
			return ret;
		}

	}; //#### end class sgConstMapIterator


	/** sgCMPMapIterator
		@param
			Inner std::map<inner_key_type, value_type>
		@param
			Outer std::map<outer_key_type, Inner*>
	*/
	template<class Outer, class Inner>
	class sgCMPMapIterator{
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
		explicit sgCMPMapIterator(Outer &oMap) :
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

		sgCMPMapIterator(const sgCMPMapIterator<Outer, Inner> &cmpMap) :
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

		~sgCMPMapIterator(void){
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
		sgCMPMapIterator<Outer, Inner> &operator = (const sgCMPMapIterator<Outer, Inner> &cmpMap){
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
		const sgCMPMapIterator<Outer, Inner> &operator ++ (void){
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

		sgCMPMapIterator<Outer, Inner> operator ++ (int){
			sgCMPMapIterator<Outer, Inner> ret(*this);
			this->operator ++();
			return ret;
		}

	}; //#### end class sgCMPMapIterator


	/** sgConstCMPMapIterator
		@param
			Inner std::map<inner_key_type, value_type>
		@param
			Outer std::map<outer_key_type, Inner*>
	*/
	template<class Outer, class Inner>
	class sgConstCMPMapIterator{
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
		explicit sgConstCMPMapIterator(const Outer &oMap) :
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

		sgConstCMPMapIterator(const sgConstCMPMapIterator<Outer, Inner> &cmpMap) :
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

		~sgConstCMPMapIterator(void){
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
		sgConstCMPMapIterator<Outer, Inner> &operator = (const sgConstCMPMapIterator<Outer, Inner> &cmpMap){
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
		const sgConstCMPMapIterator<Outer, Inner> &operator ++ (void){
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

		sgConstCMPMapIterator<Outer, Inner> operator ++ (int){
			sgCMPMapIterator<Outer, Inner> ret(*this);
			this->operator ++();
			return ret;
		}

	}; //#### end class sgConstCMPMapIterator

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGITERATOR_H__

