//Mirror
#include <Mirror\Core\Display.hpp>
#include <Mirror\Core\Input.hpp>

#include <Mirror\ECS\Entity.hpp>
#include <Mirror\Scene\Scene.hpp>
#include <Mirror\Util\Time.hpp>

#include <Mirror\ECS\Systems\SpriteRenderer.hpp>
#include <Mirror\ECS\Systems\BatchSpriteRenderer.hpp>

#include <Mirror\ECS\Components\BoxCollider.hpp>

//Glfix
#include <Glfix\Context.h>
#include <Glfix\Texture.h>
#include <Glfix\Shader.h>
#include <Glfix\IndexBuffer.h>
#include <Glfix\VertexBuffer.h>

//Game

//other
#include <stdio.h>

inline void OnPlayerColllide(Mirror::ECS::Entity* self, Mirror::ECS::Entity* other)
{
	printf("Collide!\n");
}

int main(int args, char* argv[])
{
	//init app

	//engine
	//engine would store a list of tags and layers

	//init display
	Mirror::Core::Display* display = new Mirror::Core::Display("Floopy Bird", 800, 600);
	display->SetClearColor(0.0f, 173.0f, 173.0f);
	Glfix_Context_EnableBlending(true);

	//init input manager
	Mirror::Core::Input* input = new Mirror::Core::Input(display->GetWindow());

	//inits asset manager
	//Mirror::Core::AssetManager* assetManager = new Mirror::Core::AssetManager(5);

	//load assets
	//assetManager->LoadAsset("FlappyIdle", "res/Textures/Bird/BirdIdle.asset");
	//assetManager->LoadAsset("FlappyFlap", "res/Textures/Bird/BirdFlap.asset");

	//assetManager->LoadAsset("Pipe Head", "res/Textures/Pipe/PipeHead.asset");
	//assetManager->LoadAsset("Pipe Mid", "res/Textures/Pipe/PipeBodyMid.asset");
	//assetManager->LoadAsset("Pipe End", "res/Textures/Pipe/PipeBodyEnd.asset");

	//seeds random
	Mirror::Util::Time::SeedRNG();

	Glfix_Texture_CreateInfo info;
	Glfix_Texture_InitCreateInfo(&info);
	info.magFilterMode = Glfix_Texture_FilterMode_Nearest;
	info.minFilterMode = Glfix_Texture_FilterMode_Nearest;
	//bird
	Glfix_Texture* flappyIdle = Glfix_Texture_Create2D("res/Textures/Bird/BirdIdle.png", &info, true);
	Glfix_Texture* flappyFlap = Glfix_Texture_Create2D("res/Textures/Bird/BirdFlap.png", &info, true);
	
	//bird
	Glfix_Texture* pipeHead = Glfix_Texture_Create2D("res/Textures/Pipe/PipeHead.png", &info, true);
	Glfix_Texture* pipeBodyMid = Glfix_Texture_Create2D("res/Textures/Pipe/PipeBodyMid.png", &info, true);
	Glfix_Texture* pipeBodyEnd = Glfix_Texture_Create2D("res/Textures/Pipe/PipeBodyEnd.png", &info, true);

	//set up systems

	//sprite renderer
	const float baseSpriteScale = 100.0f;
	const uint32_t maxQuadPerBatch = 1000;
	Mirror::ECS::System::SpriteRenderer spriteRenderer;
	spriteRenderer.Init(true, baseSpriteScale, maxQuadPerBatch);
	Mirror::ECS::System::BatchSpriteRenderer batchRenderer;
	batchRenderer.Init(maxQuadPerBatch, baseSpriteScale);

	//text renderer

	//physics system
	//Mirror::ECS::Systems::PhysicsWorld physicsWorld;
	//physicsWorld.Init();

	//game data
	Mirror::Scene::Scene scene(5);

	//camera
	Mirror::ECS::Entity* camera = scene.CreateEntity("Camera");
	camera->AddComponent< Mirror::ECS::Comp::Camera>();
	Mirror::Util::Vector3<float> pos = {(float)display->GetWidth() / 2, (float)display->GetHeight() / 2, 0.0f};
	camera->AddComponent<Mirror::ECS::Comp::Transform>(pos, Mirror::Util::Vector3<float>::Zero(), Mirror::Util::Vector3<float>::One());

	//bird
	Glfix_Texture* currentBirdFrame = flappyIdle;
	Mirror::ECS::Entity* bird = scene.CreateEntity("Bird", "Bird");
	pos = { (float)display->GetWidth() / 7, (float)display->GetHeight() / 2, 0.0f };
	Mirror::Util::Vector3<float> s = { 100.0f, 100.0f, 1.0f };
	bird->AddComponent<Mirror::ECS::Comp::Transform>(pos, Mirror::Util::Vector3<float>::Zero(), s);
	bird->AddComponent<Mirror::ECS::Comp::Sprite>(Mirror::Util::Color::While(), (uint32_t)0, true);
	Mirror::Util::Vector2<float> size = {100.0f, 100.0f};
	bird->AddComponent<Mirror::ECS::Comp::BoxCollider>(size, Mirror::Util::Vector2<float>::Zero(), OnPlayerColllide, false, true);

	//pipes
	const uint32_t pipeCount = 10;
	
	const float topYPipePos = 0.0f + 10.0f;
	const float bottomYPipePos = (float)display->GetHeight() - baseSpriteScale;
	const float pointTriigerPos = topYPipePos - bottomYPipePos;

	const float baseXPipePos = (float)display->GetWidth() - baseSpriteScale;
	
	const float pipeStartScale = 5.0f;
	const float pipeScaleLimit = 3.0f;
	const float pipeDist = 50.0f;
	const float pipeMoveRate = 125.0f;

	const int32_t minPipeMoveRand = 20;
	const int32_t maxPipeMoveRand = 120;

	Mirror::ECS::Entity** topPipes = nullptr;
	Mirror::ECS::Entity** bottomPipes = nullptr;
	Mirror::ECS::Entity** pointTriggers = nullptr;

	Mirror::Util::Vector3<float> scale = { 1.0f, pipeStartScale, 1.0f };
	//genarates top pipes
	Mirror::ECS::Entity* topPipe = nullptr;
	for (int32_t i = 0; i < pipeCount; ++i)
	{
		topPipe = scene.CreateEntity("Top Pipe", "Pipe");
		
		if (!i)
		{
			pos.x = baseXPipePos;
			pos.y = topYPipePos - 250.0f;
		}
		else
			pos.x += pipeDist + baseSpriteScale;

		//randomize the scale
		//scale.y = Mirror::Util::Time::RNG(pipeStartScale, pipeScaleLimit);
		//scale.y = rand() % (int32_t)pipeScaleLimit;
		topPipe->AddComponent<Mirror::ECS::Comp::Transform>(pos, Mirror::Util::Vector3<float>::Zero(), scale);
		topPipe->AddComponent<Mirror::ECS::Comp::Sprite>(Mirror::Util::Color::While(), (uint32_t)1, true);
		topPipe->AddComponent<Mirror::ECS::Comp::BoxCollider>(size, Mirror::Util::Vector2<float>::Zero(), nullptr, false, true);
	}
	topPipe = nullptr;

	//genarates bottom pipes
	Mirror::ECS::Entity* bottomPipe = nullptr;
	for (int32_t i = 0; i < pipeCount; ++i)
	{
		bottomPipe = scene.CreateEntity("Bottom Pipe", "Pipe");
		
		if (!i)
		{
			pos.x = baseXPipePos;
			pos.y = bottomYPipePos - 150.0f;
		}
		else
			pos.x += pipeDist + baseSpriteScale;

		//scale.y = Mirror::Util::Time::RNG(pipeStartScale, pipeScaleLimit);
		bottomPipe->AddComponent<Mirror::ECS::Comp::Transform>(pos, Mirror::Util::Vector3<float>::Zero(), scale);
		bottomPipe->AddComponent<Mirror::ECS::Comp::Sprite>(Mirror::Util::Color::While(), (uint32_t)1, true);
		//collider
	}
	bottomPipe = nullptr;

	//genarates point trigger space
	Mirror::ECS::Entity* pipeTrigger = nullptr;
	for (int32_t i = 0; i < pipeCount; ++i)
	{
		pipeTrigger = scene.CreateEntity("Pipe Trigger", "Score");

		if (!i)
		{
			pos.x = baseXPipePos;
			pos.y = pointTriigerPos;
		}
		else
			pos.x += pipeDist + baseSpriteScale;

		//scale.y = topPipes[i]->GetComponent< Mirror::ECS::Comp::Transform>()->scale.y - bottomPipes[i]->GetComponent< Mirror::ECS::Comp::Transform>()->scale.y;
		pipeTrigger->AddComponent<Mirror::ECS::Comp::Transform>(pos, Mirror::Util::Vector3<float>::Zero(), scale);
		pipeTrigger->AddComponent<Mirror::ECS::Comp::Sprite>(Mirror::Util::Color::While(), (uint32_t)1, true);
		//collider
	}
	pipeTrigger = nullptr;
	
	//gets pipes
	pointTriggers = scene.GetAllEntitiesByName("Pipe Trigger");
	bottomPipes = scene.GetAllEntitiesByName("Bottom Pipe");
	topPipes = scene.GetAllEntitiesByName("Top Pipe");

	//text

	//get bird
	bird = scene.GetEntity("Bird");
	camera = scene.GetEntity("Camera");

	//game loop
	float deltaTime = 0.0f;

	const float gravity = 150.0f;

	//bird
	const float jumpGravityPorsion = 5.0f;
	const float jumpBoost = 2.5f;
	const float jumpVelGravityPortion = 2.0f;
	bool jumpBoostEnabled = false;
	
	float birdAcc = 0.0f;
	float birdVel = 0.0f;

	//game loop
	while (display->IsRunning())
	{
		//cal delta time and fixed time
		deltaTime = Mirror::Util::Time::CalDeltaTime();

		//--input and update should be in a thread

		//--rendering will be in a thread

		//input
		display->PollEvents();

		//escape app
		if (input->GetKey(Mirror::Platform::Keycode::Escape))
			break;

		//jump
		//input->GetKey(Mirror::Core::Input::Keycode::Space)
		if (input->GetKey(Mirror::Platform::Keycode::Space) && birdVel > gravity / jumpGravityPorsion)
		{
			currentBirdFrame = flappyFlap; //change to flap frame
			//move up
			birdAcc = 0.0f;
			birdVel = (jumpBoostEnabled == true ? (-gravity / jumpVelGravityPortion) * jumpBoost : -gravity / jumpVelGravityPortion);
		}
		//fall
		else
		{
			currentBirdFrame = flappyIdle; //change to idle frame
			birdAcc += gravity * deltaTime; //change acceleration
		}

		//turns jump boost on and off
		if (input->GetKey(Mirror::Platform::Keycode::J))
			jumpBoostEnabled = !jumpBoostEnabled;

		//update

		//update bird position
		if (birdAcc > gravity)
			birdAcc = gravity;
		birdVel += birdAcc * deltaTime;
		bird->GetComponent<Mirror::ECS::Comp::Transform>()->position.y += birdVel * deltaTime;

		//moves through the level
		for (int32_t i = 0; i < pipeCount; ++i)
		{
			Mirror::ECS::Comp::Transform* topPipeTrans = topPipes[i]->GetComponent<Mirror::ECS::Comp::Transform>();
			Mirror::ECS::Comp::Transform* pipeTriggerTrans = pointTriggers[i]->GetComponent<Mirror::ECS::Comp::Transform>();
			Mirror::ECS::Comp::Transform* bottomPipeTrans = bottomPipes[i]->GetComponent<Mirror::ECS::Comp::Transform>();

			topPipeTrans->position.x -= pipeMoveRate * deltaTime;
			pipeTriggerTrans->position.x -= pipeMoveRate * deltaTime;
			bottomPipeTrans->position.x -= pipeMoveRate * deltaTime;

			//if it goes past the screen move to the other side
			if (topPipeTrans->position.x < 0.0f - baseSpriteScale)
			{
				//move to the other side
				topPipeTrans->position.x = (float)display->GetWidth() + (baseSpriteScale + pipeDist);
				pipeTriggerTrans->position.x = (float)display->GetWidth() + (baseSpriteScale + pipeDist);
				bottomPipeTrans->position.x = (float)display->GetWidth() + (baseSpriteScale + pipeDist);
			
				//randomize the pipes
				//int32_t topOffset = Mirror::Util::Time::RNG(minPipeMoveRand, maxPipeMoveRand);
				//int32_t bottomOffset = Mirror::Util::Time::RNG(minPipeMoveRand, maxPipeMoveRand);

				//uint32_t offsetCounter = 0;
				/*while (topOffset - bottomOffset < baseSpriteScale + 5.0f)
				{
					//topOffset = Mirror::Util::Time::RNG(minPipeMoveRand, maxPipeMoveRand);
					bottomOffset = Mirror::Util::Time::RNG(minPipeMoveRand, maxPipeMoveRand);
					offsetCounter++;
					if (offsetCounter >= 5)
						break;
				}*/

				topPipeTrans->position.y -= Mirror::Util::Time::RNG(0, 20);
				bottomPipeTrans->position.y -= Mirror::Util::Time::RNG(20, 100);
			}
		}
	
		//collider check
		for (int32_t i = 0; i < pipeCount; ++i)
		{
			if (Mirror::Util::Physics::RectVRect<float, float>
				(topPipes[i]->GetComponent<Mirror::ECS::Comp::Transform>()->position.ToVec2() +
					topPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->posOffset,
				topPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size,
				bird->GetComponent<Mirror::ECS::Comp::Transform>()->position.ToVec2(),
				bird->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size))
			{
				bird->GetComponent<Mirror::ECS::Comp::BoxCollider>()->funcCallback(bird, topPipes[i]);
			}
		}
		
		/*
		for(int32_t i = 0; i < pipeCount; ++i)
		{
			Mirror::Util::Physics::RectVRect(
		}

		//check bird agienst each pipe
		
		*/

		//render
		display->Clear();

		//render stuff

		//batchRenderer.QuadPrebind();
		//batchRenderer.SetQuadBatchCamera(camera->GetComponent<Mirror::ECS::Comp::Transform>(), camera->GetComponent<Mirror::ECS::Comp::Camera>());
		//batchRenderer.StartQuadBatch();
		//spriteRenderer.BeginBatch();
		//spriteRenderer.SetCamera(camera->GetComponent<Mirror::ECS::Comp::Camera>(), camera->GetComponent<Mirror::ECS::Comp::Transform>());

		//render the pipes
	
		//data for colliders
		Mirror::ECS::Comp::Sprite colSprite;
		Mirror::ECS::Comp::Transform colTrans;

		//const float spriteScaleStore = spriteRenderer.GetBaseSpriteScale();
		for (int32_t i = 0; i < pipeCount; ++i)
		{
			//renders pipes
			spriteRenderer.SingleSpriteRender(*camera->GetComponent<Mirror::ECS::Comp::Camera>(), *camera->GetComponent<Mirror::ECS::Comp::Transform>(),
				*topPipes[i]->GetComponent<Mirror::ECS::Comp::Sprite>(), *topPipes[i]->GetComponent<Mirror::ECS::Comp::Transform>(), pipeBodyMid);
			spriteRenderer.SingleSpriteRender(*camera->GetComponent<Mirror::ECS::Comp::Camera>(), *camera->GetComponent<Mirror::ECS::Comp::Transform>(),
				*bottomPipes[i]->GetComponent<Mirror::ECS::Comp::Sprite>(), *bottomPipes[i]->GetComponent<Mirror::ECS::Comp::Transform>(), pipeBodyMid);
		
			//renders colliders
			//top pipes
			/*if (topPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->render)
			{
				//spriteRenderer.SetBaseSpriteScale(1.0f);
				colTrans.position = topPipes[i]->GetComponent<Mirror::ECS::Comp::Transform>()->position;
				colTrans.scale = { topPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.x,
					topPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.y, 1.0f };
				colSprite.color = topPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->renderColor;
				spriteRenderer.SingleSpriteRender(*camera->GetComponent<Mirror::ECS::Comp::Camera>(), *camera->GetComponent<Mirror::ECS::Comp::Transform>(),
					colSprite, colTrans, 1.0f, nullptr);
				//spriteRenderer.SetBaseSpriteScale(spriteScaleStore);
			}*/
			//point trigger
			/*if (pointTriggers[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->render)
			{
				//spriteRenderer.SetBaseSpriteScale(1.0f);
				colTrans.position = pointTriggers[i]->GetComponent<Mirror::ECS::Comp::Transform>()->position;
				colTrans.scale = { pointTriggers[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.x,
					pointTriggers[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.y, 1.0f };
				colSprite.color = pointTriggers[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->renderColor;
				spriteRenderer.SingleSpriteRender(*camera->GetComponent<Mirror::ECS::Comp::Camera>(), *camera->GetComponent<Mirror::ECS::Comp::Transform>(),
					colSprite, colTrans, 1.0f, nullptr);
				//spriteRenderer.SetBaseSpriteScale(spriteScaleStore);
			}*/
			//bottom pipes
			/*if (bottomPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->render)
			{
				//spriteRenderer.SetBaseSpriteScale(1.0f);
				colTrans.position = bottomPipes[i]->GetComponent<Mirror::ECS::Comp::Transform>()->position;
				colTrans.scale = { bottomPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.x,
					bottomPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.y, 1.0f };
				colSprite.color = bottomPipes[i]->GetComponent<Mirror::ECS::Comp::BoxCollider>()->renderColor;
				spriteRenderer.SingleSpriteRender(*camera->GetComponent<Mirror::ECS::Comp::Camera>(), *camera->GetComponent<Mirror::ECS::Comp::Transform>(),
					colSprite, colTrans, 1.0f, nullptr);
				//spriteRenderer.SetBaseSpriteScale(spriteScaleStore);
			}*/
		}

		//render player flap
		//batchRenderer.AddQuad(bird->GetComponent<Mirror::ECS::Comp::Transform>(), bird->GetComponent<Mirror::ECS::Comp::Sprite>());
		
		
		//spriteRenderer.AddQuad(bird->GetComponent<Mirror::ECS::Comp::Sprite>(), bird->GetComponent<Mirror::ECS::Comp::Transform>());
		//spriteRenderer.SingleSpriteRender(*camera->GetComponent<Mirror::ECS::Comp::Camera>(), *camera->GetComponent<Mirror::ECS::Comp::Transform>(),
			//*bird->GetComponent<Mirror::ECS::Comp::Sprite>(), *bird->GetComponent<Mirror::ECS::Comp::Transform>(), currentBirdFrame);
		//renders collider
		/*if (bird->GetComponent<Mirror::ECS::Comp::BoxCollider>()->render)
		{
			//spriteRenderer.SetBaseSpriteScale(5.0f);
			colTrans.position = bird->GetComponent<Mirror::ECS::Comp::Transform>()->position;
			colTrans.scale = { bird->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.x,
				bird->GetComponent<Mirror::ECS::Comp::BoxCollider>()->size.y, 1.0f };
			colSprite.color = bird->GetComponent<Mirror::ECS::Comp::BoxCollider>()->renderColor;
			spriteRenderer.SingleSpriteRender(*camera->GetComponent<Mirror::ECS::Comp::Camera>(), *camera->GetComponent<Mirror::ECS::Comp::Transform>(),
				colSprite, colTrans, 1.0f, nullptr);
			//spriteRenderer.SetBaseSpriteScale(spriteScaleStore);
		}*/

		//spriteRenderer.Flush();
		//batchRenderer.EndQuadBatch();

		//renders text

		display->SwapRenderBuffer();
	}

	//clean up

	//scenes
	scene.Destroy();
	//game.Destroy();
	//menu.Destroy();

	//systems
	batchRenderer.Shutdown();
	spriteRenderer.Shutdown();

	//assets
	delete flappyFlap;
	delete flappyIdle;
	delete pipeBodyEnd;
	delete pipeBodyMid;
	delete pipeHead;

	//engine

	/*
	if(engine)
	{
		engine->Destroy();
		engine = nullptr;
	}
	*/

	/*
	if(assetManager)
	{
		delete assetManager;
		assetManager = nullptr;
	}
	*/

	if(input)
	{
		delete input;
		input = nullptr;
	}

	if (display)
	{
		delete display;
		display = nullptr;
	}

	getchar();
	return 0;
}