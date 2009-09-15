//////////////////////////////////////////////////////
// file: SPlane.cpp @ 2008-1-28 by Zhang Xiang
// defines of the class SPlane
// SPlane is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SPlane.h"
#include "SAxisAlignedBox.h"
#include "SMatrix3.h"

// DEFINES ///////////////////////////////////////////

namespace Sagitta{

	Plane::Plane(){
		m_vNormal = Vector3::ZERO;
		m_fd = 0.0f;
	}

	Plane::Plane(const Plane& vprp){
		// maybe a bug...
		m_vNormal = vprp.m_vNormal;
		m_fd = vprp.m_fd;
	}

	Plane::Plane(const Vector3& vvrNormal, float vfConstant){
		m_vNormal = vvrNormal;
		m_fd = -vfConstant;
	}

	Plane::Plane(const Vector3& vvrNormal, const Vector3& vvrPoint){
		redefine(vvrNormal, vvrPoint);
	}

	Plane::Plane(const Vector3& vvrPoint0, const Vector3& vvrPiont1, const Vector3& vvrPoint2){
		redefine(vvrPoint0, vvrPiont1, vvrPoint2);
	}

	float Plane::getDistance(const Vector3& vrvPoint) const{
		return m_vNormal.dotProduct(vrvPoint) + m_fd;
	}

	Plane::Side Plane::getSide(const Vector3& vvrPoint) const{
		float fDistance = getDistance(vvrPoint);

		if(fDistance < 0.0)
			return Plane::NEGATIVE_SIDE;

		if(fDistance > 0.0)
			return Plane::POSITIVE_SIDE;

		return Plane::NO_SIDE;
	}


	Plane::Side Plane::getSide(const AxisAlignedBox& varBox) const{
		if(varBox.isNull())
			return NO_SIDE;
		if(varBox.isInfinite())
			return BOTH_SIDE;

		return getSide(varBox.getCenter(), varBox.getHalfSize());
	}

	Plane::Side Plane::getSide(const Vector3& centre, const Vector3& halfSize) const{
		// Calculate the distance between box centre and the plane.
		float dist = getDistance(centre);

		// Calculate the maximise allows absolute distance for
		// the distance between box centre and plane.
		float maxAbsDist = m_vNormal.absDotProduct(halfSize);

		if(dist < -maxAbsDist)
			return Plane::NEGATIVE_SIDE;

		if(dist > +maxAbsDist)
			return Plane::POSITIVE_SIDE;

		return Plane::BOTH_SIDE;
	}

	void Plane::redefine(const Vector3& vvrPoint0, const Vector3& vvrPiont1, const Vector3& vvrPoint2){
		Vector3 vEdge1 = vvrPiont1 - vvrPoint0;
		Vector3 vEdge2 = vvrPoint2 - vvrPoint0;
		m_vNormal = vEdge1.crossProduct(vEdge2);
		m_vNormal.normalise();
		m_fd = -m_vNormal.dotProduct(vvrPoint0);
	}

	void Plane::redefine(const Vector3& vvrNormal, const Vector3& vvrPoint){
		m_vNormal = vvrNormal;
		m_fd = -vvrNormal.dotProduct(vvrPoint);
	}

	Vector3 Plane::projectVector(const Vector3& p){
		// We know plane normal is unit length, so use simple method
		Matrix3 xform;
		xform[0][0] = m_vNormal.x() * m_vNormal.x() - 1.0f;
		xform[0][1] = m_vNormal.x() * m_vNormal.y();
		xform[0][2] = m_vNormal.x() * m_vNormal.z();
		xform[1][0] = m_vNormal.y() * m_vNormal.x();
		xform[1][1] = m_vNormal.y() * m_vNormal.y() - 1.0f;
		xform[1][2] = m_vNormal.y() * m_vNormal.z();
		xform[2][0] = m_vNormal.z() * m_vNormal.x();
		xform[2][1] = m_vNormal.z() * m_vNormal.y();
		xform[2][2] = m_vNormal.z() * m_vNormal.z() - 1.0f;

		return xform * p;
	}

	float Plane::normalise(){
		float fLength = m_vNormal.length();

		// Will also work for zero-sized vectors, but will change nothing.
		if(fLength > Math::EPSILON_E8){
			float fInvLength = 1.0f / fLength;
			m_vNormal *= fInvLength;
			m_fd *= fInvLength;
		}

		return fLength;
	}

	std::ostream& operator << (std::ostream& o, const Plane& p){
		o << "Plane(normal=" << p.m_vNormal << ", d=" << p.m_fd << ")";

		return o;
	}

} // namespace Sagitta