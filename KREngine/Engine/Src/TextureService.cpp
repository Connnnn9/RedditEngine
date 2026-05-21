#include "Precompiled.h"
#include "TextureService.h"

#include <algorithm>
#include <cctype>

namespace KREngine
{
	TextureService& TextureService::GetInstance()
	{
		static TextureService instance;
		return instance;
	}

	const sf::Texture* TextureService::ImportTexture(const std::string& name, const std::string& filePath)
	{
		if (const sf::Texture* existing = GetTexture(name))
		{
			return existing;
		}

		sf::Texture texture;
		if (!texture.loadFromFile(filePath))
		{
			return nullptr;
		}

		texture.setSmooth(true);
		auto [iter, inserted] = mTextures.emplace(name, std::move(texture));
		return &iter->second;
	}

	const sf::Texture* TextureService::ImportTexture(const std::filesystem::path& filePath)
	{
		return ImportTexture(filePath.stem().string(), filePath.string());
	}

	std::vector<std::string> TextureService::ImportTexturesFromDirectory(const std::filesystem::path& directoryPath)
	{
		std::vector<std::string> importedNames;
		if (!std::filesystem::exists(directoryPath))
		{
			return importedNames;
		}

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directoryPath))
		{
			if (!entry.is_regular_file())
			{
				continue;
			}

			std::string extension = entry.path().extension().string();
			std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
			{
				if (ImportTexture(entry.path()) != nullptr)
				{
					importedNames.push_back(entry.path().stem().string());
				}
			}
		}

		std::sort(importedNames.begin(), importedNames.end());
		return importedNames;
	}

	const sf::Texture* TextureService::GetTexture(const std::string& name) const
	{
		const auto iter = mTextures.find(name);
		return iter != mTextures.end() ? &iter->second : nullptr;
	}

	std::vector<std::string> TextureService::GetTextureNames() const
	{
		std::vector<std::string> names;
		names.reserve(mTextures.size());
		for (const auto& [name, texture] : mTextures)
		{
			names.push_back(name);
		}
		std::sort(names.begin(), names.end());
		return names;
	}

	size_t TextureService::GetTextureCount() const
	{
		return mTextures.size();
	}

	void TextureService::Clear()
	{
		mTextures.clear();
	}
}
