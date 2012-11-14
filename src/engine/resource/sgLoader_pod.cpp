
#include "sgLoader.h"
#include "sgResource.h"
#include "sgResourceCenter.h"
#include "engine/common/sgLogSystem.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/resource/sgMesh.h"
#include "engine/component/sgMeshComponent.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgUtil.h"
#include "engine/common/sgException.h"
#include <pvrt/PVRTModelPOD.h>

namespace Sagitta{
    
    sgSceneObject *sgLoader::load_pod(const std::string &filename)
	{
		std::string fullpath = sgResourceCenter::instance()->getResourcePath(filename);

		CPVRTModelPOD podScene;

		EPVRTError podErr = podScene.ReadFromFile(fullpath.c_str());
		if(podErr == PVR_FAIL)
		{
			sgLogSystem::instance()->error(std::string("sgLoader::load_pod, load pod scene failed, ") + filename);
			return NULL;
		}
		else if(podErr == PVR_OVERFLOW)
		{
			sgLogSystem::instance()->error(std::string("sgLoader::load_pod, load pod scene overflow, ") + filename);
			return NULL;
		}

		sgSceneObject *sceneRoot = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());

		// create meshes
		for(size_t iMesh=0; iMesh<podScene.nNumMesh; ++iMesh)
		{
			SPODMesh &podMesh = podScene.pMesh[iMesh];
			if(podMesh.ePrimitiveType != ePODTriangles)
				continue;

			if(podMesh.sVertex.eType != EPODDataFloat)
				continue;
			if(podMesh.sVertex.n != 3)
				continue;
			if(podMesh.sNormals.eType != EPODDataFloat)
				continue;
			if(podMesh.psUVW->eType != EPODDataFloat)
				continue;

			sgMesh *mesh = (sgMesh*)sgResourceCenter::instance()->createResource(sgMesh::GetClassName(), (filename + "_mesh_" + sgStringUtil::to_string(iMesh)).c_str());
			mesh->reset(3, podMesh.nNumVertex, podMesh.nNumFaces);

			sgVertexData *vdata = mesh->getVertexData();
			sgIndexData *idata = mesh->getIndexData();

			Vector3 *vertices = NULL;
			Vector3 *normals = NULL;
			Vector2 *uv0s = NULL;
			size_t *vindices = NULL;
			size_t *nindices = NULL;
			size_t *tindices = NULL;

			// vertex
			{
				vertices = static_cast<Vector3*>( vdata->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, podMesh.nNumVertex)->data() );
				vindices = static_cast<size_t*>( idata->createElement(sgVertexBufferElement::ET_VERTEX)->data() );

				float *podMeshVertexData = (float*)podMesh.sVertex.pData;
				for(size_t i=0; i<podMesh.nNumVertex; ++i)
				{
					size_t podMeshVI = i * (3 + podMesh.sVertex.nStride);
					vertices[i].setValues(podMeshVertexData[podMeshVI]
										, podMeshVertexData[podMeshVI+1]
										, podMeshVertexData[podMeshVI+2]);
				}
				if(!podMesh.sFaces.pData)
				{
					// no index
					// create it
					for(size_t i=0; i<podMesh.nNumVertex; ++i)
					{
						vindices[i] = i;
					}
				}
				else
				{
					//GLenum type = (pMesh->sFaces.eType == EPODDataUnsignedShort) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
					/*if(podMesh.sFaces.eType == EPODDataUnsignedShort)
					{
						unsigned short *podMeshIndexData = (unsigned short*)podMesh.sFaces.pData;
						for(size_t i=0; i<podMesh.nNumFaces*3; ++i)
						{
							size_t podMeshVI = i * (3 + podMesh.sVertex.nStride);
							vindices[i] = meshInfo.vertexIndex[i];
						}
					}
					else
					{

					}*/
					
				}
				
			}

		}
		

		return sceneRoot;

	}
    
} // namespace Sagitta

