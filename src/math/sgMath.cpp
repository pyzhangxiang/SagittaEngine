//////////////////////////////////////////////////////
// file: SMath.cpp @ 2008-1-20 by Zhang Xiang
// 
// 
//////////////////////////////////////////////////////


// INCLUDES //////////////////////////////////////////
#include "sgMath.h"
#include "sgMathHeader.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	// constant defines for Fixed16
	const Fixed16 Fixed16::ONE_DEV_2 = Fixed16(1) * 0.5f;

	// constant defines for math
	const Real Math::PI = 3.141592654f;
	const Real Math::PI_X_2 = 6.283185307f;
	const Real Math::PI_DIV_2 = 1.570796327f;
	const Real Math::PI_DIV_3 = 1.047197551f;
	const Real Math::PI_DIV_4 = 0.785398163f;
	const Real Math::PI_INV = 0.318309886f;
	const Real Math::PI_DIV_180 = 0.017453292f;

	// defines for small numbers for math
	const Real Math::EPSILON_E3 = 1e-03f;
	const Real Math::EPSILON_E4 = 1e-04f;
	const Real Math::EPSILON_E5 = 1e-05f;
	const Real Math::EPSILON_E6 = 1e-06f;
	const Real Math::EPSILON_E8 = 1e-08f;

	// defines for infinity numbers
	const Real Math::POS_INFINITY = std::numeric_limits<Real>::infinity();
	const Real Math::NEG_INFINITY = -std::numeric_limits<Real>::infinity();

	// defines for conversion between degree and radian
	const Real Math::DEG_TO_RAD = Math::PI / 180.0f;
	const Real Math::RAD_TO_DEG = 180.0f / Math::PI;

	// defines for Color convert(d3d to gl || gl to d3d)
	const Real Math::COLOR_D3D_TO_GL = 1.0f / 255.0f;
	const Real Math::COLOR_GL_TO_D3D = 255.0f;

	bool Math::RealEqual(Real a, Real b, Real tolerance/* = EPSILON_E6 */){
		if(fabs(b-a) <= tolerance)
			return true;
		else
			return false;
	}

	Real Math::InvSqrt(Real vfValue){
		return 1 / sqrt(vfValue);
	}

	//  [6/17/2009 zhangxiang]
	int Math::ISign(int aiValue){
		return (aiValue > 0 ? +1 : (aiValue < 0 ? -1 : 0));
	}

	//  [6/17/2009 zhangxiang]
	Real Math::Sign(Real afValue){
		if(afValue > 0.0)
			return 1.0;

		if(afValue < 0.0)
			return -1.0;

		return 0.0;
	}

	Radian Math::ACos(Real vfValue){
		if(-1.0 < vfValue){
			if(vfValue < 1.0)
				return Radian(acos(vfValue));
			else
				return Radian(0.0f);
		}else{
			return Radian(Math::PI);
		}
	}

	Radian Math::ASin(Real vfValue){
		if(-1.0 < vfValue){
			if(vfValue < 1.0)
				return Radian(asin(vfValue));
			else
				return Radian(Math::PI_DIV_2);
		}else{
			return Radian(-Math::PI_DIV_2);
		}
	}

	int Math::Round(Real afValue){
		int deci = static_cast<int>(afValue);
		return (afValue - deci) < 0.5 ? deci : deci + 1;
	}

	//  [6/17/2009 zhangxiang]
	Real Math::UnitRandom(void){
		return Real(rand()) / Real(RAND_MAX);
	}

	//  [6/17/2009 zhangxiang]
	Real Math::RangeRandom(Real afLow, Real afHigh){
		return (afHigh - afLow) * UnitRandom() + afLow;
	}

	//  [6/17/2009 zhangxiang]
	Real Math::SymmetricRandom(void){
		return 2.0f * UnitRandom() - 1.0f;
	}

	//  [6/21/2009 zhangxiang]
	Real Math::GaussianDistribution(Real x, Real offset /* = 0.0f */, Real scale /* = 1.0f */){
		Real nom = Math::Exp(-Math::Sqr(x - offset) / (2 * Math::Sqr(scale)));
		Real denom = scale * Math::Sqrt(2 * Math::PI);

		return nom / denom;
	}

	//  [6/21/2009 zhangxiang]
	bool Math::PointInTri2D(const Vector2 &p, const Vector2 &a, const Vector2 &b, const Vector2 &c){
		// Winding must be consistent from all edges for point to be inside
		Vector2 v1, v2;
		Real dot[3];
		bool zeroDot[3];

		v1 = b - a;
		v2 = p - a;

		// Note we don't care about normalisation here since sign is all we need
		// It means we don't have to worry about magnitude of cross products either
		dot[0] = v1.crossProduct(v2);
		zeroDot[0] = Math::RealEqual(dot[0], 0.0f, 1e-3);


		v1 = c - b;
		v2 = p - b;

		dot[1] = v1.crossProduct(v2);
		zeroDot[1] = Math::RealEqual(dot[1], 0.0f, 1e-3);

		// Compare signs (ignore colinear / coincident points)
		if(!zeroDot[0] && !zeroDot[1] && Math::Sign(dot[0]) != Math::Sign(dot[1])){
			return false;
		}

		v1 = a - c;
		v2 = p - c;

		dot[2] = v1.crossProduct(v2);
		zeroDot[2] = Math::RealEqual(dot[2], 0.0f, 1e-3);
		// Compare signs (ignore colinear / coincident points)
		if((!zeroDot[0] && !zeroDot[2] &&
			Math::Sign(dot[0]) != Math::Sign(dot[2])) ||
			(!zeroDot[1] && !zeroDot[2] &&
			Math::Sign(dot[1]) != Math::Sign(dot[2]))){
			return false;
		}

		return true;
	} //#### end PointInTri2D

	//  [6/21/2009 zhangxiang]
	bool Math::PointInTri3D(const Vector3 &p, const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &normal){
		// Winding must be consistent from all edges for point to be inside
		Vector3 v1, v2;
		Real dot[3];
		bool zeroDot[3];

		v1 = b - a;
		v2 = p - a;

		// Note we don't care about normalisation here since sign is all we need
		// It means we don't have to worry about magnitude of cross products either
		dot[0] = v1.crossProduct(v2).dotProduct(normal);
		zeroDot[0] = Math::RealEqual(dot[0], 0.0f, 1e-3);

		v1 = c - b;
		v2 = p - b;

		dot[1] = v1.crossProduct(v2).dotProduct(normal);
		zeroDot[1] = Math::RealEqual(dot[1], 0.0f, 1e-3);

		// Compare signs (ignore colinear / coincident points)
		if(!zeroDot[0] && !zeroDot[1] && Math::Sign(dot[0]) != Math::Sign(dot[1])){
			return false;
		}

		v1 = a - c;
		v2 = p - c;

		dot[2] = v1.crossProduct(v2).dotProduct(normal);
		zeroDot[2] = Math::RealEqual(dot[2], 0.0f, 1e-3);
		// Compare signs (ignore colinear / coincident points)
		if((!zeroDot[0] && !zeroDot[2] &&
			Math::Sign(dot[0]) != Math::Sign(dot[2])) ||
			(!zeroDot[1] && !zeroDot[2] &&
			Math::Sign(dot[1]) != Math::Sign(dot[2]))){
			return false;
		}

		return true;
	} //#### end PointInTri3D

	//  [6/21/2009 zhangxiang]
	std::pair<bool, Real> Math::Intersects(const Ray &ray, const Plane &plane){
		Real denom = plane.normal().dotProduct(ray.direction());
		if(Math::Abs(denom) < std::numeric_limits<Real>::epsilon()){
			// Parallel
			return std::pair<bool, Real>(false, 0);
		}else{
			Real nom = plane.normal().dotProduct(ray.origin()) + plane.distance();
			Real t = -(nom / denom);
			return std::pair<bool, Real>(t >= 0, t);
		}
	}

	//  [6/21/2009 zhangxiang]
	std::pair<bool, Real> Math::Intersects(const Ray &ray, const BoundingSphere &sphere, bool discardInside /* = true */){
		const Vector3 &raydir = ray.direction();
		// Adjust ray origin relative to sphere center
		const Vector3 &rayorig = ray.origin() - sphere.center();
		Real radius = sphere.radius();

		// Check origin inside first
		if(rayorig.squaredLength() <= radius*radius && discardInside){
			return std::pair<bool, Real>(true, 0);
		}

		// Mmm, quadratics
		// Build coeffs which can be used with std quadratic solver
		// ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
		Real a = raydir.dotProduct(raydir);
		Real b = 2 * rayorig.dotProduct(raydir);
		Real c = rayorig.dotProduct(rayorig) - radius * radius;

		// Calc determinant
		Real d = (b*b) - (4 * a * c);
		if(d < 0){
			// No intersection
			return std::pair<bool, Real>(false, 0);
		}else{
			// BTW, if d=0 there is one intersection, if d > 0 there are 2
			// But we only want the closest one, so that's ok, just use the 
			// '-' version of the solver
			Real t = ( -b - Math::Sqrt(d) ) / (2 * a);
			if (t < 0)
				t = ( -b + Math::Sqrt(d) ) / (2 * a);
			return std::pair<bool, Real>(true, t);
		}
	}

	//  [6/21/2009 zhangxiang]
	std::pair<bool, Real> Math::Intersects(const Ray &ray, const AxisAlignedBox &box){
		if(box.isNull()) return std::pair<bool, Real>(false, 0);
		if(box.isInfinite()) return std::pair<bool, Real>(true, 0);

		Real lowt = 0.0f;
		Real t;
		bool hit = false;
		Vector3 hitpoint;
		const Vector3 &min = box.getMinimum();
		const Vector3 &max = box.getMaximum();
		const Vector3 &rayorig = ray.origin();
		const Vector3 &raydir = ray.direction();

		// Check origin inside first
		if(rayorig > min && rayorig < max){
			return std::pair<bool, Real>(true, 0);
		}

		// Check each face in turn, only check closest 3
		// Min x
		if(rayorig.x() <= min.x() && raydir.x() > 0){
			t = (min.x() - rayorig.x()) / raydir.x();
			if(t >= 0){
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if(hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt)){
					hit = true;
					lowt = t;
				}
			}
		}
		// Max x
		if(rayorig.x() >= max.x() && raydir.x() < 0){
			t = (max.x() - rayorig.x()) / raydir.x();
			if(t >= 0){
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt)){
					hit = true;
					lowt = t;
				}
			}
		}
		// Min y
		if (rayorig.y() <= min.y() && raydir.y() > 0){
			t = (min.y() - rayorig.y()) / raydir.y();
			if(t >= 0){
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if(hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt)){
					hit = true;
					lowt = t;
				}
			}
		}
		// Max y
		if(rayorig.y() >= max.y() && raydir.y() < 0){
			t = (max.y() - rayorig.y()) / raydir.y();
			if(t >= 0){
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt)){
					hit = true;
					lowt = t;
				}
			}
		}
		// Min z
		if(rayorig.z() <= min.z() && raydir.z() > 0){
			t = (min.z() - rayorig.z()) / raydir.z();
			if(t >= 0){
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if(hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					(!hit || t < lowt)){
					hit = true;
					lowt = t;
				}
			}
		}
		// Max z
		if(rayorig.z() >= max.z() && raydir.z() < 0){
			t = (max.z() - rayorig.z()) / raydir.z();
			if(t >= 0){
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					(!hit || t < lowt)){
					hit = true;
					lowt = t;
				}
			}
		}

		return std::pair<bool, Real>(hit, lowt);
	} //#### end Intersects(Ray&, AxisAlignedBox&)

	//  [6/21/2009 zhangxiang]
	bool Math::Intersects(const Ray &ray, const AxisAlignedBox &box, Real *d1, Real *d2){
		if(box.isNull())
			return false;

		if(box.isInfinite()){
			if(d1) *d1 = 0;
			if(d2) *d2 = Math::POS_INFINITY;
			return true;
		}

		const Vector3& min = box.getMinimum();
		const Vector3& max = box.getMaximum();
		const Vector3& rayorig = ray.origin();
		const Vector3& raydir = ray.direction();

		Vector3 absDir;
		absDir[0] = Math::Abs(raydir[0]);
		absDir[1] = Math::Abs(raydir[1]);
		absDir[2] = Math::Abs(raydir[2]);

		// Sort the axis, ensure check minimise floating error axis first
		int imax = 0, imid = 1, imin = 2;
		if(absDir[0] < absDir[2]){
			imax = 2;
			imin = 0;
		}

		if(absDir[1] < absDir[imin]){
			imid = imin;
			imin = 1;
		}else if(absDir[1] > absDir[imax]){
			imid = imax;
			imax = 1;
		}

		Real start = 0, end = Math::POS_INFINITY;

#define _CALC_AXIS(i)                                       \
	do {                                                    \
		Real denom = 1 / raydir[i];                         \
		Real newstart = (min[i] - rayorig[i]) * denom;      \
		Real newend = (max[i] - rayorig[i]) * denom;        \
		if(newstart > newend) std::swap(newstart, newend);  \
		if(newstart > end || newend < start) return false;  \
		if(newstart > start) start = newstart;              \
		if(newend < end) end = newend;                      \
	}while(0)

		// Check each axis in turn

		_CALC_AXIS(imax);

		if(absDir[imid] < std::numeric_limits<Real>::epsilon()){
			// Parallel with middle and minimise axis, check bounds only
			if(rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
				rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
				return false;
		}else{
			_CALC_AXIS(imid);

			if(absDir[imin] < std::numeric_limits<Real>::epsilon()){
				// Parallel with minimise axis, check bounds only
				if(rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
					return false;
			}else{
				_CALC_AXIS(imin);
			}
		}
#undef _CALC_AXIS

		if(d1) *d1 = start;
		if(d2) *d2 = end;

		return true;
	} //#### end Intersects(Ray&, AxisAlignedBox&, Real*, Real*)

	std::pair<bool, Real> Math::Intersects(const Ray &ray, const Vector3 &a, const Vector3 &b, const Vector3 &c,
										const Vector3 &normal, bool positiveSide /* = true */, bool negativeSide /* = true */){
		//
        // Calculate intersection with plane.
        //
        Real t;
        {
            Real denom = normal.dotProduct(ray.direction());

            // Check intersect side
            if(denom > + std::numeric_limits<Real>::epsilon()){
                if(!negativeSide)
                    return std::pair<bool, Real>(false, 0);
            }else if(denom < - std::numeric_limits<Real>::epsilon()){
                if(!positiveSide)
                    return std::pair<bool, Real>(false, 0);
            }else{
                // Parallel or triangle area is close to zero when
                // the plane normal not normalised.
                return std::pair<bool, Real>(false, 0);
            }

            t = normal.dotProduct(a - ray.origin()) / denom;

            if(t < 0){
                // Intersection is behind origin
                return std::pair<bool, Real>(false, 0);
            }
        }

        //
        // Calculate the largest area projection plane in X, Y or Z.
        //
        size_t i0, i1;
        {
            Real n0 = Math::Abs(normal[0]);
            Real n1 = Math::Abs(normal[1]);
            Real n2 = Math::Abs(normal[2]);

            i0 = 1; i1 = 2;
            if(n1 > n2){
                if(n1 > n0) i0 = 0;
            }else{
                if(n2 > n0) i1 = 0;
            }
        }

        //
        // Check the intersection point is inside the triangle.
        //
        {
            Real u1 = b[i0] - a[i0];
            Real v1 = b[i1] - a[i1];
            Real u2 = c[i0] - a[i0];
            Real v2 = c[i1] - a[i1];
            Real u0 = t * ray.direction()[i0] + ray.origin()[i0] - a[i0];
            Real v0 = t * ray.direction()[i1] + ray.origin()[i1] - a[i1];

            Real alpha = u0 * v2 - u2 * v0;
            Real beta  = u1 * v0 - u0 * v1;
            Real area  = u1 * v2 - u2 * v1;

            // epsilon to avoid float precision error
            const Real EPSILON = 1e-3f;

            Real tolerance = - EPSILON * area;

            if(area > 0){
                if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
                    return std::pair<bool, Real>(false, 0);
            }else{
                if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
                    return std::pair<bool, Real>(false, 0);
            }
        }

        return std::pair<bool, Real>(true, t);
	} //#### end Intersects(ray, a, b, c, normal, positiveSide, negativeSide)

	//  [6/21/2009 zhangxiang]
	std::pair<bool, Real> Math::Intersects(const Ray &ray, const Vector3 &a, const Vector3 &b, const Vector3 &c,
										bool positiveSide /* = true */, bool negativeSide /* = true */){
		Vector3 normal = CalculateBasicFaceNormalWithoutNormalize(a, b, c);
		return Intersects(ray, a, b, c, normal, positiveSide, negativeSide);
	}

	//  [6/21/2009 zhangxiang]
	bool Math::Intersects(const BoundingSphere &sphere, const AxisAlignedBox &box){
		if(box.isNull()) return false;
		if(box.isInfinite()) return true;

		// Use splitting planes
		const Vector3& center = sphere.center();
		Real radius = sphere.radius();
		const Vector3 &min = box.getMinimum();
		const Vector3 &max = box.getMaximum();

		// just test facing planes, early fail if sphere is totally outside
		if(min.x() - center.x() > radius){
			return false;
		}
		if(center.x() - max.x() > radius){
			return false;
		}

		if(min.y() - center.y() > radius){
			return false;
		}
		if(center.y() - max.y() > radius){
			return false;
		}

		if(min.z() - center.z() > radius){
			return false;
		}
		if(center.z() - max.z() > radius){
			return false;
		}

		// Must intersect
		return true;
	}

	//  [6/21/2009 zhangxiang]
	bool Math::Intersects(const Plane &plane, const AxisAlignedBox &box){
		return (plane.getSide(box) == Plane::BOTH_SIDE);
	}

	//  [6/21/2009 zhangxiang]
	std::pair<bool, Real> Math::Intersects(const Ray& ray, const std::vector<Plane>& planeList, bool normalIsOutside){
		std::vector<Plane>::const_iterator planeit, planeitend;
		planeitend = planeList.end();
		bool allInside = true;
		std::pair<bool, Real> ret;
		ret.first = false;
		ret.second = 0.0f;

		// derive side
		// NB we don't pass directly since that would require Plane::Side in 
		// interface, which results in recursive includes since Math is so fundamental
		Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

		for(planeit=planeList.begin(); planeit!=planeitend; ++planeit){
			const Plane& plane = *planeit;
			// is origin outside?
			if(plane.getSide(ray.origin()) == outside){
				allInside = false;
				// Test single plane
				std::pair<bool, Real> planeRes = ray.intersects(plane);
				if (planeRes.first){
					// Ok, we intersected
					ret.first = true;
					// Use the most distant result since convex volume
					ret.second = std::max(ret.second, planeRes.second);
				}
			}
		}

		if(allInside){
			// Intersecting at 0 distance since inside the volume!
			ret.first = true;
			ret.second = 0.0f;
		}

		return ret;
	}

	//  [6/21/2009 zhangxiang]
	std::pair<bool, Real> Math::Intersects(const Ray &ray, const std::list<Plane> &planeList, bool normalIsOutside){
		std::list<Plane>::const_iterator planeit, planeitend;
		planeitend = planeList.end();
		bool allInside = true;
		std::pair<bool, Real> ret;
		ret.first = false;
		ret.second = 0.0f;

		// derive side
		// NB we don't pass directly since that would require Plane::Side in 
		// interface, which results in recursive includes since Math is so fundamental
		Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

		for(planeit=planeList.begin(); planeit!=planeitend; ++planeit){
			const Plane& plane = *planeit;
			// is origin outside?
			if(plane.getSide(ray.origin()) == outside){
				allInside = false;
				// Test single plane
				std::pair<bool, Real> planeRes = ray.intersects(plane);
				if (planeRes.first){
					// Ok, we intersected
					ret.first = true;
					// Use the most distant result since convex volume
					ret.second = std::max(ret.second, planeRes.second);
				}
			}
		}

		if (allInside){
			// Intersecting at 0 distance since inside the volume!
			ret.first = true;
			ret.second = 0.0f;
		}

		return ret;
	}

	//  [6/21/2009 zhangxiang]
	bool Math::Intersects(const BoundingSphere &sphere, const Plane &plane){
		return (Math::Abs(plane.normal().dotProduct(sphere.center())) <= sphere.radius());
	}

	//  [6/21/2009 zhangxiang]
	Vector3 Math::CalculateTangentSpaceVector(const Vector3 &position1, const Vector3 &position2, const Vector3 &position3,
											Real u1, Real v1, Real u2, Real v2, Real u3, Real v3){
		//side0 is the vector along one side of the triangle of vertices passed in, 
	    //and side1 is the vector along another side. Taking the cross product of these returns the normal.
	    Vector3 side0 = position1 - position2;
	    Vector3 side1 = position3 - position1;
	    //Calculate face normal
	    Vector3 normal = side1.crossProduct(side0);
	    normal.normalise();
	    //Now we use a formula to calculate the tangent. 
	    Real deltaV0 = v1 - v2;
	    Real deltaV1 = v3 - v1;
	    Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
	    tangent.normalise();
	    //Calculate binormal
	    Real deltaU0 = u1 - u2;
	    Real deltaU1 = u3 - u1;
	    Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
	    binormal.normalise();
	    //Now, we take the cross product of the tangents to get a vector which 
	    //should point in the same direction as our normal calculated above. 
	    //If it points in the opposite direction (the dot product between the normals is less than zero), 
	    //then we need to reverse the s and t tangents. 
	    //This is because the triangle has been mirrored when going from tangent space to object space.
	    //reverse tangents if necessary
	    Vector3 tangentCross = tangent.crossProduct(binormal);
	    if(tangentCross.dotProduct(normal) < 0.0f){
		    tangent = -tangent;
		    binormal = -binormal;
	    }

        return tangent;
	}

	//  [6/21/2009 zhangxiang]
	Matrix4 Math::BuildReflectionMatrix(const Plane &p){
		return Matrix4(
			-2 * p.normal().x() * p.normal().x() + 1,   -2 * p.normal().x() * p.normal().y(),       -2 * p.normal().x() * p.normal().z(),       -2 * p.normal().x() * p.distance(), 
			-2 * p.normal().y() * p.normal().x(),       -2 * p.normal().y() * p.normal().y() + 1,   -2 * p.normal().y() * p.normal().z(),       -2 * p.normal().y() * p.distance(), 
			-2 * p.normal().z() * p.normal().x(),       -2 * p.normal().z() * p.normal().y(),       -2 * p.normal().z() * p.normal().z() + 1,   -2 * p.normal().z() * p.distance(), 
			0,											0,											0,											1);
	}

	//  [6/21/2009 zhangxiang]
	Vector4 Math::CalculateFaceNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3){
		Vector3 normal = CalculateBasicFaceNormal(v1, v2, v3);
		// Now set up the w (distance of tri from origin
		return Vector4(normal.x(), normal.y(), normal.z(), -(normal.dotProduct(v1)));
	}

	//  [6/21/2009 zhangxiang]
	Vector3 Math::CalculateBasicFaceNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3){
		Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
		normal.normalise();
		return normal;
	}

	//  [6/21/2009 zhangxiang]
	Vector4 Math::CalculateFaceNormalWithoutNormalize(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3){
		Vector3 normal = CalculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
		// Now set up the w (distance of tri from origin)
		return Vector4(normal.x(), normal.y(), normal.z(), -(normal.dotProduct(v1)));
	}

	//  [6/21/2009 zhangxiang]
	Vector3 Math::CalculateBasicFaceNormalWithoutNormalize(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3){
		Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
		return normal;
	}

}