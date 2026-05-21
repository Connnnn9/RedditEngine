#include "Precompiled.h"
#include "UISpriteRenderer.h"

#include "GraphicsSystem.h"
#include "UISprite.h"

using namespace KREngine;
using namespace KREngine::Graphics;

namespace
{
	std::unique_ptr<UISpriteRenderer> sSpriteRenderer;
}

void UISpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "UISpriteRenderer: is already initialized");
	sSpriteRenderer = std::make_unique<UISpriteRenderer>();
	sSpriteRenderer->Initialize();
}

void UISpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		sSpriteRenderer.reset();
	}
}

UISpriteRenderer* UISpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "UISpriteRenderer: is not initialize");
	return sSpriteRenderer.get();
}

UISpriteRenderer::~UISpriteRenderer()
{

	ASSERT(sSpriteRenderer == nullptr, "UISpriteRenderer: Terminate must be called");
}

void UISpriteRenderer::Initialize()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	mCommonStates = new DirectX::CommonStates(gs->GetDevice());
	mSpriteBatch = new DirectX::SpriteBatch(gs->GetContext());
}

void UISpriteRenderer::Terminate()
{
	if (mCommonStates!= nullptr)
	{
		delete mCommonStates;
		mCommonStates = nullptr;
	}
	if (mSpriteBatch != nullptr)
	{
		delete mSpriteBatch;
		mSpriteBatch = nullptr;
	}
}

void UISpriteRenderer::BeginRender()
{
	ASSERT(mSpriteBatch != nullptr, "UISpriteRenderer: was not initialize");
	mSpriteBatch->Begin(
		DirectX::SpriteSortMode_Deferred,
		mCommonStates->NonPremultiplied(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		mTransform);
}

void UISpriteRenderer::EndRender()
{
	mSpriteBatch->End();

	// restore the state objects
	auto blendState = mCommonStates->Opaque();
	auto depthStencilState = mCommonStates->DepthDefault();
	auto rasterizerState = mCommonStates->CullCounterClockwise();

	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState, 0);
	context->RSSetState(rasterizerState);
}

void UISpriteRenderer::Render(const UISprite* uiSprite)
{
	const Texture* texture = TextureManager::Get()->GetTexture(uiSprite->mTextureId);
	if (texture!=nullptr)
	{
		mSpriteBatch->Draw(
			texture->GetShaderResourceView(),
			uiSprite->mPosition,
			&uiSprite->mRect,
			uiSprite->mColor,
			uiSprite->mRotation,
			uiSprite->mOrigin,
			uiSprite->mScale,
			uiSprite->mFlip
		);
	}
}
