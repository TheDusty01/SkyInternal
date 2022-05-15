#pragma once

#include "IMenuModule.h"

#include "../Core/Debug.h"

namespace Sky
{
	namespace Modules
	{
		using PageFunction = void (*)(uint8_t);
		class IPageMenuModule : public IMenuModule
		{
		// Fields
		private:
#if _DEBUG
			std::shared_ptr<Logger> m_logger = Logger::Create(TYPE_NAME(VisualsModule), LogLevel::Information);
#endif
			uint8_t m_activePage = 0;
		public:
			std::map<uint8_t, PageFunction> m_pages;

		protected:
			uint8_t m_pageCount = 2;

			//void AddOrReplacePage(uint8_t pageIndex, PageFunction callback);
			//void AddPage(PageFunction callback);

			//virtual void OnMenuMainPageRender() = 0;
		public:

		// Methods
		public:
			virtual void OnMenuContentRender() final;
			virtual void OnMenuPageRender(uint8_t pageIndex) = 0;
		};

	}
}

