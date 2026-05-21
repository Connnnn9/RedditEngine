#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "TextureService.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <random>
#include <string>
#include <vector>

namespace
{
	constexpr float Pi = 3.1415926535f;
	constexpr float DegToRad = Pi / 180.0f;

	struct Vec3
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	struct Planet
	{
		std::string name;
		std::string textureName;
		std::string atmosphereTextureName;
		std::string nightTextureName;
		float orbitRadius = 0.0f;
		float radius = 0.0f;
		float orbitSpeed = 0.0f;
		float selfSpinSpeed = 0.0f;
		float angle = 0.0f;
		float axialTilt = 0.0f;
	};

	struct Star
	{
		Vec3 position;
		float size = 1.0f;
	};

	struct Asteroid
	{
		float orbitRadius = 0.0f;
		float angle = 0.0f;
		float speed = 0.0f;
		float radius = 0.25f;
		float height = 0.0f;
	};

	std::filesystem::path ResolveAssetRoot()
	{
		const std::vector<std::filesystem::path> roots = {
			"Assets/GalaxyDemo",
			"../Assets/GalaxyDemo",
			"../../Assets/GalaxyDemo",
			"../../../Assets/GalaxyDemo",
			"KREngine/Assets/GalaxyDemo"
		};

		for (const std::filesystem::path& root : roots)
		{
			if (std::filesystem::exists(root))
			{
				return root;
			}
		}

		return "Assets/GalaxyDemo";
	}

	Vec3 GetOrbitPosition(float orbitRadius, float angle)
	{
		const float radians = angle * DegToRad;
		return { std::cos(radians) * orbitRadius, 0.0f, std::sin(radians) * orbitRadius };
	}

	void SetPerspective(float fovY, float aspect, float nearPlane, float farPlane)
	{
		const float top = std::tan(fovY * 0.5f * DegToRad) * nearPlane;
		const float right = top * aspect;
		glFrustum(-right, right, -top, top, nearPlane, farPlane);
	}

	void ConfigureOpenGL(const sf::Vector2u& size)
	{
		glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		SetPerspective(45.0f, static_cast<float>(size.x) / static_cast<float>(size.y), 1.0f, 4000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		const GLfloat ambient[] = { 0.16f, 0.18f, 0.23f, 1.0f };
		const GLfloat diffuse[] = { 1.0f, 0.94f, 0.82f, 1.0f };
		const GLfloat specular[] = { 0.72f, 0.66f, 0.55f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

		const GLfloat materialSpecular[] = { 0.32f, 0.32f, 0.30f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0f);
	}

	void ApplyCamera(float yaw, float pitch, float distance, const Vec3& focus)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -distance);
		glRotatef(pitch, 1.0f, 0.0f, 0.0f);
		glRotatef(yaw, 0.0f, 1.0f, 0.0f);
		glTranslatef(-focus.x, -focus.y, -focus.z);

		const GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	}

	void BindTexture(const sf::Texture* texture)
	{
		if (texture != nullptr)
		{
			sf::Texture::bind(texture, sf::Texture::Normalized);
		}
		else
		{
			sf::Texture::bind(nullptr);
		}
	}

	void DrawSphere(float radius, int slices, int stacks, const sf::Texture* texture, float alpha = 1.0f)
	{
		BindTexture(texture);
		glColor4f(1.0f, 1.0f, 1.0f, alpha);

		for (int stack = 0; stack < stacks; ++stack)
		{
			const float v0 = static_cast<float>(stack) / static_cast<float>(stacks);
			const float v1 = static_cast<float>(stack + 1) / static_cast<float>(stacks);
			const float phi0 = (v0 - 0.5f) * Pi;
			const float phi1 = (v1 - 0.5f) * Pi;

			glBegin(GL_QUAD_STRIP);
			for (int slice = 0; slice <= slices; ++slice)
			{
				const float u = static_cast<float>(slice) / static_cast<float>(slices);
				const float theta = u * Pi * 2.0f;

				const float x0 = std::cos(phi0) * std::cos(theta);
				const float y0 = std::sin(phi0);
				const float z0 = std::cos(phi0) * std::sin(theta);
				const float x1 = std::cos(phi1) * std::cos(theta);
				const float y1 = std::sin(phi1);
				const float z1 = std::cos(phi1) * std::sin(theta);

				glTexCoord2f(u, 1.0f - v0);
				glNormal3f(x0, y0, z0);
				glVertex3f(x0 * radius, y0 * radius, z0 * radius);

				glTexCoord2f(u, 1.0f - v1);
				glNormal3f(x1, y1, z1);
				glVertex3f(x1 * radius, y1 * radius, z1 * radius);
			}
			glEnd();
		}
	}

	void DrawOrbit(float radius)
	{
		glDisable(GL_LIGHTING);
		BindTexture(nullptr);
		glColor4f(0.45f, 0.58f, 0.82f, 0.28f);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 240; ++i)
		{
			const float a = (static_cast<float>(i) / 240.0f) * Pi * 2.0f;
			glVertex3f(std::cos(a) * radius, 0.0f, std::sin(a) * radius);
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}

	void DrawSaturnRing(float innerRadius, float outerRadius, const sf::Texture* texture)
	{
		glDisable(GL_LIGHTING);
		BindTexture(texture);
		glColor4f(1.0f, 1.0f, 1.0f, 0.86f);
		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i <= 240; ++i)
		{
			const float u = static_cast<float>(i) / 240.0f;
			const float a = u * Pi * 2.0f;
			const float x = std::cos(a);
			const float z = std::sin(a);

			glTexCoord2f(u, 0.0f);
			glVertex3f(x * outerRadius, 0.0f, z * outerRadius);
			glTexCoord2f(u, 1.0f);
			glVertex3f(x * innerRadius, 0.0f, z * innerRadius);
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}

	void DrawStars(const std::vector<Star>& stars)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		BindTexture(nullptr);
		glBegin(GL_POINTS);
		for (const Star& star : stars)
		{
			glColor4f(0.78f, 0.86f, 1.0f, 0.78f);
			glVertex3f(star.position.x, star.position.y, star.position.z);
		}
		glEnd();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

	void DrawSun(const sf::Texture* texture, float totalTime)
	{
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glRotatef(totalTime * 5.0f, 0.0f, 1.0f, 0.0f);
		DrawSphere(20.0f, 64, 32, texture);
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glDisable(GL_LIGHTING);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		BindTexture(nullptr);
		for (int i = 0; i < 3; ++i)
		{
			const float radius = 29.0f + i * 10.0f;
			const float alpha = 0.14f - i * 0.035f;
			glColor4f(1.0f, 0.47f, 0.10f, alpha);
			DrawSphere(radius, 48, 20, nullptr, alpha);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);
		glEnable(GL_LIGHTING);
	}

	void DrawText(sf::RenderWindow& window, const sf::Font& font, const std::string& text, unsigned int size, sf::Vector2f position, sf::Color color)
	{
		sf::Text label(text, font, size);
		label.setFillColor(color);
		label.setPosition(position);
		window.draw(label);
	}

	void DrawPanel(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::string>& importedTextures, float speed, bool paused)
	{
		sf::RectangleShape panel({ 394.0f, 318.0f });
		panel.setPosition(986.0f, 28.0f);
		panel.setFillColor(sf::Color(8, 12, 26, 218));
		panel.setOutlineColor(sf::Color(95, 124, 170, 118));
		panel.setOutlineThickness(1.0f);
		window.draw(panel);

		DrawText(window, font, "3D Solar System", 22, { 1006.0f, 46.0f }, sf::Color(238, 244, 255));
		DrawText(window, font, "OpenGL spheres + KREngine texture import", 13, { 1006.0f, 76.0f }, sf::Color(150, 174, 210));
		DrawText(window, font, "Loaded textures: " + std::to_string(importedTextures.size()), 14, { 1006.0f, 106.0f }, sf::Color(196, 218, 245));

		float y = 132.0f;
		for (const std::string& name : importedTextures)
		{
			DrawText(window, font, "- " + name, 12, { 1006.0f, y }, sf::Color(204, 216, 232));
			y += 16.0f;
			if (y > 236.0f)
			{
				break;
			}
		}

		const std::string state = paused ? "Paused" : "Running";
		DrawText(window, font, "State: " + state + "   Speed: x" + std::to_string(static_cast<int>(speed)), 13, { 1006.0f, 268.0f }, sf::Color(226, 204, 145));
		DrawText(window, font, "Drag mouse orbit  Wheel zoom  Space pause", 12, { 1006.0f, 294.0f }, sf::Color(142, 164, 196));
		DrawText(window, font, "1-8 focus planet  0 sun  WASD pan  R reset", 12, { 1006.0f, 314.0f }, sf::Color(142, 164, 196));
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 2;
	settings.minorVersion = 1;

	sf::RenderWindow window(sf::VideoMode(1400, 900), "KREngine Galaxy Demo: 3D Solar System", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	ConfigureOpenGL(window.getSize());

	auto& textureService = KREngine::TextureService::GetInstance();
	const std::filesystem::path assetRoot = ResolveAssetRoot();
	const std::vector<std::string> importedTextures = textureService.ImportTexturesFromDirectory(assetRoot);

	std::vector<Planet> planets = {
		{ "Mercury", "2k_mercury", "", "", 44.0f, 3.8f, 46.0f, 160.0f, 20.0f, 2.0f },
		{ "Venus", "2k_venus_surface", "2k_venus_atmosphere", "", 64.0f, 5.8f, 32.0f, 95.0f, 80.0f, 177.0f },
		{ "Earth", "2k_earth_daymap", "2k_earth_clouds_alpha", "2k_earth_nightmap", 86.0f, 6.5f, 24.0f, 140.0f, 160.0f, 23.5f },
		{ "Mars", "2k_mars", "", "", 112.0f, 4.6f, 18.0f, 110.0f, 260.0f, 25.0f },
		{ "Jupiter", "2k_jupiter", "", "", 150.0f, 15.5f, 10.0f, 82.0f, 315.0f, 3.0f },
		{ "Saturn", "2k_saturn", "", "", 198.0f, 13.5f, 7.0f, 64.0f, 45.0f, 26.7f },
		{ "Uranus", "2k_uranus", "", "", 242.0f, 10.2f, 5.2f, 52.0f, 215.0f, 97.0f },
		{ "Neptune", "2k_neptune", "", "", 284.0f, 10.0f, 4.2f, 48.0f, 300.0f, 28.0f }
	};

	std::mt19937 random(42);
	std::uniform_real_distribution<float> starDist(-900.0f, 900.0f);
	std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
	std::uniform_real_distribution<float> asteroidOrbitDist(122.0f, 136.0f);
	std::uniform_real_distribution<float> asteroidSpeedDist(7.0f, 14.0f);
	std::uniform_real_distribution<float> asteroidHeightDist(-3.5f, 3.5f);

	std::vector<Star> stars;
	stars.reserve(520);
	for (int i = 0; i < 520; ++i)
	{
		stars.push_back({ { starDist(random), starDist(random), starDist(random) }, 1.0f });
	}

	std::vector<Asteroid> asteroids;
	asteroids.reserve(140);
	for (int i = 0; i < 140; ++i)
	{
		asteroids.push_back({ asteroidOrbitDist(random), angleDist(random), asteroidSpeedDist(random), 0.35f + (i % 3) * 0.12f, asteroidHeightDist(random) });
	}

	sf::Font font;
	const bool hasFont = font.loadFromFile("C:/Windows/Fonts/segoeui.ttf");

	sf::Clock clock;
	float totalTime = 0.0f;
	float simulationSpeed = 1.0f;
	bool paused = false;
	bool dragging = false;
	sf::Vector2i lastMousePosition;

	float cameraYaw = -33.0f;
	float cameraPitch = 31.0f;
	float cameraDistance = 335.0f;
	Vec3 cameraFocus{ 0.0f, 0.0f, 0.0f };
	int focusPlanetIndex = -1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				ConfigureOpenGL({ event.size.width, event.size.height });
			}
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				dragging = true;
				lastMousePosition = { event.mouseButton.x, event.mouseButton.y };
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				dragging = false;
			}
			else if (event.type == sf::Event::MouseMoved && dragging)
			{
				const sf::Vector2i current{ event.mouseMove.x, event.mouseMove.y };
				const sf::Vector2i delta = current - lastMousePosition;
				cameraYaw += static_cast<float>(delta.x) * 0.32f;
				cameraPitch = std::clamp(cameraPitch + static_cast<float>(delta.y) * 0.22f, -15.0f, 82.0f);
				lastMousePosition = current;
			}
			else if (event.type == sf::Event::MouseWheelScrolled)
			{
				cameraDistance = std::clamp(cameraDistance - event.mouseWheelScroll.delta * 24.0f, 90.0f, 900.0f);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					paused = !paused;
				}
				else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal)
				{
					simulationSpeed = std::min(8.0f, simulationSpeed + 1.0f);
				}
				else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen)
				{
					simulationSpeed = std::max(1.0f, simulationSpeed - 1.0f);
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					cameraYaw = -33.0f;
					cameraPitch = 31.0f;
					cameraDistance = 335.0f;
					cameraFocus = { 0.0f, 0.0f, 0.0f };
					focusPlanetIndex = -1;
				}
				else if (event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Numpad0)
				{
					focusPlanetIndex = -1;
					cameraFocus = { 0.0f, 0.0f, 0.0f };
					cameraDistance = 210.0f;
				}
				else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num8)
				{
					focusPlanetIndex = event.key.code - sf::Keyboard::Num1;
					cameraDistance = focusPlanetIndex <= 3 ? 74.0f : 118.0f;
				}
				else if (event.key.code >= sf::Keyboard::Numpad1 && event.key.code <= sf::Keyboard::Numpad8)
				{
					focusPlanetIndex = event.key.code - sf::Keyboard::Numpad1;
					cameraDistance = focusPlanetIndex <= 3 ? 74.0f : 118.0f;
				}
			}
		}

		const float rawDeltaTime = clock.restart().asSeconds();
		const float deltaTime = paused ? 0.0f : rawDeltaTime * simulationSpeed;
		totalTime += deltaTime;

		const float panSpeed = 38.0f * rawDeltaTime * (cameraDistance / 335.0f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { cameraFocus.x -= panSpeed; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { cameraFocus.x += panSpeed; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { cameraFocus.z -= panSpeed; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { cameraFocus.z += panSpeed; }

		for (Planet& planet : planets)
		{
			planet.angle += planet.orbitSpeed * deltaTime;
			if (planet.angle > 360.0f)
			{
				planet.angle -= 360.0f;
			}
		}

		for (Asteroid& asteroid : asteroids)
		{
			asteroid.angle += asteroid.speed * deltaTime;
			if (asteroid.angle > 360.0f)
			{
				asteroid.angle -= 360.0f;
			}
		}

		if (focusPlanetIndex >= 0 && focusPlanetIndex < static_cast<int>(planets.size()))
		{
			const Vec3 position = GetOrbitPosition(planets[focusPlanetIndex].orbitRadius, planets[focusPlanetIndex].angle);
			cameraFocus.x += (position.x - cameraFocus.x) * std::min(1.0f, rawDeltaTime * 5.0f);
			cameraFocus.y += (position.y - cameraFocus.y) * std::min(1.0f, rawDeltaTime * 5.0f);
			cameraFocus.z += (position.z - cameraFocus.z) * std::min(1.0f, rawDeltaTime * 5.0f);
		}

		glClearColor(0.012f, 0.018f, 0.04f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ApplyCamera(cameraYaw, cameraPitch, cameraDistance, cameraFocus);

		glPointSize(2.0f);
		DrawStars(stars);

		for (const Planet& planet : planets)
		{
			DrawOrbit(planet.orbitRadius);
		}

		glDisable(GL_LIGHTING);
		BindTexture(nullptr);
		for (const Asteroid& asteroid : asteroids)
		{
			const Vec3 position = GetOrbitPosition(asteroid.orbitRadius, asteroid.angle);
			glPushMatrix();
			glTranslatef(position.x, asteroid.height, position.z);
			glColor4f(0.58f, 0.52f, 0.44f, 1.0f);
			DrawSphere(asteroid.radius, 12, 6, nullptr);
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);

		DrawSun(textureService.GetTexture("sun"), totalTime);

		for (const Planet& planet : planets)
		{
			const Vec3 position = GetOrbitPosition(planet.orbitRadius, planet.angle);

			glPushMatrix();
			glTranslatef(position.x, 0.0f, position.z);
			glRotatef(planet.axialTilt, 0.0f, 0.0f, 1.0f);

			if (planet.name == "Saturn")
			{
				glPushMatrix();
				glRotatef(8.0f, 1.0f, 0.0f, 0.0f);
				DrawSaturnRing(planet.radius * 1.25f, planet.radius * 2.45f, textureService.GetTexture("2k_saturn_ring_alpha"));
				glPopMatrix();
			}

			glPushMatrix();
			glRotatef(totalTime * planet.selfSpinSpeed, 0.0f, 1.0f, 0.0f);
			DrawSphere(planet.radius, 72, 36, textureService.GetTexture(planet.textureName));
			glPopMatrix();

			if (!planet.nightTextureName.empty())
			{
				glDisable(GL_LIGHTING);
				glDepthMask(GL_FALSE);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glPushMatrix();
				glRotatef(totalTime * planet.selfSpinSpeed, 0.0f, 1.0f, 0.0f);
				DrawSphere(planet.radius * 1.006f, 72, 36, textureService.GetTexture(planet.nightTextureName), 0.18f);
				glPopMatrix();
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDepthMask(GL_TRUE);
				glEnable(GL_LIGHTING);
			}

			if (!planet.atmosphereTextureName.empty())
			{
				glDisable(GL_LIGHTING);
				glDepthMask(GL_FALSE);
				glPushMatrix();
				glRotatef(totalTime * planet.selfSpinSpeed * 0.45f, 0.0f, 1.0f, 0.0f);
				DrawSphere(planet.radius * 1.035f, 72, 36, textureService.GetTexture(planet.atmosphereTextureName), planet.name == "Earth" ? 0.62f : 0.22f);
				glPopMatrix();
				glDepthMask(GL_TRUE);
				glEnable(GL_LIGHTING);
			}

			if (planet.name == "Earth")
			{
				const Vec3 moonPosition = GetOrbitPosition(planet.radius * 3.3f, totalTime * 130.0f);
				glPushMatrix();
				glTranslatef(moonPosition.x, 0.4f, moonPosition.z);
				DrawSphere(1.35f, 32, 16, nullptr);
				glPopMatrix();
			}

			glPopMatrix();
		}

		window.pushGLStates();
		if (hasFont)
		{
			sf::View uiView = window.getDefaultView();
			window.setView(uiView);
			DrawText(window, font, "Solar System Demo", 30, { 28.0f, 22.0f }, sf::Color(236, 243, 255));
			DrawText(window, font, "Now rendered as a 3D OpenGL scene", 15, { 30.0f, 60.0f }, sf::Color(151, 176, 210));
			DrawPanel(window, font, importedTextures, simulationSpeed, paused);
		}
		window.popGLStates();

		window.display();
	}

	sf::Texture::bind(nullptr);
	return 0;
}
