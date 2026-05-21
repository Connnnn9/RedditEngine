#include "Precompiled.h"
#include "UIService.h"
#include <imgui.h>
#include <algorithm>
#include <cmath>

namespace KREngine
{
	void UIService::RenderToolbar(sf::RenderWindow& window)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
					ClearCurrentScene();
				}
				if (ImGui::MenuItem("Load Demo Scene")) CreateDemoScene();
				if (ImGui::MenuItem("Save")) AddLog("[INFO] Save clicked.");
				if (ImGui::MenuItem("Exit"))
				{
					AddLog("[INFO] Exit clicked.");
					window.close();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Duplicate", nullptr, false, !mSelectedShape.empty())) DuplicateSelectedShape();
				if (ImGui::MenuItem("Reset Transform", nullptr, false, !mSelectedShape.empty())) ResetSelectedShape();
				if (ImGui::MenuItem("Random Color", nullptr, false, !mSelectedShape.empty())) RandomizeSelectedShapeColor();
				if (ImGui::MenuItem(Is2DScene() ? "Center 2D Objects" : "Center 3D Objects")) CenterAllObjects();
				if (Is2DScene())
				{
					ImGui::Separator();
					if (ImGui::MenuItem("Mouse Edit", nullptr, mMouseEditEnabled))
					{
						mMouseEditEnabled = !mMouseEditEnabled;
						mMouseDraggingSelection = false;
						mMouseBoxSelecting = false;
						AddLog(mMouseEditEnabled ? "[INFO] Mouse editor enabled." : "[INFO] Mouse editor disabled.");
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("2D Scene", nullptr, Is2DScene()))
				{
					SetSceneMode(SceneMode::Scene2D);
				}
				if (ImGui::MenuItem("3D Scene", nullptr, Is3DScene()))
				{
					SetSceneMode(SceneMode::Scene3D);
				}
				if (Is3DScene())
				{
					ImGui::Separator();
					if (ImGui::MenuItem("Wireframe 3D", nullptr, mWireframe3D))
					{
						mWireframe3D = !mWireframe3D;
					}
					if (ImGui::MenuItem("Auto Rotate 3D", nullptr, mAutoRotate3D))
					{
						mAutoRotate3D = !mAutoRotate3D;
					}
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Console", nullptr, mShowConsole))
				{
					mShowConsole = !mShowConsole;
				}
				if (ImGui::MenuItem("Dark Theme", nullptr, mThemeDark))
				{
					mThemeDark = !mThemeDark;
					ApplyCustomStyle();
					AddLog("[INFO] Theme toggled.");
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			RenderSceneModeSwitch();
			ImGui::Separator();
			ImGui::Text(Is2DScene() ? "2D: %zu" : "3D: %zu", Is2DScene() ? mShapes.size() : mPrimitives3D.size());
			ImGui::Separator();
			ImGui::TextDisabled("Hidden %s: %zu", Is2DScene() ? "3D" : "2D", Is2DScene() ? mPrimitives3D.size() : mShapes.size());
			if (!mSelectedShape.empty())
			{
				ImGui::Separator();
				ImGui::Text("Selected: %zu", mSelectedObjects.empty() ? 1 : mSelectedObjects.size());
			}
			ImGui::EndMainMenuBar();
		}
	}

	void UIService::RenderSceneModeSwitch()
	{
		const bool is2DActive = Is2DScene();
		const bool is3DActive = Is3DScene();

		if (is2DActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.42f, 0.55f, 1.0f));
		}
		if (ImGui::Button("2D Scene"))
		{
			SetSceneMode(SceneMode::Scene2D);
		}
		if (is2DActive)
		{
			ImGui::PopStyleColor();
		}

		ImGui::SameLine();

		if (is3DActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.42f, 0.55f, 1.0f));
		}
		if (ImGui::Button("3D Scene"))
		{
			SetSceneMode(SceneMode::Scene3D);
		}
		if (is3DActive)
		{
			ImGui::PopStyleColor();
		}
	}

	void UIService::RenderHierarchy()
	{
		ImGui::Text("Active Scene");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.38f, 0.72f, 0.70f, 1.0f), Is2DScene() ? "2D" : "3D");
		ImGui::Separator();

		if (Is2DScene())
		{
			ImGui::TextUnformatted("Create 2D");
			ImGui::Separator();
			if (ImGui::Button("Circle", ImVec2(-1.0f, 0.0f))) { mDraggedShapeType = "Circle"; AddLog("[INFO] Circle placement armed."); }
			if (ImGui::Button("Rectangle", ImVec2(-1.0f, 0.0f))) { mDraggedShapeType = "Rectangle"; AddLog("[INFO] Rectangle placement armed."); }
			if (ImGui::Button("Triangle", ImVec2(-1.0f, 0.0f))) { mDraggedShapeType = "Triangle"; AddLog("[INFO] Triangle placement armed."); }

			ImGui::Spacing();
			ImGui::TextUnformatted("2D Tools");
			ImGui::Separator();
			if (ImGui::Button(mMouseEditEnabled ? "Mouse Edit: On" : "Mouse Edit: Off", ImVec2(-1.0f, 0.0f)))
			{
				mMouseEditEnabled = !mMouseEditEnabled;
				mMouseDraggingSelection = false;
				mMouseBoxSelecting = false;
				AddLog(mMouseEditEnabled ? "[INFO] Mouse editor enabled." : "[INFO] Mouse editor disabled.");
			}
			if (ImGui::Button("Load 2D Demo", ImVec2(-1.0f, 0.0f))) CreateDemoScene();
			if (ImGui::Button("Center 2D Objects", ImVec2(-1.0f, 0.0f))) CenterAllObjects();
			if (ImGui::Button("Clear 2D Scene", ImVec2(-1.0f, 0.0f))) ClearCurrentScene();

			ImGui::Spacing();
			ImGui::Checkbox("Snap to grid", &mSnapToGrid);
			ImGui::DragFloat("Grid size", &mGridSize, 1.0f, 5.0f, 100.0f);

			ImGui::Spacing();
			ImGui::Text("2D Objects (%zu)", mShapes.size());
			ImGui::Separator();
			for (const auto& [name, shape] : mShapes)
			{
				const sf::Color color = shape->getFillColor();
				const ImVec4 previewColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
				ImGui::PushID(name.c_str());
				ImGui::ColorButton("Color", previewColor, ImGuiColorEditFlags_NoTooltip, ImVec2(14.0f, 14.0f));
				ImGui::SameLine();
				if (ImGui::Selectable(name.c_str(), IsObjectSelected(name)))
				{
					SelectObject(name);
				}
				ImGui::PopID();
			}
		}
		else
		{
			ImGui::TextUnformatted("Create 3D");
			ImGui::Separator();
			if (ImGui::Button("Cube", ImVec2(-1.0f, 0.0f))) Create3DPrimitive("Cube");
			if (ImGui::Button("Pyramid", ImVec2(-1.0f, 0.0f))) Create3DPrimitive("Pyramid");

			ImGui::Spacing();
			ImGui::TextUnformatted("3D Tools");
			ImGui::Separator();
			if (ImGui::Button("Load 3D Demo", ImVec2(-1.0f, 0.0f))) CreateDemoScene();
			if (ImGui::Button("Center 3D Objects", ImVec2(-1.0f, 0.0f))) CenterAllObjects();
			if (ImGui::Button("Clear 3D Scene", ImVec2(-1.0f, 0.0f))) ClearCurrentScene();

			ImGui::Spacing();
			if (ImGui::Button(mMouseCamera3DEnabled ? "Mouse Camera: On" : "Mouse Camera: Off", ImVec2(-1.0f, 0.0f)))
			{
				mMouseCamera3DEnabled = !mMouseCamera3DEnabled;
				AddLog(mMouseCamera3DEnabled ? "[INFO] 3D mouse camera enabled." : "[INFO] 3D mouse camera disabled.");
			}
			ImGui::Checkbox("Wireframe", &mWireframe3D);
			ImGui::Checkbox("Auto rotate", &mAutoRotate3D);
			ImGui::DragFloat("Auto rotate speed", &mAutoRotateSpeed, 1.0f, 5.0f, 180.0f);

			ImGui::Spacing();
			ImGui::Text("3D Objects (%zu)", mPrimitives3D.size());
			ImGui::Separator();
			for (const auto& [name, primitive] : mPrimitives3D)
			{
				const sf::Color color = primitive.color;
				const ImVec4 previewColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
				ImGui::PushID(name.c_str());
				ImGui::ColorButton("Color", previewColor, ImGuiColorEditFlags_NoTooltip, ImVec2(14.0f, 14.0f));
				ImGui::SameLine();
				if (ImGui::Selectable(name.c_str(), IsObjectSelected(name)))
				{
					SelectObject(name);
				}
				ImGui::PopID();
			}
		}

		ImGui::Separator();
		if (ImGui::Button("Duplicate", ImVec2(-1.0f, 0.0f))) DuplicateSelectedShape();
		if (ImGui::Button("Delete", ImVec2(-1.0f, 0.0f))) DeleteSelectedObject();
	}

	void UIService::RenderInspector()
	{
		if (mSelectedShape.empty())
		{
			ImGui::TextDisabled(Is2DScene() ? "No 2D object selected." : "No 3D object selected.");
			return;
		}

		if ((Is2DScene() && IsSelected3D()) || (Is3DScene() && !IsSelected3D()))
		{
			mSelectedShape.clear();
			ImGui::TextDisabled(Is2DScene() ? "No 2D object selected." : "No 3D object selected.");
			return;
		}

		if (IsSelected3D())
		{
			auto primitiveIter = mPrimitives3D.find(mSelectedShape);
			if (primitiveIter == mPrimitives3D.end())
			{
				AddLog("[ERROR] Selected 3D object is missing: " + mSelectedShape);
				mSelectedShape.clear();
				ImGui::TextDisabled("Selection was removed.");
				return;
			}

			auto& primitive = primitiveIter->second;
			float position[3] = { primitive.position.x, primitive.position.y, primitive.position.z };
			float rotation[3] = { primitive.rotation.x, primitive.rotation.y, primitive.rotation.z };
			float scale[3] = { primitive.scale.x, primitive.scale.y, primitive.scale.z };
			float colorAsFloat[3] = {
				primitive.color.r / 255.0f,
				primitive.color.g / 255.0f,
				primitive.color.b / 255.0f
			};

			ImGui::Text("Selected 3D");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.38f, 0.72f, 0.70f, 1.0f), "%s", mSelectedShape.c_str());
			ImGui::Separator();

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::DragFloat3("Position", position, 0.1f))
				{
					primitive.position = { position[0], position[1], position[2] };
				}
				if (ImGui::DragFloat3("Rotation", rotation, 1.0f))
				{
					primitive.rotation = { rotation[0], rotation[1], rotation[2] };
				}
				if (ImGui::DragFloat3("Scale", scale, 0.05f, 0.1f, 10.0f))
				{
					primitive.scale = { scale[0], scale[1], scale[2] };
				}
				if (ImGui::Button("Reset Transform", ImVec2(-1.0f, 0.0f))) ResetSelectedShape();
			}

			if (ImGui::CollapsingHeader("Appearance", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::ColorEdit3("Color", colorAsFloat))
				{
					primitive.color.r = static_cast<sf::Uint8>(colorAsFloat[0] * 255);
					primitive.color.g = static_cast<sf::Uint8>(colorAsFloat[1] * 255);
					primitive.color.b = static_cast<sf::Uint8>(colorAsFloat[2] * 255);
				}
				if (ImGui::Button("Random Color", ImVec2(-1.0f, 0.0f))) RandomizeSelectedShapeColor();
			}

			ImGui::Separator();
			if (ImGui::Button("Duplicate 3D Object", ImVec2(-1.0f, 0.0f))) DuplicateSelectedShape();
			if (ImGui::Button("Delete 3D Object", ImVec2(-1.0f, 0.0f))) DeleteSelectedObject();
			return;
		}

		auto shapeIter = mShapes.find(mSelectedShape);
		if (shapeIter == mShapes.end() || !shapeIter->second)
		{
			AddLog("[ERROR] Selected shape is missing: " + mSelectedShape);
			mSelectedShape.clear();
			ImGui::TextDisabled("Selection was removed.");
			return;
		}

		sf::Shape& shape = *shapeIter->second;
		sf::Vector2f position = shape.getPosition();
		float size = shape.getScale().x;
		float rotation = shape.getRotation();
		sf::Color color = shape.getFillColor();
		float colorAsFloat[3] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };

		ImGui::Text("Selected");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.38f, 0.72f, 0.70f, 1.0f), "%s", mSelectedShape.c_str());
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::DragFloat2("Position", &position.x, 1.0f)) shape.setPosition(position);
			if (ImGui::DragFloat("Scale", &size, 0.1f, 0.1f, 10.0f)) shape.setScale(size, size);
			if (ImGui::DragFloat("Rotation", &rotation, 1.0f, 0.0f, 360.0f)) shape.setRotation(rotation);
			if (ImGui::Button("Reset Transform", ImVec2(-1.0f, 0.0f))) ResetSelectedShape();
		}

		if (ImGui::CollapsingHeader("Appearance", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::ColorEdit3("Color", colorAsFloat))
			{
				color.r = static_cast<sf::Uint8>(colorAsFloat[0] * 255);
				color.g = static_cast<sf::Uint8>(colorAsFloat[1] * 255);
				color.b = static_cast<sf::Uint8>(colorAsFloat[2] * 255);
				shape.setFillColor(color);
			}
			if (ImGui::Button("Random Color", ImVec2(-1.0f, 0.0f))) RandomizeSelectedShapeColor();
		}

		ImGui::Separator();
		if (ImGui::Button("Duplicate Shape", ImVec2(-1.0f, 0.0f))) DuplicateSelectedShape();
		if (ImGui::Button("Delete Shape", ImVec2(-1.0f, 0.0f))) DeleteSelectedObject();
	}

	void UIService::RenderConsole()
	{
		if (ImGui::Button("Clear")) mLogs.clear();
		ImGui::SameLine();
		ImGui::TextDisabled("%zu messages", mLogs.size());
		ImGui::Separator();
		for (const std::string& log : mLogs)
		{
			ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			if (log.find("[ERROR]") != std::string::npos)
			{
				color = ImVec4(1.0f, 0.35f, 0.30f, 1.0f);
			}
			else if (log.find("[INFO]") != std::string::npos)
			{
				color = ImVec4(0.38f, 0.72f, 0.70f, 1.0f);
			}
			else if (log.find("[DEBUG]") != std::string::npos)
			{
				color = ImVec4(0.62f, 0.65f, 0.70f, 1.0f);
			}
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextWrapped("%s", log.c_str());
			ImGui::PopStyleColor();
		}
	}

	void UIService::RenderViewport(sf::RenderWindow& window)
	{
		mViewportAcceptsCameraInput = false;

		ImGui::Text(Is2DScene() ? "2D Scene" : "3D Scene");
		ImGui::SameLine();
		ImGui::TextDisabled("%zu objects", Is2DScene() ? mShapes.size() : mPrimitives3D.size());
		if (Is2DScene())
		{
			ImGui::SameLine();
			ImGui::TextDisabled("| Grid %.0f %s", mGridSize, mSnapToGrid ? "on" : "off");
			ImGui::SameLine();
			if (ImGui::Checkbox("Mouse edit", &mMouseEditEnabled))
			{
				mMouseDraggingSelection = false;
				mMouseBoxSelecting = false;
				AddLog(mMouseEditEnabled ? "[INFO] Mouse editor enabled." : "[INFO] Mouse editor disabled.");
			}
		}
		else
		{
			ImGui::SameLine();
			ImGui::Checkbox("Wireframe", &mWireframe3D);
			ImGui::SameLine();
			ImGui::Checkbox("Auto rotate", &mAutoRotate3D);
			ImGui::SameLine();
			if (ImGui::Checkbox("Mouse camera", &mMouseCamera3DEnabled))
			{
				AddLog(mMouseCamera3DEnabled ? "[INFO] 3D mouse camera enabled." : "[INFO] 3D mouse camera disabled.");
			}
		}
		ImGui::Separator();

		if (Is2DScene())
		{
			if (mMouseEditEnabled)
			{
				ImGui::TextColored(ImVec4(0.38f, 0.72f, 0.70f, 1.0f), "Mouse edit active: click select, drag move, drag empty space for group select.");
			}
			else if (!mDraggedShapeType.empty())
			{
				ImGui::TextColored(ImVec4(0.38f, 0.72f, 0.70f, 1.0f), "Click here to place a %s.", mDraggedShapeType.c_str());
			}
			else
			{
				ImGui::TextDisabled("Choose a 2D shape from Hierarchy, then click here to place it.");
			}
		}
		else
		{
			ImGui::TextDisabled(mMouseCamera3DEnabled ? "Mouse camera active: right drag orbit, middle drag pan, wheel zoom." : "Enable Mouse camera to orbit, pan, and zoom in the 3D viewport.");
		}

		if (Is3DScene() && ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& camera = CameraService::GetInstance();
			ImGui::DragFloat("Distance", &camera.Distance(), 0.1f, 3.0f, 20.0f);
			ImGui::DragFloat("Yaw", &camera.Yaw(), 1.0f, -180.0f, 180.0f);
			ImGui::DragFloat("Pitch", &camera.Pitch(), 1.0f, -80.0f, 80.0f);
			float target[3] = { camera.Target().x, camera.Target().y, camera.Target().z };
			if (ImGui::DragFloat3("Target", target, 0.1f))
			{
				camera.Target() = { target[0], target[1], target[2] };
			}
			if (ImGui::Button("Reset Camera", ImVec2(-1.0f, 0.0f)))
			{
				camera.Reset();
				AddLog("[INFO] Camera reset.");
			}
			ImGui::DragFloat("Auto rotate speed", &mAutoRotateSpeed, 1.0f, 5.0f, 180.0f);
			ImGui::TextDisabled("Keyboard: WASD pan | Q/E height | Arrows orbit | Z/X zoom");
			ImGui::TextDisabled("Mouse: right drag orbit | middle drag pan | wheel zoom");
		}

		const ImVec2 viewportPos = ImGui::GetCursorScreenPos();
		const ImVec2 availableViewportSize = ImGui::GetContentRegionAvail();
		const ImVec2 viewportSize(
			std::max(1.0f, availableViewportSize.x),
			std::max(1.0f, availableViewportSize.y));

		if (Is3DScene() && viewportSize.x > 1.0f && viewportSize.y > 1.0f)
		{
			Render3DScene(window, viewportPos, viewportSize);
		}

		ImGui::Dummy(viewportSize);
		const bool viewportCanvasHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		mViewportAcceptsCameraInput = Is3DScene() && viewportCanvasHovered && !ImGui::IsAnyItemActive();
		if (Is3DScene())
		{
			CameraService::GetInstance().UpdateMouseControls(
				window,
				mMouseCamera3DEnabled && mViewportAcceptsCameraInput,
				ImGui::GetIO().MouseWheel);
		}
		DrawMouseEditorOverlay(window, viewportPos, viewportSize);
		ProcessMouseEditor(window, viewportCanvasHovered);

		const bool canPlaceShape =
			Is2DScene() &&
			!mMouseEditEnabled &&
			!mDraggedShapeType.empty() &&
			viewportCanvasHovered &&
			ImGui::IsMouseClicked(ImGuiMouseButton_Left);

		if (canPlaceShape)
		{
			sf::Vector2f mousePos = SnapPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			std::unique_ptr<sf::Shape> shape = CreateShapeFromType(mDraggedShapeType);

			if (shape)
			{
				shape->setPosition(mousePos);
				const std::string shapeName = mDraggedShapeType + std::to_string(mShapeCounter++);
				CreateShape(shapeName, std::move(shape));
				SelectShape(shapeName);
				mDraggedShapeType.clear();
			}
		}
	}

	void UIService::ProcessMouseEditor(sf::RenderWindow& window, bool viewportCanvasHovered)
	{
		if (!Is2DScene() || !mMouseEditEnabled || !viewportCanvasHovered || !mDraggedShapeType.empty())
		{
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				mMouseDraggingSelection = false;
				mMouseBoxSelecting = false;
			}
			return;
		}

		const ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse && !ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			return;
		}

		const sf::Vector2f mouseWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			const std::string hitObject = HitTest2DObject(mouseWorld);
			const bool additiveSelection = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

			if (!hitObject.empty())
			{
				if (additiveSelection)
				{
					AddObjectToSelection(hitObject);
				}
				else if (!IsObjectSelected(hitObject))
				{
					SelectOnlyObject(hitObject);
				}
				else
				{
					mSelectedShape = hitObject;
				}

				mMouseDraggingSelection = true;
				mMouseBoxSelecting = false;
				mLastMouseWorldPosition = mouseWorld;
				return;
			}

			if (!additiveSelection)
			{
				ClearObjectSelection();
			}
			mMouseDraggingSelection = false;
			mMouseBoxSelecting = true;
			mSelectionBoxStartWorld = mouseWorld;
			mSelectionBoxEndWorld = mouseWorld;
			return;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (mMouseDraggingSelection)
			{
				const sf::Vector2f delta = mouseWorld - mLastMouseWorldPosition;
				if (std::abs(delta.x) > 0.001f || std::abs(delta.y) > 0.001f)
				{
					for (const std::string& name : mSelectedObjects)
					{
						auto iter = mShapes.find(name);
						if (iter != mShapes.end() && iter->second)
						{
							iter->second->move(delta);
						}
					}
					mLastMouseWorldPosition = mouseWorld;
				}
			}
			else if (mMouseBoxSelecting)
			{
				mSelectionBoxEndWorld = mouseWorld;
			}
			return;
		}

		if (mMouseBoxSelecting)
		{
			const float left = std::min(mSelectionBoxStartWorld.x, mSelectionBoxEndWorld.x);
			const float top = std::min(mSelectionBoxStartWorld.y, mSelectionBoxEndWorld.y);
			const float width = std::abs(mSelectionBoxEndWorld.x - mSelectionBoxStartWorld.x);
			const float height = std::abs(mSelectionBoxEndWorld.y - mSelectionBoxStartWorld.y);
			const sf::FloatRect selectionRect(left, top, width, height);

			ClearObjectSelection();
			for (const auto& [name, shape] : mShapes)
			{
				if (shape && selectionRect.intersects(shape->getGlobalBounds()))
				{
					AddObjectToSelection(name);
				}
			}

			if (!mSelectedObjects.empty())
			{
				AddLog("[INFO] Group selected 2D objects: " + std::to_string(mSelectedObjects.size()));
			}
		}

		mMouseDraggingSelection = false;
		mMouseBoxSelecting = false;
	}

	void UIService::DrawMouseEditorOverlay(sf::RenderWindow& window, const ImVec2& viewportPos, const ImVec2& viewportSize)
	{
		if (!Is2DScene() || !mMouseEditEnabled)
		{
			return;
		}

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImU32 selectedColor = IM_COL32(242, 219, 89, 255);
		const ImU32 selectionFill = IM_COL32(90, 170, 210, 45);
		const ImU32 selectionBorder = IM_COL32(90, 170, 210, 220);
		const ImVec2 viewportMin = viewportPos;
		const ImVec2 viewportMax(viewportPos.x + viewportSize.x, viewportPos.y + viewportSize.y);

		drawList->PushClipRect(viewportMin, viewportMax, true);
		for (const std::string& name : mSelectedObjects)
		{
			auto iter = mShapes.find(name);
			if (iter == mShapes.end() || !iter->second)
			{
				continue;
			}

			const sf::FloatRect bounds = iter->second->getGlobalBounds();
			const sf::Vector2i topLeft = window.mapCoordsToPixel({ bounds.left, bounds.top });
			const sf::Vector2i bottomRight = window.mapCoordsToPixel({ bounds.left + bounds.width, bounds.top + bounds.height });
			drawList->AddRect(
				ImVec2(static_cast<float>(topLeft.x), static_cast<float>(topLeft.y)),
				ImVec2(static_cast<float>(bottomRight.x), static_cast<float>(bottomRight.y)),
				selectedColor,
				0.0f,
				0,
				2.0f);
		}

		if (mMouseBoxSelecting)
		{
			const sf::Vector2i start = window.mapCoordsToPixel(mSelectionBoxStartWorld);
			const sf::Vector2i end = window.mapCoordsToPixel(mSelectionBoxEndWorld);
			const ImVec2 minPoint(
				static_cast<float>(std::min(start.x, end.x)),
				static_cast<float>(std::min(start.y, end.y)));
			const ImVec2 maxPoint(
				static_cast<float>(std::max(start.x, end.x)),
				static_cast<float>(std::max(start.y, end.y)));
			drawList->AddRectFilled(minPoint, maxPoint, selectionFill);
			drawList->AddRect(minPoint, maxPoint, selectionBorder, 0.0f, 0, 1.5f);
		}
		drawList->PopClipRect();
	}
}
