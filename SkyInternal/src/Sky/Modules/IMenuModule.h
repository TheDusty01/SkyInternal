#pragma once

#include "SkyModule.h"

namespace Sky
{

	namespace Modules
	{

		class IMenuModule : public SkyModule
		{
		// Methods
		public:
			virtual void OnMenuRender() final;
			virtual void OnMenuContentRender() = 0;
		};
	
	}
}