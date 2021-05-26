#include <Mirror\Scene\Scene.hpp>

#include <Mirror\ECS\Components\EntityData.hpp>
#include <Mirror\ECS\Components\Camera.hpp>
#include <Mirror\ECS\Components\Transform.hpp>

void Mirror::Scene::Scene::Destroy()
{
	for (auto& e : entities)
		e.Destroy();
	entities.clear();
	reg.clear();
}

Mirror::ECS::Entity* Mirror::Scene::Scene::CreateEntity(const std::string& name, const std::string& tag, const std::string& layer, const bool _isActive)
{
	entt::entity e = reg.create();
	entities.emplace_back(Mirror::ECS::Entity(&reg, e, name, tag, layer, _isActive));
	//entities[entities.size() - 1].AddComponent<Mirror::ECS::Comp::EntityData>(name, tag, layer, isActive);

	return &entities[entities.size() - 1];
}

void Mirror::Scene::Scene::DestroyEntity(const std::string& name)
{
	//check if entity has name
	const char* n = name.c_str();
	for (int32_t i = 0; i < entities.size(); ++i)
	{
		if (!strcmp(n, entities[i].GetName().c_str()))
		{
			entities[i].Destroy();
			//std::vector<ECS::Entity>::iterator it = entities.begin() + i;
			entities.erase(entities.begin() + i);
			return;
		}
	}

	//if no name was found
	printf("Mirror Warning: No Entity named %s was found to destroy!\n", name.c_str());
}

Mirror::ECS::Entity* Mirror::Scene::Scene::GetEntity(const std::string& name)
{
	const char* n = name.c_str();
	for (int32_t i = 0; i < entities.size(); ++i)
	{
		if (!strcmp(n, entities[i].GetName().c_str()))
			return &entities[i];
	}

	printf("Mirror Warning: NULL Name || No Entity was found with the name %s!\n", n);
	return nullptr;
}

Mirror::ECS::Entity** Mirror::Scene::Scene::GetAllEntitiesByName(uint32_t& arraySize, const std::string& name) const
{
	const size_t count = entities.size();
	Mirror::ECS::Entity** e = new Mirror::ECS::Entity*[count];
	uint32_t s = 0;
	const char* n = name.c_str();

	//add to array
	for (int32_t i = 0; i < count; ++i)
	{
		if (!strcmp(n, entities[i].GetName().c_str()))
		{
			e[s] = (ECS::Entity*)&entities[i];
			s++;
		}
	}

	//reallocate empty space
	if (e[s] == nullptr)
		s--;
	realloc(e, sizeof(ECS::Entity*) * s);

	arraySize = s;
	return e;
}

Mirror::ECS::Entity** Mirror::Scene::Scene::GetAllEntitiesByTag(uint32_t& arraySize, const std::string& tag) const
{
	const size_t count = entities.size();
	Mirror::ECS::Entity** e = new Mirror::ECS::Entity * [count];
	uint32_t s = 0;
	const char* n = tag.c_str();

	//add to array
	for (int32_t i = 0; i < count; ++i)
	{
		if (!strcmp(n, entities[i].GetTag().c_str()))
		{
			e[s] = (ECS::Entity*)&entities[i];
			s++;
		}
	}

	//reallocate empty space
	if (e[s] == nullptr)
		s--;
	realloc(e, sizeof(ECS::Entity*) * s);

	arraySize = s;
	return e;
}

Mirror::ECS::Entity** Mirror::Scene::Scene::GetAllEntitiesByLayer(uint32_t& arraySize, const std::string& layer) const
{
	const size_t count = entities.size();
	Mirror::ECS::Entity** e = new Mirror::ECS::Entity * [count];
	uint32_t s = 0;
	const char* n = layer.c_str();

	//add to array
	for (int32_t i = 0; i < count; ++i)
	{
		if (!strcmp(n, entities[i].GetLayer().c_str()))
		{
			e[s] = (ECS::Entity*)&entities[i];
			s++;
		}
	}

	//reallocate empty space
	if (e[s] == nullptr)
		s--;
	realloc(e, sizeof(ECS::Entity*) * s);

	arraySize = s;
	return e;
}

Mirror::ECS::Entity** Mirror::Scene::Scene::GetAllEntitiesByName(const std::string& name) const
{
	const size_t count = entities.size();
	Mirror::ECS::Entity** e = new Mirror::ECS::Entity * [count];
	uint32_t s = 0;
	const char* n = name.c_str();

	//add to array
	for (int32_t i = 0; i < count; ++i)
	{
		if (!strcmp(n, entities[i].GetName().c_str()))
		{
			e[s] = (ECS::Entity*)&entities[i];
			s++;
		}
	}

	//reallocate empty space
	if (e[s] == nullptr)
		s--;
	realloc(e, sizeof(ECS::Entity*) * s);

	return e;
}

Mirror::ECS::Entity** Mirror::Scene::Scene::GetAllEntitiesByTag(const std::string& tag) const
{
	const size_t count = entities.size();
	Mirror::ECS::Entity** e = new Mirror::ECS::Entity * [count];
	uint32_t s = 0;
	const char* n = tag.c_str();

	//add to array
	for (int32_t i = 0; i < count; ++i)
	{
		if (!strcmp(n, entities[i].GetName().c_str()))
		{
			e[s] = (ECS::Entity*)&entities[i];
			s++;
		}
	}

	//reallocate empty space
	if (e[s] == nullptr)
		s--;
	realloc(e, sizeof(ECS::Entity*) * s);

	return e;
}

Mirror::ECS::Entity** Mirror::Scene::Scene::GetAllEntitiesByLayer(const std::string& layer) const
{
	const size_t count = entities.size();
	Mirror::ECS::Entity** e = new Mirror::ECS::Entity * [count];
	uint32_t s = 0;
	const char* n = layer.c_str();

	//add to array
	for (int32_t i = 0; i < count; ++i)
	{
		if (!strcmp(n, entities[i].GetName().c_str()))
		{
			e[s] = (ECS::Entity*)&entities[i];
			s++;
		}
	}

	//reallocate empty space
	if (e[s] == nullptr)
		s--;
	realloc(e, sizeof(ECS::Entity*) * s);

	return e;
}

Mirror::ECS::Entity* Mirror::Scene::Scene::MakeCamera(float width, float height)
{
	Mirror::ECS::Entity* cam = CreateEntity("Camera", "Main Camera");
	cam->AddComponent<Mirror::ECS::Comp::Camera>(width, height);
	cam->AddComponent<Mirror::ECS::Comp::Transform>();
	return cam;
}
