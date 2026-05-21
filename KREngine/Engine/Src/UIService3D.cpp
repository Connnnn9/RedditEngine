#include "Precompiled.h"
#include "UIService.h"
#include <imgui.h>
#include <SFML/OpenGL.hpp>
#include <algorithm>

namespace KREngine
{
	void UIService::Render3DScene(sf::RenderWindow& window, const ImVec2& viewportPos, const ImVec2& viewportSize)
	{
		window.setActive(true);
		window.pushGLStates();

		const int viewportX = static_cast<int>(viewportPos.x);
		const int viewportY = static_cast<int>(ImGui::GetIO().DisplaySize.y - viewportPos.y - viewportSize.y);
		const int viewportWidth = static_cast<int>(viewportSize.x);
		const int viewportHeight = static_cast<int>(viewportSize.y);
		const float aspect = viewportSize.x / std::max(1.0f, viewportSize.y);

		glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
		glScissor(viewportX, viewportY, viewportWidth, viewportHeight);
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.055f, 0.065f, 0.075f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CameraService::GetInstance().Apply3DView(aspect);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(0.18f, 0.20f, 0.22f);
		for (int i = -10; i <= 10; ++i)
		{
			glVertex3f(static_cast<float>(i), -1.0f, -10.0f);
			glVertex3f(static_cast<float>(i), -1.0f, 10.0f);
			glVertex3f(-10.0f, -1.0f, static_cast<float>(i));
			glVertex3f(10.0f, -1.0f, static_cast<float>(i));
		}
		glColor3f(0.42f, 0.50f, 0.52f);
		glVertex3f(-10.0f, -1.0f, 0.0f);
		glVertex3f(10.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, -1.0f, -10.0f);
		glVertex3f(0.0f, -1.0f, 10.0f);
		glEnd();

		for (const auto& [name, primitive] : mPrimitives3D)
		{
			glPolygonMode(GL_FRONT_AND_BACK, mWireframe3D ? GL_LINE : GL_FILL);
			Draw3DPrimitive(primitive);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (name == mSelectedShape)
			{
				glPushMatrix();
				glTranslatef(primitive.position.x, primitive.position.y, primitive.position.z);
				glRotatef(primitive.rotation.x, 1.0f, 0.0f, 0.0f);
				glRotatef(primitive.rotation.y, 0.0f, 1.0f, 0.0f);
				glRotatef(primitive.rotation.z, 0.0f, 0.0f, 1.0f);
				glScalef(primitive.scale.x * 1.04f, primitive.scale.y * 1.04f, primitive.scale.z * 1.04f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				Draw3DPrimitive({ primitive.type, {}, {}, { 1.0f, 1.0f, 1.0f }, sf::Color(242, 219, 89) });
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glPopMatrix();
			}
		}

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_SCISSOR_TEST);
		window.popGLStates();
		window.resetGLStates();
	}

	void UIService::UpdateCameraControls(const sf::RenderWindow& window, sf::Time deltaTime)
	{
		const ImGuiIO& io = ImGui::GetIO();
		if (!Is3DScene() || !mViewportAcceptsCameraInput || !window.hasFocus() || io.WantCaptureKeyboard || io.WantTextInput)
		{
			return;
		}

		CameraService::GetInstance().UpdateKeyboardControls(deltaTime);
	}

	void UIService::Draw3DPrimitive(const Primitive3D& primitive) const
	{
		glPushMatrix();
		glTranslatef(primitive.position.x, primitive.position.y, primitive.position.z);
		glRotatef(primitive.rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(primitive.rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(primitive.rotation.z, 0.0f, 0.0f, 1.0f);
		glScalef(primitive.scale.x, primitive.scale.y, primitive.scale.z);
		glColor3ub(primitive.color.r, primitive.color.g, primitive.color.b);

		if (primitive.type == Primitive3DType::Cube)
		{
			DrawCube();
		}
		else
		{
			DrawPyramid();
		}

		glPopMatrix();
	}

	void UIService::DrawCube() const
	{
		glBegin(GL_QUADS);
		glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f); glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
		glEnd();
	}

	void UIService::DrawPyramid() const
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.65f, 0.0f); glVertex3f(-0.6f, -0.5f, 0.6f); glVertex3f(0.6f, -0.5f, 0.6f);
		glVertex3f(0.0f, 0.65f, 0.0f); glVertex3f(0.6f, -0.5f, 0.6f); glVertex3f(0.6f, -0.5f, -0.6f);
		glVertex3f(0.0f, 0.65f, 0.0f); glVertex3f(0.6f, -0.5f, -0.6f); glVertex3f(-0.6f, -0.5f, -0.6f);
		glVertex3f(0.0f, 0.65f, 0.0f); glVertex3f(-0.6f, -0.5f, -0.6f); glVertex3f(-0.6f, -0.5f, 0.6f);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3f(-0.6f, -0.5f, 0.6f); glVertex3f(-0.6f, -0.5f, -0.6f); glVertex3f(0.6f, -0.5f, -0.6f); glVertex3f(0.6f, -0.5f, 0.6f);
		glEnd();
	}
}
