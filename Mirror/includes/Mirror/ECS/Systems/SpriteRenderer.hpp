//defines a sprite renderer

#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include <SmokCore\ECS\Components\Camera.hpp>
#include <SmokCore\ECS\Components\Transform.hpp>
#include <Mirror\ECS\Components\Sprite.hpp>

#include <Glfix\Shader.h>
#include <Glfix\Texture.h>
#include <Glfix\IndexBuffer.h>
#include <Glfix\VertexBuffer.h>

#include <stdint.h>

namespace Mirror::ECS::System
{
	//defines a Quad
	/*struct Quad
	{
		Mirror::Util::Vector3<float> position;
		Mirror::Util::Vector2<float> texCords;
		Mirror::Util::Color color;
		float textureId = 0;
	};*/

	//defines the sprite renderer
	class SpriteRenderer
	{
		//vars
	private:

		float baseSpriteScale = 15.0f;
		Glfix_Texture* blankTexture = nullptr;

		//data needed for the SingleSpriteRender
		bool singleQuadRenderIsEnabled = false;
		Glfix_VertexBuffer* singleQuadVertexBuffer = nullptr;
		Glfix_Shader* singleQuadShader = nullptr;

		//data needed for batch rendering
		uint32_t maxQuadPerBatch = 1;
		uint32_t maxVerticeCount = 0;
		uint32_t maxIndicesCount = 0;
		
		uint32_t indexCount = 0;
		
		uint32_t maxTextureCount = 0;
		int32_t* textureIds = nullptr;
		
		Glfix_VertexBuffer* batchQuadVertexBuffer = nullptr;
		Glfix_IndexBuffer* batchQuadIndexBuffer = nullptr;
		Glfix_Shader* batchQuadShader = nullptr;
		
		//Quad* quadBuffer = nullptr;
		//Quad* quadPtr = nullptr;

		//methods
	public:

		//sets up the renderer
		void Init(bool setUpSingleQuadRender, const float _baseSpriteScale = 15.0f, uint32_t _maxQuadPerBatch = 1000);
		//shutsdown the renderer
		void Shutdown();

		//renders a single Quad with a draw call (not batched)
		void SingleSpriteRender(SmokCore::ECS::Comp::Camera& cam, SmokCore::ECS::Comp::Transform& camTrans,
			ECS::Comp::Sprite& entitySprite, SmokCore::ECS::Comp::Transform& entityTrans, Glfix_Texture* texture = nullptr);
		//renders a single Quad with a draw call (not batched)
		void SingleSpriteRender(SmokCore::ECS::Comp::Camera& cam, SmokCore::ECS::Comp::Transform& camTrans,
			ECS::Comp::Sprite& entitySprite, SmokCore::ECS::Comp::Transform& entityTrans, const float baseSpriteScaleOverride, Glfix_Texture* texture = nullptr);
	
		//sets the camera
		void SetCamera(SmokCore::ECS::Comp::Camera* cam, SmokCore::ECS::Comp::Transform* camTrans);
		//begins the batch
		void BeginBatch();
		//ends the batch
		void EndBatch();
		//push data into the batch
		void AddQuad(ECS::Comp::Sprite* sprite, SmokCore::ECS::Comp::Transform* transform);
		//flushs data
		void Flush();

		//gets the base sprite scale
		inline float GetBaseSpriteScale() const { return baseSpriteScale; }
		//sets the base sprite scale
		inline void SetBaseSpriteScale(const float scale) { baseSpriteScale = scale; }
	};
}

#endif