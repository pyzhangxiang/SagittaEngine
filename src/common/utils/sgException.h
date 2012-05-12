//////////////////////////////////////////////////////
// file: sgException.h @ 2008-9-12 by Zhang Xiang
// declares of the class sgException
// sgException is a class ...
//////////////////////////////////////////////////////
#ifndef __SGEXCEPTION_H__
#define __SGEXCEPTION_H__

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
	class _SG_CommonExport sgException : public std::exception{
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
		sgException(int aCode, const std::string &aDescription, const std::string &aSourceFunc);
		sgException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aTypeName, const char *aFileName, long aLine);
		sgException(const sgException &aException);
		~sgException(void) throw(){}

		// member functions
	public:
		/// Assignment operator
		void operator =(const sgException &aException);

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

	}; //#### end class sgException

	/** Template struct which creates a distinct type for each exception code.
		@note
			This is useful because it allows us to create an overloaded method
			for returning different exception types by value without ambiguity. 
			From 'Modern C++ Design' (Alexandrescu 2001).
	*/
	template <int code>
	struct sgExceptionCodeType{
		enum { CODE = code };
	};

	// Specialised exceptions allowing each to be caught specifically
	// backwards-compatible since exception codes still used

	class _SG_CommonExport sgUnimplementedException : public sgException{
	public:
		sgUnimplementedException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "sgUnimplementedException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgFileNotFoundException : public sgException{
	public:
		sgFileNotFoundException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "sgFileNotFoundException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgIOException : public sgException{
	public:
		sgIOException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "sgIOException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgInvalidStateException : public sgException{
	public:
		sgInvalidStateException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "InvalidStateException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgInvalidParametersException : public sgException{
	public:
		sgInvalidParametersException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "InvalidParametersException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgItemIdentityException : public sgException{
	public:
		sgItemIdentityException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "ItemIdentityException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgInternalErrorException : public sgException{
	public:
		sgInternalErrorException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "InternalErrorException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgRenderingAPIException : public sgException{
	public:
		sgRenderingAPIException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "RenderingAPIException", aFileName, aLine){

		}
	};

	class _SG_CommonExport sgRuntimeAssertionException : public sgException{
	public:
		sgRuntimeAssertionException(int aCode, const std::string &aDescription, const std::string &aSourceFunc, const char *aFileName, long aLine)
			: sgException(aCode, aDescription, aSourceFunc, "RuntimeAssertionException", aFileName, aLine){

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
	class sgExceptionFactory{
	private:
		/// Private constructor, no construction.
		sgExceptionFactory(void){}

	public:
		static sgUnimplementedException create(
			sgExceptionCodeType<sgException::ERR_NOT_IMPLEMENTED> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgUnimplementedException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgFileNotFoundException create(
			sgExceptionCodeType<sgException::ERR_FILE_NOT_FOUND> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgFileNotFoundException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgIOException create(
			sgExceptionCodeType<sgException::ERR_CANNOT_WRITE_TO_FILE> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgIOException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgInvalidStateException create(
			sgExceptionCodeType<sgException::ERR_INVALID_STATE> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgInvalidStateException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgInvalidParametersException create(
			sgExceptionCodeType<sgException::ERR_INVALIDPARAMS> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgInvalidParametersException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgItemIdentityException create(
			sgExceptionCodeType<sgException::ERR_ITEM_NOT_FOUND> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgItemIdentityException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgItemIdentityException create(
			sgExceptionCodeType<sgException::ERR_DUPLICATE_ITEM> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){

				return sgItemIdentityException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgInternalErrorException create(
			sgExceptionCodeType<sgException::ERR_INTERNAL_ERROR> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgInternalErrorException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgRenderingAPIException create(
			sgExceptionCodeType<sgException::ERR_RENDERINGAPI_ERROR> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgRenderingAPIException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}

		static sgRuntimeAssertionException create(
			sgExceptionCodeType<sgException::ERR_RT_ASSERTION_FAILED> aCode,
			const std::string &aDescription,
			const std::string &aSourceFunc,
			const char *aFileName,
			long aLine){
			
			return sgRuntimeAssertionException(aCode.CODE, aDescription, aSourceFunc, aFileName, aLine);
		}


	}; //#### end class sgExceptionFactory

#define THROW_SAGI_EXCEPT(code, desc, src) throw Sagitta::sgExceptionFactory::create( \
	Sagitta::sgExceptionCodeType<code>(), desc, src, __FILE__, __LINE__ );


} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGEXCEPTION_H__

