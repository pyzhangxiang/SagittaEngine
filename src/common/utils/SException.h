//////////////////////////////////////////////////////
// file: SException.h @ 2008-9-12 by Zhang Xiang
// declares of the class SException
// SException is a class ...
//////////////////////////////////////////////////////
#ifndef __SEXCEPTION_H__
#define __SEXCEPTION_H__

// INCLUDES //////////////////////////////////////////
#include "../configs/SagittaPrerequisites.h"
#include <exception>
#include <string>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SException : public std::exception{
	public:
		enum ExceptionCode{
			ERR_CANNOT_WRITE_TO_FILE,
			ERR_INVALID_STATE,
			ERR_INVALIDPARAMS,
			ERR_RENDERINGAPI_ERROR,
			ERR_DUPLICATE_ITEM,
			ERR_ITEM_NOT_FOUND,
			ERR_FILE_NOT_FOUND,
			ERR_INTERNAL_ERROR,
			ERR_RT_ASSERTION_FAILED, 
			ERR_NOT_IMPLEMENTED
		};
		// member variables
	protected:
		long m_iLine;
		int m_iCode;
		std::string m_sTypeName;
		std::string m_sDescription;
		std::string m_sSourceFunc;
		std::string m_sFileName;
		mutable std::string m_sFullDesc;

		// constructors & destructor
	public:
		SException(int aCode, const std::string &aDescription, const std::string &aSourceFunc);
		SException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aTypeName, const char *aFileName, long aLine);
		SException(const SException &aException);
		~SException(void) throw(){}

		// member functions
	public:
		/// Assignment operator
		void operator =(const SException &aException);

		/** Returns a string with the full description of this error.
            @remarks
                The description contains the error number, the description
                supplied by the thrower, what routine threw the exception,
                and will also supply extra platform-specific information
                where applicable. For example - in the case of a rendering
                library error, the description of the error will include both
                the place in which OGRE found the problem, and a text
                description from the 3D rendering library, if available.
        */
		virtual const std::string &fullDescription(void) const;

		/** Gets the error code. */
		virtual int code(void) const throw(){ return m_iCode; }

		/** Gets the source function. */
		virtual const std::string &sourceFunc(void) const{ return m_sSourceFunc; }

		/** Gets source file name. */
		virtual const std::string &fileName(void) const{ return m_sFileName; }

		/** Gets the line number. */
		virtual long line(void) const{ return m_iLine; }

		/** Returns a string with only the 'description' field of this exception. Use 
			getFullDescriptionto get a full description of the error including line number,
			error number and what function threw the exception.
        */
		virtual const std::string &description(void) const{ return m_sDescription; }

		/// Override std::exception::what
		const char* what() const throw(){ return fullDescription().c_str(); }

	}; //#### end class SException

	/** Template struct which creates a distinct type for each exception code.
		@note
			This is useful because it allows us to create an overloaded method
			for returning different exception types by value without ambiguity. 
			From 'Modern C++ Design' (Alexandrescu 2001).
	*/
	template <int code>
	struct SExceptionCodeType{
		enum { CODE = code };
	};

	// Specialised exceptions allowing each to be caught specifically
	// backwards-compatible since exception codes still used

	class _SagittaExport SUnimplementedException : public SException{
	public:
		SUnimplementedException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "SUnimplementedException", aFileName, aLine){

		}
	};

	class _SagittaExport SFileNotFoundException : public SException{
	public:
		SFileNotFoundException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "SFileNotFoundException", aFileName, aLine){

		}
	};

	class _SagittaExport SIOException : public SException{
	public:
		SIOException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "SIOException", aFileName, aLine){

		}
	};

	class _SagittaExport SInvalidStateException : public SException{
	public:
		SInvalidStateException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "InvalidStateException", aFileName, aLine){

		}
	};

	class _SagittaExport SInvalidParametersException : public SException{
	public:
		SInvalidParametersException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "InvalidParametersException", aFileName, aLine){

		}
	};

	class _SagittaExport SItemIdentityException : public SException{
	public:
		SItemIdentityException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "ItemIdentityException", aFileName, aLine){

		}
	};

	class _SagittaExport SInternalErrorException : public SException{
	public:
		SInternalErrorException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "InternalErrorException", aFileName, aLine){

		}
	};

	class _SagittaExport SRenderingAPIException : public SException{
	public:
		SRenderingAPIException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "RenderingAPIException", aFileName, aLine){

		}
	};

	class _SagittaExport SRuntimeAssertionException : public SException{
	public:
		SRuntimeAssertionException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: SException(aCode, aDescription, aSourceFunc, "RuntimeAssertionException", aFileName, aLine){

		}
	};

	/** Class implementing dispatch methods in order to construct by-value
		exceptions of a derived type based just on an exception code.
	@remarks
		This nicely handles construction of derived Exceptions by value (needed
		for throwing) without suffering from ambiguity - each code is turned into
		a distinct type so that methods can be overloaded. This allows OGRE_EXCEPT
		to stay small in implementation (desirable since it is embedded) whilst
		still performing rich code-to-type mapping. 
	*/
	class SExceptionFactory{
	private:
		/// Private constructor, no construction.
		SExceptionFactory(void){}

	public:
		static SUnimplementedException create(
			SExceptionCodeType<SException::ERR_NOT_IMPLEMENTED> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SUnimplementedException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SFileNotFoundException create(
			SExceptionCodeType<SException::ERR_FILE_NOT_FOUND> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SFileNotFoundException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SIOException create(
			SExceptionCodeType<SException::ERR_CANNOT_WRITE_TO_FILE> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SIOException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SInvalidStateException create(
			SExceptionCodeType<SException::ERR_INVALID_STATE> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SInvalidStateException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SInvalidParametersException create(
			SExceptionCodeType<SException::ERR_INVALIDPARAMS> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SInvalidParametersException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SItemIdentityException create(
			SExceptionCodeType<SException::ERR_ITEM_NOT_FOUND> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SItemIdentityException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SItemIdentityException create(
			SExceptionCodeType<SException::ERR_DUPLICATE_ITEM> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){

				return SItemIdentityException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SInternalErrorException create(
			SExceptionCodeType<SException::ERR_INTERNAL_ERROR> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SInternalErrorException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SRenderingAPIException create(
			SExceptionCodeType<SException::ERR_RENDERINGAPI_ERROR> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SRenderingAPIException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static SRuntimeAssertionException create(
			SExceptionCodeType<SException::ERR_RT_ASSERTION_FAILED> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return SRuntimeAssertionException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}


	}; //#### end class SExceptionFactory

#define THROW_SAGI_EXCEPT(code, desc, src) throw Sagitta::SExceptionFactory::create( \
	Sagitta::SExceptionCodeType<code>(), desc, src, __FILE__, __LINE__ );


} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SEXCEPTION_H__