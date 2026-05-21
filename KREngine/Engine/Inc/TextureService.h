#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace KREngine
{
	class TextureService
	{
	public:
		static TextureService& GetInstance();

		const sf::Texture* ImportTexture(const std::string& name, const std::string& filePath);
		const sf::Texture* ImportTexture(const std::filesystem::path& filePath);
		std::vector<std::string> ImportTexturesFromDirectory(const std::filesystem::path& directoryPath);
		const sf::Texture* GetTexture(const std::string& name) const;
		std::vector<std::string> GetTextureNames() const;
		size_t GetTextureCount() const;
		void Clear();

	private:
		TextureService() = default;

		std::unordered_map<std::string, sf::Texture> mTextures;
	};
}
