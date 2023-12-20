#pragma once
#include "Elements/Root.h"

namespace MyEngine
{
	namespace SimpleUi
	{
		class Elem;

		class SimpleUiSystem
		{
		public:
			SimpleUiSystem();
			~SimpleUiSystem() = default;

			SimpleUiSystem(const SimpleUiSystem& other) = delete;
			SimpleUiSystem(SimpleUiSystem&& other) noexcept = delete;
			SimpleUiSystem& operator=(const SimpleUiSystem& other) = delete;
			SimpleUiSystem& operator=(SimpleUiSystem&& other) noexcept = delete;

			Elem* GetRoot() { return &m_Root; }

		private:
			Root m_Root;
		};
	}
}
