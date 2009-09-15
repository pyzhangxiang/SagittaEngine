//////////////////////////////////////////////////////
// file: SCamera.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class SCamera
// SCamera is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SCamera.h"
#include "SSceneNode.h"
#include "SSceneManager.h"
#include "../renderer/SViewport.h"
#include "../../common/utils/SString.h"
#include "../../common/utils/SException.h"


// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong SCamera::ms_CameraCount = 1;

	//  [8/2/2008 zhangxiang]
	SCamera::SCamera(void) :
	SEsse(SET_CAMERA),
	m_UpDirection(0, 1, 0),
	m_RightDirection(1, 0, 0),
	m_ShootDirection(0, 0, -1),
	m_TargetPosition(position() + m_ShootDirection),
	m_CameraMode(CM_FIXER), m_pViewport(0),
	m_fFovY(Math::PI_DIV_3),
	m_fNear(1.0f), m_fFar(10000.0f),
	m_bGeomChanged(true), m_bPropChanged(true){
		StdString name = "Camera" + SString::to_string(SCamera::ms_CameraCount++);
		SSceneObject::_setName(name);
	}

	//  [8/2/2008 zhangxiang]
	SCamera::SCamera(const Vector3 &aUpDir, const Vector3 &aShootDirOrTargetPos, const CameraMode &aCM,
					Real aFovY/* = Math::PI_DIV_3 */, Real aNear/* = 1.0f */, Real aFar/* = 10000.0f */) :
	SEsse(SET_CAMERA),
	m_CameraMode(aCM), m_pViewport(0),
	m_fFovY(aFovY),
	m_fNear(aNear), m_fFar(aFar),
	m_bGeomChanged(true), m_bPropChanged(true){
		StdString name = "Camera" + SString::to_string(SCamera::ms_CameraCount++);
		SSceneObject::_setName(name);

		m_UpDirection = aUpDir;
		m_UpDirection.normalise();

		if(aCM == CM_FIXER){
			m_ShootDirection = aShootDirOrTargetPos;
			m_ShootDirection.normalise();

			m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
			m_RightDirection.normalise();

			m_TargetPosition = position() + m_ShootDirection;

		}else if(aCM == CM_TRACKER){
			m_TargetPosition = aShootDirOrTargetPos;
			
			m_ShootDirection = m_TargetPosition - position();
			m_ShootDirection.normalise();

			m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
			m_RightDirection.normalise();
			
		}
		
	}

	//  [8/2/2008 zhangxiang]
	SCamera::SCamera(const StdString &asName, const Vector3 &aUpDir, const Vector3 &aShootDirOrTargetPos, const CameraMode &aCM,
					Real aFovY/* = Math::PI_DIV_3 */, Real aNear/* = 1.0f */, Real aFar/* = 10000.0f */) :
	SEsse(asName, SET_CAMERA),
	m_CameraMode(aCM), m_pViewport(0),
	m_fFovY(aFovY),
	m_fNear(aNear), m_fFar(aFar),
	m_bGeomChanged(true), m_bPropChanged(true){
		++ms_CameraCount;

		m_UpDirection = aUpDir;
		m_UpDirection.normalise();

		if(aCM == CM_FIXER){
			m_ShootDirection = aShootDirOrTargetPos;
			m_ShootDirection.normalise();

			m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
			m_RightDirection.normalise();

			m_TargetPosition = position() + m_ShootDirection;

		}else if(aCM == CM_TRACKER){
			m_TargetPosition = aShootDirOrTargetPos;

			m_ShootDirection = m_TargetPosition - position();
			m_ShootDirection.normalise();

			m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
			m_RightDirection.normalise();

		}
	}

	//  [8/2/2008 zhangxiang]
	SCamera::~SCamera(void){
		if(m_pViewport){
			m_pViewport->setCamera(0);
		}
	}

	//  [1/17/2009 zhangxiang]
	void SCamera::_updateGeometries(void) const{
		Quaternion inverseq = attachedNode()->absoluteOrientation();//.inverse();
		Vector3 r = inverseq * m_RightDirection;
		r.normalise();

		Vector3 d = inverseq * m_ShootDirection;
		d.normalise();

		Vector3 u = inverseq * m_UpDirection;
		u.normalise();

		Vector3 pos = position();

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
	void SCamera::_updateProperties(void) const{
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
	void SCamera::computeDirections(int aMode){
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

					m_TargetPosition = position() + m_ShootDirection;
				}
				break;

			case 1:
				{
					m_ShootDirection = m_TargetPosition - position();
					m_ShootDirection.normalise();
					
					m_RightDirection = m_ShootDirection.crossProduct(m_UpDirection);
					m_RightDirection.normalise();
				}
		}
	}

	SSceneObject *SCamera::clone(void) const{
		// for future...
		return 0;
	}

	//  [8/2/2008 zhangxiang]
	const Matrix4 &SCamera::getViewMatrix(void) const{
		if(m_bGeomChanged){
			_updateGeometries();
		}

		return m_ViewMatrix;
	}

	const Matrix4 &SCamera::getProjectionMatrix(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}

		return m_ProjectionMatrix;
	}

	//  [8/6/2008 zhangxiang]
	SViewport *SCamera::getViewport(void) const{
		return m_pViewport;
	}

	//  [1/3/2009 zhangxiang]
	void SCamera::_setViewport(SViewport *aViewport){
		if(m_pViewport != aViewport){
			m_pViewport = aViewport;
			_setAspect(Real(m_pViewport->actwidth()) / Real(aViewport->actheight()));
		}
	}

	//  [8/6/2008 zhangxiang]
	Real SCamera::fovY(void) const{
		return m_fFovY;
	}

	//  [8/6/2008 zhangxiang]
	Real SCamera::zNear(void) const{
		return m_fNear;
	}

	//  [8/6/2008 zhangxiang]
	Real SCamera::zFar(void) const{
		return m_fFar;
	}

	//  [8/21/2008 zhangxiang]
	Real SCamera::aspect(void) const{
		return m_fAspect;
	}

	//  [1/3/2009 zhangxiang]
	void SCamera::_setAspect(Real aAspect){
		m_fAspect = aAspect;
		m_bPropChanged = true;
	}

	//  [8/6/2008 zhangxiang]
	void SCamera::setPerspective(Real afFovY, Real afNear, Real afFar){
		m_fFovY = afFovY;
		m_fNear = afNear;
		m_fFar = afFar;
		m_bPropChanged = true;
	}

	//  [1/17/2009 zhangxiang]
	Real SCamera::viewHalfWidth(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_fHWidth;
	}

	//  [1/17/2009 zhangxiang]
	Real SCamera::viewHalfHeight(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_fHHeight;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &SCamera::leftPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_LeftNormal;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &SCamera::rightPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_RightNormal;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &SCamera::upPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_UpNormal;
	}

	//  [1/17/2009 zhangxiang]
	const Vector3 &SCamera::downPlaneNormal(void) const{
		if(m_bPropChanged){
			_updateProperties();
		}
		return m_DownNormal;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &SCamera::target(void) const{
		return m_TargetPosition;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::setTarget(const Vector3 &aTargetPos){
		m_TargetPosition = aTargetPos;
		computeDirections(1);
		m_bGeomChanged = true;
	}

	//  [1/18/2009 zhangxiang]
	Vector3 SCamera::shootDirectionInWorld(void) const{
		if(!isAttached()){
			return m_ShootDirection;
		}
		return attachedNode()->absoluteOrientation() * m_ShootDirection;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &SCamera::shootDirection(void) const{
		return m_ShootDirection;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::setShootDirection(const Vector3 &aDir){
		m_ShootDirection = aDir;
		computeDirections(0);
		m_bGeomChanged = true;
	}

	//  [1/18/2009 zhangxiang]
	Vector3 SCamera::upDirectionInWorld(void) const{
		if(!isAttached()){
			return m_UpDirection;
		}
		return attachedNode()->absoluteOrientation() * m_UpDirection;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &SCamera::upDirection(void) const{
		return m_UpDirection;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::setUpDirection(const Vector3 &aDir){
		m_UpDirection = aDir;
		computeDirections(-1);
		m_bGeomChanged = true;
	}

	//  [1/18/2009 zhangxiang]
	Vector3 SCamera::rightDirectionInWorld(void) const{
		if(!isAttached()){
			return m_RightDirection;
		}
		return attachedNode()->absoluteOrientation() * m_RightDirection;
	}

	//  [8/2/2008 zhangxiang]
	const Vector3 &SCamera::rightDirection(void) const{
		return m_RightDirection;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::translate(const Vector3 &aTranslate, SNode::TransformSpace aRelativeTo /* = SNode::TS_LOCAL */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::translate");
		}
		attachedNode()->translate(aTranslate, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::translate(const Matrix3 aAxes, const Vector3 aTranslate, SNode::TransformSpace aRelativeTo /* = SNode::TS_LOCAL */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::translate");
		}
		attachedNode()->translate(aAxes, aTranslate, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::roll(const Radian &aAngle, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::roll");
		}
		attachedNode()->roll(aAngle, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::rotationZ(const Radian &aAngle, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::rotationZ");
		}
		attachedNode()->rotationZ(aAngle, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::pitch(const Radian &aAngle, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::pitch");
		}
		attachedNode()->pitch(aAngle, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::rotationX(const Radian &aAngle, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::rotationX");
		}
		attachedNode()->rotationX(aAngle, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::yaw(const Radian &aAngle, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::yaw");
		}
		attachedNode()->yaw(aAngle, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::rotationY(const Radian &aAngle, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::rotationY");
		}
		attachedNode()->rotationY(aAngle, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::rotate(const Vector3 &aAxis, const Radian& aAngle, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::rotate");
		}
		attachedNode()->rotate(aAxis, aAngle, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	void SCamera::rotate(const Quaternion &aq, SNode::TransformSpace aRelativeTo /* = SNode::TS_WORLD */){
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::rotate");
		}
		attachedNode()->rotate(aq, aRelativeTo);
		m_bGeomChanged = true;
	}

	//  [8/2/2008 zhangxiang]
	Matrix3 SCamera::getLocalAxes(void) const{
		// for future...
		if(!isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "This camera has not attached to a node.", "SCamera::getLocalAxes");
		}
		return attachedNode()->getLocalAxes();
		m_bGeomChanged = true;
	}

} // namespace Sagitta