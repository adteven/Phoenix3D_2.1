//  PX2MaterialManager.hpp

#ifndef PX2MATERIALMANAGER_HPP
#define PX2MATERIALMANAGER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2Material.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2FString.hpp"

namespace PX2
{

	class MaterialManager : public Singleton<MaterialManager>
	{
	public:
		MaterialManager();
		virtual ~MaterialManager();

		bool Initlize();
		bool Terminate();

		Material *GetMaterial(const FString &filename, bool share);
		ShaderFloat *CreateShaderFloat(const FString &name, int numRegister);

	protected:
		void _SetMaterialShaderKey(Material *mtl, int shaderKey);

		struct _MtlObject
		{
			_MtlObject()
			{
				ShaderKey = 0;
			}

			MaterialPtr TheMaterial;
			int ShaderKey;
		};

		std::map<FString, _MtlObject> mMaterialMap;
		int mNextShaderKey;
	};
#define PX2_MATERIALMAN MaterialManager::GetSingleton()

}

#endif