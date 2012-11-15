
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
			if(podMesh.nNumStrips != 0)
				continue;

			int podMeshVertexStart = -1;
			int podMeshNormalStart = -1;
			int podMeshUV0Start = -1;
			int podMeshColorStart = -1;

			size_t totalStride = 0;

			podMeshVertexStart = 0;
			if(podMesh.sVertex.eType != EPODDataFloat || podMesh.sVertex.n != 3)
				continue;
			totalStride += podMesh.sVertex.n * sizeof(float);

			podMeshNormalStart = podMeshVertexStart + totalStride;
			if(podMesh.sNormals.eType != EPODDataFloat || (podMesh.sNormals.n != 0 && podMesh.sNormals.n != 3) )
				continue;
			totalStride += podMesh.sNormals.n * sizeof(float);
			
			//if(podMesh.sTangents.n != 0)
			{
				totalStride += podMesh.sTangents.n * sizeof(float);
			}

			//if(podMesh.sBinormals.n != 0)
			{
				totalStride += podMesh.sBinormals.n * sizeof(float);
			}

			if(podMesh.nNumUVW > 0)
			{
				podMeshUV0Start = podMeshVertexStart + totalStride;
				if(podMesh.psUVW[0].eType != EPODDataFloat || (podMesh.psUVW[0].n != 0 && podMesh.psUVW[0].n != 2))
					continue ;
				totalStride += podMesh.psUVW[0].n * sizeof(float);
			}
			for(size_t i=1; i<podMesh.nNumUVW; ++i)
			{
				totalStride += podMesh.psUVW[0].n * sizeof(float);
			}

			if(podMesh.sVtxColours.n != 0)
			{
				PVRTModelPODDataConvert(podMesh.sVtxColours, podMesh.nNumVertex, EPODDataRGBA);
				podMeshColorStart = podMeshVertexStart + totalStride;
				totalStride += podMesh.sNormals.n * sizeof(unsigned int);
			}

			if(totalStride != podMesh.sVertex.nStride)
				continue;

			sgMesh *mesh = (sgMesh*)sgResourceCenter::instance()->createResource(sgMesh::GetClassName(), (filename + "_mesh_" + sgStringUtil::to_string(iMesh)).c_str());
			mesh->reset(3, podMesh.nNumVertex, podMesh.nNumFaces);

			sgVertexData *vdata = mesh->getVertexData();
			sgIndexData *idata = mesh->getIndexData();

			//Vector3 *vertices = NULL;
			//Vector3 *normals = NULL;
			//Vector2 *uv0s = NULL;
			size_t *vindices = NULL;
			size_t *nindices = NULL;
			size_t *tindices = NULL;

			// vertex
			{
				Vector3 *vertices = static_cast<Vector3*>( vdata->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, podMesh.nNumVertex)->data() );
				size_t *vindices = static_cast<size_t*>( idata->createElement(sgVertexBufferElement::ET_VERTEX)->data() );

				for(size_t i=0; i<podMesh.nNumVertex; ++i)
				{
					memcpy(&(vertices[i]), podMesh.pInterleaved + (podMeshVertexStart + (i * totalStride)), sizeof(Vector3));
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
					for(size_t i=0; i<podMesh.nNumFaces*3; ++i)
					{
						memcpy(&(vindices[i]), podMesh.sFaces.pData + (podMesh.sFaces.nStride * i), podMesh.sFaces.nStride);
					}
				}
				
			}

			// normal
			if(podMeshNormalStart > 0)
			{
				Vector3 *normals = static_cast<Vector3*>( vdata->createElement(sgVertexBufferElement::NormalAttributeName, RDT_F, 3, podMesh.nNumVertex)->data() );

				for(size_t i=0; i<podMesh.nNumVertex; ++i)
				{
					memcpy(&(normals[i]), podMesh.pInterleaved + (podMeshNormalStart + (i * totalStride)), sizeof(Vector3));
				}
			}

			// uv0
			if(podMeshUV0Start > 0)
			{
				Vector2 *uv0s = static_cast<Vector2*>( vdata->createElement(sgVertexBufferElement::UV0AttributeName, RDT_F, 2, podMesh.nNumVertex)->data() );

				for(size_t i=0; i<podMesh.nNumVertex; ++i)
				{
					memcpy(&(uv0s[i]), podMesh.pInterleaved + (podMeshUV0Start + (i * totalStride)), sizeof(Vector2));
				}
			}

			// color
			if(podMeshColorStart > 0)
			{
				Color *colors = static_cast<Color*>( vdata->createElement(sgVertexBufferElement::ColorAttributeName, RDT_UBYTE, 4, podMesh.nNumVertex)->data() );

				for(size_t i=0; i<podMesh.nNumVertex; ++i)
				{
					memcpy(&(colors[i]), podMesh.pInterleaved + (podMeshColorStart + (i * totalStride)), sizeof(Color));
				}
			}

		}
		

		return sceneRoot;

	}
    
} // namespace Sagitta

