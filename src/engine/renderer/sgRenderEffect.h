//  [4/11/2012  zhangxiang]

#ifndef __SGRENDEREFFECT_H__
#define __SGRENDEREFFECT_H__

#include "engine/common/sgObject.h"
#include "engine/common/sgStrHandle.h"
#include "engine/common/sgStlAllocator.h"


namespace Sagitta{
    
    class sgRenderPass;
    class sgBuffer;
	class sgSceneObject;

	namespace sg_render
	{
		struct CurrentRenderParam;
	};

	class _SG_KernelExport sgRenderEffect : public sgObject
	{
        SG_META_DECLARE_ABSTRACT(sgRenderEffect)

	public:
        // matrices
        static const sgStrHandle ModelMatrix;
		static const sgStrHandle ViewMatrix;
		static const sgStrHandle ProjectionMatrix;
        static const sgStrHandle MVMatrix;
        static const sgStrHandle MVPMatrix;
        
        // light
		static const sgStrHandle Light0_Position;
		static const sgStrHandle Light1_Position;
		static const sgStrHandle Light2_Position;
		static const sgStrHandle Light3_Position;

		static const sgStrHandle Light0_Ambient;
		static const sgStrHandle Light1_Ambient;
		static const sgStrHandle Light2_Ambient;
		static const sgStrHandle Light3_Ambient;

		static const sgStrHandle Light0_Diffuse;
		static const sgStrHandle Light1_Diffuse;
		static const sgStrHandle Light2_Diffuse;
		static const sgStrHandle Light3_Diffuse;

		static const sgStrHandle Light0_Specular;
		static const sgStrHandle Light1_Specular;
		static const sgStrHandle Light2_Specular;
		static const sgStrHandle Light3_Specular;

		static const sgStrHandle Light0_Intensity;
		static const sgStrHandle Light1_Intensity;
		static const sgStrHandle Light2_Intensity;
		static const sgStrHandle Light3_Intensity;
		

		// material
		static const sgStrHandle Material_Ambient;
		static const sgStrHandle Material_Diffuse;
		static const sgStrHandle Material_Specular;
		static const sgStrHandle Material_Emission;
		static const sgStrHandle Material_Shininess;
		static const sgStrHandle Material_SpecularAmount;
		static const sgStrHandle Material_ReflectFraction;
        
        // texture
        //static const size_t Texture_Max = 4;
        enum{ Texture_Max = 4 };
        static const sgStrHandle Texture0;
        static const sgStrHandle Texture1;
        static const sgStrHandle Texture2;
        static const sgStrHandle Texture3;
        static const sgStrHandle TextureDepth;


    protected:
     
		struct FrameUniform
		{
			int extra;	// if transpose for matrix data, 1 - transpose, 0 - no transpose
			sgBuffer *data;
			FrameUniform(void) : extra(0), data(NULL){}
			void setMatrixExtra(size_t count, bool transpose)
			{
				extra = (count << 1) | (int)transpose;
			}
			void setNonMatrixExtra(size_t count)
			{
				extra = (int)count;
			}
		};

        typedef sg_map(sgStrHandle, FrameUniform) FrameUniformMap;
        FrameUniformMap mFrameUniformMap;

		typedef sg_vector(sgRenderPass*) PassList;
		PassList mPassList;

	private:
		size_t mCurrentPass;
	protected:
		// for subclasses
		size_t getCurrentPass(void) const{ return mCurrentPass; }
		void setCurrentPass(size_t pass){ mCurrentPass = pass; }

    public:
        sgRenderPass *addPass(const sgStrHandle &queueType = sgStrHandle::EmptyString);

	protected:
		/// same name data will be replaced
        void addFrameUniform(const sgStrHandle &dataName, FrameUniform uniform);

    public:
        sgRenderEffect(void);
        virtual ~sgRenderEffect(void) = 0;
        
        bool emptyRenderPass(void){ return mPassList.empty(); }
        size_t getRenderPassNum(void) const{ return mPassList.size(); }
        sgRenderPass *getRenderPass(size_t index) const;
        
        virtual void update(Float32 deltaTime){}

	private:
		/// set uniforms that are not changed in a frame 
		void setUniformFrame(sg_render::CurrentRenderParam *param);
		/// set uniforms from object, e.g. material color
		void setUniformObject(sg_render::CurrentRenderParam *param, sgSceneObject *object);

	protected:
		/// set extra user defined uniform data, implement by subclasses
		virtual void setUniformObjectExtra(sg_render::CurrentRenderParam *param, sgSceneObject *object){};
        
		/// render the object in the current pass
		void renderObject(sg_render::CurrentRenderParam *param, sgSceneObject *object);

	};

} // namespace Sagitta


#endif // __SGRENDEREFFECT_H__

