//////////////////////////////////////////////////////
// file: SagiRenderSystem.cpp @ 2008-10-5 by Zhang Xiang
// defines of the class SagiRenderSystem
// SMenu is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SagiRenderer.h"
#include "sgViewport.h"
#include "engine/buffer/sgFrameBufferManager.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/buffer/sgFrameBuffer.h"
#include "engine/scenegraph/sgScene.h"
#include "engine/resource/sgMaterial.h"
#include "engine/common/sgException.h"
#include "engine/component/sgLightComponent.h"
#include "engine/component/sgRenderStateComponent.h"
#include "engine/component/sgMeshComponent.h"
#include "engine/component/sgCameraComponent.h"
#include "engine/resource/sgMesh.h"
#include "sgRenderState.h"

#include "sgGLInclude.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
    
    SG_META_DEFINE(SagiRenderer, sgRenderer)

	//  [1/15/2009 zhangxiang]
	SagiRenderer::SagiRenderer() 
    : sgRenderer(),
	m_FrameBuffers(1, 1, false),
	m_CurFrameBuffers(),
	m_bLighting(false)
    {

	}

	//  [1/15/2009 zhangxiang]
	SagiRenderer::~SagiRenderer(void){

	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::init(void){

	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::doSthWhenResized(void){
		m_FrameBuffers._resize(tgtWidth(), tgtHeight());
	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::setViewport(sgViewport *aViewport) const{
		int vwidth = aViewport->actwidth();
		int vheight = aViewport->actheight();
		m_CurFrameBuffers.getSliceFromAnother(m_FrameBuffers,
											aViewport->actleft(),
											aViewport->acttop(),
											vwidth,
											vheight);
		m_CurScrMatrix = Matrix4(m_iTargetWidth * 0.5, 0, 0, m_iTargetWidth * 0.5,
								0, m_iTargetHeight * 0.5, 0, m_iTargetHeight * 0.5,
								0, 0, 1, 0,
								0, 0, 0, 1);
	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::clearFrameBuffers(uInt aFlags, const Color &aBkColor,
										Real aBkDepth, int aBkStencil) const{
		// clear buffers
		if(aFlags & sgViewport::CB_COLOR){
			// clear color buffer
			m_CurFrameBuffers.clearColor(aBkColor.toGLColor());
		}
		if(aFlags & sgViewport::CB_DEPTH){
			// clear depth buffer
			m_CurFrameBuffers.clearDepth(aBkDepth);
		}
		if(aFlags & sgViewport::CB_STENCIL){
			// clear stencil buffer
			m_CurFrameBuffers.clearStencil(aBkStencil);
		}
	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::setProjMatrix(const Matrix4 &aMatrix) const{
		m_CurProjMatrix = aMatrix.transpose();
	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::setViewMatrix(const Matrix4 &aMatrix) const{
		m_CurViewMatrix = aMatrix;//.transpose();
	}

	//  [1/15/2009 zhangxiang]
	int SagiRenderer::setupLightsImpl(const Color &aGlobalAmbiantColor) const{
		m_bLighting = true;

		sgLightComponent *light;
		LightList::const_iterator lit = m_CurRenderParam.lightlist.begin();
		LightList::const_iterator leit = m_CurRenderParam.lightlist.end();

		int lightNum = static_cast<int>(m_CurRenderParam.lightlist.size());
		m_CurSagiLightList.reserve(lightNum);
		for(; leit!=lit; ++lit)
        {
			light = *lit;
			
			m_CurSagiLightList.push_back(SagiLight(light->getParent()->position(),
												light->ambientColor().toGLColor(),
												light->diffuseColor().toGLColor(),
												light->specularColor().toGLColor()));
		}

		return lightNum;
	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::render(const sgRenderState &aGlobalRop, sgSceneObject *aRenderable) const{
		sgMeshComponent *meshComp = (sgMeshComponent*)(aRenderable->getComponent(sgMeshComponent::GetClassName()));
        if(!meshComp)
            return ;
        sgMesh *mesh = meshComp->getMesh();
        if(!mesh)
            return ;
        
        sgRenderStateComponent *renderState = (sgRenderStateComponent*)(aRenderable->getComponent(sgRenderStateComponent::GetClassName()));
        sgMaterial *material = 0;
        if(renderState)
            material = (sgMaterial*)(renderState->getMaterial());
        
        // todo setup material
		// ...
		
		m_CurVertexBuffers.renderable = aRenderable;
        m_CurVertexBuffers.mesh = mesh;
        m_CurVertexBuffers.material = material;

		sgVertexData *pvb = new sgVertexData();
		sgVertexIndexBuffer *pvib = new sgVertexIndexBuffer(sgVertexBufferElement::ET_VERTEX);
		mesh->getVertexBuffer(pvb, pvib);

		// vertex buffers
		sgVertexData::ConstIterator elemIt = pvb->getConstIterator();
		m_CurVertexBuffers.vertexnum = pvb->vertexNum();
		size_t vertexnum_x_2 = m_CurVertexBuffers.vertexnum + m_CurVertexBuffers.vertexnum;
		for(; elemIt.hasMoreElements(); elemIt++){
			sgVertexBufferElement *element = elemIt.value();
			switch(element->type()){
					case sgVertexBufferElement::ET_VERTEX:
					{
						Vector3 *vertices = static_cast<Vector3*>(element->data());
						m_CurVertexBuffers.pvertices.reserve(vertexnum_x_2);
						m_CurVertexBuffers.padjacentcount.reserve(vertexnum_x_2);
						for(size_t i=0; i<m_CurVertexBuffers.vertexnum; ++i){
							m_CurVertexBuffers.pvertices.push_back(vertices[i]);
							m_CurVertexBuffers.padjacentcount.push_back(0);
						}
						break;
					}

					case sgVertexBufferElement::ET_COLOR:
					{
						Color *colors = static_cast<Color*>(element->data());
						m_CurVertexBuffers.pcolors.reserve(vertexnum_x_2);
						for(size_t i=0; i<m_CurVertexBuffers.vertexnum; ++i){
							m_CurVertexBuffers.pcolors.push_back(colors[i].toGLColor());
						}
						break;
					}

					case sgVertexBufferElement::ET_NORMAL:
					{
						Vector3 *normals = static_cast<Vector3*>(element->data());
						m_CurVertexBuffers.pnormals.reserve(vertexnum_x_2);
						for(size_t i=0; i<m_CurVertexBuffers.vertexnum; ++i){
							m_CurVertexBuffers.pnormals.push_back(normals[i]);
						}
						break;
					}

					case sgVertexBufferElement::ET_TEXTURE_COORD:
					{
						Vector2 *texcoords = static_cast<Vector2*>(element->data());
						m_CurVertexBuffers.ptexcoords.reserve(vertexnum_x_2);
						for(size_t i=0; i<m_CurVertexBuffers.vertexnum; ++i){
							m_CurVertexBuffers.ptexcoords.push_back(texcoords[i]);
						}
						break;
					}

					case sgVertexBufferElement::ET_FOG_COORDINATE:
					{
						Real *flogcoords = static_cast<Real*>(element->data());
						m_CurVertexBuffers.pflogcoords.reserve(vertexnum_x_2);
						for(size_t i=0; i<m_CurVertexBuffers.vertexnum; ++i){
							m_CurVertexBuffers.pflogcoords.push_back(flogcoords[i]);
						}
						break;
					}

					default:
					{
						THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Invalid render state.", "SGLRenderSystem::render");
						break;
					}
			}
		}

		// index buffer
		size_t *indices = static_cast<size_t*>(pvib->data());
		size_t datanum = pvib->dataNum();
//		m_CurVertexBuffers.pindices.reserve(datanum + datanum);
		for(size_t i=0; i<datanum; ++i){
			m_CurVertexBuffers.pindices.push_back(indices[i]);
			++(m_CurVertexBuffers.padjacentcount[indices[i]]);
		}

		m_CurVertexBuffers.elementtype = mesh->polyType();
		m_CurVertexBuffers.facenum = pvib->polyNum();

		

		// model transform matrix
		m_CurModelMatrix = aRenderable->getFullTransform();
		
		// cull faces, only for triangles
		if(pvib->polyType() == 3 &&
			m_CurRenderParam.pscene->getRenderState().isFaceCullingEnable()){
			cullFaces();
		}

		// view trnasform
		viewTransform();

		// clip faces
		clipFaces();

		// calculate lighting
		if(m_bLighting)
			vertexShader();

		// screen transform
		screenTransform();

		// render to frame buffer
		renderToFrameBuffer();

		// release current vertex buffers for next renderable object
		m_CurVertexBuffers.release();

		delete pvb;
		delete pvib;
	}

	//  [1/17/2009 zhangxiang]
	void SagiRenderer::cullFaces(void) const{
		if(m_CurRenderParam.pscene->getRenderState().faceToCull() == sgRenderState::FTC_BACK){
			cullBackFacesImpl();
		}else{
			cullFrontFacesImpl();
		}
	}

	//  [1/17/2009 zhangxiang]
	void SagiRenderer::cullBackFacesImpl(void) const{
		// have not done view transformation,
		// transform the camera position to the object's local space
		Matrix4 invModelMatrix;
		invModelMatrix.makeInverseTransform(m_CurVertexBuffers.renderable->absolutePosition(),
											m_CurVertexBuffers.renderable->absoluteScale(),
											m_CurVertexBuffers.renderable->absoluteOrientation());
		Vector3 camera_local_pos = invModelMatrix * m_CurRenderParam.pcamera->getParent()->position();
		Vector3 local_viewray;
		
		IndexList::iterator ait, bit, cit;	// store current three vertices' indices

		V3List &normals = m_CurVertexBuffers.pnormals;
		V3List &vertices = m_CurVertexBuffers.pvertices;
		IndexList &indices = m_CurVertexBuffers.pindices;
		CountList &adjacentcouts = m_CurVertexBuffers.padjacentcount;

		Vector3 *face_normals = static_cast<Vector3*>(m_CurVertexBuffers.mesh->getFaceNormalBuffer()->data());

		size_t orifacenum = m_CurVertexBuffers.facenum;
		IndexList::iterator iit = indices.begin();
		for(size_t i=0; i<orifacenum; ++i){
			ait = iit++;
			bit = iit++;
			cit = iit++;

			// using the right face normal is ok

			local_viewray = vertices[*ait] - camera_local_pos;

			if(local_viewray.dotProduct(face_normals[i]) > 0){	// invisible
				--(adjacentcouts[(*ait)]);
				indices.erase(ait);

				--(adjacentcouts[(*bit)]);
				indices.erase(bit);

				--(adjacentcouts[(*cit)]);
				iit = indices.erase(cit);

				--m_CurVertexBuffers.facenum;
				
			}
		}
	}

	//  [1/17/2009 zhangxiang]
	void SagiRenderer::cullFrontFacesImpl(void) const{
		// have not done view transformation,
		// transform the camera position to the object's local space
		Matrix4 invModelMatrix;
		invModelMatrix.makeInverseTransform(m_CurVertexBuffers.renderable->absolutePosition(),
											m_CurVertexBuffers.renderable->absoluteScale(),
											m_CurVertexBuffers.renderable->absoluteOrientation());
		Vector3 camera_local_pos = invModelMatrix * m_CurRenderParam.pcamera->getParent()->position();
		Vector3 local_viewray;
		
		IndexList::iterator ait, bit, cit;	// store current three vertices' indices

		V3List &normals = m_CurVertexBuffers.pnormals;
		V3List &vertices = m_CurVertexBuffers.pvertices;
		IndexList &indices = m_CurVertexBuffers.pindices;
		CountList &adjacentcouts = m_CurVertexBuffers.padjacentcount;

		Vector3 *face_normals = static_cast<Vector3*>(m_CurVertexBuffers.mesh->getFaceNormalBuffer()->data());

		size_t orifacenum = m_CurVertexBuffers.facenum;
		IndexList::iterator iit = indices.begin();
		for(size_t i=0; i<orifacenum; ++i){
			ait = iit++;
			bit = iit++;
			cit = iit++;

			// using the right face normal is ok

			local_viewray = vertices[*ait] - camera_local_pos;

			if(local_viewray.dotProduct(face_normals[i]) < 0){	// invisible
				--(adjacentcouts[(*ait)]);
				indices.erase(ait);

				--(adjacentcouts[(*bit)]);
				indices.erase(bit);

				--(adjacentcouts[(*cit)]);
				iit = indices.erase(cit);

				--m_CurVertexBuffers.facenum;
				
			}
		}
	}

	//  [1/17/2009 zhangxiang]
	void SagiRenderer::viewTransform(void) const{
		Matrix4 ftm = m_CurViewMatrix * m_CurModelMatrix;
		
		V3List::iterator vit = m_CurVertexBuffers.pvertices.begin();
		V3List::iterator veit = m_CurVertexBuffers.pvertices.end();
		CountList::iterator cit = m_CurVertexBuffers.padjacentcount.begin();
		if(!m_CurVertexBuffers.pnormals.empty()){
			Quaternion normal_rot_q = m_CurRenderParam.pcamera->getParent()->absoluteOrientation().inverse() *
									m_CurVertexBuffers.renderable->absoluteOrientation();

			V3List::iterator nit = m_CurVertexBuffers.pnormals.begin();
			for(; vit!=veit; ++vit, ++nit, ++cit){
				if((*cit) <= 0){	// invisible vertex
					continue;
				}

				*vit = ftm * (*vit);
				*nit = normal_rot_q * (*nit);
			//	(*nit).normalise();
			}
		}else{
			for(; vit!=veit; ++vit, ++cit){
				if((*cit) <= 0){	// invisible vertex
					continue;
				}

				*vit = ftm * (*vit);
			}
		}
	}

	//  [1/17/2009 zhangxiang]
	void SagiRenderer::clipFaces(void) const{
		switch(m_CurVertexBuffers.elementtype){
			case sgMesh::RET_POINTS:
			{
				clipPointsImpl();
				break;
			}

			case sgMesh::RET_LINES:
			{
				clipLinesImpl();
				break;
			}

			case sgMesh::RET_TRIANGLES:
			{
				clipTrianglesImpl();
				break;
			}

			default:
			{
				THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
					"Unknown render element type.",
					"SagiRender::clipFaces");
			}
		}
	}

	//  [1/17/2009 zhangxiang]
	void SagiRenderer::clipPointsImpl(void) const{
		Real znear = -m_CurRenderParam.pcamera->zNear();
		Real zfar = -m_CurRenderParam.pcamera->zFar();

		Real viewplaneHalfHeight = m_CurRenderParam.pcamera->viewHalfHeight();
		Real viewplaneHalfWidth = m_CurRenderParam.pcamera->viewHalfWidth();
		Real invnear = 1 / znear;

		Real xtestFactor = viewplaneHalfWidth * invnear;
		Real ytestFactor = viewplaneHalfHeight * invnear;

		Real xtestdist, ytestdist;

		// current point's position
		Real curx, cury, curz;

		V3List &vertices = m_CurVertexBuffers.pvertices;
		V3List::iterator vit = vertices.begin();
		V3List::iterator veit = vertices.end();
		for(; vit!=veit; ){
			curx = (*vit).x();
			cury = (*vit).y();
			curz = (*vit).z();

			xtestdist = xtestFactor * curz;
			ytestdist = ytestFactor * curz;

			if((curz < zfar || curz > znear) ||
				(curx > xtestdist || curx < -xtestdist) ||
				(cury > ytestdist || cury < -ytestdist)){
				vit = vertices.erase(vit);
				--m_CurVertexBuffers.vertexnum;
				--m_CurVertexBuffers.facenum;
				continue;
			}

			++vit;
		}
	}

	//  [1/18/2009 zhangxiang]
	void SagiRenderer::clipLinesImpl(void) const{
		Real znear = -m_CurRenderParam.pcamera->zNear();
		Real zfar = -m_CurRenderParam.pcamera->zFar();

		Real viewplaneHalfHeight = m_CurRenderParam.pcamera->viewHalfHeight();
		Real viewplaneHalfWidth = m_CurRenderParam.pcamera->viewHalfWidth();
		Real invnear = 1 / znear;

		Real xtestFactor = viewplaneHalfWidth * invnear;
		Real ytestFactor = viewplaneHalfHeight * invnear;

		Real xtestdist, ytestdist;

		uByte clip_code[2];	// store two vertices' clip code
		clip_code[0] = clip_code[1] = CC_IN;
		int out_near_count = 0;	// store how many vertices out of the near clip plane

		// current point's position
		Real curx, cury, curz;

		Vector3 intersecion;				// stores current intersection's position
		Vector2 interuv;					// stores current intersection's texture coordinates
		Color::GLColor intercolor;			// stores current intersection's color
		Vector3 edge_vector_from_a;			// vb - va
		Vector2 edge_uv_from_a;				// tb - ta
		Color::GLColor edge_color_from_a;	// cb - ca

		Color::GLColor ca;					// stores v0's color

		size_t a, b;	// stores the three vertices' index, a--v0, b--v1

		Real t_ax;	// parameter value of line ab or ac at z == znear

		IndexList::iterator tvit[2];	// store two vertices' index iterator

		V3List &vertices = m_CurVertexBuffers.pvertices;
		V2List &texcoods = m_CurVertexBuffers.ptexcoords;
		ColorList &colors = m_CurVertexBuffers.pcolors;
		IndexList &indices = m_CurVertexBuffers.pindices;
		CountList &adjacentcouts = m_CurVertexBuffers.padjacentcount;

		IndexList::iterator iit = indices.begin();
		size_t orifacenum = m_CurVertexBuffers.facenum;
		for(size_t i=0; i<orifacenum; ++i){
			out_near_count = 0;	// clear
			clip_code[0] = clip_code[1] = CC_IN;	// clear

			for(int vi=0; vi<2; ++vi){
				tvit[vi] = iit++;

				curx = vertices[(*(tvit[vi]))].x();
				cury = vertices[(*(tvit[vi]))].y();
				curz = vertices[(*(tvit[vi]))].z();

				xtestdist = xtestFactor * curz;
				ytestdist = ytestFactor * curz;

				if(curz > znear){
					clip_code[vi] |= CC_OUT_NEAR;
					xtestdist = -xtestdist;
					ytestdist = -ytestdist;
					++out_near_count;
				}
				if(curz < zfar){
					clip_code[vi] |= CC_OUT_FAR;
				}
				if(curx > xtestdist){
					clip_code[vi] |= CC_OUT_RIGHT;
				}
				if(curx < -xtestdist){
					clip_code[vi] |= CC_OUT_LEFT;
				}
				if(cury > ytestdist){
					clip_code[vi] |= CC_OUT_UP;
				}
				if(cury < -ytestdist){
					clip_code[vi] |= CC_OUT_DOWN;
				}
			}	// end for three vertices
		

			if(((clip_code[0] & CC_OUT_NEAR) && (clip_code[1] & CC_OUT_NEAR)) ||
				((clip_code[0] & CC_OUT_FAR) && (clip_code[1] & CC_OUT_FAR)) ||
				((clip_code[0] & CC_OUT_LEFT) && (clip_code[1] & CC_OUT_LEFT)) ||
				((clip_code[0] & CC_OUT_RIGHT) && (clip_code[1] & CC_OUT_RIGHT)) ||
				((clip_code[0] & CC_OUT_UP) && (clip_code[1] & CC_OUT_UP)) ||
				((clip_code[0] & CC_OUT_DOWN) && (clip_code[1] & CC_OUT_DOWN))){
				// total out of the frustum, cull it
				--(adjacentcouts[(*(tvit[0]))]);
				--(adjacentcouts[(*(tvit[1]))]);

				indices.erase(tvit[0]);
				iit = indices.erase(tvit[1]);

				// decrease number of lines
				--m_CurVertexBuffers.facenum;

				continue;
			}

			// part in the frustum, test if has any vertex out of the near clip plane
			size_t last_v_index = vertices.size() - 1;
			if(out_near_count > 0){
				// have one vertex out of near clip plane, clip it

				// find intersection and insert it back to the vertices list,
				// and then modify the this line's index in the indices list.

				// find the vertex not out of the near clip plane
				if(!(clip_code[1] & CC_OUT_NEAR)){
					std::swap(*(tvit[0]), *(tvit[1]));
				}
				a = *(tvit[0]);	// the one not out of the near clip plane
				b = *(tvit[1]);

				// find intersection
				
				// a -> b
				edge_vector_from_a = vertices[b] - vertices[a];

				t_ax = (znear - vertices[a].z()) / edge_vector_from_a.z();
				intersecion.setValues(vertices[a].x() + edge_vector_from_a.x() * t_ax,
									vertices[a].y() + edge_vector_from_a.y() * t_ax,
									znear);

				vertices.push_back(intersecion);
				++m_CurVertexBuffers.vertexnum;

				--(adjacentcouts[b]);
				adjacentcouts.push_back(1);

				*(tvit[1]) = ++last_v_index;

				// calculate new vertex's texture coordinations and color if have
				if(!texcoods.empty()){	// if have texture coodinations
					edge_uv_from_a = texcoods[b] - texcoods[a];
				/*	interuv.setValues(texcoods[a].x() + edge_uv_from_a.x() * t_ax,
									texcoods[a].y() + edge_uv_from_a.y() * t_ax); */
					texcoods.push_back(texcoods[a] + edge_uv_from_a * t_ax);
				}
				if(!colors.empty()){	// if have color
			//		ca = colors[a];
					edge_color_from_a = colors[b] - colors[a];//ca;
			/*		intercolor.setValues(ca.x() + edge_color_from_a.x() * t_ax,
										ca.y() + edge_color_from_a.y() * t_ax,
										ca.z() + edge_color_from_a.z() * t_ax,
										ca.w() + edge_color_from_a.w() * t_ax); */
					colors.push_back(colors[a] + edge_color_from_a * t_ax);
				}

			} //#### end if out_near_count > 0
		} //#### end for lines
	} //#### end clipLinesImpl

	//  [1/18/2009 zhangxiang]
	void SagiRenderer::clipTrianglesImpl(void) const{
		Real znear = -m_CurRenderParam.pcamera->zNear();
		Real zfar = -m_CurRenderParam.pcamera->zFar();

		Real viewplaneHalfHeight = m_CurRenderParam.pcamera->viewHalfHeight();
		Real viewplaneHalfWidth = m_CurRenderParam.pcamera->viewHalfWidth();
		Real invnear = 1 / znear;

		Real xtestFactor = viewplaneHalfWidth * invnear;
		Real ytestFactor = viewplaneHalfHeight * invnear;

		Real xtestdist, ytestdist;

		uByte clip_code[3];	// store three vertices' clip code
		clip_code[0] = clip_code[1] = clip_code[2] = CC_IN;
		int out_near_count = 0;	// store how many vertices out of the near clip plane

		// current point's position
		Real curx, cury, curz;

		Vector3 intersecion;				// stores current intersection's position
		Vector3 internormal;				// stores current intersection's normal
		Vector2 interuv;					// stores current intersection's texture coordinates
		Color::GLColor intercolor;			// stores current intersection's color
		Vector3 edge_vector_from_a;			// vb - va or vc - va
		Vector3 edge_normal_from_a;			// nb - na or nc - na
		Vector2 edge_uv_from_a;				// tb - ta or tc - ta
		Color::GLColor edge_color_from_a;	// cb - ca or cc - ca

		Color::GLColor ca;					// stores v0's color

		size_t a, b, c;	// stores the three vertices' index, a--v0, b--v1, c--v2

		Real t_ax;	// parameter value of line ab or ac at z == znear

		IndexList::iterator tvit[3];	// store three vertices' index iterator

		V3List &vertices = m_CurVertexBuffers.pvertices;
		V3List &normals = m_CurVertexBuffers.pnormals;
		V2List &texcoods = m_CurVertexBuffers.ptexcoords;
		ColorList &colors = m_CurVertexBuffers.pcolors;
		IndexList &indices = m_CurVertexBuffers.pindices;
		CountList &adjacentcouts = m_CurVertexBuffers.padjacentcount;

		IndexList::iterator iit = indices.begin();
		size_t orifacenum = m_CurVertexBuffers.facenum;
		for(size_t i=0; i<orifacenum; ++i){
			out_near_count = 0;
			clip_code[0] = clip_code[1] = clip_code[2] = CC_IN;

			for(int vi=0; vi<3; ++vi){
				tvit[vi] = iit++;

				curx = vertices[(*(tvit[vi]))].x();
				cury = vertices[(*(tvit[vi]))].y();
				curz = vertices[(*(tvit[vi]))].z();

				xtestdist = xtestFactor * curz;
				ytestdist = ytestFactor * curz;

				if(curz > znear){
					clip_code[vi] |= CC_OUT_NEAR;
					xtestdist = -xtestdist;
					ytestdist = -ytestdist;
					++out_near_count;
				}
				if(curz < zfar){
					clip_code[vi] |= CC_OUT_FAR;
				}
				if(curx > xtestdist){
					clip_code[vi] |= CC_OUT_RIGHT;
				}
				if(curx < -xtestdist){
					clip_code[vi] |= CC_OUT_LEFT;
				}
				if(cury > ytestdist){
					clip_code[vi] |= CC_OUT_UP;
				}
				if(cury < -ytestdist){
					clip_code[vi] |= CC_OUT_DOWN;
				}
			}	// end for three vertices
		

			if(((clip_code[0] & CC_OUT_NEAR) && (clip_code[1] & CC_OUT_NEAR) && (clip_code[2] & CC_OUT_NEAR)) ||
				((clip_code[0] & CC_OUT_FAR) && (clip_code[1] & CC_OUT_FAR) && (clip_code[2] & CC_OUT_FAR)) ||
				((clip_code[0] & CC_OUT_LEFT) && (clip_code[1] & CC_OUT_LEFT) && (clip_code[2] & CC_OUT_LEFT)) ||
				((clip_code[0] & CC_OUT_RIGHT) && (clip_code[1] & CC_OUT_RIGHT) && (clip_code[2] & CC_OUT_RIGHT)) ||
				((clip_code[0] & CC_OUT_UP) && (clip_code[1] & CC_OUT_UP) && (clip_code[2] & CC_OUT_UP)) ||
				((clip_code[0] & CC_OUT_DOWN) && (clip_code[1] & CC_OUT_DOWN) && (clip_code[2] & CC_OUT_DOWN))){
				// total out of the frustum, cull it
				--(adjacentcouts[(*(tvit[0]))]);
				--(adjacentcouts[(*(tvit[1]))]);
				--(adjacentcouts[(*(tvit[2]))]);

				indices.erase(tvit[0]);
				indices.erase(tvit[1]);
				iit = indices.erase(tvit[2]);

				// decrease number of triangles
				--m_CurVertexBuffers.facenum;

				continue;
			}

			// part in the frustum, test if has any vertex out of near clip plane
			size_t last_v_index = vertices.size() - 1;
			if(out_near_count > 0){
				// have one or two vertices out of near clip plane, clip it
				// there are two cases:
				// case 1: one vertex is not out of the near clip plane, and two are out;
				// case 2: two vertices are not out of the near clip plane, and one is out.

				if(out_near_count == 2){
					// process case 1
					// find two intersection and insert them back to the vertices list,
					// and then modify the this face's index in the indices list.

					// find the vertex not out of the near clip plane
					if(!(clip_code[1] & CC_OUT_NEAR)){
						std::swap(*(tvit[0]), *(tvit[1]));
					}else if(!(clip_code[2] & CC_OUT_NEAR)){
						std::swap(*(tvit[0]), *(tvit[2]));
					}
					a = *(tvit[0]);	// the one not out of the near clip plane
					b = *(tvit[1]);
					c = *(tvit[2]);

					// find intersections
					
					// a -> b
					edge_vector_from_a = vertices[b] - vertices[a];

					t_ax = (znear - vertices[a].z()) / edge_vector_from_a.z();
					intersecion.setValues(vertices[a].x() + edge_vector_from_a.x() * t_ax,
										vertices[a].y() + edge_vector_from_a.y() * t_ax,
										znear);

					vertices.push_back(intersecion);
					++m_CurVertexBuffers.vertexnum;

					--(adjacentcouts[b]);
					adjacentcouts.push_back(1);

					*(tvit[1]) = ++last_v_index;

					// calculate new vertex's normal and texture coordinations
					edge_normal_from_a = normals[b] - normals[a];
				/*	internormal.setValues(normals[a].x() + edge_normal_from_a.x() * t_ax,
										normals[a].y() + edge_normal_from_a.y() * t_ax,
										normals[a].z() + edge_normal_from_a.z() * t_ax); */
					internormal = normals[a] + edge_normal_from_a * t_ax;
					internormal.normalise();
					normals.push_back(internormal);

					if(!texcoods.empty()){	// if have texture coordinations
						edge_uv_from_a = texcoods[b] - texcoods[a];
				/*		interuv.setValues(texcoods[a].x() + edge_uv_from_a.x() * t_ax,
										texcoods[a].y() + edge_uv_from_a.y() * t_ax); */
						texcoods.push_back(texcoods[a] + edge_uv_from_a * t_ax/*interuv*/);
					}
					if(!colors.empty()){	// if have color
				//		ca = colors[a].toGLColor();
						edge_color_from_a = colors[b] - colors[a];
				/*		intercolor.setValues(ca.x() + edge_color_from_a.x() * t_ax,
											ca.y() + edge_color_from_a.y() * t_ax,
											ca.z() + edge_color_from_a.z() * t_ax,
											ca.w() + edge_color_from_a.w() * t_ax); */
						colors.push_back(colors[a] + edge_color_from_a * t_ax);
					}


					// a -> c
					edge_vector_from_a = vertices[c] - vertices[a];

					t_ax = (znear - vertices[a].z()) / edge_vector_from_a.z();
					intersecion.setValues(vertices[a].x() + edge_vector_from_a.x() * t_ax,
										vertices[a].y() + edge_vector_from_a.y() * t_ax,
										znear);

					vertices.push_back(intersecion);
					++m_CurVertexBuffers.vertexnum;

					--(adjacentcouts[c]);
					adjacentcouts.push_back(1);

					*(tvit[2]) = ++last_v_index;

					// calculate new vertex's normal and texture coordinations
					edge_normal_from_a = normals[c] - normals[a];
				/*	internormal.setValues(normals[a].x() + edge_normal_from_a.x() * t_ax,
										normals[a].y() + edge_normal_from_a.y() * t_ax,
										normals[a].z() + edge_normal_from_a.z() * t_ax); */
					internormal = normals[a] + edge_normal_from_a * t_ax;
					internormal.normalise();
					normals.push_back(internormal);

					if(!texcoods.empty()){	// if have texture coodinations
						edge_uv_from_a = texcoods[c] - texcoods[a];
				/*		interuv.setValues(texcoods[a].x() + edge_uv_from_a.x() * t_ax,
										texcoods[a].y() + edge_uv_from_a.y() * t_ax); */
						texcoods.push_back(texcoods[a] + edge_uv_from_a * t_ax);
					}
					if(!colors.empty()){	// if have color
					//	ca = colors[a].toGLColor();
						edge_color_from_a = colors[c] - colors[a];
					/*	intercolor.setValues(ca.x() + edge_color_from_a.x() * t_ax,
											ca.y() + edge_color_from_a.y() * t_ax,
											ca.z() + edge_color_from_a.z() * t_ax,
											ca.w() + edge_color_from_a.w() * t_ax); */
						colors.push_back(colors[a] + edge_color_from_a * t_ax);
					}
				}else{
					// process case 2
					// find two intersection and insert them back to the vertices list,
					// and then modify the first face's index in the indices list, and
					// insert a new face indices into the indices list

					// find the vertex out of the near clip plane
					if(clip_code[1] & CC_OUT_NEAR){
						std::swap(*(tvit[0]), *(tvit[1]));
					}else if(clip_code[2] && CC_OUT_NEAR){
						std::swap(*(tvit[0]), *(tvit[2]));
					}
					a = *(tvit[0]);	// the one out of the near clip plane
					b = *(tvit[1]);
					c = *(tvit[2]);

					// find intersections
					// a -> b
					edge_vector_from_a = vertices[b] - vertices[a];

					t_ax = (znear - vertices[a].z()) / edge_vector_from_a.z();
					intersecion.setValues(vertices[a].x() + edge_vector_from_a.x() * t_ax,
										vertices[a].y() + edge_vector_from_a.y() * t_ax,
										znear);

					vertices.push_back(intersecion);
					++m_CurVertexBuffers.vertexnum;

					adjacentcouts.push_back(2);	// adjacent count is 2, because the next new face
												// is adjacent this intersection

					*(tvit[0]) = ++last_v_index;

					// calculate new vertex's normal and texture coordinations
					edge_normal_from_a = normals[b] - normals[a];
				/*	internormal.setValues(normals[a].x() + edge_normal_from_a.x() * t_ax,
										normals[a].y() + edge_normal_from_a.y() * t_ax,
										normals[a].z() + edge_normal_from_a.z() * t_ax); */
					internormal = normals[a] + edge_normal_from_a * t_ax;
					internormal.normalise();
					normals.push_back(internormal);

					if(!texcoods.empty()){	// if have texture coodinations
						edge_uv_from_a = texcoods[b] - texcoods[a];
				/*		interuv.setValues(texcoods[a].x() + edge_uv_from_a.x() * t_ax,
										texcoods[a].y() + edge_uv_from_a.y() * t_ax); */
						texcoods.push_back(texcoods[a] + edge_uv_from_a * t_ax);
					}
					if(!colors.empty()){	// if have color
					//	ca = colors[a].toGLColor();
						edge_color_from_a = colors[b] - colors[a];
					/*	intercolor.setValues(ca.x() + edge_color_from_a.x() * t_ax,
											ca.y() + edge_color_from_a.y() * t_ax,
											ca.z() + edge_color_from_a.z() * t_ax,
											ca.w() + edge_color_from_a.w() * t_ax); */
						colors.push_back(colors[a] + edge_color_from_a * t_ax);
					}


					// a -> c
					edge_vector_from_a = vertices[c] - vertices[a];

					t_ax = (znear - vertices[a].z()) / edge_vector_from_a.z();
					intersecion.setValues(vertices[a].x() + edge_vector_from_a.x() * t_ax,
										vertices[a].y() + edge_vector_from_a.y() * t_ax,
										znear);

					vertices.push_back(intersecion);
					++m_CurVertexBuffers.vertexnum;

					--(adjacentcouts[a]);	// decrease v1's adjacent count
					++(adjacentcouts[c]);	// increase v2's adjacent count
					adjacentcouts.push_back(1);
					// insert new face
					indices.push_back(c);
					indices.push_back(last_v_index);
					indices.push_back(++last_v_index);

					// increase number of lines
					++m_CurVertexBuffers.facenum;

					// calculate new vertex's normal and texture coordinations
					edge_normal_from_a = normals[c] - normals[a];
			/*		internormal.setValues(normals[a].x() + edge_normal_from_a.x() * t_ax,
										normals[a].y() + edge_normal_from_a.y() * t_ax,
										normals[a].z() + edge_normal_from_a.z() * t_ax); */
					internormal = normals[a] + edge_normal_from_a * t_ax;
					internormal.normalise();
					normals.push_back(internormal);

					if(!texcoods.empty()){	// if have texture coodinations
						edge_uv_from_a = texcoods[c] - texcoods[a];
				/*		interuv.setValues(texcoods[a].x() + edge_uv_from_a.x() * t_ax,
										texcoods[a].y() + edge_uv_from_a.y() * t_ax); */
						texcoods.push_back(texcoods[a] + edge_uv_from_a * t_ax);
					}
					if(!colors.empty()){	// if have color
					//	ca = colors[a].toGLColor();
						edge_color_from_a = colors[c] - colors[a];
					/*	intercolor.setValues(ca.x() + edge_color_from_a.x() * t_ax,
											ca.y() + edge_color_from_a.y() * t_ax,
											ca.z() + edge_color_from_a.z() * t_ax,
											ca.w() + edge_color_from_a.w() * t_ax); */
						colors.push_back(colors[a] + edge_color_from_a * t_ax);
					}


					// decrease v0's adjacent count
					--(adjacentcouts[a]);

				} //#### end case 2
			} //#### end if out_near_count > 0
		} //#### end for faces
	} //#### end clipTrianglesImpl

	//  [1/21/2009 zhangxiang]
	void SagiRenderer::vertexShader(void) const{
		gourandShaderImpl();
	}

	//  [1/21/2009 zhangxiang]
	void SagiRenderer::gourandShaderImpl(void) const{
		V3List &normals = m_CurVertexBuffers.pnormals;
		if(normals.empty()){
			// do nothing
			return ;
		}
		V3List &vertices = m_CurVertexBuffers.pvertices;
		CountList &adjacentcounts = m_CurVertexBuffers.padjacentcount;
		ColorList &colors = m_CurVertexBuffers.pcolors;
		if(colors.empty()){
			colors.resize(m_CurVertexBuffers.vertexnum);
		}

		size_t light_num = m_CurSagiLightList.size();
		Vector3 light_pos;
		Vector3 light_2_vertex;
		Real ln_dot;

		Vector3 _s;	// (light_2_vertex + vertex).normalise
		Real sn_dot;

        Color::GLColor mAmbient = Color::GLColor(0.2f, 0.2f, 0.2f, 1.0f);
        Color::GLColor mDiffuse = Color::GLColor(0.8f, 0.8f, 0.8f, 1.0f);
        Color::GLColor mSpecular = Color::GLColor(0.0f, 0.0f, 0.0f, 1.0f);
        Color::GLColor mEmission = Color::GLColor(0.0f, 0.0f, 0.0f, 1.0f);
        Real mShininess = 0.0f;
        Real mSpecularAmount = 0.5f;
        Real mReflectFraction = 1.0f;
        if(m_CurVertexBuffers.material)
        {
            mAmbient = m_CurVertexBuffers.material->ambientColor().toGLColor();
            mDiffuse = m_CurVertexBuffers.material->diffuseColor().toGLColor();
            mSpecular = m_CurVertexBuffers.material->specularColor().toGLColor();
            mEmission = m_CurVertexBuffers.material->emissionColor().toGLColor();

            mShininess = m_CurVertexBuffers.material->shininess();
            mSpecularAmount = m_CurVertexBuffers.material->specularAmount();
            mReflectFraction = m_CurVertexBuffers.material->reflectFraction();
        }

		// global ambient color
		Color::GLColor res_ambient = mEmission + m_CurRenderParam.pscene->getAmbiantColor().toGLColor() * mAmbient;
		
		Color::GLColor vc;
		Color::GLColor light_color_effect;
		Real attenuation_factor;
		for(size_t vi=0; vi<m_CurVertexBuffers.vertexnum; ++vi){
			if(adjacentcounts[vi] <= 0){
				// don't belong to any polygon, do nothing
				continue;
			}

			vc = res_ambient;
			
			for(size_t i=0; i<light_num; ++i){
				const SagiLight &cur_light = m_CurSagiLightList[i];
				light_pos = m_CurViewMatrix * cur_light.position;
				light_2_vertex = vertices[vi] - light_pos;

				// attenuation factor
				attenuation_factor = 1.0;
				
				// light ambient
				light_color_effect = cur_light.ambient * mAmbient;

				ln_dot = light_2_vertex.dotProduct(normals[vi]);
				if(ln_dot < 0){
					light_color_effect += cur_light.diffuse * mDiffuse * (-ln_dot);
					
					_s = light_2_vertex + vertices[vi];
					_s.normalise();
				
					sn_dot = _s.dotProduct(normals[vi]);
					if(sn_dot < 0){
						light_color_effect +=cur_light.specular * mSpecular * pow(-sn_dot, mShininess);
					}
				}

				vc += light_color_effect * attenuation_factor;
			} //#### end for lights

			Color::rgbLimit(vc);
			colors[vi] = vc;
		} //#### end for vertices
	}

	//  [1/17/2009 zhangxiang]
	void SagiRenderer::screenTransform(void) const{
		Vector3 v0(0, 0, -1000.0f);
		Vector3 v1(0, 0, -500.0f);
		Vector3 v2(0, 0, -100.0f);
		Vector3 v3(0, 0, -2.0f);
		v0 = m_CurProjMatrix * v0;
		v1 = m_CurProjMatrix * v1;
		v2 = m_CurProjMatrix * v2;
		v3 = m_CurProjMatrix * v3;
		// projection
		V3List::iterator vit = m_CurVertexBuffers.pvertices.begin();
		V3List::iterator veit = m_CurVertexBuffers.pvertices.end();
		CountList::iterator cit = m_CurVertexBuffers.padjacentcount.begin();
		for(; vit!=veit; ++vit, ++cit){
			if((*cit) <= 0){	// invisible vertex
				continue;
			}

			if(Math::RealEqual((*vit).z(), 0)){
				// on the near clip plane
				// does not do projection transformation
				continue;
			}

			*vit = m_CurProjMatrix * (*vit);
		}

		// screen
		int hvwidth = m_CurRenderParam.pviewport->actwidth() * 0.5;
		int hvheight = m_CurRenderParam.pviewport->actheight() * 0.5;
		vit = m_CurVertexBuffers.pvertices.begin();
		cit = m_CurVertexBuffers.padjacentcount.begin();
		for(; vit!=veit; ++vit, ++cit){
			if((*cit) <= 0){ // invisible vertex
				continue;
			}

			(*vit).setX(hvwidth * ((*vit).x() + 1));
			(*vit).setY(hvheight * ((*vit).y() + 1));
		}
	}

	//  [1/16/2009 zhangxiang]
	void SagiRenderer::renderToFrameBuffer(void) const{
		switch(m_CurVertexBuffers.elementtype){
			case sgMesh::RET_POINTS:
			{
				renderPointsImpl();
				break;
			}

			case sgMesh::RET_LINES:
			{
				renderLinesImpl();
				break;
			}

			case sgMesh::RET_TRIANGLES:
			{
				renderTrianglesImpl();
				break;
			}

			default:
			{
				THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
					"Unknown render element type.",
					"SagiRender::renderToFrameBuffer");
			}
		}
	}

	//  [1/16/2009 zhangxiang]
	void SagiRenderer::renderPointsImpl(void) const{
		Color::GLColor *pcolorbuffer = static_cast<Color::GLColor*>(m_CurFrameBuffers.getColorBuffer()->data());
		size_t topwidth = m_CurFrameBuffers.getTopParentWidth();
		int width = static_cast<int>(m_CurFrameBuffers.width());
		int height = static_cast<int>(m_CurFrameBuffers.height());

		V3List::iterator vit = m_CurVertexBuffers.pvertices.begin();
		V3List::iterator veit = m_CurVertexBuffers.pvertices.end();
		int x, y;
		if(!m_CurVertexBuffers.pcolors.empty()){
			ColorList::iterator cit = m_CurVertexBuffers.pcolors.begin();
			for(; vit!=veit; ++vit, ++cit){
				x = (*vit).x();
				y = (*vit).y();
				if(x < 0 || x >= width ||
					y < 0 || y >= height){
					continue;
				}

				pcolorbuffer[y * topwidth + x] = *cit;
			}
		}else{
			for(; vit!=veit; ++vit){
				x = (*vit).x();
				y = (*vit).y();
				if(x < 0 || x >= width ||
					y < 0 || y >= height){
					continue;
				}

				pcolorbuffer[y * topwidth + x] = Color::GLColor::UNIT_SCALE;
			}
		}
	}

	//  [1/16/2009 zhangxiang]
	void SagiRenderer::renderLinesImpl(void) const{
		V3List &vertices = m_CurVertexBuffers.pvertices;

		size_t v1, v2;
		IndexList::iterator iit = m_CurVertexBuffers.pindices.begin();
		if(!m_CurVertexBuffers.pcolors.empty()){
			if(m_CurRenderParam.pscene->getRenderState().isDepthTestEnable()){
				ColorList &colors = m_CurVertexBuffers.pcolors;
				for(size_t i=0; i<m_CurVertexBuffers.facenum; ++i){
					v1 = (*iit); ++iit;	// 2 * i
					v2 = (*iit); ++iit;	// 2 * i + 1
					lineColorInterpolation(vertices[v1], colors[v1],
										vertices[v2], colors[v2]);
				}
			}else{
				ColorList &colors = m_CurVertexBuffers.pcolors;
				for(size_t i=0; i<m_CurVertexBuffers.facenum; ++i){
					v1 = (*iit); ++iit;	// 2 * i
					v2 = (*iit); ++iit;	// 2 * i + 1
					lineColorInterpolation(vertices[v1].x(), vertices[v1].y(), colors[v1],
										vertices[v2].x(), vertices[v2].y(), colors[v2]);
				}
			}
		}else{
			for(size_t i=0; i<m_CurVertexBuffers.facenum; ++i){
				v1 = (*iit); ++iit;	// 2 * i
				v2 = (*iit); ++iit;	// 2 * i + 1
				lineInterpolation(vertices[v1].x(), vertices[v1].y(),
									vertices[v2].x(), vertices[v2].y());
			}
		}
	}

	//  [1/21/2009 zhangxiang]
	bool SagiRenderer::clipLinesInScreen(int &aX1, int &aY1, Color::GLColor &aColor1,
									int &aX2, int &aY2, Color::GLColor &aColor2) const{
		int width_1 = static_cast<int>(m_CurFrameBuffers.width()) - 1;
		int height_1 = static_cast<int>(m_CurFrameBuffers.height()) - 1;

		int p1_code = LCC_IN;
		int p2_code = LCC_IN;

		// determine codes for p1 and p2
		if(aY1 < 0){
			p1_code |= LCC_N;
		}else if(aY1 > height_1){
			p1_code |= LCC_S;
		}

		if(aX1 < 0){
			p1_code |= LCC_W;
		}else if(aX1 > width_1){
			p1_code |= LCC_E;
		}

		if(aY2 < 0){
			p2_code |= LCC_N;
		}else if(aY2 > height_1){
			p2_code |= LCC_S;
		}

		if(aX2 < 0){
			p2_code |= LCC_W;
		}else if(aX2 > width_1){
			p2_code |= LCC_E;
		}

		// test for totally visible, if so leave points untouched
		if(p1_code == LCC_IN && p2_code == LCC_IN){
			return true;
		}

		// test for totally invisible
		if(((p1_code & LCC_E) && (p2_code & LCC_E)) ||
			((p1_code & LCC_W) && (p2_code & LCC_W)) ||
			((p1_code & LCC_N) && (p2_code & LCC_N)) ||
			((p1_code & LCC_S) && (p2_code & LCC_S))){
			return false;
		}

		int xc1 = aX1;
		int yc1 = aY1;
		int xc2 = aX2;
		int yc2 = aY2;

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;

		Real dx = Real(aX2 - aX1) / Real(aY2 - aY1);
		Real dy = Real(aY2 - aY1) / Real(aX2 - aX1);
		// determin end clip point for p1
		switch(p1_code){
			case LCC_IN:
			{
				break;
			}

			case LCC_N:
			{
				yc1 = 0;
				xc1 = aX1 + 0.5 + (-aY1) * dx;
				c1 = aColor1 + (-aY1) * (aColor2 - aColor1) / (aY2 - aY1);
				break;
			}

			case LCC_S:
			{
				yc1 = height_1;
				xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;
				c1 = aColor1 + (height_1 - aY1) * (aColor2 - aColor1) / (aY2 - aY1);
				break;
			}

			case LCC_W:
			{
				xc1 = 0;
				yc1 = aY1 + 0.5 + (-aX1) * dy;
				c1 = aColor1 + (-aX1) * (aColor2 - aColor1) / (aX2 - aX1);
				break;
			}

			case LCC_E:
			{
				xc1 = width_1;
				yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
				c1 = aColor1 + (width_1 - aX1) * (aColor2 - aColor1) / (aX2 - aX1);
				break;
			}

			// these cases are more complex, must compute 2 intersections
			case LCC_NE:
			{
				// north hline intersection
				yc1 = 0;
				xc1 = aX1 + 0.5 + (-aY1) * dx;

				// test if intersecion is valid,
				// if so then done, else compute next
				if(xc1 < 0 || xc1 > width_1){
					// east vline intersecion
					xc1 = width_1;
					yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
					c1 = aColor1 + (width_1 - aX1) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c1 = aColor1 + (-aY1) * (aColor2 - aColor1) / (aY2 - aY1);

				break;
			}

			case LCC_SE:
			{
				yc1 = height_1;
				xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;

				if(xc1 < 0 || xc1 > width_1){
					// east vline intersection
					xc1 = width_1;
					yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
					c1 = aColor1 + (width_1 - aX1) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c1 = aColor1 + (height_1 - aY1) * (aColor2 - aColor1) / (aY2 - aY1);

				break;
			}

			case LCC_NW:
			{
				yc1 = 0;
				xc1 = aX1 + 0.5 + (-aY1) * dx;

				if(xc1 < 0 || xc1 > width_1){
					xc1 = 0;
					yc1 = aY1 + 0.5 + (-aX1) * dy;
					c1 = aColor1 + (-aX1) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c1 = aColor1 + (-aY1) * (aColor2 - aColor1) / (aY2 - aY1);
				
				break;
			}

			case LCC_SW:
			{
				yc1 = height_1;
				xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;

				if(xc1 < 0 || xc1 > width_1){
					xc1 = 0;
					yc1 = aY1 + 0.5 + (-aX1) * dy;
					c1 = aColor1 + (-aX1) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c1 = aColor1 + (height_1 - aY1) * (aColor2 - aColor1) / (aY2 - aY1);

				break;
			}

			default:
				break;
		} //#### end swith p1

		// determine clip pointer for p2
		switch(p2_code){
			case LCC_IN:
			{
				break;
			}

			case LCC_N:
			{
				yc2 = 0;
				xc2 = aX2 + 0.5 + (-aY2) * dx;
				c2 = aColor2 + (-aY2) * (aColor2 - aColor1) / (aY2 - aY1);
				break;
			}

			case LCC_S:
			{
				yc2 = height_1;
				xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;
				c2 = aColor2 + (height_1 - aY2) * (aColor2 - aColor1) / (aY2 - aY1);
				break;
			}

			case LCC_W:
			{
				xc2 = 0;
				yc2 = aY2 + 0.5 + (-aX2) * dy;
				c2 = aColor2 + (-aX2) * (aColor2 - aColor1) / (aX2 - aX1);
				break;
			}

			case LCC_E:
			{
				xc2 = width_1;
				yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
				c2 = aColor2 + (width_1 - aX2) * (aColor2 - aColor1) / (aX2 - aX1);
				break;
			}

			// these cases are more complex, must compute 2 intersections
			case LCC_NE:
			{
				// north hline intersection
				yc2 = 0;
				xc2 = aX2 + 0.5 + (-aY2) * dx;

				// test if intersecion is valid,
				// if so then done, else compute next
				if(xc2 < 0 || xc2 > width_1){
					// ease vline intersecion
					xc2 = width_1;
					yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
					c2 = aColor2 + (width_1 - aX2) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c2 = aColor2 + (-aY2) * (aColor2 - aColor1) / (aY2 - aY1);

				break;
			}

			case LCC_SE:
			{
				yc2 = height_1;
				xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;

				if(xc2 < 0 || xc2 > width_1){
					// ease vline intersection
					xc2 = width_1;
					yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
					c2 = aColor2 + (width_1 - aX2) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c2 = aColor2 + (height_1 - aY2) * (aColor2 - aColor1) / (aY2 - aY1);

				break;
			}

			case LCC_NW:
			{
				yc2 = 0;
				xc2 = aX2 + 0.5 + (-aY2) * dx;

				if(xc2 < 0 || xc2 > width_1){
					xc2 = 0;
					yc2 = aY2 + 0.5 + (-aX2) * dy;
					c2 = aColor2 + (-aX2) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c2 = aColor2 + (-aY2) * (aColor2 - aColor1) / (aY2 - aY1);
				
				break;
			}

			case LCC_SW:
			{
				yc2 = height_1;
				xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;

				if(xc2 < 0 || xc2 > width_1){
					xc2 = 0;
					yc2 = aY2 + 0.5 + (-aX2) * dy;
					c2 = aColor2 + (-aX2) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				c2 = aColor2 + (height_1 - aY2) * (aColor2 - aColor1) / (aY2 - aY1);

				break;
			}

			default:
				break;
		} //#### end swith p2

		// do bounds check
		if((xc1 < 0) || (xc1 > width_1) ||
			(yc1 < 0) || (yc1 > height_1) ||
			(xc2 < 0) || (xc2 > width_1) ||
			(yc2 < 0) || (yc2 > height_1)){
			return false;
		}

		// store vars back
		aX1 = xc1;
		aY1 = yc1;
		aX2 = xc2;
		aY2 = yc2;

		aColor1 = c1;
		aColor2 = c2;

		return true;
	} //#### end clipLinesInScreen

	//  [1/21/2009 zhangxiang]
	bool SagiRenderer::clipLinesInScreen(int &aX1, int &aY1, int &aX2, int &aY2) const{
		int width_1 = static_cast<int>(m_CurFrameBuffers.width()) - 1;
		int height_1 = static_cast<int>(m_CurFrameBuffers.height()) - 1;

		int xc1 = aX1;
		int yc1 = aY1;
		int xc2 = aX2;
		int yc2 = aY2;

		int p1_code = LCC_IN;
		int p2_code = LCC_IN;

		// determine codes for p1 and p2
		if(aY1 < 0){
			p1_code |= LCC_N;
		}else if(aY1 > height_1){
			p1_code |= LCC_S;
		}

		if(aX1 < 0){
			p1_code |= LCC_W;
		}else if(aX1 > width_1){
			p1_code |= LCC_E;
		}

		if(aY2 < 0){
			p2_code |= LCC_N;
		}else if(aY2 > height_1){
			p2_code |= LCC_S;
		}

		if(aX2 < 0){
			p2_code |= LCC_W;
		}else if(aX2 > width_1){
			p2_code |= LCC_E;
		}

		// test for totally visible, if so leave points untouched
		if(p1_code == LCC_IN && p2_code == LCC_IN){
			return true;
		}

		// test for totally invisible
		if(((p1_code & LCC_E) && (p2_code & LCC_E)) ||
			((p1_code & LCC_W) && (p2_code & LCC_W)) ||
			((p1_code & LCC_N) && (p2_code & LCC_N)) ||
			((p1_code & LCC_S) && (p2_code & LCC_S))){
			return false;
		}

		Real dx = Real(aX2 - aX1) / Real(aY2 - aY1);
		Real dy = Real(aY2 - aY1) / Real(aX2 - aX1);
		// determin end clip point for p1
		switch(p1_code){
			case LCC_IN:
				{
					break;
				}

			case LCC_N:
				{
					yc1 = 0;
					xc1 = aX1 + 0.5 + (-aY1) * dx;
					break;
				}

			case LCC_S:
				{
					yc1 = height_1;
					xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;
					break;
				}

			case LCC_W:
				{
					xc1 = 0;
					yc1 = aY1 + 0.5 + (-aX1) * dy;
					break;
				}

			case LCC_E:
				{
					xc1 = width_1;
					yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
					break;
				}

				// these cases are more complex, must compute 2 intersections
			case LCC_NE:
				{
					// north hline intersection
					yc1 = 0;
					xc1 = aX1 + 0.5 + (-aY1) * dx;

					// test if intersecion is valid,
					// if so then done, else compute next
					if(xc1 < 0 || xc1 > width_1){
						// east vline intersecion
						xc1 = width_1;
						yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
					}

					break;
				}

			case LCC_SE:
				{
					yc1 = height_1;
					xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;

					if(xc1 < 0 || xc1 > width_1){
						// east vline intersection
						xc1 = width_1;
						yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
					}

					break;
				}

			case LCC_NW:
				{
					yc1 = 0;
					xc1 = aX1 + 0.5 + (-aY1) * dx;

					if(xc1 < 0 || xc1 > width_1){
						xc1 = 0;
						yc1 = aY1 + 0.5 + (-aX1) * dy;
					}

					break;
				}

			case LCC_SW:
				{
					yc1 = height_1;
					xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;

					if(xc1 < 0 || xc1 > width_1){
						xc1 = 0;
						yc1 = aY1 + 0.5 + (-aX1) * dy;
					}

					break;
				}

			default:
				break;
		} //#### end swith p1

		// determine clip pointer for p2
		switch(p2_code){
			case LCC_IN:
				{
					break;
				}

			case LCC_N:
				{
					yc2 = 0;
					xc2 = aX2 + 0.5 + (-aY2) * dx;
					break;
				}

			case LCC_S:
				{
					yc2 = height_1;
					xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;
					break;
				}

			case LCC_W:
				{
					xc2 = 0;
					yc2 = aY2 + 0.5 + (-aX2) * dy;
					break;
				}

			case LCC_E:
				{
					xc2 = width_1;
					yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
					break;
				}

				// these cases are more complex, must compute 2 intersections
			case LCC_NE:
				{
					// north hline intersection
					yc2 = 0;
					xc2 = aX2 + 0.5 + (-aY2) * dx;

					// test if intersecion is valid,
					// if so then done, else compute next
					if(xc2 < 0 || xc2 > width_1){
						// ease vline intersecion
						xc2 = width_1;
						yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
					}

					break;
				}

			case LCC_SE:
				{
					yc2 = height_1;
					xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;

					if(xc2 < 0 || xc2 > width_1){
						// ease vline intersetion
						xc2 = width_1;
						yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
					}

					break;
				}

			case LCC_NW:
				{
					yc2 = 0;
					xc2 = aX2 + 0.5 + (-aY2) * dx;

					if(xc2 < 0 || xc2 > width_1){
						xc2 = 0;
						yc2 = aY2 + 0.5 + (-aX2) * dy;
					}

					break;
				}

			case LCC_SW:
				{
					yc2 = height_1;
					xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;

					if(xc2 < 0 || xc2 > width_1){
						xc2 = 0;
						yc2 = aY2 + 0.5 + (-aX2) * dy;
					}

					break;
				}

			default:
				break;
		} //#### end swith p2

		// do bounds check
		if((xc1 < 0) || (xc1 > width_1) ||
			(yc1 < 0) || (yc1 > height_1) ||
			(xc2 < 0) || (xc2 > width_1) ||
			(yc2 < 0) || (yc2 > height_1)){
			return false;
		}

		// store vars back
		aX1 = xc1;
		aY1 = yc1;
		aX2 = xc2;
		aY2 = yc2;

		return true;
	}

	//  [1/22/2009 zhangxiang]
	bool SagiRenderer::clipLinesInScreen(int &aX1, int &aY1, Real &aZ1, Color::GLColor &aColor1,
										int &aX2, int &aY2, Real &aZ2, Color::GLColor &aColor2) const{
		int width_1 = static_cast<int>(m_CurFrameBuffers.width()) - 1;
		int height_1 = static_cast<int>(m_CurFrameBuffers.height()) - 1;

		int p1_code = LCC_IN;
		int p2_code = LCC_IN;

		// determine codes for p1 and p2
		if(aY1 < 0){
			p1_code |= LCC_N;
		}else if(aY1 > height_1){
			p1_code |= LCC_S;
		}

		if(aX1 < 0){
			p1_code |= LCC_W;
		}else if(aX1 > width_1){
			p1_code |= LCC_E;
		}

		if(aY2 < 0){
			p2_code |= LCC_N;
		}else if(aY2 > height_1){
			p2_code |= LCC_S;
		}

		if(aX2 < 0){
			p2_code |= LCC_W;
		}else if(aX2 > width_1){
			p2_code |= LCC_E;
		}

		// test for totally visible, if so leave points untouched
		if(p1_code == LCC_IN && p2_code == LCC_IN){
			return true;
		}

		// test for totally invisible
		if(((p1_code & LCC_E) && (p2_code & LCC_E)) ||
			((p1_code & LCC_W) && (p2_code & LCC_W)) ||
			((p1_code & LCC_N) && (p2_code & LCC_N)) ||
			((p1_code & LCC_S) && (p2_code & LCC_S))){
			return false;
		}

		int xc1 = aX1;
		int yc1 = aY1;
		int xc2 = aX2;
		int yc2 = aY2;
		Real zc1 = aZ1;
		Real zc2 = aZ2;

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;

		Real invDx = 1.0f / Real(aX2 - aX1);
		Real invDy = 1.0f / Real(aY2 - aY1);
		Real dx = Real(aX2 - aX1) * invDy;
		Real dy = Real(aY2 - aY1) * invDx;
		// determin end clip point for p1
		switch(p1_code){
			case LCC_IN:
			{
				break;
			}

			case LCC_N:
			{
				yc1 = 0;
				xc1 = aX1 + 0.5 + (-aY1) * dx;
				zc1 = aZ1 + (-aY1) * (aZ2 - aZ1) * invDy;
				c1 = aColor1 + (-aY1) * (aColor2 - aColor1) * invDy;
				break;
			}

			case LCC_S:
			{
				yc1 = height_1;
				xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;
				zc1 = aZ1 + (height_1 - aY1) * (aZ2 - aZ1) * invDy;
				c1 = aColor1 + (height_1 - aY1) * (aColor2 - aColor1) * invDy;
				break;
			}

			case LCC_W:
			{
				xc1 = 0;
				yc1 = aY1 + 0.5 + (-aX1) * dy;
				zc1 = aZ1 + (-aX1) * (aZ2 - aZ1) * invDx;
				c1 = aColor1 + (-aX1) * (aColor2 - aColor1) * invDx;
				break;
			}

			case LCC_E:
			{
				xc1 = width_1;
				yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
				zc1 = aZ1 + (width_1 - aX1) * (aZ2 - aZ1) * invDx;
				c1 = aColor1 + (width_1 - aX1) * (aColor2 - aColor1) * invDx;
				break;
			}

			// these cases are more complex, must compute 2 intersections
			case LCC_NE:
			{
				// north hline intersection
				yc1 = 0;
				xc1 = aX1 + 0.5 + (-aY1) * dx;

				// test if intersecion is valid,
				// if so then done, else compute next
				if(xc1 < 0 || xc1 > width_1){
					// east vline intersecion
					xc1 = width_1;
					yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
					zc1 = aZ1 + (width_1 - aX1) * (aZ2 - aZ1) * invDx;
					c1 = aColor1 + (width_1 - aX1) * (aColor2 - aColor1) * invDx;
					break;
				}

				zc1 = aZ1 + (-aY1) * (aZ2 - aZ1) * invDy;
				c1 = aColor1 + (-aY1) * (aColor2 - aColor1) * invDy;

				break;
			}

			case LCC_SE:
			{
				yc1 = height_1;
				xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;

				if(xc1 < 0 || xc1 > width_1){
					// east vline intersection
					xc1 = width_1;
					yc1 = aY1 + 0.5 + (width_1 - aX1) * dy;
					zc1 = aZ1 + (width_1 - aX1) * (aZ2 - aZ1) * invDx;
					c1 = aColor1 + (width_1 - aX1) * (aColor2 - aColor1) * invDx;
					break;
				}

				zc1 = aZ1 + (height_1 - aY1) * (aZ2 - aZ1) * invDy;
				c1 = aColor1 + (height_1 - aY1) * (aColor2 - aColor1) * invDy;

				break;
			}

			case LCC_NW:
			{
				yc1 = 0;
				xc1 = aX1 + 0.5 + (-aY1) * dx;

				if(xc1 < 0 || xc1 > width_1){
					xc1 = 0;
					yc1 = aY1 + 0.5 + (-aX1) * dy;
					zc1 = aZ1 + (-aX1) * (aZ2 - aZ1) * invDx;
					c1 = aColor1 + (-aX1) * (aColor2 - aColor1) * invDx;
					break;
				}

				zc1 = aZ1 + (-aY1) * (aZ2 - aZ1) * invDy;
				c1 = aColor1 + (-aY1) * (aColor2 - aColor1) * invDy;
				
				break;
			}

			case LCC_SW:
			{
				yc1 = height_1;
				xc1 = aX1 + 0.5 + (height_1 - aY1) * dx;

				if(xc1 < 0 || xc1 > width_1){
					xc1 = 0;
					yc1 = aY1 + 0.5 + (-aX1) * dy;
					zc1 = aZ1 + (-aX1) * (aZ2 - aZ1) * invDx;
					c1 = aColor1 + (-aX1) * (aColor2 - aColor1) * invDx;
					break;
				}

				zc1 = aZ1 + (height_1 - aY1) * (aZ2 - aZ1) * invDy;
				c1 = aColor1 + (height_1 - aY1) * (aColor2 - aColor1) * invDy;

				break;
			}

			default:
				break;
		} //#### end swith p1

		// determine clip pointer for p2
		switch(p2_code){
			case LCC_IN:
			{
				break;
			}

			case LCC_N:
			{
				yc2 = 0;
				xc2 = aX2 + 0.5 + (-aY2) * dx;
				zc2 = aZ2 + (-aY2) * (aZ2 - aZ1) * invDy;
				c2 = aColor2 + (-aY2) * (aColor2 - aColor1) * invDy;
				break;
			}

			case LCC_S:
			{
				yc2 = height_1;
				xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;
				zc2 = aZ2 + (height_1 - aY2) * (aZ2 - aZ1) * invDy;
				c2 = aColor2 + (height_1 - aY2) * (aColor2 - aColor1) * invDy;
				break;
			}

			case LCC_W:
			{
				xc2 = 0;
				yc2 = aY2 + 0.5 + (-aX2) * dy;
				zc2 = aZ2 + (-aX2) * (aZ2 - aZ1) * invDx;
				c2 = aColor2 + (-aX2) * (aColor2 - aColor1) * invDx;
				break;
			}

			case LCC_E:
			{
				xc2 = width_1;
				yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
				zc2 = aZ2 + (width_1 - aX2) * (aZ2 - aZ1) * invDx;
				c2 = aColor2 + (width_1 - aX2) * (aColor2 - aColor1) * invDx;
				break;
			}

			// these cases are more complex, must compute 2 intersections
			case LCC_NE:
			{
				// north hline intersection
				yc2 = 0;
				xc2 = aX2 + 0.5 + (-aY2) * dx;

				// test if intersecion is valid,
				// if so then done, else compute next
				if(xc2 < 0 || xc2 > width_1){
					// ease vline intersecion
					xc2 = width_1;
					yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
					zc2 = aZ2 + (width_1 - aX2) * (aZ2 - aZ1) * invDx;
					c2 = aColor2 + (width_1 - aX2) * (aColor2 - aColor1) * invDx;
					break;
				}

				zc2 = aZ2 + (-aY2) * (aZ2 - aZ1) * invDy;
				c2 = aColor2 + (-aY2) * (aColor2 - aColor1) * invDy;

				break;
			}

			case LCC_SE:
			{
				yc2 = height_1;
				xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;

				if(xc2 < 0 || xc2 > width_1){
					// ease vline intersection
					xc2 = width_1;
					yc2 = aY2 + 0.5 + (width_1 - aX2) * dy;
					zc2 = aZ2 + (width_1 - aX2) * (aZ2 - aZ1) * invDx;
					c2 = aColor2 + (width_1 - aX2) * (aColor2 - aColor1) * invDx;
					break;
				}

				zc2 = aZ2 + (height_1 - aY2) * (aZ2 - aZ1) * invDy;
				c2 = aColor2 + (height_1 - aY2) * (aColor2 - aColor1) * invDy;

				break;
			}

			case LCC_NW:
			{
				yc2 = 0;
				xc2 = aX2 + 0.5 + (-aY2) * dx;

				if(xc2 < 0 || xc2 > width_1){
					xc2 = 0;
					yc2 = aY2 + 0.5 + (-aX2) * dy;
					zc2 = aZ2 + (-aX2) * (aZ2 - aZ1) * invDx;
					c2 = aColor2 + (-aX2) * (aColor2 - aColor1) * invDx;
					break;
				}

				zc2 = aZ2 + (-aY2) * (aZ2 - aZ1) * invDy;
				c2 = aColor2 + (-aY2) * (aColor2 - aColor1) * invDy;
				
				break;
			}

			case LCC_SW:
			{
				yc2 = height_1;
				xc2 = aX2 + 0.5 + (height_1 - aY2) * dx;

				if(xc2 < 0 || xc2 > width_1){
					xc2 = 0;
					yc2 = aY2 + 0.5 + (-aX2) * dy;
					zc2 = aZ2 + (-aX2) * (aZ2 - aZ1) * invDx;
					c2 = aColor2 + (-aX2) * (aColor2 - aColor1) / (aX2 - aX1);
					break;
				}

				zc2 = aZ2 + (height_1 - aY2) * (aZ2 - aZ1) * invDy;
				c2 = aColor2 + (height_1 - aY2) * (aColor2 - aColor1) * invDy;

				break;
			}

			default:
				break;
		} //#### end swith p2

		// do bounds check
		if((xc1 < 0) || (xc1 > width_1) ||
			(yc1 < 0) || (yc1 > height_1) ||
			(xc2 < 0) || (xc2 > width_1) ||
			(yc2 < 0) || (yc2 > height_1)){
			return false;
		}

		// store vars back
		aX1 = xc1;
		aY1 = yc1;
		aX2 = xc2;
		aY2 = yc2;
		aZ1 = zc1;
		aZ2 = zc2;

		aColor1 = c1;
		aColor2 = c2;

		return true;
	}

	//  [1/16/2009 zhangxiang]
	void SagiRenderer::lineColorInterpolation(int aX1, int aY1, const Color::GLColor &aColor1,
											int aX2, int aY2, const Color::GLColor &aColor2) const{
		if(aY1 == aY2){
			// hline
			hlineColorInterpolation(aX1, aX2, aY1, aColor1, aColor2);
			return ;
		}
												
		Color::GLColor *pcolorbuffer = static_cast<Color::GLColor*>(m_CurFrameBuffers.getColorBuffer()->data());
		int topwidth = static_cast<int>(m_CurFrameBuffers.getTopParentWidth());
		int width = static_cast<int>(m_CurFrameBuffers.width());
		int height = static_cast<int>(m_CurFrameBuffers.height());

		// this function draws a line from aX1, aY1 to aX2, aY2
		// using differential error
		// terms (based on Bresenhams work)

		int Dx,		// difference in x's
			Dy,		// difference in y's
			Dx2,	// Dx * 2
			Dy2,	// Dy * 2
			x_inc,	// amount in pixel space to move during drawing
			y_inc,
			error,	// the discriminant i.e. error i.e. decision variable
			index;	// used for looping

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;

		// clip first
		if(!clipLinesInScreen(aX1, aY1, c1, aX2, aY2, c2))
			return ;

		if(aY1 == aY2){
			hlineColorInterpolation(aX1, aX2, aY1, c1, c2);
			return ;
		}

		// pre-compute first pixel address in color buffer
		pcolorbuffer += aY1 * topwidth + aX1;

		// compute horizontal and vertical deltas
		Dx = aX2 - aX1;
		Dy = aY2 - aY1;

		Color::GLColor dc;
		if(Dx > Dy){
			// set different of color
			dc = (c2 - c1)  / Dx;
		}else{
			// set different of color
			dc = (c2 - c1) / Dy;
		}

		Color::GLColor hc = c1;

		// test which direction the line is going in i.e. slope angle
		if(Dx >= 0){
			// line is moving right
			x_inc = 1;
		}else{
			// moving left
			x_inc = -1;
			Dx = -Dx;	// need absolute value
		}

		// test y component of slope
		if(Dy >= 0){
			// line is moving down
			y_inc = topwidth;
		}else{
			// moving up
			y_inc = -topwidth;
			Dy = -Dy;
		}

		// compute (Dx, Dy) * 2
		Dx2 = Dx << 1;
		Dy2 = Dy << 1;

		// now based on which delta is greater we can draw the line
		if(Dx > Dy){
			// initialize error term
			error = Dy2 - Dx;

			// draw the line
			for(index=0; index<=Dx; ++index){
				// set the pixel
				//pcolorbuffer[hy * topwidth + hx.ceil()] = hc
				*pcolorbuffer = hc;

				// test if error has overflowed
				if(error >= 0){
					error -= Dx2;

					// move to next line
					pcolorbuffer += y_inc;
				}

				// adjust the error term
				error += Dy2;

				// move to the next pixel
				pcolorbuffer += x_inc;

				// next x's color
				hc += dc;
			}
		}else{
			// initialize error term
			error = Dx2 - Dy;

			// draw the line
			for(index=0; index<=Dy; ++index){
				// set the pixel
				*pcolorbuffer = hc;

				// test if error overflowed
				if(error >= 0){
					error -= Dy2;

					// move to next line
					pcolorbuffer += x_inc;
				}

				// adjust the error term
				error += Dx2;

				// move to the next pixel
				pcolorbuffer += y_inc;

				// next y's color
				hc += dc;
			}
		}

/*		Color::GLColor c1 = aColor1;//.toGLColor();
		Color::GLColor c2 = aColor2;//.toGLColor();

		// ascending sort by y -- v1, v2, v3		------ scan line ------
		if(aY2 < aY1){								// |	v1
			// let y1 <= y2							// |	
			std::swap(aX1, aX2);					// |	v2
			std::swap(aY1, aY2);					//\|/
			std::swap(c1, c2);						
		}

		// scissor
		if(aY2 < 0 || aY1 >= height ||
			(aX1 < 0 && aX2 < 0) ||
			(aX1 >= width && aX2 >= width)){
			return ;
		}
*/
		// scissor for case:
		/*				B			B
						/			\
					   /	-------  \
				      /    |	   |  \
				B    /     |	   |   \A	 B
				\	A	    -------			/
				 \						   /
				  \	A				    A /
		*/
		/*Vector2 A(aX2, aY2);
		Vector2 AB = Vector2(aX1, aY1) - A;
		if((AB.crossProduct(-A) >= 0 && AB.crossProduct(Vector2(0, height-1)) >= 0) ||
			(AB.crossProduct(Vector2(width-1, 0)) <= 0 && AB.crossProduct(Vector2(width-1, height-1)) <= 0)){
			return ;
		}*/

		// test if horizontal line
/*		if(aY1 == aY2){
			// horizontal line
			hlineColorInterpolation(aX1, aX2, aY1, c1, c2);
			return ;
		}

		// part or total in the area
		Real invDy = 1.0 / Real(aY1 - aY2);
		Fixed16 dx(Real(aX1 - aX2) * invDy);

		
		Color::GLColor dyc = (c1 - c2) * invDy;
		

		int hy = aY1;
		Fixed16 hx(aX1);
		Color::GLColor hc = c1;
		
		// scissor
		if(hy < 0){
			hx += dx * (-hy);
			hc += dyc * (-hy);
			hy = 0;
		}
		
		if((hx < 0 && aX2 < hx) ||
			(hx >= width && aX2 >hx)){
			return ;
		}

		if(hx < 0 || hx >= width){
			Real invDx = 1.0 / Real(aX1 - aX2);
			Fixed16 dy(Real(aY1 - aY2) * invDx);
			Color::GLColor dxc = (c1 - c2) * invDx;
			
			if(hx < 0){
				hy += (dy * (-hx)).ceil();
				hc += dxc * (-hx).toReal();
				hx = 0;
			}else if(hx >= width){
				int deltax = hx.ceil() - width + 1;
				hy -= (dy * deltax).ceil();
				hc -= dxc * deltax;
				hx = width - 1;
			}
		}

		if(hy < 0){
			return ;
		}

		if(aY2 >= height){
			aY2 = height - 1;
		}
*/
		// interpolate
		/*while(hy <= aY2 && hx < width && hx >= 0){
			pcolorbuffer[hy * topwidth + int(hx)] = Color(hc);
			++hy;
			hx += dx;
			hc += dyc;
		}*/
/*		if(dx > 0.0f){
			if(dx < 1.5f){
				while(hy < aY2 && hx < width){
					pcolorbuffer[hy * topwidth + hx.ceil()] = hc;
					++hy;
					hx += dx;
					hc += dyc;
				}
			}else{
				Fixed16 prehx = hx;
				hx += dx;
				Color::GLColor prehc = hc;
				hc += dyc;
				while(hy < aY2 && hx < width){
					hlineColorInterpolation(prehx.ceil(), hx.ceil(), hy, prehc, hc);
					prehx = hx + 1;
					hx += dx;
					hc += dyc;
					++hy;
				}
			}
		}else{
			if(dx > -1.5f){
				while(hy < aY2 && hx >= 0){
					pcolorbuffer[hy * topwidth + hx.ceil()] = hc;
					++hy;
					hx += dx;
					hc += dyc;
				}
			}else{
				Fixed16 prehx = hx;
				hx += dx;
				Color::GLColor prehc = hc;
				hc += dyc;
				while(hy < aY2 && hx >= 0){
					hlineColorInterpolation(prehx.ceil(), hx.ceil(), hy, prehc, hc);
					prehx = hx - 1;
					hx += dx;
					hc += dyc;
					++hy;
				}
			}
		}
*/
	}

	//  [1/22/2009 zhangxiang]
	void SagiRenderer::lineColorInterpolation(const Vector3 &aV1, const Color::GLColor &aColor1,
											const Vector3 &aV2, const Color::GLColor &aColor2) const{
		int aX1 = static_cast<int>(aV1.x());
		int aY1 = static_cast<int>(aV1.y());
		Real aZ1 = aV1.z();

		int aX2 = static_cast<int>(aV2.x());
		int aY2 = static_cast<int>(aV2.y());
		Real aZ2 = aV2.z();

		if(aY1 == aY2){
			// hline
			hlineColorInterpolation(aX1, aZ1, aX2, aZ2, aY1, aColor1, aColor2);
			return ;
		}
												
		Color::GLColor *pcolorbuffer = static_cast<Color::GLColor*>(m_CurFrameBuffers.getColorBuffer()->data());
		Real *pdepthbuffer = static_cast<Real*>(m_CurFrameBuffers.getDepthBuffer()->data());
		int topwidth = static_cast<int>(m_CurFrameBuffers.getTopParentWidth());
		int width = static_cast<int>(m_CurFrameBuffers.width());
		int height = static_cast<int>(m_CurFrameBuffers.height());

		// this function draws a line from aX1, aY1 to aX2, aY2
		// using differential error
		// terms (based on Bresenhams work)

		int Dx,		// difference in x's
			Dy,		// difference in y's
			Dx2,	// Dx * 2
			Dy2,	// Dy * 2
			x_inc,	// amount in pixel space to move during drawing
			y_inc,
			error,	// the discriminant i.e. error i.e. decision variable
			index;	// used for looping

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;

		// clip first
		if(!clipLinesInScreen(aX1, aY1, aZ1, c1, aX2, aY2, aZ2, c2))
			return ;

		if(aY1 == aY2){
			hlineColorInterpolation(aX1, aZ1, aX2, aZ2, aY1, c1, c2);
			return ;
		}

		// pre-compute first pixel address in color buffer and depth buffer
		size_t pitch = aY1 * topwidth + aX1;
		pcolorbuffer += pitch;
		pdepthbuffer += pitch;

		// compute horizontal and vertical deltas
		Dx = aX2 - aX1;
		Dy = aY2 - aY1;

		Real dz;
		Color::GLColor dc;
		if(Dx > Dy){
			Real invDx = 1.0f / Real(Dx);
			// set different of z value
			dz = (aZ2 - aZ1) * invDx;
			// set different of color
			dc = (c2 - c1) * invDx;
		}else{
			Real invDy = 1.0f / Real(Dy);
			// set different of z value
			dz = (aZ2 - aZ1) * invDy;
			// set different of color
			dc = (c2 - c1) * invDy;
		}


		Real hz = aZ1;
		Color::GLColor hc = c1;

		// test which direction the line is going in i.e. slope angle
		if(Dx >= 0){
			// line is moving right
			x_inc = 1;
		}else{
			// moving left
			x_inc = -1;
			Dx = -Dx;	// need absolute value
		}

		// test y component of slope
		if(Dy >= 0){
			// line is moving down
			y_inc = topwidth;
		}else{
			// moving up
			y_inc = -topwidth;
			Dy = -Dy;
		}

		// compute (Dx, Dy) * 2
		Dx2 = Dx << 1;
		Dy2 = Dy << 1;

		// now based on which delta is greater we can draw the line
		if(Dx > Dy){
			// initialize error term
			error = Dy2 - Dx;

			// draw the line
			for(index=0; index<=Dx; ++index){
				// set the pixel
				if(*pdepthbuffer  > hz){
					*pcolorbuffer = hc;
					*pdepthbuffer = hz;
				}

				// test if error has overflowed
				if(error >= 0){
					error -= Dx2;

					// move to next line
					pcolorbuffer += y_inc;
					pdepthbuffer += y_inc;
				}

				// adjust the error term
				error += Dy2;

				// move to the next pixel
				pcolorbuffer += x_inc;
				pdepthbuffer += x_inc;

				// next x's z
				hz += dz;

				// next x's color
				hc += dc;
			}
		}else{
			// initialize error term
			error = Dx2 - Dy;

			// draw the line
			for(index=0; index<=Dy; ++index){
				// set the pixel
				if(*pdepthbuffer  > hz){
					*pcolorbuffer = hc;
					*pdepthbuffer = hz;
				}
				// test if error overflowed
				if(error >= 0){
					error -= Dy2;

					// move to next line
					pcolorbuffer += x_inc;
					pdepthbuffer += x_inc;
				}

				// adjust the error term
				error += Dx2;

				// move to the next pixel
				pcolorbuffer += y_inc;
				pdepthbuffer += y_inc;

				// next y's z
				hz += dz;

				// next y's color
				hc += dc;
			}
		}
	} //#### end lineColorInterpolation

	//  [1/21/2009 zhangxiang]
	void SagiRenderer::lineInterpolation(int aX1, int aY1, int aX2, int aY2) const{
		if(aY1 == aY2){
			// hline
			hlineInterpolation(aX1, aX2, aY1);
			return ;
		}
												
		Color::GLColor *pcolorbuffer = static_cast<Color::GLColor*>(m_CurFrameBuffers.getColorBuffer()->data());
		int topwidth = static_cast<int>(m_CurFrameBuffers.getTopParentWidth());
		int width = static_cast<int>(m_CurFrameBuffers.width());
		int height = static_cast<int>(m_CurFrameBuffers.height());

		// this function draws a line from aX1, aY1 to aX2, aY2
		// using differential error
		// terms (based on Bresenhams work)

		int Dx,		// difference in x's
			Dy,		// difference in y's
			Dx2,	// Dx * 2
			Dy2,	// Dy * 2
			x_inc,	// amount in pixel space to move during drawing
			y_inc,
			error,	// the discriminant i.e. error i.e. decision variable
			index;	// used for looping

		// clip first
		if(!clipLinesInScreen(aX1, aY1, aX2, aY2))
			return ;

		Color::GLColor hc = Color::GLColor::UNIT_SCALE;

		// pre-compute first pixel address in color buffer
		pcolorbuffer += aY1 * topwidth + aX1;

		// compute horizontal and vertical deltas
		Dx = aX2 - aX1;
		Dy = aY2 - aY1;

		// test which direction the line is going in i.e. slope angle
		if(Dx >= 0){
			// line is moving right
			x_inc = 1;
		}else{
			// moving left
			x_inc = -1;
			Dx = -Dx;	// need absolute value
		}

		// test y component of slope
		if(Dy >= 0){
			// line is moving down
			y_inc = topwidth;
		}else{
			// moving up
			y_inc = -topwidth;
			Dy = -Dy;
		}

		// compute (Dx, Dy) * 2
		Dx2 = Dx << 1;
		Dy2 = Dy << 1;

		// now based on which delta is greater we can draw the line
		if(Dx > Dy){
			// initialize error term
			error = Dy2 - Dx;

			// draw the line
			for(index=0; index<=Dx; ++index){
				// set the pixel
				//pcolorbuffer[hy * topwidth + hx.ceil()] = hc
				*pcolorbuffer = hc;

				// test if error has overflowed
				if(error >= 0){
					error -= Dx2;

					// move to next line
					pcolorbuffer += y_inc;
				}

				// adjust the error term
				error += Dy2;

				// move to the next pixel
				pcolorbuffer += x_inc;
			}
		}else{
			// initialize error term
			error = Dx2 - Dy;

			// draw the line
			for(index=0; index<=Dy; ++index){
				// set the pixel
				*pcolorbuffer = hc;

				// test if error overflowed
				if(error >= 0){
					error -= Dy2;

					// move to next line
					pcolorbuffer += x_inc;
				}

				// adjust the error term
				error += Dx2;

				// move to the next pixel
				pcolorbuffer += y_inc;
			}
		}
	}

	//  [1/16/2009 zhangxiang]
	void SagiRenderer::hlineColorInterpolation(int aX1, int aX2, int aY,
											const Color::GLColor &aColor1,
											const Color::GLColor &aColor2) const{
		Color::GLColor *pcolorbuffer = static_cast<Color::GLColor*>(m_CurFrameBuffers.getColorBuffer()->data());
		size_t topwidth = m_CurFrameBuffers.getTopParentWidth();
		int width = static_cast<int>(m_CurFrameBuffers.width());

		if((aX1 < 0 && aX2 < 0) ||
			(aX1 >= width && aX2 >= width)){
				return ;
		}
		if(aX1 == aX2){
			// only a point, do nothing, because we use left-top filling policy
		//	pcolorbuffer[aY * topwidth + aX1] = (aColor1 + aColor2) / 2;
			return ;
		}

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;
		Color::GLColor dxc = (c1 - c2) / Real(aX1 - aX2);
		int lx, rx;
		Color::GLColor lc;
		if(aX1 < aX2){
			lx = aX1;
			lc = c1;
			rx = aX2;
		}else{
			lx = aX2;
			lc = c2;
			rx = aX1;
		}

		// scissor
		if(lx < 0){
			lc += dxc * (-lx);
			lx = 0;
		}else if(lx >= width){
			return ;
		}

		if(rx >= width){
			rx = width - 1;
		}

		// interpolate
		pcolorbuffer += aY * topwidth + lx;
		while(lx < rx){
			*pcolorbuffer = lc;
			++pcolorbuffer;
			++lx;
			lc += dxc;
		}
	}

	//  [1/22/2009 zhangxiang]
	void SagiRenderer::hlineColorInterpolation(int aX1, Real aZ1, int aX2, Real aZ2, int aY,
											const Color::GLColor &aColor1, const Color::GLColor &aColor2) const{
		Color::GLColor *pcolorbuffer = static_cast<Color::GLColor*>(m_CurFrameBuffers.getColorBuffer()->data());
		Real *pdepthbuffer = static_cast<Real*>(m_CurFrameBuffers.getDepthBuffer()->data());
		size_t topwidth = m_CurFrameBuffers.getTopParentWidth();
		int width = static_cast<int>(m_CurFrameBuffers.width());

		if((aX1 < 0 && aX2 < 0) ||
			(aX1 >= width && aX2 >= width)){
				return ;
		}
		if(aX1 == aX2){
			// only a point, do nothing, because we use left-top filling policy
		//	pcolorbuffer[aY * topwidth + aX1] = (aColor1 + aColor2) / 2;
			return ;
		}

		Real invDx = 1.0f / Real(aX1 - aX2);
		Real dz = (aZ1 - aZ2) * invDx;

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;
		Color::GLColor dxc = (c1 - c2) * invDx;
		int lx, rx;
		Real lz;
		Color::GLColor lc;
		if(aX1 < aX2){
			lx = aX1;
			lz = aZ1;
			lc = c1;
			rx = aX2;
		}else{
			lx = aX2;
			lz = aZ2;
			lc = c2;
			rx = aX1;
		}

		// scissor
		if(lx < 0){
			lc += dxc * (-lx);
			lz += dz * (-lx);
			lx = 0;
		}else if(lx >= width){
			return ;
		}

		if(rx >= width){
			rx = width - 1;
		}

		// interpolate
		size_t pitch = aY * topwidth + lx;
		pcolorbuffer += pitch;
		pdepthbuffer += pitch;
		while(lx < rx){
			if(*pdepthbuffer > lz){
				*pcolorbuffer = lc;
				*pdepthbuffer = lz;
			}
			++pcolorbuffer;
			++pdepthbuffer;
			++lx;
			lz += dz;
			lc += dxc;
		}
	}

	//  [1/21/2009 zhangxiang]
	void SagiRenderer::hlineInterpolation(int aX1, int aX2, int aY) const{
		Color::GLColor *pcolorbuffer = static_cast<Color::GLColor*>(m_CurFrameBuffers.getColorBuffer()->data());
		size_t topwidth = m_CurFrameBuffers.getTopParentWidth();
		int width = static_cast<int>(m_CurFrameBuffers.width());

		if((aX1 < 0 && aX2 < 0) ||
			(aX1 >= width && aX2 >= width)){
			return ;
		}
		if(aX1 == aX2){
			// only a point, do nothing, because we use left-top filling policy
			//	pcolorbuffer[aY * topwidth + aX1] = (aColor1 + aColor2) / 2;
			return ;
		}

		int lx, rx;
		Color::GLColor lc = Color::GLColor::UNIT_SCALE;
		if(aX1 < aX2){
			lx = aX1;
			rx = aX2;
		}else{
			lx = aX2;
			rx = aX1;
		}

		// scissor
		if(lx < 0){
			lx = 0;
		}else if(lx >= width){
			return ;
		}

		if(rx >= width){
			rx = width - 1;
		}

		// interpolate
		pcolorbuffer += aY * topwidth + lx;
		while(lx < rx){
			*pcolorbuffer = lc;
			++lx;
			++pcolorbuffer;
		}
	}

	//  [1/16/2009 zhangxiang]
	void SagiRenderer::renderTrianglesImpl(void) const{
		V3List &vertices = m_CurVertexBuffers.pvertices;
		
		size_t v1, v2, v3;
		IndexList::iterator iit = m_CurVertexBuffers.pindices.begin();
		if(!m_CurVertexBuffers.pcolors.empty()){
			if(m_CurRenderParam.pscene->getRenderState().isDepthTestEnable()){
				ColorList &colors = m_CurVertexBuffers.pcolors;
				for(size_t i=0; i<m_CurVertexBuffers.facenum; ++i){
					v1 = (*iit); ++iit;	// 3 * i
					v2 = (*iit); ++iit;	// 3 * i + 1
					v3 = (*iit); ++iit;	// 3 * i + 1
					triangleColorInterpolation(vertices[v1], colors[v1],
											vertices[v2], colors[v2],
											vertices[v3], colors[v3]);
				}
			}else{
				ColorList &colors = m_CurVertexBuffers.pcolors;
				for(size_t i=0; i<m_CurVertexBuffers.facenum; ++i){
					v1 = (*iit); ++iit;	// 3 * i
					v2 = (*iit); ++iit;	// 3 * i + 1
					v3 = (*iit); ++iit;	// 3 * i + 1
					triangleColorInterpolation(vertices[v1].x(), vertices[v1].y(), colors[v1],
											vertices[v2].x(), vertices[v2].y(), colors[v2],
											vertices[v3].x(), vertices[v3].y(), colors[v3]);
				}
			}
		}else{
			for(size_t i=0; i<m_CurVertexBuffers.facenum; ++i){
				v1 = (*iit); ++iit;	// 3 * i
				v2 = (*iit); ++iit;	// 3 * i + 1
				v3 = (*iit); ++iit;	// 3 * i + 1
				triangleInterpolation(vertices[v1].x(), vertices[v1].y(),
										vertices[v2].x(), vertices[v2].y(),
										vertices[v3].x(), vertices[v3].y());
			}
		}
	}

	//  [1/16/2009 zhangxiang]
	void SagiRenderer::triangleColorInterpolation(int aX1, int aY1, const Color::GLColor &aColor1,
												int aX2, int aY2, const Color::GLColor &aColor2,
												int aX3, int aY3, const Color::GLColor &aColor3) const{
		int width = static_cast<int>(m_CurFrameBuffers.width());
		int height = static_cast<int>(m_CurFrameBuffers.height());

		if((aX1 == aX2 && aX2 == aX3) ||
			(aY1 == aY2 && aY2 == aY3)){
			// only a line
			return ;
		}

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;
		Color::GLColor c3 = aColor3;

		// ascending sort by y -- v1, v2, v3		------ scan line ------
		if(aY2 < aY1){								// |	v1
			// let y1 <= y2							// |	
			std::swap(aX1, aX2);					// |	v2
			std::swap(aY1, aY2);					// |
			std::swap(c1, c2);						//\|/	v3
		}

		if(aY3 < aY1){
			// let y1 <= y3
			std::swap(aX3, aX1);
			std::swap(aY3, aY1);
			std::swap(c3, c1);
		}

		if(aY3 < aY2){
			// let y2 <= y3
			std::swap(aX3, aX2);
			std::swap(aY3, aY2);
			std::swap(c3, c2);
		}

		// scissor
		if(aY3 < 0 || aY1 >= height ||
			(aX1 < 0 && aX2 < 0 && aX3 < 0) ||
			(aX1 >= width && aX2 >= width && aX3 >= width)){
			return ;
		}

		Real invDy12, invDy13, invDy23;
		Fixed16 dx12, dx13, dx23;
		Color::GLColor dc12, dc13, dc23;
		invDy12 = invDy13 = invDy23 = 0;
	//	dx12 = dx13 = dx23 = 0;

		if(aY1 == aY2)
			goto V13;

		invDy12 = 1.0 / Real(aY1 - aY2);
		dx12 = Real(aX1 - aX2) * invDy12;
		dc12 = (c1 - c2) * invDy12;

V13:
		if(aY1 == aY3)	// only a point
			return ;
		
		invDy13 = 1.0 / Real(aY1 - aY3);
		dx13 = Real(aX1 - aX3) * invDy13;
		dc13 = (c1 - c3) * invDy13;


		if(aY2 == aY3)
			goto INTERPOLATION;

		invDy23 = 1.0 / Real(aY2 - aY3);
		dx23 = Real(aX2 - aX3) * invDy23;
		dc23 = (c2 - c3) * invDy23;

INTERPOLATION:
		int hy = aY1;
	//	Real hxl = aX1;
	//	Real hxr = aX1;
		Fixed16 hxl(aX1);
		Fixed16 hxr(aX1);
		Color::GLColor hcl = c1;
		Color::GLColor hcr = c1;
		if(hy < 0){
			if(aY1 == aY2){
			//	hxl = aX2 + dx23 * (-hy);
				hxl = dx23 * (-hy) + aX2;
				hxr += dx13 * (-hy);
				hcl = c2 + dc23 * (-hy);
				hcr += dc13 * (-hy);

				aY1 = aY2 = 0;
			//	aX2 = hxl;
				aX2 = hxl.ceil();
				c2 = hcl;
			}else{
				hxr += dx13 * (-hy);
				hcr += dc13 * (-hy);
				if(aY2 < 0){
				//	hxl = aX2 + dx23 * (-aY2);
					hxl = dx23 * (-aY2) + aX2;
					hcl = c2 + dc23 * (-aY2);

					aY2 = 0;
				//	aX2 = hxl;
					aX2 = hxl.ceil();
					c2 = hcl;
				}else{
					hxl += dx12 * (-hy);
					hcl += dc12 * (-hy);
				}
				aY1 = 0;
			}
			hy = 0;
		}

		while(hy < aY2 && hy < height){
			hlineColorInterpolation(hxl.ceil(), hxr.ceil(), hy, hcl, hcr);
			++hy;
			hxl += dx12;
			hxr += dx13;
			hcl += dc12;
			hcr += dc13;
		}

		// hy == aY2
		hxl = aX2;
		hcl = c2;

		while(hy < aY3 && hy < height){
			hlineColorInterpolation(hxl.ceil(), hxr.ceil(), hy, hcl, hcr);
			++hy;
			hxl += dx23;
			hxr += dx13;
			hcl += dc23;
			hcr += dc13;
		}
	}

	//  [1/22/2009 zhangxiang]
	void SagiRenderer::triangleColorInterpolation(const Vector3 &aV1, const Color::GLColor &aColor1,
												const Vector3 &aV2, const Color::GLColor &aColor2,
												const Vector3 &aV3, const Color::GLColor &aColor3) const{
		int width = static_cast<int>(m_CurFrameBuffers.width());
		int height = static_cast<int>(m_CurFrameBuffers.height());

		int aX1 = static_cast<int>(aV1.x());
		int aY1 = static_cast<int>(aV1.y());
		Real aZ1 = aV1.z();

		int aX2 = static_cast<int>(aV2.x());
		int aY2 = static_cast<int>(aV2.y());
		Real aZ2 = aV2.z();

		int aX3 = static_cast<int>(aV3.x());
		int aY3 = static_cast<int>(aV3.y());
		Real aZ3 = aV3.z();

		if((aX1 == aX2 && aX2 == aX3) ||
			(aY1 == aY2 && aY2 == aY3)){
			// only a line
			return ;
		}

		Color::GLColor c1 = aColor1;
		Color::GLColor c2 = aColor2;
		Color::GLColor c3 = aColor3;

		// ascending sort by y -- v1, v2, v3		------ scan line ------
		if(aY2 < aY1){								// |	v1
			// let y1 <= y2							// |	
			std::swap(aX1, aX2);					// |	v2
			std::swap(aY1, aY2);					// |
			std::swap(aZ1, aZ2);					// |
			std::swap(c1, c2);						//\|/	v3
		}

		if(aY3 < aY1){
			// let y1 <= y3
			std::swap(aX3, aX1);
			std::swap(aY3, aY1);
			std::swap(aZ3, aZ1);
			std::swap(c3, c1);
		}

		if(aY3 < aY2){
			// let y2 <= y3
			std::swap(aX3, aX2);
			std::swap(aY3, aY2);
			std::swap(aZ3, aZ2);
			std::swap(c3, c2);
		}

		// scissor
		if(aY3 < 0 || aY1 >= height ||
			(aX1 < 0 && aX2 < 0 && aX3 < 0) ||
			(aX1 >= width && aX2 >= width && aX3 >= width)){
			return ;
		}

		Real invDy12, invDy13, invDy23;
		Fixed16 dx12, dx13, dx23;
		Real dz12, dz13, dz23;
		Color::GLColor dc12, dc13, dc23;
		invDy12 = invDy13 = invDy23 = 0;

		if(aY1 == aY2)
			goto V13;

		invDy12 = 1.0 / Real(aY1 - aY2);
		dx12 = Real(aX1 - aX2) * invDy12;
		dz12 = (aZ1 - aZ2) * invDy12;
		dc12 = (c1 - c2) * invDy12;

V13:
		if(aY1 == aY3)	// only a point
			return ;
		
		invDy13 = 1.0 / Real(aY1 - aY3);
		dx13 = Real(aX1 - aX3) * invDy13;
		dz13 = (aZ1 - aZ3) * invDy13;
		dc13 = (c1 - c3) * invDy13;


		if(aY2 == aY3)
			goto INTERPOLATION;

		invDy23 = 1.0 / Real(aY2 - aY3);
		dx23 = Real(aX2 - aX3) * invDy23;
		dz23 = (aZ2 - aZ3) * invDy23;
		dc23 = (c2 - c3) * invDy23;

INTERPOLATION:
		int hy = aY1;
		Fixed16 hxl(aX1);
		Fixed16 hxr(aX1);
		Real hzl = aZ1;
		Real hzr = aZ1;
		Color::GLColor hcl = c1;
		Color::GLColor hcr = c1;
		if(hy < 0){
			if(aY1 == aY2){
				hxl = dx23 * (-hy) + aX2;
				hxr += dx13 * (-hy);

				hzl = dz23 * (-hy) + aZ2;
				hzr += dz13 * (-hy);

				hcl = c2 + dc23 * (-hy);
				hcr += dc13 * (-hy);

				aY1 = aY2 = 0;
				aX2 = hxl.ceil();
				aZ2 = hzl;
				c2 = hcl;
			}else{
				hxr += dx13 * (-hy);
				hzr += dz13 * (-hy);
				hcr += dc13 * (-hy);
				if(aY2 < 0){
					hxl = dx23 * (-aY2) + aX2;
					hzl = dz23 * (-aY2) + aZ2;
					hcl = c2 + dc23 * (-aY2);

					aY2 = 0;
					aX2 = hxl.ceil();
					aZ2 = hzl;
					c2 = hcl;
				}else{
					hxl += dx12 * (-hy);
					hzl += dz12 * (-hy);
					hcl += dc12 * (-hy);
				}
				aY1 = 0;
			}
			hy = 0;
		}

		while(hy < aY2 && hy < height){
			hlineColorInterpolation(hxl.ceil(), hzl, hxr.ceil(), hzr, hy, hcl, hcr);
			++hy;
			hxl += dx12;
			hxr += dx13;
			hzl += dz12;
			hzr += dz13;
			hcl += dc12;
			hcr += dc13;
		}

		// hy == aY2
		hxl = aX2;
		hzl = aZ2;
		hcl = c2;

		while(hy < aY3 && hy < height){
			hlineColorInterpolation(hxl.ceil(), hzl, hxr.ceil(), hzr, hy, hcl, hcr);
			++hy;
			hxl += dx23;
			hxr += dx13;
			hzl += dz23;
			hzr += dz13;
			hcl += dc23;
			hcr += dc13;
		}
	}

	//  [1/21/2009 zhangxiang]
	void SagiRenderer::triangleInterpolation(int aX1, int aY1, int aX2, int aY2, int aX3, int aY3) const{
		int width = static_cast<int>(m_CurFrameBuffers.width());
		int height = static_cast<int>(m_CurFrameBuffers.height());

		if((aX1 == aX2 && aX2 == aX3) ||
			(aY1 == aY2 && aY2 == aY3)){
				// only a line
				return ;
		}

		// ascending sort by y -- v1, v2, v3		------ scan line ------
		if(aY2 < aY1){								// |	v1
			// let y1 <= y2							// |	
			std::swap(aX1, aX2);					// |	v2
			std::swap(aY1, aY2);					// |
		}											//\|/	v3

		if(aY3 < aY1){
			// let y1 <= y3
			std::swap(aX3, aX1);
			std::swap(aY3, aY1);
		}

		if(aY3 < aY2){
			// let y2 <= y3
			std::swap(aX3, aX2);
			std::swap(aY3, aY2);
		}

		// scissor
		if(aY3 < 0 || aY1 >= height ||
			(aX1 < 0 && aX2 < 0 && aX3 < 0) ||
			(aX1 >= width && aX2 >= width && aX3 >= width)){
				return ;
		}

		Real invDy12, invDy13, invDy23;
		Fixed16 dx12, dx13, dx23;
		invDy12 = invDy13 = invDy23 = 0;

		if(aY1 == aY2)
			goto V13;

		invDy12 = 1.0 / Real(aY1 - aY2);
		dx12 = Real(aX1 - aX2) * invDy12;

V13:
		if(aY1 == aY3)	// only a point
			return ;

		invDy13 = 1.0 / Real(aY1 - aY3);
		dx13 = Real(aX1 - aX3) * invDy13;


		if(aY2 == aY3)
			goto INTERPOLATION;

		invDy23 = 1.0 / Real(aY2 - aY3);
		dx23 = Real(aX2 - aX3) * invDy23;

INTERPOLATION:
		int hy = aY1;
		Fixed16 hxl(aX1);
		Fixed16 hxr(aX1);
		if(hy < 0){
			if(aY1 == aY2){
				hxl = dx23 * (-hy) + aX2;
				hxr += dx13 * (-hy);

				aY1 = aY2 = 0;
				aX2 = hxl.ceil();
			}else{
				hxr += dx13 * (-hy);
				if(aY2 < 0){
					hxl = dx23 * (-aY2) + aX2;

					aY2 = 0;
					//	aX2 = hxl;
					aX2 = hxl.ceil();
				}else{
					hxl += dx12 * (-hy);
				}
				aY1 = 0;
			}
			hy = 0;
		}

		while(hy < aY2 && hy < height){
			hlineInterpolation(hxl.ceil(), hxr.ceil(), hy);
			++hy;
			hxl += dx12;
			hxr += dx13;
		}

		// hy == aY2
		hxl = aX2;

		while(hy < aY3 && hy < height){
			hlineInterpolation(hxl.ceil(), hxr.ceil(), hy);
			++hy;
			hxl += dx23;
			hxr += dx13;
		}
	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::postRenderImpl(void) const{
		m_CurFrameBuffers._release();

		glDrawPixels(m_iTargetWidth, m_iTargetHeight, GL_RGBA, GL_FLOAT,
					m_FrameBuffers.getColorBuffer()->data());
	}

	//  [1/15/2009 zhangxiang]
	void SagiRenderer::resetLights(int aLightNum) const{
		m_CurSagiLightList.clear();
		m_bLighting = false;
	}

} // namespace Sagitta