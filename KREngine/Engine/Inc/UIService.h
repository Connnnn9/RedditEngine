#pragma once

#include <SFML/Graphics.hpp>
#include "CameraService.h"
#include "GameObject.h"

struct ImVec2;

namespace KREngine
{
	enum class Primitive3DType
	{
		Cube,
		Pyramid
	};

	enum class SceneMode
	{
		Scene2D,
		Scene3D
	};

	struct Primitive3D
	{
		Primitive3DType type = Primitive3DType::Cube;
		sf::Vector3f position = { 0.0f, 0.0f, 0.0f };
		sf::Vector3f rotation = { 25.0f, 35.0f, 0.0f };
		sf::Vector3f scale = { 1.0f, 1.0f, 1.0f };
		sf::Color color = sf::Color::White;
	};

	class UIService
	{
	public:
		void Initialize(sf::RenderWindow& window);
		void Terminate();
		void Update(sf::RenderWindow& window, sf::Time deltaTime);
		void Render(sf::RenderWindow& window);

		void CreateShape(const std::string& name, std::unique_ptr<sf::Shape> shape);
		void SelectShape(const std::string& name);
		void DeleteSelectedShape();

		SceneMode GetSceneMode() const;
		bool ShouldRender2DShapes() const;
		const std::unordered_map<std::string, std::unique_ptr<sf::Shape>>& GetShapes() const;

	private:
		void RenderToolbar(sf::RenderWindow& window);
		void RenderSceneModeSwitch();
		void RenderHierarchy();
		void RenderInspector();
		void RenderConsole();
		void RenderViewport(sf::RenderWindow& window);
		void Render3DScene(sf::RenderWindow& window, const ImVec2& viewportPos, const ImVec2& viewportSize);
		void UpdateCameraControls(const sf::RenderWindow& window, sf::Time deltaTime);
		void ProcessMouseEditor(sf::RenderWindow& window, bool viewportCanvasHovered);
		void DrawMouseEditorOverlay(sf::RenderWindow& window, const ImVec2& viewportPos, const ImVec2& viewportSize);
		void Draw3DPrimitive(const Primitive3D& primitive) const;
		void DrawCube() const;
		void DrawPyramid() const;
		void ApplyCustomStyle();
		void AddLog(const std::string& message);
		void DuplicateSelectedShape();
		void ResetSelectedShape();
		void RandomizeSelectedShapeColor();
		void Create3DPrimitive(const std::string& type);
		void CreateDemoScene();
		void ClearCurrentScene();
		void CenterAllObjects();
		void SetSceneMode(SceneMode sceneMode);
		void SelectObject(const std::string& name);
		void SelectOnlyObject(const std::string& name);
		void AddObjectToSelection(const std::string& name);
		void ClearObjectSelection();
		void DeleteSelectedObject();
		std::string HitTest2DObject(sf::Vector2f worldPosition) const;
		bool Is2DScene() const;
		bool Is3DScene() const;
		bool IsSelected3D() const;
		bool IsObjectSelected(const std::string& name) const;
		sf::Vector2f SnapPosition(sf::Vector2f position) const;
		std::unique_ptr<sf::Shape> CreateShapeFromType(const std::string& type) const;
		std::unique_ptr<sf::Shape> CloneShape(const sf::Shape& shape) const;

		std::unordered_map<std::string, std::unique_ptr<sf::Shape>> mShapes;
		std::unordered_map<std::string, Primitive3D> mPrimitives3D;
		std::string mSelectedShape;
		std::vector<std::string> mSelectedObjects;
		std::vector<std::string> mLogs;
		std::string mDraggedShapeType;
		uint32_t mShapeCounter = 0;
		uint32_t mPrimitiveCounter = 0;
		SceneMode mSceneMode = SceneMode::Scene2D;
		bool mAutoRotate3D = false;
		bool mWireframe3D = false;
		bool mSnapToGrid = true;
		bool mShowConsole = true;
		bool mThemeDark = true;
		bool mViewportAcceptsCameraInput = false;
		bool mMouseEditEnabled = false;
		bool mMouseCamera3DEnabled = false;
		bool mMouseDraggingSelection = false;
		bool mMouseBoxSelecting = false;
		float mGridSize = 25.0f;
		float mAutoRotateSpeed = 35.0f;
		sf::Vector2f mLastMouseWorldPosition = { 0.0f, 0.0f };
		sf::Vector2f mSelectionBoxStartWorld = { 0.0f, 0.0f };
		sf::Vector2f mSelectionBoxEndWorld = { 0.0f, 0.0f };
	};
}
