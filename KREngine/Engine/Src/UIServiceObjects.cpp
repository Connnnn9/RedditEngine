#include "Precompiled.h"
#include "UIService.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace KREngine
{
	void UIService::CreateShape(const std::string& name, std::unique_ptr<sf::Shape> shape)
	{
		if (!shape) { AddLog("[ERROR] Null shape passed to CreateShape."); return; }
		mShapes[name] = std::move(shape);
		AddLog("[DEBUG] Created shape: " + name);
	}

	void UIService::SelectShape(const std::string& name)
	{
		SelectObject(name);
	}

	void UIService::DeleteSelectedShape()
	{
		DeleteSelectedObject();
	}

	const std::unordered_map<std::string, std::unique_ptr<sf::Shape>>& UIService::GetShapes() const
	{
		return mShapes;
	}

	void UIService::DuplicateSelectedShape()
	{
		if (mSelectedShape.empty())
		{
			AddLog("[ERROR] No shape selected to duplicate.");
			return;
		}

		if (IsSelected3D())
		{
			Primitive3D copy = mPrimitives3D[mSelectedShape];
			copy.position.x += 0.8f;
			copy.position.z += 0.8f;
			const std::string objectName = mSelectedShape + "_copy" + std::to_string(mPrimitiveCounter++);
			mPrimitives3D[objectName] = copy;
			SelectObject(objectName);
			AddLog("[DEBUG] Duplicated 3D object: " + objectName);
			return;
		}

		auto iter = mShapes.find(mSelectedShape);
		if (iter == mShapes.end())
		{
			AddLog("[ERROR] Selected shape is missing.");
			mSelectedShape.clear();
			return;
		}

		std::unique_ptr<sf::Shape> copy = CloneShape(*iter->second);
		if (!copy)
		{
			AddLog("[ERROR] Could not duplicate selected shape.");
			return;
		}

		copy->move(24.0f, 24.0f);
		const std::string shapeName = mSelectedShape + "_copy" + std::to_string(mShapeCounter++);
		CreateShape(shapeName, std::move(copy));
		SelectShape(shapeName);
	}

	void UIService::ResetSelectedShape()
	{
		if (IsSelected3D())
		{
			auto iter = mPrimitives3D.find(mSelectedShape);
			if (iter == mPrimitives3D.end()) return;

			iter->second.rotation = { 25.0f, 35.0f, 0.0f };
			iter->second.scale = { 1.0f, 1.0f, 1.0f };
			AddLog("[INFO] Reset 3D transform: " + mSelectedShape);
			return;
		}

		auto iter = mShapes.find(mSelectedShape);
		if (iter == mShapes.end()) return;

		iter->second->setScale(1.0f, 1.0f);
		iter->second->setRotation(0.0f);
		AddLog("[INFO] Reset transform: " + mSelectedShape);
	}

	void UIService::RandomizeSelectedShapeColor()
	{
		if (IsSelected3D())
		{
			auto iter = mPrimitives3D.find(mSelectedShape);
			if (iter == mPrimitives3D.end()) return;

			const sf::Uint8 red = static_cast<sf::Uint8>(80 + (std::rand() % 176));
			const sf::Uint8 green = static_cast<sf::Uint8>(80 + (std::rand() % 176));
			const sf::Uint8 blue = static_cast<sf::Uint8>(80 + (std::rand() % 176));
			iter->second.color = sf::Color(red, green, blue);
			AddLog("[INFO] Randomized 3D color: " + mSelectedShape);
			return;
		}

		auto iter = mShapes.find(mSelectedShape);
		if (iter == mShapes.end()) return;

		const sf::Uint8 red = static_cast<sf::Uint8>(80 + (std::rand() % 176));
		const sf::Uint8 green = static_cast<sf::Uint8>(80 + (std::rand() % 176));
		const sf::Uint8 blue = static_cast<sf::Uint8>(80 + (std::rand() % 176));
		iter->second->setFillColor(sf::Color(red, green, blue));
		AddLog("[INFO] Randomized color: " + mSelectedShape);
	}

	void UIService::Create3DPrimitive(const std::string& type)
	{
		Primitive3D primitive;
		std::string objectName;

		if (type == "Cube")
		{
			primitive.type = Primitive3DType::Cube;
			primitive.position = { -1.0f + static_cast<float>(mPrimitives3D.size()) * 0.35f, 0.0f, 0.0f };
			primitive.color = sf::Color(85, 170, 245);
			objectName = "Cube" + std::to_string(mPrimitiveCounter++);
		}
		else if (type == "Pyramid")
		{
			primitive.type = Primitive3DType::Pyramid;
			primitive.position = { 1.0f + static_cast<float>(mPrimitives3D.size()) * 0.35f, 0.0f, 0.0f };
			primitive.color = sf::Color(245, 145, 95);
			objectName = "Pyramid" + std::to_string(mPrimitiveCounter++);
		}
		else
		{
			AddLog("[ERROR] Unknown 3D primitive type: " + type);
			return;
		}

		mPrimitives3D[objectName] = primitive;
		SelectObject(objectName);
		AddLog("[INFO] Created 3D primitive: " + objectName);
	}

	void UIService::CreateDemoScene()
	{
		ClearCurrentScene();

		if (Is2DScene())
		{
			auto circle = CreateShapeFromType("Circle");
			circle->setPosition(120.0f, 160.0f);
			CreateShape("DemoCircle", std::move(circle));

			auto rectangle = CreateShapeFromType("Rectangle");
			rectangle->setPosition(280.0f, 220.0f);
			CreateShape("DemoRectangle", std::move(rectangle));

			auto triangle = CreateShapeFromType("Triangle");
			triangle->setPosition(470.0f, 180.0f);
			CreateShape("DemoTriangle", std::move(triangle));

			mSelectedShape = "DemoCircle";
			AddLog("[INFO] 2D demo scene loaded.");
			return;
		}

		Primitive3D cube;
		cube.type = Primitive3DType::Cube;
		cube.position = { -1.3f, 0.0f, 0.0f };
		cube.rotation = { 20.0f, 35.0f, 0.0f };
		cube.color = sf::Color(80, 170, 250);
		mPrimitives3D["DemoCube"] = cube;

		Primitive3D pyramid;
		pyramid.type = Primitive3DType::Pyramid;
		pyramid.position = { 1.3f, 0.0f, 0.0f };
		pyramid.rotation = { 20.0f, -25.0f, 0.0f };
		pyramid.color = sf::Color(245, 135, 90);
		mPrimitives3D["DemoPyramid"] = pyramid;

		mSelectedShape = "DemoCube";
		mAutoRotate3D = true;
		CameraService::GetInstance().Reset();
		AddLog("[INFO] 3D demo scene loaded.");
	}

	void UIService::ClearCurrentScene()
	{
		mSelectedShape.clear();
		mSelectedObjects.clear();
		mDraggedShapeType.clear();
		mMouseDraggingSelection = false;
		mMouseBoxSelecting = false;

		if (Is2DScene())
		{
			mShapes.clear();
			mShapeCounter = 0;
			AddLog("[INFO] 2D scene cleared.");
			return;
		}

		mPrimitives3D.clear();
		mPrimitiveCounter = 0;
		AddLog("[INFO] 3D scene cleared.");
	}

	void UIService::CenterAllObjects()
	{
		if (Is2DScene())
		{
			float shapeOffset = -static_cast<float>(mShapes.size()) * 45.0f;
			for (auto& [name, shape] : mShapes)
			{
				shape->setPosition(250.0f + shapeOffset, 180.0f);
				shapeOffset += 90.0f;
			}

			AddLog("[INFO] Centered 2D objects.");
			return;
		}

		float offset = -static_cast<float>(mPrimitives3D.size()) * 0.5f;
		for (auto& [name, primitive] : mPrimitives3D)
		{
			primitive.position = { offset, 0.0f, 0.0f };
			offset += 1.0f;
		}

		CameraService::GetInstance().Reset();
		AddLog("[INFO] Centered 3D objects.");
	}

	void UIService::SetSceneMode(SceneMode sceneMode)
	{
		if (mSceneMode == sceneMode)
		{
			return;
		}

		mSceneMode = sceneMode;
		mSelectedShape.clear();
		mSelectedObjects.clear();
		mDraggedShapeType.clear();
		mViewportAcceptsCameraInput = false;
		mMouseDraggingSelection = false;
		mMouseBoxSelecting = false;
		if (Is3DScene())
		{
			CameraService::GetInstance().Reset();
			if (mPrimitives3D.empty())
			{
				Create3DPrimitive("Cube");
			}
		}
		AddLog(Is2DScene() ? "[INFO] Switched to 2D scene." : "[INFO] Switched to 3D scene.");
	}

	void UIService::SelectObject(const std::string& name)
	{
		SelectOnlyObject(name);
	}

	void UIService::SelectOnlyObject(const std::string& name)
	{
		const bool is2DObject = mShapes.find(name) != mShapes.end();
		const bool is3DObject = mPrimitives3D.find(name) != mPrimitives3D.end();
		if ((Is2DScene() && is2DObject) || (Is3DScene() && is3DObject))
		{
			mSelectedShape = name;
			mSelectedObjects.clear();
			mSelectedObjects.push_back(name);
			AddLog("[DEBUG] Selected object: " + name);
			return;
		}

		AddLog("[ERROR] Object not available in current scene: " + name);
		ClearObjectSelection();
	}

	void UIService::AddObjectToSelection(const std::string& name)
	{
		const bool is2DObject = mShapes.find(name) != mShapes.end();
		const bool is3DObject = mPrimitives3D.find(name) != mPrimitives3D.end();
		if ((Is2DScene() && !is2DObject) || (Is3DScene() && !is3DObject))
		{
			return;
		}

		if (!IsObjectSelected(name))
		{
			mSelectedObjects.push_back(name);
		}
		mSelectedShape = name;
	}

	void UIService::ClearObjectSelection()
	{
		mSelectedShape.clear();
		mSelectedObjects.clear();
	}

	void UIService::DeleteSelectedObject()
	{
		if (mSelectedShape.empty())
		{
			return;
		}

		std::vector<std::string> namesToDelete = mSelectedObjects;
		if (namesToDelete.empty())
		{
			namesToDelete.push_back(mSelectedShape);
		}

		size_t deletedCount = 0;
		for (const std::string& name : namesToDelete)
		{
			if (mShapes.erase(name) > 0)
			{
				++deletedCount;
				continue;
			}
			if (mPrimitives3D.erase(name) > 0)
			{
				++deletedCount;
			}
		}

		ClearObjectSelection();
		AddLog(deletedCount > 0 ? "[DEBUG] Deleted selected objects: " + std::to_string(deletedCount) : "[ERROR] No selected objects were found for delete.");
	}

	std::string UIService::HitTest2DObject(sf::Vector2f worldPosition) const
	{
		for (const auto& [name, shape] : mShapes)
		{
			if (shape && shape->getGlobalBounds().contains(worldPosition))
			{
				return name;
			}
		}

		return {};
	}

	bool UIService::IsSelected3D() const
	{
		return !mSelectedShape.empty() && mPrimitives3D.find(mSelectedShape) != mPrimitives3D.end();
	}

	bool UIService::IsObjectSelected(const std::string& name) const
	{
		return std::find(mSelectedObjects.begin(), mSelectedObjects.end(), name) != mSelectedObjects.end();
	}

	bool UIService::Is2DScene() const
	{
		return mSceneMode == SceneMode::Scene2D;
	}

	bool UIService::Is3DScene() const
	{
		return mSceneMode == SceneMode::Scene3D;
	}

	sf::Vector2f UIService::SnapPosition(sf::Vector2f position) const
	{
		if (!mSnapToGrid || mGridSize <= 0.0f)
		{
			return position;
		}

		position.x = std::round(position.x / mGridSize) * mGridSize;
		position.y = std::round(position.y / mGridSize) * mGridSize;
		return position;
	}

	std::unique_ptr<sf::Shape> UIService::CreateShapeFromType(const std::string& type) const
	{
		if (type == "Circle")
		{
			auto shape = std::make_unique<sf::CircleShape>(50.0f);
			shape->setFillColor(sf::Color(96, 220, 150));
			return shape;
		}
		if (type == "Rectangle")
		{
			auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(120.0f, 70.0f));
			shape->setFillColor(sf::Color(85, 160, 240));
			return shape;
		}
		if (type == "Triangle")
		{
			auto shape = std::make_unique<sf::CircleShape>(60.0f, 3);
			shape->setFillColor(sf::Color(240, 110, 95));
			return shape;
		}

		return nullptr;
	}

	std::unique_ptr<sf::Shape> UIService::CloneShape(const sf::Shape& shape) const
	{
		if (const auto* circle = dynamic_cast<const sf::CircleShape*>(&shape))
		{
			return std::make_unique<sf::CircleShape>(*circle);
		}
		if (const auto* rectangle = dynamic_cast<const sf::RectangleShape*>(&shape))
		{
			return std::make_unique<sf::RectangleShape>(*rectangle);
		}
		if (const auto* convex = dynamic_cast<const sf::ConvexShape*>(&shape))
		{
			return std::make_unique<sf::ConvexShape>(*convex);
		}

		return nullptr;
	}
}
