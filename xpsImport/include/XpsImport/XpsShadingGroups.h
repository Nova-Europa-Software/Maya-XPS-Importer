#pragma once
#include "XpsTypes.h"

namespace XpsImport
{
	static const XpsShadingGroupType g_ShadingGroup1 = {
		.renderGroupName = "Shading group 1",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 6,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Bump1,
			TextureType::Texture_Type_Bump2
		}
	};

	static const XpsShadingGroupType g_ShadingGroup2 = {
		.renderGroupName = "Shading group 2",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump,
		}
	};

	static const XpsShadingGroupType g_ShadingGroup3 = {
		.renderGroupName = "Shading group 3",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
		}
	};

	static const XpsShadingGroupType g_ShadingGroup4 = {
		.renderGroupName = "Shading group 4",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
		}
	};

	static const XpsShadingGroupType g_ShadingGroup5 = {
		.renderGroupName = "Shading group 5",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
		}
	};

	static const XpsShadingGroupType g_ShadingGroup6 = {
	.renderGroupName = "Shading group 6",
	.hasShading = true,
	.hasAlpha = true,
	.isPosable = true,
	.hasSpecular = true,
	.bump1Reg = false,
	.bump2Reg = false,
	.spec1Reg = false,
	.texCount = 2,
	.texTypes = {
		TextureType::Texture_Type_Diffuse,
		TextureType::Texture_Type_Bump
	}
	};

	static const XpsShadingGroupType g_ShadingGroup7 = {
		.renderGroupName = "Shading group 7",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup8 = {
		.renderGroupName = "Shading group 8",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump
		}
	};

	static const XpsShadingGroupType g_ShadingGroup9 = {
		.renderGroupName = "Shading group 9",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light
		}
	};

	static const XpsShadingGroupType g_ShadingGroup10 = {
		.renderGroupName = "Shading group 10",
		.hasShading = false,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup11 = {
		.renderGroupName = "Shading group 11",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = false,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump
		}
	};

	static const XpsShadingGroupType g_ShadingGroup12 = {
		.renderGroupName = "Shading group 12",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = false,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump
		}
	};

	static const XpsShadingGroupType g_ShadingGroup13 = {
		.renderGroupName = "Shading group 13",
		.hasShading = false,
		.hasAlpha = false,
		.isPosable = false,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup14 = {
		.renderGroupName = "Shading group 14",
		.hasShading = false,
		.hasAlpha = false,
		.isPosable = false,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump
		}
	};

	static const XpsShadingGroupType g_ShadingGroup15 = {
		.renderGroupName = "Shading group 15",
		.hasShading = false,
		.hasAlpha = true,
		.isPosable = false,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump
		}
	};

	static const XpsShadingGroupType g_ShadingGroup16 = {
		.renderGroupName = "Shading group 16",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = false,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup17 = {
		.renderGroupName = "Shading group 17",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = false,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light
		}
	};

	static const XpsShadingGroupType g_ShadingGroup18 = {
		.renderGroupName = "Shading group 18",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = false,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup19 = {
		.renderGroupName = "Shading group 19",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = false,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 2,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light
		}
	};

	static const XpsShadingGroupType g_ShadingGroup20 = {
		.renderGroupName = "Shading group 20",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 6,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Bump1,
			TextureType::Texture_Type_Bump2
		}
	};

	static const XpsShadingGroupType g_ShadingGroup21 = {
		.renderGroupName = "Shading group 21",
		.hasShading = false,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = false,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup22 = {
		.renderGroupName = "Shading group 22",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 7,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Bump1,
			TextureType::Texture_Type_Bump2,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup23 = {
		.renderGroupName = "Shading group 23",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 7,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Bump1,
			TextureType::Texture_Type_Bump2,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup24 = {
		.renderGroupName = "Shading group 24",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 4,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup25 = {
		.renderGroupName = "Shading group 25",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 4,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Light,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup26 = {
		.renderGroupName = "Shading group 26",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 4,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Environment,
			TextureType::Texture_Type_Mask
		}
	};

	static const XpsShadingGroupType g_ShadingGroup27 = {
		.renderGroupName = "Shading group 27",
		.hasShading = true, // 'Yes/No'
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 4,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Environment,
			TextureType::Texture_Type_Mask
		}
	};

	static const XpsShadingGroupType g_ShadingGroup28 = {
		.renderGroupName = "Shading group 28",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 6,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Bump1,
			TextureType::Texture_Type_Bump2,
			TextureType::Texture_Type_Environment
		}
	};

	static const XpsShadingGroupType g_ShadingGroup29 = {
		.renderGroupName = "Shading group 29",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 6,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Bump1,
			TextureType::Texture_Type_Bump2,
			TextureType::Texture_Type_Environment
		}
	};

	static const XpsShadingGroupType g_ShadingGroup30 = {
		.renderGroupName = "Shading group 30",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Emission
		}
	};

	static const XpsShadingGroupType g_ShadingGroup31 = {
		.renderGroupName = "Shading group 31",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Emission
		}
	};

	static const XpsShadingGroupType g_ShadingGroup32 = {
		.renderGroupName = "Shading group 32",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup33 = {
		.renderGroupName = "Shading group 33",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 1,
		.texTypes = {
			TextureType::Texture_Type_Diffuse
		}
	};

	static const XpsShadingGroupType g_ShadingGroup34 = {
		.renderGroupName = "Shading group 34",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 6,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup35 = {
		.renderGroupName = "Shading group 35",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = true,
		.spec1Reg = false,
		.texCount = 6,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Mask,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup36 = {
		.renderGroupName = "Shading group 36",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Emission
		}
	};

	static const XpsShadingGroupType g_ShadingGroup37 = {
		.renderGroupName = "Shading group 37",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Emission
		}
	};

	static const XpsShadingGroupType g_ShadingGroup38 = {
		.renderGroupName = "Shading group 38",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 4,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular,
			TextureType::Texture_Type_Emission
		}
	};

	static const XpsShadingGroupType g_ShadingGroup39 = {
		.renderGroupName = "Shading group 39",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = true,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 4,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular,
			TextureType::Texture_Type_Emission
		}
	};

	static const XpsShadingGroupType g_ShadingGroup40 = {
		.renderGroupName = "Shading group 40",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup41 = {
		.renderGroupName = "Shading group 41",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = false,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup42 = {
		.renderGroupName = "Shading group 42",
		.hasShading = true,
		.hasAlpha = false,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = true,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular
		}
	};

	static const XpsShadingGroupType g_ShadingGroup43 = {
		.renderGroupName = "Shading group 43",
		.hasShading = true,
		.hasAlpha = true,
		.isPosable = true,
		.hasSpecular = true,
		.bump1Reg = false,
		.bump2Reg = false,
		.spec1Reg = true,
		.texCount = 3,
		.texTypes = {
			TextureType::Texture_Type_Diffuse,
			TextureType::Texture_Type_Bump,
			TextureType::Texture_Type_Specular
		}
	};

}