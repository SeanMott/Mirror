#include <Mirror\ECS\Systems\BatchSpriteRenderer.hpp>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <string> //temportary

void Mirror::ECS::System::BatchSpriteRenderer::Init(const uint32_t _maxQuadsPerBatch, const float _baseSpriteScale) 
{
	//sets the renderer's settings
	baseSpriteScale = _baseSpriteScale;
	maxQuadsPerBatch = _maxQuadsPerBatch;
	maxQuadVerticesPerBatch = maxQuadsPerBatch * 4;
	maxQuadIndicesPerBatch = maxQuadsPerBatch * 6;

	//allocate Quad buffer
	quadBuffer = new Quad[maxQuadVerticesPerBatch];

	//make blank texture
	uint8_t color = 0xffffffff;
	Glfix_Texture_CreateInfo info;
	Glfix_Texture_InitCreateInfo(&info);
	info.internalFormate = Glfix_Formate_RGBA8;
	info.extraData = &color;
	blankTexture = Glfix_Texture_Create(&info);

	//makes batch shader
	const std::string v = "#version 330 core\n"
		"layout (location = 0) in vec2 position;"
		"layout (location = 1) in vec2 texCords;"
		"layout (location = 2) in vec4 color;"
		"layout (location = 3) in float texId;"
		"out vec2 TexCords;"
		"out vec4 Color;"
		"out float TexId;"
		"uniform mat4 PV;"
		"void main()"
		"{"
		"TexCords = texCords;"
		"Color = color;"
		"TexId = texId;"
		"gl_Position = PV * vec4(position, 0.0, 1.0);"
		"}";
	const std::string f = "#version 330 core\n"
		"layout (location = 0) out vec4 fragColor;"
		"in vec2 TexCords;"
		"in vec4 Color;"
		"in float TexId;"
		"uniform sampler2D sprites[32];"
		"void main()"
		"{"
		"int s = int(TexId);"
		"fragColor = Color * texture(sprites[s], TexCords);"
		"}";
	shader = Glfix_Shader_CreateVertexSource(v.c_str(), f.c_str(), nullptr);

	//allocate vertex buffer
	quadVertexBuffer = Glfix_VertexBuffer_CreateDynamic(sizeof(Quad) * maxQuadVerticesPerBatch);
	Glfix_VertexBuffer_Bind(quadVertexBuffer);
#define LAYOUT_COUNT 4
	Glfix_VertexLayout_Element elements[LAYOUT_COUNT] = { Glfix_LayoutType_Float2, Glfix_LayoutType_Float2,
		Glfix_LayoutType_Float4, Glfix_LayoutType_Float1 };
	bool normalize[LAYOUT_COUNT] = { false, false, false, false };
	Glfix_VertexLayout_Layout layout;
	layout.elements = elements;
	layout.normalize = normalize;
	layout.elementCount = LAYOUT_COUNT;
	Glfix_VertexBuffer_Layout(quadVertexBuffer, &layout);

	//allocate index buffer
	uint32_t* indices =  new uint32_t[maxQuadIndicesPerBatch];
	uint32_t offset = 0;
	for (int32_t i = 0; i < maxQuadIndicesPerBatch; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;


		offset += 4;
	}
	quadIndexBuffer = Glfix_IndexBuffer_CreateStatic(indices, maxQuadIndicesPerBatch);
	delete[] indices;

	//allocate texture Ids
	maxTextureIds = Glfix_Shader_FragTextureCap();
	textureIds = new int32_t[maxTextureIds];
	for (int32_t i = 0; i < maxTextureIds; ++i)
		textureIds[i] = i;
}

void Mirror::ECS::System::BatchSpriteRenderer::Shutdown()
{
	delete[] textureIds;
	delete[] quadBuffer;
	quadBufferPtr = nullptr;
	Glfix_IndexBuffer_Destroy(quadIndexBuffer);
	Glfix_VertexBuffer_Destroy(quadVertexBuffer);
	Glfix_Shader_Destroy(shader);
	Glfix_Texture_Destroy(blankTexture);
}

void Mirror::ECS::System::BatchSpriteRenderer::Prebind()
{
	Glfix_Shader_Bind(shader);
	Glfix_Texture_Bind(blankTexture, 0);
	Glfix_Shader_SetIntArray(shader, "sprites", maxTextureIds, textureIds);
	Glfix_VertexBuffer_Bind(quadVertexBuffer);
	Glfix_IndexBuffer_Bind(quadIndexBuffer);
}

void Mirror::ECS::System::BatchSpriteRenderer::StartBatch()
{
	quadBufferPtr = quadBuffer;
	quadCount = 0;
}

void Mirror::ECS::System::BatchSpriteRenderer::EndBatch()
{
	//pass to GPU
	uint32_t size = (uint8_t*)quadBufferPtr - (uint8_t*)quadBuffer;
	Glfix_VertexBuffer_SendData(quadBuffer, size);

	//draw call
	Glfix_IndexBuffer_Draw(Glfix_DrawType_Triangles, 0, quadCount);
}

void Mirror::ECS::System::BatchSpriteRenderer::SetCamera(SmokCore::ECS::Comp::Transform* transform, SmokCore::ECS::Comp::Camera* cam)
{
	glm::mat4 projection = glm::ortho(cam->viewWidthMin, cam->viewWidth,
		cam->viewHeight, cam->viewHeightMin, cam->nearFieldClipping, cam->farFieldClipping);
	glm::mat4 camPos = glm::mat4(1.0f);
	camPos = glm::translate(glm::mat4(1.0f), Smok_Math_TypePun(transform->position, glm::vec3)) *
		glm::rotate(glm::mat4(1.0f), transform->rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));
	
	Glfix_Shader_SetMat4(shader, "PV", false, &(projection * camPos)[0][0]);
}

void Mirror::ECS::System::BatchSpriteRenderer::AddQuad(SmokCore::ECS::Comp::Transform* transform, Mirror::ECS::Comp::Sprite* sprite)
{
	if (quadCount >= maxQuadsPerBatch)
	{
		EndBatch();
		StartBatch();
	}

	quadBufferPtr->position = { transform->position.x, transform->position.y };
	quadBufferPtr->texCords = { 0.0f, 0.0f };
	quadBufferPtr->color = sprite->color;
	quadBufferPtr->textureId = (float)sprite->textureId;
	quadBufferPtr++;

	quadBufferPtr->position = { transform->position.x + (transform->scale.x * baseSpriteScale), transform->position.y };
	quadBufferPtr->texCords = { 1.0f, 0.0f };
	quadBufferPtr->color = sprite->color;
	quadBufferPtr->textureId = (float)sprite->textureId;
	quadBufferPtr++;

	quadBufferPtr->position = { transform->position.x + (transform->scale.x * baseSpriteScale), transform->position.y + (transform->scale.y * baseSpriteScale) };
	quadBufferPtr->texCords = { 1.0f, 1.0f };
	quadBufferPtr->color = sprite->color;
	quadBufferPtr->textureId = (float)sprite->textureId;
	quadBufferPtr++;

	quadBufferPtr->position = { transform->position.x, transform->position.y + (transform->scale.y * baseSpriteScale) };
	quadBufferPtr->texCords = { 0.0f, 1.0f };
	quadBufferPtr->color = sprite->color;
	quadBufferPtr->textureId = (float)sprite->textureId;
	quadBufferPtr++;

	quadCount += 6;
}