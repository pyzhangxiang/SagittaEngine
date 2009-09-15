//////////////////////////////////////////////////////
// file: SRenderOption.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class SRenderOption
// SRenderOption is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SRenderOption.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/1/2008 zhangxiang]
	SRenderOption::SRenderOption(void) :
	m_RenderElementType(RET_UNKNOWN),
	m_bLight(true), m_bFog(false),
	m_bBlend(false), m_bDepth(true),
	m_bStencil(false), m_bFaceCulling(true),
	m_bTexture(false), m_bIndividualOption(false),
	m_iFaceToCull(FTC_BACK), m_iRenderMode(RM_FILL),
	m_iTextureMode(TM_2D){
		
	}

	//  [8/1/2008 zhangxiang]
	SRenderOption::~SRenderOption(void){

	}

	//  [8/1/2008 zhangxiang]
	int SRenderOption::renderElementType(void) const{
		return m_RenderElementType;
	}

	//  [8/1/2008 zhangxiang]
	void SRenderOption::setRenderElementType(int aRet){
		m_RenderElementType = aRet;
	}

	//  [8/10/2008 zhangxiang]
	bool SRenderOption::isLightEnable(void) const{
		return m_bLight;
	}

	//  [8/10/2008 zhangxiang]
	void SRenderOption::setLightEnable(bool aEnable){
		m_bLight = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool SRenderOption::isFogEnable(void) const{
		return m_bFog;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setFogEnable(bool aEnable){
		m_bFog = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool SRenderOption::isBlendEnable(void) const{
		return m_bBlend;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setBlendEnable(bool aEnable){
		m_bBlend = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool SRenderOption::isDepthTestEnable(void) const{
		return m_bDepth;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setDepthTestEnable(bool aEnable){
		m_bDepth = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool SRenderOption::isStencilTestEnable(void) const{
		return m_bStencil;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setStencilTestEnable(bool aEnable){
		m_bStencil = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool SRenderOption::isFaceCullingEnable(void) const{
		return m_bFaceCulling;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setFaceCullingEnable(bool aEnable){
		m_bFaceCulling = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool SRenderOption::isTextureEnable(void) const{
		return m_bTexture;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setTextureEnable(bool aEnable){
		m_bTexture = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	bool SRenderOption::isIndivdualOptionEnable(void) const{
		return m_bIndividualOption;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setIndivdualOptionEnable(bool aEnable){
		m_bIndividualOption = aEnable;
	}

	//  [1/14/2009 zhangxiang]
	int SRenderOption::faceToCull(void) const{
		return m_iFaceToCull;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setFaceToCull(int aFaceToCull){
		m_iFaceToCull = aFaceToCull;
	}

	//  [1/14/2009 zhangxiang]
	int SRenderOption::renderMode(void) const{
		return m_iRenderMode;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setRenderMode(int aRenderMode){
		m_iRenderMode = aRenderMode;
	}

	//  [1/14/2009 zhangxiang]
	int SRenderOption::textureMode(void) const{
		return m_iTextureMode;
	}

	//  [1/14/2009 zhangxiang]
	void SRenderOption::setTextureMode(int aTextureMode){
		m_iTextureMode = aTextureMode;
	}

} // namespace Sagitta