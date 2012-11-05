//////////////////////////////////////////////////////
// file: sgCamera.h @ 2008-1-31 by Zhang Xiang
// declares of the class sgCamera
// sgCamera is a class ...
//////////////////////////////////////////////////////
#ifndef __SGCAMERACOMPONENT_H__
#define __SGCAMERACOMPONENT_H__

#include "math/sgMathHeader.h"
#include "sgComponent.h"

namespace Sagitta{

	class sgViewport;

	class _SG_KernelExport sgCameraComponent : public sgComponent
	{
		SG_META_DECLARE(sgCameraComponent)
	public:
		enum CameraMode{
			CM_TRACKER,	// fixed target
			CM_FIXER	// fixed front direction
		};

	// member variables
	protected:
		/// up direction
		Vector3 m_UpDirection;
		/// right direction
		Vector3 m_RightDirection;
		/// shoot direction
		Vector3 m_ShootDirection;
		/// target position
		Vector3 m_TargetPosition;

		/// left plane normal
		mutable Vector3 m_LeftNormal;
		/// right plane normal
		mutable Vector3 m_RightNormal;
		/// up plane normal
		mutable Vector3 m_UpNormal;
		/// down plane normal
		mutable Vector3 m_DownNormal;

		/// camera mode
		CameraMode m_CameraMode;

		/// viewport
		sgViewport *m_pViewport;

		// frustrm properties //////////////////////////////////////////////////////////////////////////

		/// Radian angle in y-axis
		Real m_fFovY;
		/// near and far plane, should be positive
		Real m_fNear, m_fFar;
		/// width / height
		Real m_fAspect;
		/// half width of view plane
		mutable Real m_fHWidth;
		/// half height of view plane
		mutable Real m_fHHeight;

		// matrixes //////////////////////////////////////////////////////////////////////////

		/// view matrix
		mutable Matrix4 m_ViewMatrix;
		/// projection matrix
		mutable Matrix4 m_ProjectionMatrix;

		/// geometry changed
		mutable bool m_bGeomChanged;
		/// properties changed
		mutable bool m_bPropChanged;

	// constructors & destructor
	public:
		sgCameraComponent(void);
		virtual ~sgCameraComponent(void);

	// member functions
	private:
		/** Updates view matrix when geometry changed. */
		void _updateGeometries(void) const;

		/** Updates properties when properties changed. */
		void _updateProperties(void) const;

	protected:
		/** Compute all directions. 
			@param
				aMode tells compute directions according to which direction
				-1 according to up direction
				0 according to shoot direction
				1 according to target position
		*/
		void computeDirections(int aMode);
		
	public:

		/** Gets the view matrix. */
		const Matrix4 &getViewMatrix(void) const;

		/** Gets the projection matrix. */
		const Matrix4 &getProjectionMatrix(void) const;

		/** Returns the pointer of the viewport this camera should perspective. */
		sgViewport *getViewport(void) const;

		/** Sets the viewport this camera combined. Internal method, called by sgViewport::setCamera. */
		void _setViewport(sgViewport *aViewport);

		/** Gets fov in y axes. */
		Real fovY(void) const;

		/** Gets near plane. */
		Real zNear(void) const;

		/** Gets far plane. */
		Real zFar(void) const;

		/** Gets aspect of this camera. */
		Real aspect(void) const;
		/** Sets aspect of this camera.
			@remarks Internal method. Only called by sgViewport::_updateActDimensions.
		*/
		void _setAspect(Real aAspect);

		/** Sets the perspective parameters. */
		void setPerspective(Real afFovY, Real afNear, Real afFar);

		/** Gets half width of view plane. */
		Real viewHalfWidth(void) const;

		/** Gets half height of view plane. */
		Real viewHalfHeight(void) const;

		/** Gets left plane normal. */
		const Vector3 &leftPlaneNormal(void) const;

		/** Gets right plane normal. */
		const Vector3 &rightPlaneNormal(void) const;

		/** Gets up plane normal. */
		const Vector3 &upPlaneNormal(void) const;

		/** Get down plane normal. */
		const Vector3 &downPlaneNormal(void) const;

		/** Gets the target's position. */
		const Vector3 &target(void) const;
		/** Sets the target's position. */
		void setTarget(const Vector3 &aTargetPos);

		/** Gets the shoot direction in the world space. */
		Vector3 shootDirectionInWorld(void) const;
		/** Gets the shoot direction in local space. */
		const Vector3 &shootDirection(void) const;
		/** Sets the shoot direction and it will recompute the right direction. */
		void setShootDirection(const Vector3 &aDir);

		/** Gets the up direction in the world space. */
		Vector3 upDirectionInWorld(void) const;
		/** Gets the up direction in local space. */
		const Vector3 &upDirection(void) const;
		/** Sets the up direction and it will recompute the right direction. */
		void setUpDirection(const Vector3 &aDir);

		/** Gets the right direction in the world space. */
		Vector3 rightDirectionInWorld(void) const;
		/** Gets the right direction in local space. */
		const Vector3 &rightDirection(void) const;
        
        virtual void update(Float32 deltaTime);

	}; //#### end class sgCamera

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGCAMERACOMPONENT_H__

