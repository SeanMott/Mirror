#include <Mirror\ECS\Systems\SpriteRenderer.hpp>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

void Mirror::ECS::System::SpriteRenderer::Init(bool setUpSingleQuadRender, const float _baseSpriteScale, uint32_t _maxQuadPerBatch)
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

	//single quad render data
	if (setUpSingleQuadRender)
	{
		singleQuadRenderIsEnabled = setUpSingleQuadRender;
		//set up shader
		const char* vertexSource = "#version 330 core\nlayout(location = 0) in vec2 position;\nlayout(location = 1) in vec2 texCords;\nout vec2 TexCoords;\nuniform mat4 PVM;\nvoid main()\n{\nTexCoords = texCords;\ngl_Position = PVM * vec4(position, 0.0f, 1.0);\n}\n";
		const char* fragSource = "#version 330 core\nin vec2 TexCoords;\nout vec4 fragColor;\nuniform sampler2D sprite;uniform vec4 color;\nvoid main()\n{\nfragColor = color * texture(sprite, TexCoords);\n}";
		singleQuadShader = Glfix_Shader_CreateVertexSource(vertexSource, fragSource, nullptr);

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

		singleQuadVertexBuffer = Glfix_VertexBuffer_CreateStatic(vertices, sizeof(vertices));
		Glfix_VertexLayout_Element elements[2] = { Glfix_LayoutType_Float2, Glfix_LayoutType_Float2 }; //Glfix_LayoutType_Float3, Glfix_LayoutType_Float2 };
		bool normalize[2] = { false, false};
		Glfix_VertexLayout_Layout layout;
		layout.elements = elements;
		layout.normalize = normalize;
		layout.elementCount = 2;
		Glfix_VertexBuffer_Layout(singleQuadVertexBuffer, &layout);
	}

	//batch render data
	maxQuadPerBatch = _maxQuadPerBatch;
	maxVerticeCount = maxQuadPerBatch * 4;
	maxIndicesCount = maxQuadPerBatch * 6;

	//make shader
	const char* batchVertexShader = "#version 330 core\nlayout(location = 0) in vec3 position;\nlayout(location = 1) in vec2 texCords;\nlayout(location = 2) in vec4 color;\nlayout(location = 3) in float textureId;\nout vec2 TexCoords;\nout vec4 Color;\nout float TextureId;\nuniform mat4 ProjView;\nvoid main()\n{\nTexCoords = texCords;\nColor = color;\nTextureId = textureId;\ngl_Position = ProjView * vec4(position, 1.0);\n}";
	const char* batchFragmentShader = "#version 330 core\nout vec4 fragColor;\nin vec2 TexCoords;\nin vec4 Color;\nin float TextureId;\nuniform sampler2D sprite[32];\nuniform sampler2D sprite1;\nuniform sampler2D sprite2;\nvoid main()\n{\nfragColor = Color * texture(sprite1, TexCoords);\n}";

	batchQuadShader = Glfix_Shader_CreateVertexSource(batchVertexShader, batchFragmentShader, nullptr);

	//makes vertex buffer
	//batchQuadVertexBuffer = Glfix_VertexBuffer_CreateDynamic(sizeof(Quad) * maxVerticeCount);
#define LAYOUT_COUNT 4
	Glfix_VertexLayout_Element elements[LAYOUT_COUNT] = { Glfix_LayoutType_Float3, Glfix_LayoutType_Float2,
		Glfix_LayoutType_Float4, Glfix_LayoutType_Float1 };
	bool normalize[LAYOUT_COUNT] = { false, false, false, false };
	Glfix_VertexLayout_Layout layout;
	layout.elements = elements;
	layout.normalize = normalize;
	layout.elementCount = LAYOUT_COUNT;
	//Glfix_VertexBuffer_Layout(batchQuadVertexBuffer, &layout);

	//makes index buffer
	uint32_t* indices = new uint32_t[maxIndicesCount];
	uint32_t offset = 0;
	for (int i = 0; i < maxIndicesCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	batchQuadIndexBuffer = Glfix_IndexBuffer_CreateStatic(indices, maxIndicesCount);
	delete[] indices;

	//allocate quad buffer
	//quadBuffer = new Quad[maxVerticeCount];

	//set texture ids buffer
	//querey based on system || set up a query function
#ifdef Desktop_Build
	maxTextureCount = 32; //desktops have 32

	//phones have 8, 

	//ect
#endif

	textureIds = new int32_t[maxTextureCount];
	//memset(textureIds, 0, maxIndicesCount * sizeof(int32_t));
	//pass uniforms for the shaders
	for (int32_t i = 0; i < maxTextureCount; ++i)
		textureIds[i] = i;
	//Glfix_Shader_SetIntArray(batchQuadShader, "sprite", maxTextureCount, textureIds);
}

void Mirror::ECS::System::SpriteRenderer::Shutdown()
{
	//clean up single quad data
	if (singleQuadRenderIsEnabled)
	{
		if (singleQuadShader)
			Glfix_Shader_Destroy(singleQuadShader);

		if (singleQuadVertexBuffer)
			Glfix_VertexBuffer_Destroy(singleQuadVertexBuffer);

		singleQuadRenderIsEnabled = false;
	}

	//clean up batch render
	if(batchQuadShader)
		Glfix_Shader_Destroy(batchQuadShader);
	if(batchQuadVertexBuffer)
		Glfix_VertexBuffer_Destroy(batchQuadVertexBuffer);
	if (batchQuadIndexBuffer)
		Glfix_IndexBuffer_Destroy(batchQuadIndexBuffer);
	/*if (quadBuffer)
	{
		delete[] quadBuffer;
		quadBuffer = nullptr;
	}*/
	if (textureIds)
	{
		delete[] textureIds;
		textureIds = nullptr;
	}
	maxQuadPerBatch = 0;
}

static bool spriteRendererErrorWarning = false;

void Mirror::ECS::System::SpriteRenderer::SingleSpriteRender(ECS::Comp::Camera& cam, ECS::Comp::Transform& camTrans,
	ECS::Comp::Sprite& entitySprite, ECS::Comp::Transform& entityTrans, Glfix_Texture* texture)
{
	if (!singleQuadRenderIsEnabled)
		return;

	//check errors
	if (!singleQuadShader || !singleQuadVertexBuffer)
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
	Glfix_Shader_Bind(singleQuadShader);
	Glfix_VertexBuffer_Bind(singleQuadVertexBuffer);
	
	//if texture was NULL use the blank one
	Glfix_Texture_Bind((texture != nullptr ? texture : blankTexture), 0);
	
	//sets camera
	glm::mat4 projection = glm::ortho(cam.viewWidthMin, cam.viewWidthMax,
		cam.viewHeightMax, cam.viewHeightMin, cam.nearFieldClipping, cam.farFieldClipping);
	glm::mat4 camPos = glm::mat4(1.0f);
	camPos = glm::translate(glm::mat4(1.0f), Mirror_Math_TypePun(camTrans.position, glm::vec3)) *
		glm::rotate(glm::mat4(1.0f), camTrans.rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));

	//positions sprite
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Mirror_Math_TypePun(entityTrans.position, glm::vec3)) * //position

	//rotation
	glm::translate(model, glm::vec3(0.5f * entityTrans.scale.x /*(entityTrans.scale.x * baseSpriteScale)*/, entityTrans.scale.y /*(entityTrans.scale.y * baseSpriteScale)*/, 0.0f)) *//; // move origin of rotation to center of quad
	glm::rotate(model, glm::radians(entityTrans.rotation.x), glm::vec3(0.0f, 0.0f, 1.0f)) *// // then rotate
	glm::translate(model, glm::vec3(-0.5f * entityTrans.scale.x /*(entityTrans.scale.x * baseSpriteScale)*/, -0.5f * entityTrans.scale.y /*(entityTrans.scale.y * baseSpriteScale)*/, 0.0f)) * //; // move origin back

	//scale
	glm::scale(model, { entityTrans.scale.x * baseSpriteScale, entityTrans.scale.y * baseSpriteScale, 1.0f });//Mirror_Math_TypePun(s, glm::vec3));


	//pass uniforms
	Glfix_Shader_SetMat4(singleQuadShader, "PVM", false, &(projection * model)[0][0]); //&((projection * camPos) * model)[0][0]);
	Glfix_Shader_SetInt(singleQuadShader, "sprite", 0);
	Glfix_Shader_SetFloat4(singleQuadShader, "color", entitySprite.color.r, entitySprite.color.g, entitySprite.color.b, entitySprite.color.a);
	//Glfix_Shader_SetVec4(singleQuadShader, "color", Mirror_Math_TypePun(entitySprite.color, float*));

	//render
	Glfix_VertexBuffer_Draw(Glfix_DrawType_Triangles, 0, singleQuadVertexBuffer->size);
}

void Mirror::ECS::System::SpriteRenderer::SingleSpriteRender(ECS::Comp::Camera& cam, ECS::Comp::Transform& camTrans,
	ECS::Comp::Sprite& entitySprite, ECS::Comp::Transform& entityTrans, const float baseSpriteScaleOverride, Glfix_Texture* texture)
{
	if (!singleQuadRenderIsEnabled)
		return;

	//check errors
	if (!singleQuadShader || !singleQuadVertexBuffer)
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
	Glfix_Shader_Bind(singleQuadShader);
	Glfix_VertexBuffer_Bind(singleQuadVertexBuffer);

	//if texture was NULL use the blank one
	Glfix_Texture_Bind((texture != nullptr ? texture : blankTexture), 0);

	//sets camera
	glm::mat4 projection = glm::ortho(cam.viewWidthMin, cam.viewWidthMax,
		cam.viewHeightMax, cam.viewHeightMin, cam.nearFieldClipping, cam.farFieldClipping);
	glm::mat4 camPos = glm::mat4(1.0f);
	camPos = glm::translate(glm::mat4(1.0f), Mirror_Math_TypePun(camTrans.position, glm::vec3)) *
		glm::rotate(glm::mat4(1.0f), camTrans.rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));

	//positions sprite
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Mirror_Math_TypePun(entityTrans.position, glm::vec3)) * //position

	//rotation
		glm::translate(model, glm::vec3(0.5f * entityTrans.scale.x /*(entityTrans.scale.x * baseSpriteScale)*/, entityTrans.scale.y /*(entityTrans.scale.y * baseSpriteScale)*/, 0.0f)) *//; // move origin of rotation to center of quad
		glm::rotate(model, glm::radians(entityTrans.rotation.x), glm::vec3(0.0f, 0.0f, 1.0f)) *// // then rotate
		glm::translate(model, glm::vec3(-0.5f * entityTrans.scale.x /*(entityTrans.scale.x * baseSpriteScale)*/, -0.5f * entityTrans.scale.y /*(entityTrans.scale.y * baseSpriteScale)*/, 0.0f)) * //; // move origin back

		//scale
		glm::scale(model, { entityTrans.scale.x * baseSpriteScaleOverride, entityTrans.scale.y * baseSpriteScaleOverride, 1.0f });//Mirror_Math_TypePun(s, glm::vec3));


		//pass uniforms
	Glfix_Shader_SetMat4(singleQuadShader, "PVM", false, &(projection * model)[0][0]); //&((projection * camPos) * model)[0][0]);
	Glfix_Shader_SetInt(singleQuadShader, "sprite", 0);
	Glfix_Shader_SetFloat4(singleQuadShader, "color", entitySprite.color.r, entitySprite.color.g, entitySprite.color.b, entitySprite.color.a);
	//Glfix_Shader_SetVec4(singleQuadShader, "color", Mirror_Math_TypePun(entitySprite.color, float*));

	//render
	Glfix_VertexBuffer_Draw(Glfix_DrawType_Triangles, 0, singleQuadVertexBuffer->size);
}

void Mirror::ECS::System::SpriteRenderer::SetCamera(ECS::Comp::Camera* cam, ECS::Comp::Transform* camTrans)
{
	//sets camera
	//quadPtr = quadBuffer;
	indexCount = 0;

	//cal camera
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 transform = glm::mat4(1.0f);
	//glm::mat4 model = glm::mat4(1.0f);

	projection = glm::ortho(cam->viewWidthMin, cam->viewWidthMax, cam->viewHeightMax, cam->viewHeightMin,
		cam->nearFieldClipping, cam->farFieldClipping);

	transform = glm::translate(glm::mat4(1.0f), Mirror_Math_TypePun(camTrans->position, glm::vec3)) *
		glm::rotate(glm::mat4(1.0f), camTrans->rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));

	//transform = inverse(transform);

	//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	//pass to shader
	//Glfix_Shader_Bind(shader);
	Glfix_Shader_SetMat4(batchQuadShader, "ProjView", false, &(projection * transform)[0][0]);
	/*glm::mat4 projection = glm::ortho(cam.viewWidthMin, cam.viewWidthMax,
		cam.viewHeightMax, cam.viewHeightMin, cam.nearFieldClipping, cam.farFieldClipping);
	glm::mat4 camPos = glm::mat4(1.0f);
	camPos = glm::translate(glm::mat4(1.0f), Mirror_Math_TypePun(camTrans.position, glm::vec3)) *
		glm::rotate(glm::mat4(1.0f), camTrans.rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));
	
	//pass to the shader
	Glfix_Shader_SetMat4(batchQuadShader, "ProjView", false, &(projection)[0][0]);
	*/
}

void Mirror::ECS::System::SpriteRenderer::BeginBatch()
{
	//binds data
	Glfix_Shader_Bind(batchQuadShader);
	Glfix_VertexBuffer_Bind(batchQuadVertexBuffer);
	Glfix_IndexBuffer_Bind(batchQuadIndexBuffer);
	Glfix_Texture_Bind(blankTexture, 0);
	Glfix_Shader_SetIntArray(batchQuadShader, "sprite", maxTextureCount, textureIds);
	Glfix_Shader_SetInt(batchQuadShader, "sprite1", 0);
	Glfix_Shader_SetInt(batchQuadShader, "sprite2", 1);
	//quadPtr = quadBuffer;
	indexCount = 0;
}

void Mirror::ECS::System::SpriteRenderer::EndBatch()
{
	Flush();
}

void Mirror::ECS::System::SpriteRenderer::AddQuad(ECS::Comp::Sprite* sprite, ECS::Comp::Transform* transform)
{
	/*if (indexCount >= maxIndicesCount)
	{
		EndBatch();
		BeginBatch();
	}*/

	//push data into the Quad buffer
	/*quadPtr->position = { transform->position.x, transform->position.y, transform->position.z };
	quadPtr->color = sprite->color;
	quadPtr->textureId = (float)sprite->textureId;
	quadPtr->texCords = { 0.0f, 0.0f };
	quadPtr++; //quadPtr += 4;

	quadPtr->position = { transform->position.x + (transform->scale.x * baseSpriteScale), transform->position.y, transform->position.z };
	quadPtr->color = sprite->color;
	quadPtr->textureId = (float)sprite->textureId;
	quadPtr->texCords = { 1.0f, 0.0f };
	quadPtr++; //quadPtr += 4;

	quadPtr->position = { transform->position.x + (transform->scale.x * baseSpriteScale), transform->position.y + (transform->scale.y * baseSpriteScale), transform->position.z };
	quadPtr->color = sprite->color;
	quadPtr->textureId = (float)sprite->textureId;
	quadPtr->texCords = { 1.0f, 1.0f };
	quadPtr++; //quadPtr += 4;

	quadPtr->position = { transform->position.x, transform->position.y + (transform->scale.y * baseSpriteScale), transform->position.z };
	quadPtr->color = sprite->color;
	quadPtr->textureId = (float)sprite->textureId;
	quadPtr->texCords = { 0.0f, 1.0f };
	quadPtr++; //quadPtr += 4;*/

	indexCount += 6;
}

void Mirror::ECS::System::SpriteRenderer::Flush()
{
	//send data to the GPU
	//uint32_t size = (uint8_t*)quadPtr - (uint8_t*)quadBuffer;
	//Glfix_VertexBuffer_SendData(quadBuffer, size);

	//draw call
	Glfix_IndexBuffer_Draw(Glfix_DrawType_Triangles, 0, indexCount);
}