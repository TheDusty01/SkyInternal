#pragma once

#include "SkyModule.h"
#include "IPageMenuModule.h"

#include "../Game/GameData.h"

#include "../Core/Debug.h"

namespace Sky
{
	namespace Modules
	{

		class VisualsModule : public IPageMenuModule
		{
		// Fields
		private:
#if _DEBUG
			std::shared_ptr<Logger> m_logger = Logger::Create(TYPE_NAME(VisualsModule), LogLevel::Information);
#endif
		public:

		// Methods
		protected:
			//void OnMenuMainPageRender() override;

		public:
			void OnInitialize() override;
			void OnUpdate() override;
			void OnRender(const Visuals& visuals) override;
			//void OnMenuRender() override;
			//void OnMenuContentRender() override;
			void OnMenuPageRender(uint8_t pageIndex) override;
		};

	}
}
