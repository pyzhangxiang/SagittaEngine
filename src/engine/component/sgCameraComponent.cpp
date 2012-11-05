//////////////////////////////////////////////////////
// file: sgCameraComponent.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgCameraComponent
// sgCameraComponent is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgCameraComponent.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/renderer/sgViewport.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgException.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SG_META_DEFINE(sgCameraComponent, sgComponent)

	//  [8/2/2008 zhangxiang]
	sgCameraComponent::sgCameraComponent(void) :
	m_UpDirection(0, 1, 0),
	m_RightDirection(1, 0, 0),
	m_ShootDirection(0, 0, -1),
	m_TargetPosition(Vector3::ZERO + m_ShootDirection),
	m_CameraMode(CM_FIXER), m_pViewport(0),
	m_fFovY(Math::PI_DIV_3),
	m_fNear(0.1f), m_fFar(10000.0f),
	m_bGeomChanged(true), m_bPropChanged(true){

	}


	//  [8/2/2008 zhangxiang]
	sgCameraComponent::~sgCameraComponent(void){
		if(m_pViewport){
			m_pViewport->setCamera(0);
		}
	}

	//  [1/17/2009 zhangxiang]
	void sgCameraComponent::_updateGeometries(void) const{
		Quaternion inverseq = Quaternion::IDENTITY;
        Vector3 pos = Vector3::ZERO;
        if(getParent())
        {
            inverseq = getParent()->absoluteOrientation();//.inverse();
            pos = getParent()->position();
        }
		Vector3 r = inverseq * m_RightDirection;
		r.normalise();

		Vector3 d = inverseq * m_ShootDirection;
		d.normalise();

		Vector3 u = inverseq * m_UpDirection;
		u.normalise();

		// for GLRenderer, no need to transpose
		/*m_ViewMatrix.setValues(r.x(), u.x(), -d.x(), 0.0f,
		r.y(), u.y(), -d.y(), 0.0f,
		r.z(), u.z(), -d.z(), 0.0f,
		(-r).dotProduct(pos), (-u).dotProduct(pos), d.dotProduct(pos), 1.0f);*/
		/*m_ViewMatrix.setValues(r.x(), r.y(), r.z(), (-r).dotProduct(pos),
		u.x(), u.y(), u.z(), (-u).dotProduct(pos),
		d.x(), d.y(), d.z(), (-d).dotProduct(pos),
		0.0f, 0.0f, 0.0f, 1.0f);*/

		// for SagiRenderer, no need to transpose
		m_ViewMatrix.setValues(r.x(), r.y(), r.z(), (-r).dotProduct(pos),
							u.x(), u.y(), u.z(), (-u).dotProduct(pos),
							-d.x(), -d.y(), -d.z(), d.dotProduct(pos),
							0.0f, 0.0f, 0.0f, 1.0f);
		
		m_bGeomChanged = false;
	}

	//  [1/17/2009 zhangxiang]
	void sgCameraComponent::_updateProperties(void) const{
		Real f = 1 / Math::Tan(Radian(m_fFovY * 0.5));
		m_ProjectionMatrix.setValues(f / m_fAspect, 0, 0, 0,
									0, f, 0, 0,
									0, 0, (m_fFar + m_fNear) / (m_fNear - m_fFar), -1,
									0, 0, 2 * m_fFar * m_fNear / (m_fNear - m_fFar), 0);

		// calculate view plane width and height
		m_fHHeight = m_fNear / f;
		m_fHWidth = m_fAspect * m_fHHeight;

		// calculate left, right, up and down plane normals
		Vector3 lt(-m_fHWidth, m_fHHeight, -m_fNear);
		Vector3 ld(-m_fHWidth, -m_fHHeight, -m_fNear);
		Vector3 rd(m_fHWidth, -m_fHHeight, -m_fNear);
		Vector3 rt(m_fHWidth, m_fHHeight, -m_fNear);
		m_LeftNormal = lt.crossProduct(ld);
		m_RightNormal = rd.crossProduct(rt);
		m_UpNormal = rt.crossProduct(lt);
		m_DownNormal = ld.crossProduct(rd);

		m_LeftNormal.normalise();
		m_RightNormal.normalise();
		m_UpNormal.normalise();
		m_DownNormal.normalise();

		m_bPropChanged = false;
	}

	//  [8/2/2008 zhangxiang]
	void sgCameraComponent::computeDirections(int aMode){
		switch(aMode){
			case -1:
				{
					m_UpDirection.normalise();
					m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
					m_RightDirection.normalise();
				}
				break;

			case 0:
				{
					m_ShootDirection.normalise();
					m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
					m_RightDirection.normalise();

					m_TargetPosition = getParent()->position() + m_ShootDirection;
				}
				break;

			case 1:
				{
					m_ShootDirection = m_TargetPosition - getParent()->position();
					m_ShootDirection.normalise();
					
					m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
					m_RightDirection.normalise();
				}
		}
	}
	//  [8/2/2008 zhangxiang]
	const Matrix4 &sgCameraComponent::getViewMatrix(void) const{
		if(m_bGeomChanged)
        {
			_updateGeometries();
		}

		return m_ViewMatrix;
	}

	const Matrix4 &sgCameraComponent::getProjectionMatrix(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}

		return m_ProjectionMatrix;
	}

	//  [8/6/2008 zhangxiang]
	sgViewport *sgCameraComponent::getViewport(void) const{
		return m_pViewport;
	}

	//  [1/3/2009 zhangxiang]
	void sgCameraComponent::_setViewport(sgViewport *aViewport){
		if(m_pViewport != aViewport){
			m_pViewport = aViewport;
		}
		if(m_pViewport)
		{
			_setAspect(Real(m_pViewport->actwidth()) / Real(aViewport->actheight()));
		}
	}

	//  [8/6/2008 zhangxiang]
	Real sgCameraComponent::fovY(void) const{
		return m_fFovY;
	}

	//  [8/6/2008 zhangxiang]
	Real sgCameraComponent::zNear(void) const{
		return m_fNear;
	}

	//  [8/6/2008 zhangxiang]
	Real sgCameraComponent::zFar(void) const{
		return m_fFar;
	}

	//  [8/21/2008 zhangxiang]
	Real sgCameraComponent::aspect(void) const{
		return m_fAspect;
	}

	//  [1/3/2009 zhangxiang]
	void sgCameraComponent::_setAspect(Real aAspect){
		m_fAspect = aAspect;
		m_bPropChanged = true;
	}

	//  [8/6/2008 zhangxiang]
	void sgCameraComponent::setPerspective(Real afFovY, Real afNear, Real afFar){
		m_fFovY = afFovY;
		m_fNear = afNear;
		m_fFar = afFar;
		m_bPropChanged = true;
	}

	//  [1/17/2009 zhangxiang]
	Real sgCameraComponent::viewHalfWidth(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_fHWidth;
	}

	//  [1/17/2009 zhangxiang]
	Real sgCameraComponent::viewHalfHeight(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_fHHeight;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &sgCameraComponent::leftPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_LeftNormal;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &sgCameraComponent::rightPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_RightNormal;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &sgCameraComponent::upPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_UpNormal;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &sgCameraComponent::downPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_DownNormal;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &sgCameraComponent::target(void) const{
		return m_TargetPosition;
	}

	//  [8/2/2008 zhangxiang]
	void sgCameraComponent::setTarget(const Vector3 &aTargetPos){
		m_TargetPosition = aTargetPos;
		computeDirections(1);
		m_bGeomChanged = true;
	}

	//  [1/18/2009 zhangxiang]
	Vector3 sgCameraComponent::shootDirectionInWorld(void) const{
		if(!getParent()){
			return m_ShootDirection;
		}
		return getParent()->absoluteOrientation() * m_ShootDirection;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &sgCameraComponent::shootDirection(void) const{
		return m_ShootDirection;
	}

	//  [8/2/2008 zhangxiang]
	void sgCameraComponent::setShootDirection(const Vector3 &aDir){
		m_ShootDirection = aDir;
		computeDirections(0);
		m_bGeomChanged = true;
	}

	//  [1/18/2009 zhangxiang]
	Vector3 sgCameraComponent::upDirectionInWorld(void) const{
		if(!getParent()){
			return m_UpDirection;
		}
		return getParent()->absoluteOrientation() * m_UpDirection;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &sgCameraComponent::upDirection(void) const{
		return m_UpDirection;
	}

	//  [8/2/2008 zhangxiang]
	void sgCameraComponent::setUpDirection(const Vector3 &aDir){
		m_UpDirection = aDir;
		computeDirections(-1);
		m_bGeomChanged = true;
	}

	//  [1/18/2009 zhangxiang]
	Vector3 sgCameraComponent::rightDirectionInWorld(void) const{
		if(!getParent()){
			return m_RightDirection;
		}
		return getParent()->absoluteOrientation() * m_RightDirection;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &sgCameraComponent::rightDirection(void) const{
		return m_RightDirection;
	}
    
    void sgCameraComponent::update(Float32 deltaTime)
    {
        _updateGeometries();
    }

} // namespace Sagitta