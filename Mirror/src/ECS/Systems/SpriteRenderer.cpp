#include <Mirror\ECS\Systems\SpriteRenderer.hpp>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

void Mirror::ECS::System::SpriteRenderer::Init(const float _baseSpriteScale)
{
	baseSpriteScale = _baseSpriteScale;
	//make blank texture
	uint8_t color = 0xffffffff;
	Glfix_Texture_CreateInfo info{};
	info.type = Glfix_TextureType_2D;
	info.height = 1;
	info.width = 1;
	info.mipMapLevel = 0;
	info.internalFormate = Glfix_Formate_RGBA8;
	info.pixelDatatype = Glfix_Formate_UByte;
	info.externalFormate = Glfix_Formate_RGBA;
	info.extraData = &color;
	blankTexture = Glfix_Texture_Create(&info);

	//set up shader
	const char* vertexSource = "#version 330 core\nlayout(location = 0) in vec2 position;\nlayout(location = 1) in vec2 texCords;\nout vec2 TexCoords;\nuniform mat4 PVM;\nvoid main()\n{\nTexCoords = texCords;\ngl_Position = PVM * vec4(position, 0.0f, 1.0);\n}\n";
	const char* fragSource = "#version 330 core\nin vec2 TexCoords;\nout vec4 fragColor;\nuniform sampler2D sprite;uniform vec4 color;\nvoid main()\n{\nfragColor = color * texture(sprite, TexCoords);\n}";
	quadShader = Glfix_Shader_CreateVertexSource(vertexSource, fragSource, nullptr);

	//set up vertex buffer			 

	float vertices[] = {
		// pos      // tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
	};

	quadVertexBuffer = Glfix_VertexBuffer_CreateStatic(vertices, sizeof(vertices));
	Glfix_VertexLayout_Element elements[2] = { Glfix_LayoutType_Float2, Glfix_LayoutType_Float2 }; //Glfix_LayoutType_Float3, Glfix_LayoutType_Float2 };
	bool normalize[2] = { false, false};
	Glfix_VertexLayout_Layout layout;
	layout.elements = elements;
	layout.normalize = normalize;
	layout.elementCount = 2;
	Glfix_VertexBuffer_Layout(quadVertexBuffer, &layout);
}

void Mirror::ECS::System::SpriteRenderer::Shutdown()
{
	//clean up single quad data
	if (quadShader)
		Glfix_Shader_Destroy(quadShader);

	if (quadVertexBuffer)
		Glfix_VertexBuffer_Destroy(quadVertexBuffer);
}

static bool spriteRendererErrorWarning = false;

void Mirror::ECS::System::SpriteRenderer::SingleSpriteRender(SmokCore::ECS::Comp::Camera& cam, SmokCore::ECS::Comp::Transform& camTrans,
	ECS::Comp::Sprite& entitySprite, SmokCore::ECS::Comp::Transform& entityTrans, Glfix_Texture* texture)
{
	//check errors
	if (!quadShader || !quadVertexBuffer)
		return;

	if (!spriteRendererErrorWarning && !cam.isActive)
	{
		printf("Mirror Error: Sprite Renderer || the Camera comp is turned off, turn it on to use \"SingleSpriteRender\".\n");
		spriteRendererErrorWarning = true;
		return;
	}

	if (!spriteRendererErrorWarning && !entitySprite.isActive)
	{
		printf("Mirror Error: Sprite Renderer || the Sprite comp is turned off, turn it on to use \"SingleSpriteRender\".\n");
		spriteRendererErrorWarning = true;
		return;
	}

	//binds data
	Glfix_Shader_Bind(quadShader);
	Glfix_VertexBuffer_Bind(quadVertexBuffer);
	
	//if texture was NULL use the blank one
	Glfix_Texture_Bind((texture != nullptr ? texture : blankTexture), 0);
	
	//sets camera
	glm::mat4 projection = glm::ortho(cam.viewWidthMin, cam.viewWidth,
		cam.viewHeight, cam.viewHeightMin, cam.nearFieldClipping, cam.farFieldClipping);
	glm::mat4 camPos = glm::mat4(1.0f);
	camPos = glm::translate(glm::mat4(1.0f), Smok_Math_TypePun(camTrans.position, glm::vec3)) *
		glm::rotate(glm::mat4(1.0f), camTrans.rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));

	//positions sprite
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Smok_Math_TypePun(entityTrans.position, glm::vec3)) * //position

	//rotation
	glm::translate(model, glm::vec3(0.5f * entityTrans.scale.x, entityTrans.scale.y, 0.0f)) *// move origin of rotation to center of quad
	glm::rotate(model, glm::radians(entityTrans.rotation.x), glm::vec3(0.0f, 0.0f, 1.0f)) *//then rotate
	glm::translate(model, glm::vec3(-0.5f * entityTrans.scale.x, -0.5f * entityTrans.scale.y, 0.0f)) * // move origin back

	//scale
	glm::scale(model, { entityTrans.scale.x * baseSpriteScale, entityTrans.scale.y * baseSpriteScale, 1.0f });


	//pass uniforms
	Glfix_Shader_SetMat4(quadShader, "PVM", false, &(projection * model)[0][0]);
	Glfix_Shader_SetInt(quadShader, "sprite", 0);
	Glfix_Shader_SetFloat4(quadShader, "color", entitySprite.color.r, entitySprite.color.g, entitySprite.color.b, entitySprite.color.a);

	//render
	Glfix_VertexBuffer_Draw(Glfix_DrawType_Triangles, 0, quadVertexBuffer->size);
}

void Mirror::ECS::System::SpriteRenderer::SingleSpriteRender(SmokCore::ECS::Comp::Camera& cam, SmokCore::ECS::Comp::Transform& camTrans,
	ECS::Comp::Sprite& entitySprite, SmokCore::ECS::Comp::Transform& entityTrans, const float baseSpriteScaleOverride, Glfix_Texture* texture)
{

	//check errors
	if (!quadShader || !quadVertexBuffer)
		return;

	if (!spriteRendererErrorWarning && !cam.isActive)
	{
		printf("Mirror Error: Sprite Renderer || the Camera comp is turned off, turn it on to use \"SingleSpriteRender\".\n");
		spriteRendererErrorWarning = true;
		return;
	}

	if (!spriteRendererErrorWarning && !entitySprite.isActive)
	{
		printf("Mirror Error: Sprite Renderer || the Sprite comp is turned off, turn it on to use \"SingleSpriteRender\".\n");
		spriteRendererErrorWarning = true;
		return;
	}

	//binds data
	Glfix_Shader_Bind(quadShader);
	Glfix_VertexBuffer_Bind(quadVertexBuffer);

	//if texture was NULL use the blank one
	Glfix_Texture_Bind((texture != nullptr ? texture : blankTexture), 0);

	//sets camera
	glm::mat4 projection = glm::ortho(cam.viewWidthMin, cam.viewWidth,
		cam.viewHeight, cam.viewHeightMin, cam.nearFieldClipping, cam.farFieldClipping);
	glm::mat4 camPos = glm::mat4(1.0f);
	camPos = glm::translate(glm::mat4(1.0f), Smok_Math_TypePun(camTrans.position, glm::vec3)) *
		glm::rotate(glm::mat4(1.0f), camTrans.rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));

	//positions sprite
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Smok_Math_TypePun(entityTrans.position, glm::vec3)) * //position

	//rotation
		glm::translate(model, glm::vec3(0.5f * entityTrans.scale.x , entityTrans.scale.y, 0.0f)) * // move origin of rotation to center of quad
		glm::rotate(model, glm::radians(entityTrans.rotation.x), glm::vec3(0.0f, 0.0f, 1.0f)) * // then rotate
		glm::translate(model, glm::vec3(-0.5f * entityTrans.scale.x, -0.5f * entityTrans.scale.y, 0.0f)) * // move origin back

		//scale
		glm::scale(model, { entityTrans.scale.x * baseSpriteScaleOverride, entityTrans.scale.y * baseSpriteScaleOverride, 1.0f });


		//pass uniforms
	Glfix_Shader_SetMat4(quadShader, "PVM", false, &(projection * model)[0][0]);
	Glfix_Shader_SetInt(quadShader, "sprite", 0);
	Glfix_Shader_SetFloat4(quadShader, "color", entitySprite.color.r, entitySprite.color.g, entitySprite.color.b, entitySprite.color.a);

	//render
	Glfix_VertexBuffer_Draw(Glfix_DrawType_Triangles, 0, quadVertexBuffer->size);
}