//////////////////////////////////////////////////////
// file: sgMesh.cpp 
// created by zhangxiang on 09-10-4
// defines of the class sgMesh
// sgMesh is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgMesh.h"
#include "engine/common/sgException.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/common/sgStringUtil.h"
#include "math/sgVector2.h"
#include "math/sgVector3.h"
#include "math/sgPlane.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta
{
    
    
    SG_META_DEFINE(sgMesh, sgResource)

    //  [1/4/2009 zhangxiang]
	sgMesh::sgMesh(void)
	: sgResource(), m_bGeometryPrepared(false),
	m_iPolyType(0), m_iPolyNum(0),
	m_iVertexNum(0), m_bSmooth(true), m_bNormalOuter(true),
	m_bCounterClockWise(true), m_pVertexData(0),
    m_pIndexData(0), m_pFaceNormalBuffer(0)
    {

    }

	void sgMesh::reset(uInt aPolyType,
		size_t aVertexNum,
		size_t aPolyNum,
		bool aSmooth/* = true*/,
		bool aCounterClockWise/* = true*/)
	{
        release();
        
		m_bGeometryPrepared = false;
		m_iPolyType = aPolyType;
		m_iPolyNum = aPolyNum;
		m_iVertexNum = aVertexNum;
		m_bSmooth = aSmooth;
		m_bNormalOuter = true;
		m_bCounterClockWise = aCounterClockWise;

		m_pFaceNormalBuffer = 0;
		m_pVertexData = new sgVertexData(m_iVertexNum);
		m_pIndexData = new sgIndexData(m_iPolyType, m_iPolyNum);
	}
    
	//  [1/4/2009 zhangxiang]
	//sgMesh::sgMesh(uInt aPolyType,
 //                      size_t aVertexNum,
 //                      size_t aPolyNum,
 //                      bool aSmooth /* = true */,
 //                      bool aCounterClockWise /* = true*/)
	//: sgResource(), m_bGeometryPrepared(false),
	//m_iPolyType(aPolyType), m_iPolyNum(aPolyNum),
	//m_iVertexNum(aVertexNum), m_bSmooth(aSmooth), m_bNormalOuter(true),
	//m_bCounterClockWise(aCounterClockWise)/*, m_pFaceNormalBuffer(0)*/{
	//	if(m_iPolyType < 0 || m_iPolyNum < 0 || m_iVertexNum < 0){
	//		THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
 //                             "Negative entity parameters.",
 //                             "sgMesh::sgMesh");
	//	}
 //       
	//	StdString name = "Entity" + sgStringUtil::to_string(sgMesh::ms_iEntityCount++);
	//	sgSceneObject::_setName(name);
 //       
	//	m_pVertexData = new sgVertexData(m_iVertexNum);
	//	m_pIndexData = new sgIndexData(m_iPolyType, m_iPolyNum);
	//}
    
	//  [1/4/2009 zhangxiang]
	//sgMesh::sgMesh(const StdString& aName, 
 //                      uInt aPolyType,
 //                      size_t aVertexNum,
 //                      size_t aPolyNum,
 //                      bool aSmooth /* = true */,
 //                      bool aCounterClockWise/* = true*/)
	//: sgMesh(aName), m_bGeometryPrepared(false),
	//m_iPolyType(aPolyType), m_iPolyNum(aPolyNum),
	//m_iVertexNum(aVertexNum), m_bSmooth(aSmooth), m_bNormalOuter(true),
	//m_bCounterClockWise(aCounterClockWise)/*, m_pFaceNormalBuffer(0)*/{
	//	if(m_iPolyType < 0 || m_iPolyNum < 0 || m_iVertexNum < 0){
	//		THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
 //                             "Negative entity parameters.",
 //                             "sgMesh::sgMesh");
	//	}
 //       
	//	sgMesh::ms_iEntityCount++;
 //       
	//	m_pVertexData = new sgVertexData(m_iVertexNum);
	//	m_pIndexData = new sgIndexData(m_iPolyType, m_iPolyNum);
	//}
    
	sgMesh::~sgMesh()
    {
		release();
        
	}
    
    void sgMesh::release(void)
    {
        if(m_pVertexData){
			delete m_pVertexData;
            m_pVertexData = 0;
		}
		if(m_pIndexData){
			delete m_pIndexData;
            m_pIndexData = 0;
		}
		if(m_pFaceNormalBuffer){
			delete m_pFaceNormalBuffer;
            m_pFaceNormalBuffer = 0;
		}
    }
    
	bool sgMesh::geometryPrepared(void) const{
		return m_bGeometryPrepared;
	}
    
	//  [1/6/2009 zhangxiang]
	void sgMesh::prepareGeometry(void){
		if(!m_bGeometryPrepared){
			calCenterAndRadius();
		}
	}
    
	//  [1/4/2009 zhangxiang]
	uInt sgMesh::polyType(void) const{
		return m_iPolyType;
	}
    
	//  [1/4/2009 zhangxiang]
	size_t sgMesh::polyNum() const{
		return m_iPolyNum;
	}
    
	//  [1/4/2009 zhangxiang]
	bool sgMesh::isSmooth(void) const{
		return m_bSmooth;
	}
    
	void sgMesh::setSmooth(bool aSmooth){
		m_bSmooth = aSmooth;
	}
    
	bool sgMesh::normalOuter(void) const{
		return m_bNormalOuter;
	}
    
	void sgMesh::setNormalOuter(bool aNormalOuter){
		m_bNormalOuter = aNormalOuter;
	}
    
	//  [1/5/2009 zhangxiang]
	void sgMesh::setupNormals(void){
		if(!m_pVertexData || !m_pIndexData){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::setupNormals");
		}
        
		sgVertexBufferElement *vBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_VERTEX);
		sgVertexIndexBuffer *vIndex = m_pIndexData->getElement(sgVertexBufferElement::ET_VERTEX);
		if(!vBuffer || !vIndex){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::setupNormals");
		}
        
		if(vBuffer->vertexNum() != m_iVertexNum || vIndex->polyNum() != m_iPolyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity with difference between vertex num or polygon num and their expression.",
                              "sgMesh::setupNormals");
		}
        
		if(m_iPolyType != 3){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Not a triangle based entity.",
                              "sgMesh::setupNormals");
		}
        
		// we don't need vertex normal index buffer any more
		// for smooth entity, vertex normal indices is the same as vertex norma indices
		// and for flat entity, we have no vertex normal buffer(face normal is enough)
		sgVertexIndexBuffer *vnIndex = m_pIndexData->getElement(sgVertexBufferElement::ET_NORMAL);
		if(vnIndex){
			vnIndex->resize(0);
		}
        
		// face normal buffer, we always need it
		if(!m_pFaceNormalBuffer){
			m_pFaceNormalBuffer = new sgBuffer(m_iPolyNum * sizeof(Vector3));
		}else{
			m_pFaceNormalBuffer->resize(m_iPolyNum * sizeof(Vector3));
		}
        
		// buffer data
		Vector3 *vBufferData = static_cast<Vector3*>(vBuffer->data());
		IndexedTriangle *vIndexData = static_cast<IndexedTriangle*>(vIndex->data());
		Vector3 *fnBufferData = static_cast<Vector3*>(m_pFaceNormalBuffer->data());
		Vector3 *vnBufferData;
        
		sgVertexBufferElement *vnBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_NORMAL);
		if(m_bSmooth){
			// for smooth entity, we need vertex normal buffer
			// and the number of vertex normals is the same as vertices
			size_t normalNum = m_iVertexNum;
			if(!vnBuffer){
				// have no vertex normal buffer, create it
				vnBuffer = m_pVertexData->createElement(sgVertexBufferElement::ET_NORMAL,
                                                        3, normalNum);
			}else{
				if(vnBuffer->vertexNum() != normalNum){
					// different vertex, resize it
					vnBuffer->resize(normalNum);
				}
			}
            
			vnBufferData = static_cast<Vector3*>(vnBuffer->data());
			// for vertex normal buffer, initialize 0
			/*for(size_t i=0; i<normalNum; ++i){
             vnBufferData[i] = Vector3::ZERO;
             }*/
			memset(static_cast<void*>(vnBufferData), 0, sizeof(Vector3) * normalNum);
            
		}else{
			// for flat entity, we don't need to store vertex normal
			if(vnBuffer){
				vnBuffer->resize(0);
			}
			
		}
        
        
		Vector3 btoa, btoc, t;
		// calculate normal of each face
		if(m_bSmooth){
			// for smooth entity, the number of vertex normals is the same as vertices
			// and the vertex normal indices is the same as vertex indices
			for(size_t i=0; i<m_iPolyNum; ++i){
				// determine vectors parallel to two edges of face
				btoa = vBufferData[vIndexData[i].a] - vBufferData[vIndexData[i].b];
				btoc = vBufferData[vIndexData[i].c] - vBufferData[vIndexData[i].b];
                
				// find cross-product between these vectors
				if(!(m_bCounterClockWise ^ m_bNormalOuter))			// to the front face
					t = btoc.crossProduct(btoa);
				else												// to the back face
					t = btoa.crossProduct(btoc);
                
				// normalize the vector
				t.normalise();
				
				fnBufferData[i] = t;
                
				// add this face normal to each vertex that is adjacent to face
				for(int j=0; j<3; ++j){
					// normal index is the same as vertex index
					size_t vi = vIndexData[i][j];
					vnBufferData[vi] += t;
				}
                
			} //#### end for face
            
			// normalize all the normals at the vertices
			for(size_t i=0; i<m_iVertexNum; ++i){
				vnBufferData[i].normalise();
			}
            
		}else{
			// flat entity, needn't store vertex normal
			for(size_t i=0; i<m_iPolyNum; ++i){
				// determine vectors parallel to two edges of face
				btoa = vBufferData[vIndexData[i].a] - vBufferData[vIndexData[i].b];
				btoc = vBufferData[vIndexData[i].c] - vBufferData[vIndexData[i].b];
                
				// find cross-product between these vectors
				if(!(m_bCounterClockWise ^ m_bNormalOuter))			// to the front face
					t = btoc.crossProduct(btoa);
				else												// to the back face
					t = btoa.crossProduct(btoc);
                
				// normalize the vector
				t.normalise();
                
				fnBufferData[i] = t;
                
			} //#### end for face
		}
        
	} //#### end computeNormal
    
	//  [1/5/2009 zhangxiang]
	void sgMesh::setupNormals(bool aNormalOuter){
		m_bNormalOuter = aNormalOuter;
		setupNormals();
	} //#### end computeNormal
    
	/** for future ...
     int sgMesh::computeEdgeNormal(void) const{
     if(m_pVertexList->empty())
     return -1;
     
     if(m_iPolygonType != 3)
     return 0;
     
     if(m_pFaceNormalList->empty())
     return -2;
     
     m_pEdgeNormalList->clear();
     m_pEdgeNormalList->resize(m_iVertexNum);
     
     Vector3 atob, btoc, ctoa;
     if(!(m_bCounterClockWise ^ m_bNormalOuter)){
     for(unsigned int i=0; i<m_iPolygonNum; i++){	// edge vector cross face normal
     // determine vectors parallel to two edges of face
     atob = m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(1)) - m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(0));
     btoc = m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(2)) - m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(1));
     ctoa = m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(0)) - m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(2));
     
     m_pEdgeNormalList->at(m_pIndexedPolygonList->at(i).vertexAt(0)) = atob.crossProduct(m_pFaceNormalList->at(i)).normalise();	// edge AB
     m_pEdgeNormalList->at(m_pIndexedPolygonList->at(i).vertexAt(1)) = btoc.crossProduct(m_pFaceNormalList->at(i)).normalise();	// edge BC
     m_pEdgeNormalList->at(m_pIndexedPolygonList->at(i).vertexAt(2)) = ctoa.crossProduct(m_pFaceNormalList->at(i)).normalise();	// edge CA
     
     } //#### end for face
     
     }else{	// face normal cross edge vector
     for(unsigned int i=0; i<m_iPolygonNum; i++){	// edge vector cross face normal
     // determine vectors parallel to two edges of face
     atob = m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(1)) - m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(0));
     btoc = m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(2)) - m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(1));
     ctoa = m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(0)) - m_pVertexList->at(m_pIndexedPolygonList->at(i).vertexAt(2));
     
     m_pEdgeNormalList->at(m_pIndexedPolygonList->at(i).vertexAt(0)) = m_pFaceNormalList->at(i).crossProduct(atob).normalise();	// edge AB
     m_pEdgeNormalList->at(m_pIndexedPolygonList->at(i).vertexAt(1)) = m_pFaceNormalList->at(i).crossProduct(btoc).normalise();	// edge BC
     m_pEdgeNormalList->at(m_pIndexedPolygonList->at(i).vertexAt(2)) = m_pFaceNormalList->at(i).crossProduct(ctoa).normalise();	// edge CA
     
     } //#### end for face
     }
     
     return 1;
     } //#### end computeEdgeNormal
     */
    
	//  [7/31/2008 zhangxiang]
	//  [1/5/2009 zhangxiang]
	void sgMesh::trianglate(void){
		if(m_iPolyType <= 3)
			return ;
        
		if(!m_pVertexData || !m_pIndexData){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::trianglate");
		}
        
		sgVertexBufferElement *vBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_VERTEX);
		sgVertexIndexBuffer *vIndex = m_pIndexData->getElement(sgVertexBufferElement::ET_VERTEX);
		if(!vBuffer || !vIndex){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::trianglate");
		}
        
		if(vBuffer->vertexNum() != m_iVertexNum || vIndex->polyNum() != m_iPolyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity with difference between vertex num or polygon num and their expression.",
                              "sgMesh::trianglate");
		}
        
		// store original values
		uInt oriPolyType = m_iPolyType;
		size_t oriPolyNum = m_iPolyNum;
        
		// store original index datas
		size_t elemNum = m_pIndexData->elementNum();
		std::vector<int> oriIndexTypeList(elemNum);
		std::vector< size_t* > oriIndexDataList(elemNum);
        
		sgIndexData::ConstIterator it = m_pIndexData->getConstIterator();
		for(int i=0; it.hasMoreElements(); ++it, ++i){
			sgVertexIndexBuffer *indexBuffer = it.value();
			oriIndexTypeList[i] = indexBuffer->type();
			// we will free this memories
			oriIndexDataList[i] = new size_t[indexBuffer->dataNum()];
			indexBuffer->readData(0, indexBuffer->getSizeInBytes(), static_cast<void*>(oriIndexDataList[i]));
		}
        
        
		size_t lastVertexInOnePoly = oriPolyType - 1;
		size_t prelastVertexInOnePoly = oriPolyType - 2;
		// because we don't know how many faces after trianglate,
		// so use a std::vector to store the data temporarily
		std::vector< std::vector<IndexedTriangle> > newIndexDataList(elemNum);
		
		// trianglate
		for(size_t el=0; el<elemNum; ++el){
			size_t *oriIndexData = oriIndexDataList[el];
			for(size_t i=0; i<oriPolyNum; ++i){
				size_t oriOffset = i * oriPolyType;
				size_t lastVertexIndex = oriIndexData[oriOffset + lastVertexInOnePoly];
				for(size_t j=0; j<prelastVertexInOnePoly; ++j){
					size_t a = oriIndexData[oriOffset + j];
					size_t b = oriIndexData[oriOffset + j + 1];
                    
					// Sometimes this case is possible
					// see sgSphere
					if(a == b || a == lastVertexIndex || b == lastVertexIndex)
						continue;
                    
                    //					newFace = i * prelastVertexInOnePoly + j;
					newIndexDataList[el].push_back(IndexedTriangle(a, b, lastVertexIndex));
				}
			}
            
			// free original data
			delete oriIndexDataList[el];
		}
        
        
		// now we know how many triangles generated
		m_iPolyType = 3;
		m_iPolyNum = newIndexDataList[0].size(); //(oriPolyType - 2) * oriPolyNum;
        
		// have to clear original index data first
		m_pIndexData->clear();
		m_pIndexData->setPolyType(m_iPolyType);
		m_pIndexData->setPolyNum(m_iPolyNum);
        
		// copy data from std::vector
		for(size_t el=0; el<elemNum; ++el){
			IndexedTriangle *finalIndexData = static_cast<IndexedTriangle*>(m_pIndexData->createElement(oriIndexTypeList[el])->data());
			for(size_t i=0; i<m_iPolyNum; ++i){
				finalIndexData[i] = newIndexDataList[el][i];
			}
		}
        
		//m_RenderOption.setRenderElementType(sgRenderState::RET_TRIANGLES);
        
		setupNormals();
		// for future ...
        //	computeEdgeNormals();
        
	}
    
	//  [1/5/2009 zhangxiang]
	void sgMesh::calCenterAndRadius(void){
		if(!m_pVertexData || !m_pIndexData){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::calCenterAndRadius");
		}
        
		sgVertexBufferElement *vBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_VERTEX);
		sgVertexIndexBuffer *vIndex = m_pIndexData->getElement(sgVertexBufferElement::ET_VERTEX);
		if(!vBuffer || !vIndex){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::calCenterAndRadius");
		}
        
		if(vBuffer->vertexNum() != m_iVertexNum || vIndex->polyNum() != m_iPolyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity with difference between vertex num or polygon num and their expression.",
                              "sgMesh::calCenterAndRadius");
		}
        
		Vector3 *vBufferData = static_cast<Vector3*>(vBuffer->data());
        
		// calculate center position
		m_Center = Vector3::ZERO;
		for(size_t i=0; i<m_iVertexNum; ++i){
			m_Center += vBufferData[i];
		}
        
		m_Center /= m_iVertexNum;
        
		// calculate radius
		m_fAverageRadius = m_fMaxRadius = 0.0;
		for(size_t i=0; i<m_iVertexNum; ++i){
			Real distance = m_Center.distance(vBufferData[i]);
            
			m_fAverageRadius += distance;
			if(m_fMaxRadius < distance)
				m_fMaxRadius = distance;
		}
        
		m_fAverageRadius /= m_iVertexNum;
        
		m_bGeometryPrepared = true;
	}
    
	//  [1/6/2009 zhangxiang]
	void sgMesh::locateToCenter(void){
		if(!m_pVertexData || !m_pIndexData){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::locateToCenter");
		}
        
		sgVertexBufferElement *vBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_VERTEX);
		sgVertexIndexBuffer *vIndex = m_pIndexData->getElement(sgVertexBufferElement::ET_VERTEX);
		if(!vBuffer || !vIndex){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::locateToCenter");
		}
        
		if(vBuffer->vertexNum() != m_iVertexNum || vIndex->polyNum() != m_iPolyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity with difference between vertex num or polygon num and their expression.",
                              "sgMesh::locateToCenter");
		}
        
		Vector3 *vBufferData = static_cast<Vector3*>(vBuffer->data());
        
		if(!m_bGeometryPrepared)
			calCenterAndRadius();
        
		for(size_t i=0; i<m_iVertexNum; ++i){
			vBufferData[i] -= m_Center;
		}
        
		m_Center = Vector3::ZERO;
	}
    
	//  [1/5/2009 zhangxiang]
	sgVertexData *sgMesh::getVertexData(void) const{
		return m_pVertexData;
	}
    
	//  [1/5/2009 zhangxiang]
	sgIndexData *sgMesh::getIndexData(void) const{
		return m_pIndexData;
	}
    
    //  [6/10/2012 zhangxiang]
    sgVertexIndexBuffer *sgMesh::getVertexIndexBuffer(void) const
    {
        if(!m_pIndexData)
            return 0;
        return m_pIndexData->getElement(sgVertexBufferElement::ET_VERTEX);
        
    }
    
	//  [8/1/2008 zhangxiang]
	//  [1/5/2009 zhangxiang]
	void sgMesh::getVertexBuffer(sgVertexData *outVBuffer, sgVertexIndexBuffer *outIBuffer) const{
		if(!m_pVertexData || !m_pIndexData){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::getVertexBuffer");
		}
        
		sgVertexBufferElement *vBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_VERTEX);
		sgVertexIndexBuffer *vIndex = m_pIndexData->getElement(sgVertexBufferElement::ET_VERTEX);
		if(!vBuffer || !vIndex){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity without vertex data.",
                              "sgMesh::getVertexBuffer");
		}
        
		if(vBuffer->vertexNum() != m_iVertexNum || vIndex->polyNum() != m_iPolyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
                              "Invalid entity with difference between vertex num or polygon num and their expression.",
                              "sgMesh::getVertexBuffer");
		}
        
		if(!outVBuffer){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
                              "Null vertex buffer pointer for destinate.",
                              "sgMesh::getVertexBuffer");
		}
        
		if(!outIBuffer){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
                              "Null vertex index buffer pointer for destinate.",
                              "sgMesh::getVertexBuffer");
		}
        
		outVBuffer->clear();
		size_t outVertexNum;
		if(m_bSmooth){
			outVertexNum = m_iVertexNum;
		}else{
			outVertexNum = m_iPolyType * m_iPolyNum;
		}
		outVBuffer->setVertexNum(outVertexNum);
        
		if(outIBuffer->polyType() != m_iPolyType || outIBuffer->polyNum() != m_iPolyNum){
			outIBuffer->resize(m_iPolyType, m_iPolyNum);
		}
        
		// original buffer data
		Vector3 *vBufferData = static_cast<Vector3*>(vBuffer->data());
		size_t *vIndexData = static_cast<size_t*>(vIndex->data());
        //		IndexedPolygon<m_iPolyType> *vIndexData = static_cast<IndexedPolygon<m_iPolyType>*>(vIndex->data());
		
		// vertices
		Vector3 *outvBufferData = static_cast<Vector3*>(outVBuffer->createElement(sgVertexBufferElement::ET_VERTEX, 3, outVertexNum)->data());
		size_t *outIBufferData = static_cast<size_t*>(outIBuffer->data());
        //		IndexedPolygon<m_iPolyType> *outIBufferData = static_cast<IndexedPolygon<m_iPolyType>*>(outIBuffer->data());
		if(m_bSmooth){
			// for smooth entity, the vertex buffer for rendering is the same as the original one
			vBuffer->readData(0, vBuffer->getSizeInBytes(), static_cast<void*>(outvBufferData));
            
			if(outIBuffer){
				// the index buffer for rendering is the same as the original one too
				vIndex->readData(0, vIndex->getSizeInBytes(), static_cast<void*>(outIBufferData));
			}
		}else{
			// for flat entity, have to store duplicate vertices
			size_t vi = 0;
			for(size_t i=0; i<m_iPolyNum; ++i){
				for(size_t j=0; j<m_iPolyType; ++j){
					vi = i * m_iPolyType + j;
                    //		outvBufferData[i * m_iPolyType + j] = vBufferData[vIndexData[i][j]];
					outvBufferData[vi] = vBufferData[vIndexData[vi]];
					outIBufferData[vi] = vi;
                    //		outIBufferData[i][j] = vi;
				}
			}
		}
        
		// colors
		sgVertexBufferElement *cBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_COLOR);
		if(cBuffer){
			Color *cBufferData = static_cast<Color*>(cBuffer->data());
			Color *outcBufferData = static_cast<Color*>(outVBuffer->createElement(sgVertexBufferElement::ET_COLOR, 4, outVertexNum)->data());
			if(m_bSmooth){
				// for smooth entity, the color buffer for rendering is the same as the original one
				cBuffer->readData(0, cBuffer->getSizeInBytes(), static_cast<void*>(outcBufferData));
			}else{
				// for flat entity, have to store duplicate colors
				size_t vi = 0;
				for(size_t i=0; i<m_iPolyNum; ++i){
					for(size_t j=0; j<m_iPolyType; ++j){
						vi = i * m_iPolyType + j;
						outcBufferData[vi] = cBufferData[vIndexData[vi]];
                        //		outcBufferData[i * m_iPolyType + j] = cBufferData[vIndexData[i][j]];
					}
				}
			}
		}
        
		// normals
		sgVertexBufferElement *nBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_NORMAL);
		if(m_pFaceNormalBuffer){
			Vector3 *outnBufferData = static_cast<Vector3*>(outVBuffer->createElement(sgVertexBufferElement::ET_NORMAL, 3, outVertexNum)->data());
			if(m_bSmooth){
				// for smooth entity, the normal buffer for rendering is the same as the original one
				nBuffer->readData(0, nBuffer->getSizeInBytes(), static_cast<void*>(outnBufferData));
			}else{
				// for flat entity, have to store duplicate normals
				Vector3 *fnBufferData = static_cast<Vector3*>(m_pFaceNormalBuffer->data());
				for(size_t i=0; i<m_iPolyNum; ++i){
					for(size_t j=0; j<m_iPolyType; ++j){
						outnBufferData[i * m_iPolyType + j] = fnBufferData[i];
					}
				}
			}
		}
        
		// texture coordinates
		sgVertexBufferElement *tBuffer = m_pVertexData->getElement(sgVertexBufferElement::ET_TEXTURE_COORD);
		sgVertexIndexBuffer *tIndex = m_pIndexData->getElement(sgVertexBufferElement::ET_TEXTURE_COORD);
		if(tBuffer){
			if(m_bSmooth){
				Vector2 *outtBufferData = static_cast<Vector2*>(outVBuffer->createElement(sgVertexBufferElement::ET_TEXTURE_COORD, 2, outVertexNum)->data());
				// for smooth entity, the texture coordinate buffer for rendering is the same as the original one
				tBuffer->readData(0, tBuffer->getSizeInBytes(), static_cast<void*>(outtBufferData));
			}else{
				if(tIndex){
					Vector2 *tBufferData = static_cast<Vector2*>(tBuffer->data());
					size_t *tIndexData = static_cast<size_t*>(tIndex->data());
                    //		IndexedPolygon<m_iPolyType> *tIndexData = static_cast<IndexedPolygon<m_iPolyType>*>(tIndex->data());
					Vector2 *outtBufferData = static_cast<Vector2*>(outVBuffer->createElement(sgVertexBufferElement::ET_TEXTURE_COORD, 2, outVertexNum)->data());
					// for flat entity, have to store duplicate texture coordinates
					size_t vi = 0;
					for(size_t i=0; i<m_iPolyNum; ++i){
						for(size_t j=0; j<m_iPolyType; ++j){
							vi = i * m_iPolyType + 1;
							outtBufferData[vi] = tBufferData[tIndexData[vi]];
                            //	outtBufferData[i * m_iPolyType + j] = tBufferData[tIndexData[i][j]];
						}
					}
				}
			}
		}
        
		// fog coordinate
		// for future ...
        
	} //#### end getVertexBuffer
    
	sgBuffer *sgMesh::getFaceNormalBuffer(void) const{
		return m_pFaceNormalBuffer;
	}
    
    
	//  [8/1/2008 zhangxiang]
	//const sgRenderState &sgMesh::renderOption(void) const{
	//	return m_RenderOption;
	//}
 //   
	////  [1/5/2009 zhangxiang]
	//sgRenderState &sgMesh::renderOption(void){
	//	return m_RenderOption;
	//}
 //   
	//void sgMesh::setRenderOption(const sgRenderState &arROP){
	//	m_RenderOption = arROP;
	//}
 //   
	////  [8/11/2008 zhangxiang]
	//const sgMaterial &sgMesh::material(void) const{
	//	return m_Material;
	//}
 //   
	////  [8/11/2008 zhangxiang]
	//void sgMesh::setMaterial(const sgMaterial &aMaterial){
	//	m_Material = aMaterial;
	//}
 //   
	////  [8/8/2008 zhangxiang]
	//bool sgMesh::isVisible(void) const{
	//	return m_bVisible && isActive();
	//}
 //   
	////  [8/8/2008 zhangxiang]
	//void sgMesh::setVisible(bool abVisible){
	//	m_bVisible = abVisible;
	//}
    
	//  [8/19/2008 zhangxiang]
	const Vector3 &sgMesh::center(void) const{
		return m_Center;
	}
    
	//  [8/21/2008 zhangxiang]
	Real sgMesh::avgRadius(void) const{
		return m_fAverageRadius;
	}
    
	//  [8/21/2008 zhangxiang]
	Real sgMesh::maxRadius(void) const{
		return m_fMaxRadius;
	}
    
    void sgMesh::load(SERIALIZE_LOAD_ARCHIVE &archive)
    {
        sgResource::load(archive);
        archive & SG_MAKE_NVP("m_iPolyType", m_iPolyType);
        archive & SG_MAKE_NVP("m_iPolyNum", m_iPolyNum);
        archive & SG_MAKE_NVP("m_iVertexNum", m_iVertexNum);
        archive & SG_MAKE_NVP("m_bSmooth", m_bSmooth);
        archive & SG_MAKE_NVP("m_bNormalOuter", m_bNormalOuter);
        archive & SG_MAKE_NVP("m_bCounterClockWise", m_bCounterClockWise);
    }
    
    void sgMesh::save(SERIALIZE_SAVE_ARCHIVE &archive)
    {
        sgResource::save(archive);
        archive & SG_MAKE_NVP("m_iPolyType", m_iPolyType);
        archive & SG_MAKE_NVP("m_iPolyNum", m_iPolyNum);
        archive & SG_MAKE_NVP("m_iVertexNum", m_iVertexNum);
        archive & SG_MAKE_NVP("m_bSmooth", m_bSmooth);
        archive & SG_MAKE_NVP("m_bNormalOuter", m_bNormalOuter);
        archive & SG_MAKE_NVP("m_bCounterClockWise", m_bCounterClockWise);
    }
    
	//  [1/5/2009 zhangxiang]
	/*Matrix4 sgMesh::getModelMatrix(void) const{
		if(isAttached())
			return attachedNode()->getFullTransform();
		else
			return Matrix4::IDENTITY;
	}*/
	// sgMesh //////////////////////////////////////////////////////////////////////////
    
    
    
	
} // namespace Sagitta