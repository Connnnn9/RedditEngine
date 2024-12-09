#pragma once

#include <string>

namespace KREngine
{
	namespace UI
	{
		class OptionBox
		{
		public:
			explicit OptionBox(const std::string& name);
			void Render();

		private:
			std::string mName;
		};
	}
}
