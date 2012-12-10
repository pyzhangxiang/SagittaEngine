

#include "sgLoader.h"
#include "sgResource.h"
#include "sgResourceCenter.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/scenegraph/sgBoneObject.h"
#include "engine/scenegraph/sgSkeleton.h"
#include "engine/resource/sgAnimation.h"
#include "engine/resource/sgAnimationJoint.h"
#include "engine/resource/sgSimpleMeshes.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/component/sgAnimationComponent.h"
#include "engine/component/sgMeshComponent.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgException.h"

#include <fstream>
#include <list>
#include <vector>

namespace Sagitta{

	sgSkeleton *sgLoader::load_bvh_skeleton(const std::string &filename)
    {
        std::ifstream file((sgResourceCenter::instance()->getRootDir() + filename).c_str());
        if(file.fail())
        {
            THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
                              "cannot open the bvh file: " + filename + ".",
                              "load_bvh_skeleton");
            
        }
        
        sgSkeleton *pSkeleton = (sgSkeleton*)sgObject::createObject(sgSkeleton::GetClassTypeName());
        sgBoneObject *boneRoot = pSkeleton->getBoneRoot();
        
        sg_vector(sgBoneObject*) nodeList;
        
        sg_list(sgBoneObject*) nodeStack;
        nodeStack.push_back(boneRoot);
        
        int numEndSite = 0;
        Real offsetX, offsetY, offsetZ;
        std::string str = "";
		while(file >> str){
			if(str == "ROOT" || str == "JOINT"){
                // get joint name
				file >> str;
                sgBoneObject *node = (sgBoneObject*)sgObject::createObject(sgBoneObject::GetClassTypeName());
                node->setName(str);
                // this node will belong to pSkeleton in the setParent function
                node->setParent(nodeStack.back());
                nodeStack.push_back(node);
                
                nodeList.push_back(node);
                
                // "{"
				file >> str;
                
                // "OFFSET"
				file >> str;
                
                // offset data
				file >> offsetX >> offsetY >> offsetZ;
                node->translate(Vector3(offsetX, offsetY, offsetZ));
                
			}
            else if(str == "End")
            {
                // get joint name
				file >> str;
                
                str = "EndSite_" + sgStringUtil::to_string(numEndSite++);
                sgBoneObject *node = (sgBoneObject*)sgObject::createObject(sgBoneObject::GetClassTypeName());
                node->setName(str);
                // this node will belong to pSkeleton in the setParent function
                node->setParent(nodeStack.back());
                nodeStack.push_back(node);
                
                nodeList.push_back(node);

                
                // "{"
				file >> str;
                
                // "OFFSET"
				file >> str;
                
                // offset data
				file >> offsetX >> offsetY >> offsetZ;
                node->translate(Vector3(offsetX, offsetY, offsetZ));
            }
            else if(str == "}")
            {
                nodeStack.pop_back();
            }
            else if(str == "MOTION")
            {
                break;
            }
		}
        
        sgMesh *pMeshSphere = (sgMesh*)sgResourceCenter::instance()->createResource(sgMeshSphere::GetClassTypeName(), sgMeshSphere::InternalFileName);
        sgMesh *pMeshLine = (sgMesh*)sgResourceCenter::instance()->createResource(sgMeshLine::GetClassTypeName(), sgMeshLine::InternalFileName);
        
        // setup nodes' debug object
        for(size_t i=0; i<nodeList.size(); ++i)
        {
            sgSceneObject *debugObj = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
            debugObj->setIsDebugObj(true);

            sgSceneObject *obj = nodeList[i];
            obj->setDebugObjectToShow(debugObj);
			debugObj->setName(obj->getName() + "_debug");
            
            sgMeshComponent *pSphereComp = (sgMeshComponent*)debugObj->createComponent(sgMeshComponent::GetClassTypeName());
            pSphereComp->setMeshFile(pMeshSphere->getFilename());
            
            sgNode::ChildIterator it = obj->getChildIterator();
            for(; it.hasMoreElements(); ++it)
            {
                sgNode *child = dynamic_cast<sgSceneObject*>(it.value());
                
                sgSceneObject *lineObj = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
				lineObj->setIsDebugObj(true);
                lineObj->setParent(debugObj);
                lineObj->scale(child->position() - obj->position());
                
                sgMeshComponent *pLineComp = (sgMeshComponent*)lineObj->createComponent(sgMeshComponent::GetClassTypeName());
                pLineComp->setMeshFile(pMeshLine->getFilename());
            }
        }
        
        return pSkeleton;
        
    }
    
    sgAnimation *sgLoader::load_bvh_animation(const std::string &filename)
    {
        std::ifstream file((sgResourceCenter::instance()->getRootDir() + filename).c_str());
        if(file.fail())
        {
            THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
                              "cannot open the bvh file: " + filename + ".",
                              "load_bvh_animation");
            
        }

        
		// record joint name
		sg_vector(std::string) names;
		names.reserve(24);	// assum 24joints
        
		// record dof type
		// 0 - Xposition, 1 - Yposition, 2 - Zposition
		// 3 - Xrotation, 4 - Yrotation, 5 - Zrotation
		sg_vector(int) doftype;
		doftype.reserve(72);	// 3dof * 24joint
        
        int numEndSite = 0;
		size_t dofnum = 0;
		size_t currdof = 0;
		sg_vector(size_t) jointdof;
		jointdof.reserve(24);	// assum 24joints
        std::string xrot, yrot, zrot;
        std::string str = "";
		while(file >> str){
			if(str == "ROOT" || str == "JOINT"){
				file >> str;
				names.push_back(str);
            }/*else if(str == "End"){
                str = "EndSite_" + sgStringUtil::to_string(numEndSite++);
                names.push_back(str);
				jointdof.push_back(0);
            }*/else if(str == "CHANNELS"){
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
//		outMotionClip->setFrameTime(frametime);
        
        sgAnimation *pAnimation = (sgAnimation*)sgResourceCenter::instance()->createResource(sgAnimation::GetClassTypeName(), filename.c_str());
        pAnimation->setKeyNum(framenum);
        
		// create joints
		size_t jointnum = names.size();
		sg_vector(sgAnimationJoint*) joints(jointnum);
		for(size_t i=0; i<jointnum; ++i){
			joints[i] = pAnimation->createAnimaJoint(names[i]); //outMotionClip->createAnimaJoint(names[i], framenum);
            joints[i]->reset(framenum);
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
				joints[jn]->setKeyT(fr * frametime, fr);
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
							joints[jn]->setKeyPosition(data, fr);
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
							joints[jn]->setKeyOrientation(q, fr);
						} //#### end case rotation
					} //#### end switch type
				} //#### end for one joint dofs
			} //#### end for joints
		} //#### end for frames
        
        return pAnimation;
    }
    
} // namespace Sagitta

