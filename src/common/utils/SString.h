//////////////////////////////////////////////////////
// file: SString.h @ 2008-8-28 by Zhang Xiang
// declares of the class SString
// SString is a class ...
//////////////////////////////////////////////////////
#ifndef __SSTRING_H__
#define __SSTRING_H__

// INCLUDES //////////////////////////////////////////
#include <string>
#include <sstream>
#include <algorithm>
#include <locale>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _CommonExport SString{
		// member variables

		// constructors & destructor

		// member functions
	public:
		// wrong
		static std::wstring s2ws(const std::string &aString){
			std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

			setlocale(LC_ALL, "chs"); 

			const char *_Source = aString.c_str();
			size_t _Dsize = aString.size() + 1;
			wchar_t *_Dest = new wchar_t[_Dsize];
			wmemset(_Dest, 0, _Dsize);
			mbstowcs(_Dest, _Source, _Dsize);
			std::wstring result = _Dest;
			delete []_Dest;

			setlocale(LC_ALL, curLocale.c_str());

			return result;
		}

		//  [9/12/2008 zhangxiang] //wrong
		static std::string ws2s(const std::wstring &aWString){
			std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

			setlocale(LC_ALL, "chs"); 

			const wchar_t *_Source = aWString.c_str();
			size_t _Dsize = 2 * aWString.size() + 1;
			char *_Dest = new char[_Dsize];
			memset(_Dest, 0, _Dsize);
			wcstombs(_Dest ,_Source, _Dsize);
			std::string result = _Dest;
			delete []_Dest;

			setlocale(LC_ALL, curLocale.c_str());

			return result;
		}

		static std::string to_string(bool x){ std::ostringstream oss; oss << x; return oss.str(); }
		static std::string to_string(int x){ std::ostringstream oss; oss << x; return oss.str(); }
		static std::string to_string(unsigned int x){ std::ostringstream oss; oss << x; return oss.str(); }
		static std::string to_string(unsigned short x){ std::ostringstream oss; oss << x; return oss.str(); }
		static std::string to_string(unsigned long x){ std::ostringstream oss; oss << x; return oss.str(); }
		static std::string to_string(char x){ std::ostringstream oss; oss << (int)x; return oss.str(); }
		static std::string to_string(unsigned char x){ std::ostringstream oss; oss << (unsigned int)x; return oss.str(); }
		static std::string to_string(float x){ std::ostringstream oss; oss << x; return oss.str(); }
		static std::string to_string(double x){ std::ostringstream oss; oss << x; return oss.str(); }
		static std::string to_string(char *x){ std::ostringstream oss; if(!x) oss<<x; else oss << x; return oss.str(); }
		static std::string to_string(void *x){ std::ostringstream oss; oss << x; return oss.str(); }

	}; //#### end class SString

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SSTRING_H__