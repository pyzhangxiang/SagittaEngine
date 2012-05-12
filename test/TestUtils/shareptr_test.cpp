/*
 *  shareptr_test.cpp
 *  Sagitta
 *
 *  Created by zhangxiang on 09-10-3.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "shareptr_test.h"

int SharePtr_Test::Object::ms_gid = 0;
int SharePtr_Test::Object2::ms_gid = 0;

void SharePtr_Test::operator()(void){
	Object *pobj1 = new Object();
	ObjPtr op1(pobj1);		// template<typename R> SharePtr(R*)
	
	ObjPtr op2 = new Object();	// template<typename R> SharePtr(R*)
	
	{	
		ObjPtr op1_2(op1);	// SharePtr(const SharePtr&)
	}
	
	Object2 *pobj21 = new Object2();
//	ObjPtr op_err(pobj21); // error template<typename R> SharePtr(R*)
	Obj2Ptr op21(pobj21);
//	ObjPtr op_err2(op21); // error template<typename R> SharePtr(const SharePtr<R>&)
	
	DObject *pdobj1 = new DObject();
	ObjPtr op3(pdobj1);		// template<typename R> SharePtr(R*)
	
	DObjPtr dop1 = new DObject();
	ObjPtr op4(dop1);		// template<typename R> SharePtr(const SharePtr<R>&)
	
	{
		ObjPtr op5 = new Object();		
		op5 = new DObject();	// template<typename R> SharePtr &operator=(R*)
	}
	
	op2 = dop1;	// tamplate<typename R> SharePtr &operator=(const SharePtr<R>&)
	op1 = op3;	// SharePtr &operator=(const SharePtr&)
	
	{
		ObjPtr op7;	// SharePtr(void)
//		if(!op7)	// bool operator!(void)
//			cout << "null pointer";
		op7 = new Object();
		op7->print();
		(*op7).print();
		cout << "op7 count: " << op7.count() << "\n";
		
		ObjPtr op8(op7);
		if(op7 == op8)
			cout << "right op7 == op8\n";
		if(op7 == op2)
			cout << "wrong op7 == op2\n";
		
		if(op7 != op8)
			cout << "wrong op7 != op8\n";
		if(op7 != op2)
			cout << "right op7 != op2\n";
	}
	
//	Object *pobj2 = new Object();
}