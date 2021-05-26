#pragma once

//defines a Batch Sprite Renderer

#include <Mirror\Util\Math.hpp>
#include <Mirror\Util\Color.hpp>

#include <Glfix\VertexBuffer.h>
#include <Glfix\IndexBuffer.h>
#include <Glfix\Texture.h>
#include <Glfix\Shader.h>

#include <Mirror\ECS\Components\Camera.hpp>
#include <Mirror\ECS\Components\Transform.hpp>
#include <Mirror\ECS\Components\Sprite.hpp>

namespace Mirror::ECS::System
{
	//defines a Quad
	struct Quad
	{
		Mirror::Util::Vector2<float> position;
		Mirror::Util::Vector2<float> texCords;
		//add stuff for lighting
		Mirror::Util::Color color;
		float textureId = 0;
	};

	//defines the renderer
	class BatchSpriteRenderer
	{
		//vars
	private:

		float baseSpriteScale = 1.0f; //the base sprite scale

		uint32_t maxQuadsPerBatch = 1; //the max amount of quads per batch
		uint32_t maxQuadVerticesPerBatch = 1; //the max amount of quad vertices
		uint32_t maxQuadIndicesPerBatch = 1; //the max amount of quad indices

		Glfix_VertexBuffer* quadVertexBuffer = nullptr; //the quad vertices data
		Glfix_IndexBuffer* quadIndexBuffer = nullptr; //the quad indices data

		Quad* quadBuffer = nullptr; //the quad buffer
		Quad* quadBufferPtr = nullptr; //the quad buffer pointer
		uint32_t quadCount = 0; //the current amount of quads in this batch

		Glfix_Texture* blankTexture = nullptr; //the blank texture used if no texture was given to the sprite
		Glfix_Shader* shader = nullptr; //the batch rendering shader

		int32_t* textureIds = nullptr; //the texture Ids array
		int32_t maxTextureIds = 1; //the max number of texture ids for this platform

		//methods
	public:

		//sets up the batch renderer
		void Init(const uint32_t _maxQuadsPerBatch, const float _baseSpriteScale);
		//shutsdown the batch renderer
		void Shutdown();

		//prebinds all the data needed for rendering Quads
		void QuadPrebind();

		//starts a new Quad batch
		void StartQuadBatch();
		//ends the current Quad batch
		void EndQuadBatch();

		//sets the camera for the Quad batch
		void SetQuadBatchCamera(Mirror::ECS::Comp::Transform* transform, Mirror::ECS::Comp::Camera* cam);
		//adds a quad to the Quad batch
		void AddQuad(Mirror::ECS::Comp::Transform* transform, Mirror::ECS::Comp::Sprite* sprite);

	};
}