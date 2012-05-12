/*
 *  shareptr_test.h
 *  Sagitta
 *
 *  Created by zhangxiang on 09-10-3.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <common/utils/SharePtr.h>
#include <iostream>
#include <string>
#include <cstdio>
using namespace Sagitta;
using namespace std;

class SharePtr_Test{
public:
	class Object{
	private:
		static int ms_gid;
		
		int m_id;
	public:
		Object(void) : m_id(++ms_gid){ printf("Obj %d Constructor.\n", m_id); }
		virtual ~Object(void){ printf("Obj %d Destructor.\n", m_id); }
		
		void print(void){ printf("print Obj %d.\n", m_id); }
	};
	
	class DObject : public Object{
		
	};
	
	class Object2{
	private:
		static int ms_gid;
		
		int m_id;
		
	public:
		Object2(void) : m_id(++ms_gid){ printf("Obj2 %d Constructor.\n", m_id); }
		~Object2(void){ printf("Obj2 %d Destructor.\n", m_id); }
	};
	
	typedef SharePtr<Object> ObjPtr;
	typedef SharePtr<DObject> DObjPtr;
	typedef SharePtr<Object2> Obj2Ptr;
	
	void operator()(void);
};

