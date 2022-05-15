#pragma once

#include "SkyModule.h"

#if _DEBUG
#include "../Core/Debug.h"
#endif

namespace Sky
{
	namespace Modules
	{

		class DemoModule : public SkyModule
		{
		private:
#if _DEBUG
			std::shared_ptr<Logger> m_logger = Logger::Create(TYPE_NAME(DemoModule));
#endif
		public:
			void OnInitialize() override;
			void OnUpdate() override;
			void OnRender(const Visuals& visuals) override;
			void OnMenuRender() override;
		};

	}
}
