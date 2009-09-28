//////////////////////////////////////////////////////
// file: SCamera.h @ 2008-1-31 by Zhang Xiang
// declares of the class SCamera
// SCamera is a class ...
//////////////////////////////////////////////////////
#ifndef __SCAMERA_H__
#define __SCAMERA_H__

// INCLUDES //////////////////////////////////////////
#include "SEsse.h"
#include "SNode.h"
#include "../../math/SMathHeader.h"
#include <string>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SSceneManager;
	class SViewport;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SCamera : public SEsse{
	// enum defines
	public:
		enum SEsseType{ SET_CAMERA = 4 };

	public:
		enum CameraMode{
			CM_TRACKER,	// fixed target
			CM_FIXER	// fixed front direction
		};

	// static memebers
		static uLong ms_CameraCount;

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
		SViewport *m_pViewport;

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
		SCamera(void);
		SCamera(const Vector3 &aUpDir, const Vector3 &aShootDirOrTargetPos, const CameraMode &aCM,
				Real aFovY = Math::PI_DIV_3, Real aNear = 1.0f, Real aFar = 10000.0f);
		SCamera(const StdString &asName, const Vector3 &aUpDir, const Vector3 &aShootDirOrTargetPos, const CameraMode &aCM,
				Real aFovY = Math::PI_DIV_3, Real aNear = 1.0f, Real aFar = 10000.0f);
		~SCamera(void);

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
		/** Overridden from SSceneObjecr. */
		SSceneObject *clone(void) const;

		/** Gets the view matrix. */
		const Matrix4 &getViewMatrix(void) const;

		/** Gets the projection matrix. */
		const Matrix4 &getProjectionMatrix(void) const;

		/** Returns the pointer of the viewport this camera should perspective. */
		SViewport *getViewport(void) const;

		/** Sets the viewport this camera combined. Internal method, called by SViewport::setCamera. */
		void _setViewport(SViewport *aViewport);

		/** Gets fov in y axes. */
		Real fovY(void) const;

		/** Gets near plane. */
		Real zNear(void) const;

		/** Gets far plane. */
		Real zFar(void) const;

		/** Gets aspect of this camera. */
		Real aspect(void) const;
		/** Sets aspect of this camera.
			@remarks Internal method. Only called by SViewport::_updateActDimensions.
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

		/** Moves the node along the cartesian axes.
			@par
				This method moves the node by the supplied vector along the
				world cartesian axes, i.e. along world x,y,z
			@param 
				d Vector with x,y,z values representing the translation.
			@param
				relativeTo The space which this transform is relative to.
		*/
		virtual void translate(const Vector3 &aTranslate, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);
		
		/** Moves the node along arbitrary axes.
			@remarks
				This method translates the node by a vector which is relative to
				a custom set of axes.
			@param 
				axes A 3x3 Matrix containg 3 column vectors each representing the
				axes X, Y and Z respectively. In this format the standard cartesian
				axes would be expressed as:
				<pre>
				1 0 0
				0 1 0
				0 0 1
				</pre>
				i.e. the identity matrix.
			@param 
				move Vector relative to the axes above.
			@param
				relativeTo The space which this transform is relative to.
		*/
		void translate(const Matrix3 aAxes, const Vector3 aTranslate, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);

		/** Rotate the node around the Z-axis. */
		void roll(const Radian &aAngle, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);
		void rotationZ(const Radian &aAngle, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);

		/** Rotate the node around the X-axis. */
		void pitch(const Radian &aAngle, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);
		void rotationX(const Radian &aAngle, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);

		/** Rotate the node around the Y-axis. */
		void yaw(const Radian &aAngle, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);
		void rotationY(const Radian &aAngle, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);

		/** Rotate the node around an arbitrary axis. */
		void rotate(const Vector3 &aAxis, const Radian& aAngle, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);

		/** Rotate the node around an arbitrary axis using a Quaternion. */
		void rotate(const Quaternion &aq, SNode::TransformSpace aRelativeTo = SNode::TS_LOCAL);

		/** Gets a matrix whose columns are the local axes based on
		the nodes orientation relative to it's parent. */
		Matrix3 getLocalAxes(void) const;

	}; //#### end class SCamera

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SCAMERA_H__