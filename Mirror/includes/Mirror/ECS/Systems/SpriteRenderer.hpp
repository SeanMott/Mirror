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

		Glfix_VertexBuffer* quadVertexBuffer = nullptr;
		Glfix_Shader* quadShader = nullptr;

		//methods
	public:

		//sets up the renderer
		void Init(const float _baseSpriteScale = 15.0f);
		//shutsdown the renderer
		void Shutdown();

		//renders a single Quad with a draw call
		void SingleSpriteRender(SmokCore::ECS::Comp::Camera& cam, SmokCore::ECS::Comp::Transform& camTrans,
			ECS::Comp::Sprite& entitySprite, SmokCore::ECS::Comp::Transform& entityTrans, Glfix_Texture* texture = nullptr);
		//renders a single Quad with a draw call
		void SingleSpriteRender(SmokCore::ECS::Comp::Camera& cam, SmokCore::ECS::Comp::Transform& camTrans,
			ECS::Comp::Sprite& entitySprite, SmokCore::ECS::Comp::Transform& entityTrans, const float baseSpriteScaleOverride, Glfix_Texture* texture = nullptr);
	};
}

#endif