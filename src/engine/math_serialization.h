#ifndef __SGENGINE_H__
#define __SGENGINE_H__

#include "serialization/sg_serialization_nvp.h"
#include "math/sgMathHeader.h"

namespace Sagitta
{
    template<class Archive>
    Archive &operator & (Archive &archive, Vector2 &obj)
    {
        archive & SG_MAKE_NVP("x", obj.m_fx);
        archive & SG_MAKE_NVP("y", obj.m_fy);
        return archive;
    }
    
	template<class Archive>
    Archive &operator & (Archive &archive, Vector3 &obj)
    {
        archive & SG_MAKE_NVP("x", obj.m_fx);
        archive & SG_MAKE_NVP("y", obj.m_fy);
        archive & SG_MAKE_NVP("z", obj.m_fz);
        return archive;
    }
    
    template<class Archive>
    Archive &operator & (Archive &archive, Vector4 &obj)
    {
        archive & SG_MAKE_NVP("x", obj.m_fx);
        archive & SG_MAKE_NVP("y", obj.m_fy);
        archive & SG_MAKE_NVP("z", obj.m_fz);
        archive & SG_MAKE_NVP("w", obj.m_fw);
        return archive;
    }
    
    template<class Archive>
    Archive &operator & (Archive &archive, Euler &obj)
    {
        archive & SG_MAKE_NVP("x", obj.m_fx);
        archive & SG_MAKE_NVP("y", obj.m_fy);
        archive & SG_MAKE_NVP("z", obj.m_fz);
        return archive;
    }
    
    template<class Archive>
    Archive &operator & (Archive &archive, Quaternion &obj)
    {
        archive & SG_MAKE_NVP("w", obj.m_fw);
        archive & SG_MAKE_NVP("x", obj.m_fx);
        archive & SG_MAKE_NVP("y", obj.m_fy);
        archive & SG_MAKE_NVP("z", obj.m_fz);
        return archive;
    }
    
    template<class Archive>
    Archive &operator & (Archive &archive, Color &obj)
    {
        archive & SG_MAKE_NVP("r", obj._m_ired);
        archive & SG_MAKE_NVP("g", obj._m_igreen);
        archive & SG_MAKE_NVP("b", obj._m_iblue);
        archive & SG_MAKE_NVP("a", obj._m_ialpha);
        return archive;
    }
}


#endif  // __SGENGINE_H__
