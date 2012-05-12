//////////////////////////////////////////////////////
// file: sgAnimationDelegate.cpp 
// created by zhangxiang on 09-01-10
// define of the class sgAnimationDelegate
// sgAnimationDelegate is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgAnimationDelegate.h"
#include "../animation/sgAnimation.h"
#include "../animation/sgMotionClip.h"
#include "../animation/sgAnimaJoint.h"
#include "../io/sgFile.h"
#include "../../common/utils/sgException.h"
#include <vector>

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// sgAnimationDelegate //////////////////////////////////////////////////////////////////////////

	//  [1/10/2009 zhangxiang]
	void sgAnimationDelegate::loadImpl(const StdString &aFileName, sgResource *outResource){
		THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
			"Normal Animation resource loading is not implemented",
			"sgAnimationDelegate::loadImpl");
	}

	// sgAnimationDelegate //////////////////////////////////////////////////////////////////////////


	// sgMotionClipDelegate //////////////////////////////////////////////////////////////////////////

	//  [1/11/2009 zhangxiang]
	void sgMotionClipDelegate::loadImpl(const StdString &aFileName, sgResource *outResource){
		// test file type
		size_t lastslash = aFileName.find_last_of("\\/");
		size_t lastdot = aFileName.find_last_of(".");

		if(lastdot != StdString::npos){
			StdString postfix = aFileName.substr(lastdot);
			if(postfix == ".bvh"){
				// .obj file
				loadMotionClip_bvh(aFileName, static_cast<sgMotionClip*>(outResource));
			}else{
				THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
					postfix + " is not a supported file type.",
					"sgMotionClipDelegate::loadImpl");
			}
		}else{
			THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
				"File type without postfix is supported.",
				"sgMotionClipDelegate::loadImpl");
		}
	}

	//  [1/11/2009 zhangxiang]
	void sgMotionClipDelegate::loadMotionClip_bvh(const StdString &aFileName, sgMotionClip *outMotionClip){
		sgFile file(aFileName);
		if(!file){
			THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR,
				"Cannot open file \"" + aFileName + "\".",
				"sgMotionClipDelegate::loadMotionClip_bvh");
		}

		// record joint name
		std::vector<StdString> names;
		names.reserve(24);	// assum 24joints

		// record dof type
		// 0 - Xposition, 1 - Yposition, 2 - Zposition
		// 3 - Xrotation, 4 - Yrotation, 5 - Zrotation
		std::vector<int> doftype;
		doftype.reserve(72);	// 3dof * 24joint

		size_t dofnum = 0;
		size_t currdof = 0;
		std::vector<size_t> jointdof;
		jointdof.reserve(24);	// assum 24joints
		StdString xrot, yrot, zrot;
		StdString str = "";
		while(file >> str){
			if(str == "ROOT" || str == "JOINT"){
				file >> str;
				names.push_back(str);
			}else if(str == "CHANNELS"){
				file >> currdof;
				jointdof.push_back(currdof);
				dofnum += currdof;
			}else if(str == "Xposition"){
				doftype.push_back(0);
			}else if(str == "Yposition"){
				doftype.push_back(1);
			}else if(str == "Zposition"){
				doftype.push_back(2);
			}else if(str == "Xrotation"){
				doftype.push_back(3);
			}else if(str == "Yrotation"){
				doftype.push_back(4);
			}else if(str == "Zrotation"){
				doftype.push_back(5);
			}else if(str == "MOTION"){
				// next datas
				break;
			}
		}

		// frame num
		size_t framenum;
		file >> str >> framenum;

		// frame time
		Real frametime;
		file >> str >> str >> frametime;
		outMotionClip->setFrameTime(frametime);

		// create joints
		size_t jointnum = names.size();
		std::vector<sgAnimaJoint*> joints(jointnum);
		for(size_t i=0; i<jointnum; ++i){
			joints[i] = outMotionClip->createAnimaJoint(names[i], framenum);
		}

		// data
		Real d[3];
		Vector3 data;
		Matrix3 rotm;
		Quaternion q;
		uByte orderrecord = 0;
		uByte orderresult = 0;
		size_t typei = 0;
		for(size_t fr=0; fr<framenum; ++fr){
			typei = 0;
			for(size_t jn=0; jn<jointnum; ++jn){
				for(size_t dofn=0; dofn<jointdof[jn];){
					file >> d[0] >> d[1] >> d[2];
					dofn += 3;
					orderrecord = 0;
					for(size_t i=0; i<3; ++i, ++typei){
						switch(doftype[typei]){
							case 0:
							{
								joints[jn]->setPositionActive(true);
								data.setX(d[i]);
								break;
							}

							case 1:
							{
								joints[jn]->setPositionActive(true);
								data.setY(d[i]);
								break;
							}

							case 2:
							{
								joints[jn]->setPositionActive(true);
								data.setZ(d[i]);
								break;
							}

							/* for rotation:
								x makes lower bit not, e.t.		00 --> 01; 01 --> 00;
								y makes higher bit not, e.t.	00 --> 10; 01 --> 11;
								z makes two bits not, e.t.		00 --> 11; 01 --> 10;
								let initial bits is 00, so the result are:
								x-y-z	011100
								x-z-y	011000
								y-x-z	101100
								y-z-x	100100
								z-x-y	111000
								z-y-x	110100
							*/
							case 3:	// x
							{
								// order
								orderrecord ^= 0x01;
								orderresult <<= 2;
								orderresult += orderrecord;

								joints[jn]->setOrientationActive(true);
								data.setX(d[i]);
								break;
							}

							case 4:	// y
							{
								// order
								orderrecord ^= 0x02;
								orderresult <<= 2;
								orderresult += orderrecord;

								joints[jn]->setOrientationActive(true);
								data.setY(d[i]);
								break;
							}

							case 5: // z
							{
								// order
								orderrecord ^= 0x03;
								orderresult <<= 2;
								orderresult = orderrecord;

								joints[jn]->setOrientationActive(true);
								data.setZ(d[i]);
								break;
							}
						} // #### end switch type
					} //#### end for type
					
					switch(doftype[typei-1]){
						case 0: case 1: case 2:
						{
							joints[jn]->setPosition(data, fr);
							break;
						}

						case 3: case 4: case 5:
						{
							
							switch(orderresult){
								case 31: // x-y-z 011100
								{
									rotm.fromEulerAnglesXYZ(data.x() * Math::DEG_TO_RAD,
															data.y() * Math::DEG_TO_RAD,
															data.z() * Math::DEG_TO_RAD);	// params: y-x-z
									break;
								}

								case 24: // x-z-y 011000
								{
									rotm.fromEulerAnglesXZY(data.x() * Math::DEG_TO_RAD,
															data.z() * Math::DEG_TO_RAD,
															data.y() * Math::DEG_TO_RAD);	// params: y-x-z
									break;
								}

								case 44: // y-x-z 101100
								{
									rotm.fromEulerAnglesYXZ(data.y() * Math::DEG_TO_RAD,
															data.x() * Math::DEG_TO_RAD,
															data.z() * Math::DEG_TO_RAD);	// params: y-x-z
									break;
								}

								case 36: // y-z-x 100100
								{
									rotm.fromEulerAnglesYZX(data.y() * Math::DEG_TO_RAD,
															data.z() * Math::DEG_TO_RAD,
															data.x() * Math::DEG_TO_RAD);	// params: y-x-z
									break;
								}

								case 56: // z-x-y 111000
								{
									rotm.fromEulerAnglesZXY(data.z() * Math::DEG_TO_RAD,
															data.x() * Math::DEG_TO_RAD,
															data.y() * Math::DEG_TO_RAD);	// params: y-x-z
									break;
								}

								case 52: // z-y-x 110100
								{
									rotm.fromEulerAnglesZYX(data.z() * Math::DEG_TO_RAD,
															data.y() * Math::DEG_TO_RAD,
															data.x() * Math::DEG_TO_RAD);	// params: y-x-z
									break;
								}
							} //#### end switch order
							q.fromRotationMatrix(rotm);
							joints[jn]->setOrientation(q, fr);
						} //#### end case rotation
					} //#### end switch type
				} //#### end for one joint dofs
			} //#### end for joints
		} //#### end for frames	
	}

	// sgMotionClipDelegate //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta