//////////////////////////////////////////////////////
// file: sgRenderState.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class sgRenderState
// sgRenderState is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgRenderState.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/1/2008 zhangxiang]
	sgRenderState::sgRenderState(void) :
	m_bLight(true), m_bFog(false),
	m_bBlend(false), m_bDepth(true),
	m_bStencil(false), m_bFaceCulling(true),
	m_bTexture(false), m_bIndividualOption(false),
	m_iFaceToCull(FTC_BACK), m_iRenderMode(RM_FILL),
	m_iTextureMode(TM_2D){
		
	}

	//  [8/1/2008 zhangxiang]
	sgRenderState::~sgRenderState(void){

	}

	//  [8/10/2008 zhangxiang]
	bool sgRenderState::isLightEnable(void) const{
		return m_bLight;
	}

	//  [8/10/2008 zhangxiang]
	void sgRenderState::setLightEnable(bool aEnable){
		m_bLight = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderState::isFogEnable(void) const{
		return m_bFog;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setFogEnable(bool aEnable){
		m_bFog = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderState::isBlendEnable(void) const{
		return m_bBlend;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setBlendEnable(bool aEnable){
		m_bBlend = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderState::isDepthTestEnable(void) const{
		return m_bDepth;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setDepthTestEnable(bool aEnable){
		m_bDepth = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderState::isStencilTestEnable(void) const{
		return m_bStencil;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setStencilTestEnable(bool aEnable){
		m_bStencil = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderState::isFaceCullingEnable(void) const{
		return m_bFaceCulling;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setFaceCullingEnable(bool aEnable){
		m_bFaceCulling = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderState::isTextureEnable(void) const{
		return m_bTexture;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setTextureEnable(bool aEnable){
		m_bTexture = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderState::isIndivdualOptionEnable(void) const{
		return m_bIndividualOption;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setIndivdualOptionEnable(bool aEnable){
		m_bIndividualOption = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	int sgRenderState::faceToCull(void) const{
		return m_iFaceToCull;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setFaceToCull(int aFaceToCull){
		m_iFaceToCull = aFaceToCull;
	}

	//  [1/14/2009 zhangxiang]
	int sgRenderState::renderMode(void) const{
		return m_iRenderMode;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setRenderMode(int aRenderMode){
		m_iRenderMode = aRenderMode;
	}

	//  [1/14/2009 zhangxiang]
	int sgRenderState::textureMode(void) const{
		return m_iTextureMode;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderState::setTextureMode(int aTextureMode){
		m_iTextureMode = aTextureMode;
	}

} // namespace Sagitta