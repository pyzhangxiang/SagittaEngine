//////////////////////////////////////////////////////
// file: sgRenderOption.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class sgRenderOption
// sgRenderOption is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgRenderOption.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/1/2008 zhangxiang]
	sgRenderOption::sgRenderOption(void) :
	m_RenderElementType(RET_UNKNOWN),
	m_bLight(true), m_bFog(false),
	m_bBlend(false), m_bDepth(true),
	m_bStencil(false), m_bFaceCulling(true),
	m_bTexture(false), m_bIndividualOption(false),
	m_iFaceToCull(FTC_BACK), m_iRenderMode(RM_FILL),
	m_iTextureMode(TM_2D){
		
	}

	//  [8/1/2008 zhangxiang]
	sgRenderOption::~sgRenderOption(void){

	}

	//  [8/1/2008 zhangxiang]
	int sgRenderOption::renderElementType(void) const{
		return m_RenderElementType;
	}

	//  [8/1/2008 zhangxiang]
	void sgRenderOption::setRenderElementType(int aRet){
		m_RenderElementType = aRet;
	}

	//  [8/10/2008 zhangxiang]
	bool sgRenderOption::isLightEnable(void) const{
		return m_bLight;
	}

	//  [8/10/2008 zhangxiang]
	void sgRenderOption::setLightEnable(bool aEnable){
		m_bLight = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderOption::isFogEnable(void) const{
		return m_bFog;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setFogEnable(bool aEnable){
		m_bFog = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderOption::isBlendEnable(void) const{
		return m_bBlend;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setBlendEnable(bool aEnable){
		m_bBlend = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderOption::isDepthTestEnable(void) const{
		return m_bDepth;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setDepthTestEnable(bool aEnable){
		m_bDepth = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderOption::isStencilTestEnable(void) const{
		return m_bStencil;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setStencilTestEnable(bool aEnable){
		m_bStencil = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderOption::isFaceCullingEnable(void) const{
		return m_bFaceCulling;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setFaceCullingEnable(bool aEnable){
		m_bFaceCulling = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderOption::isTextureEnable(void) const{
		return m_bTexture;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setTextureEnable(bool aEnable){
		m_bTexture = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool sgRenderOption::isIndivdualOptionEnable(void) const{
		return m_bIndividualOption;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setIndivdualOptionEnable(bool aEnable){
		m_bIndividualOption = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	int sgRenderOption::faceToCull(void) const{
		return m_iFaceToCull;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setFaceToCull(int aFaceToCull){
		m_iFaceToCull = aFaceToCull;
	}

	//  [1/14/2009 zhangxiang]
	int sgRenderOption::renderMode(void) const{
		return m_iRenderMode;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setRenderMode(int aRenderMode){
		m_iRenderMode = aRenderMode;
	}

	//  [1/14/2009 zhangxiang]
	int sgRenderOption::textureMode(void) const{
		return m_iTextureMode;
	}

	//  [1/14/2009 zhangxiang]
	void sgRenderOption::setTextureMode(int aTextureMode){
		m_iTextureMode = aTextureMode;
	}

} // namespace Sagitta