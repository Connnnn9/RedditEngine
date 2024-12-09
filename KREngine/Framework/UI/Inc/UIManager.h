#pragma once

#include <vector>
#include <string>

namespace KREngine
{
	namespace UI
	{
		class UIManager
		{
		public:
			void Initialize();
			void Terminate();
			void Update();
			void Render();

			void AddOption(const std::string& optionName);
			const std::vector<std::string>& GetOptions() const;

		private:
			std::vector<std::string> mOptions;
		};
	}
}
