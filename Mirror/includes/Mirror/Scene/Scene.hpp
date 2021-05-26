#pragma once

//defines a scene for storing entities

#include <Mirror\ECS\Entity.hpp>

#include <vector>

namespace Mirror::Scene
{
	class Scene
	{
		//vars
	private:

		std::vector<ECS::Entity> entities;
		entt::registry reg;
		bool isActive = true; //is the scene active

		//methods
	public:

		//Constructor
		Scene(uint32_t entityReserveAmount = 1, bool _isActive = true)
			:isActive(_isActive)
		{entities.reserve(entityReserveAmount);}

		//Deconstructor
		~Scene() { Destroy(); }

		//destroys the scene
		void Destroy();

		//creates a entity
		ECS::Entity* CreateEntity(const std::string& name = "Entity", const std::string& tag = "Default", const std::string& layer = "Default",
			const bool _isActive = true);
		//destroys a entity || if multiable entities have the same name it will destroy the first one it finds.
		void DestroyEntity(const std::string& name);

		//gets entity
		ECS::Entity* GetEntity(const std::string& name);

		//gets the entity from the component
		template<typename Comp>
		ECS::Entity GetEntityFromComp(Comp* comp)
		{
			//checks if the comp is not NULL
			if (!comp)
			{
				printf("Mirror Warning: NULL Comp || Can not retrive Entity from the component!\n");
				return ECS::Entity;
			}

			//find the entity
			entt::entity e = entt::to_entity(reg, *comp);
			if (!reg->valid(entity))
			{
				printf("Smok Warning: NULL Entity || Entity does not have this Component so the Entity can not be retrived!\n");
				return ECS::Entity;
			}
			ECS::Comp::EntityData data = reg->get<Comp>(e);
			return ECS::Entity(&reg, e, data.name, data.tag, data.layer, data.isActive);
		}

		//gets all entities || DO NO DELETE THIS ARRAY OR THE SCENE WILL BE FUCKED!!!
		const ECS::Entity* GetAllEntities(uint32_t& arraySize) const { arraySize = entities.size(); return entities.data(); }
		//gets all entities by name || remeber to destroy the array after your done.
		Mirror::ECS::Entity** GetAllEntitiesByName(uint32_t& arraySize, const std::string& name) const;
		//gets all entities by tag || remeber to destroy the array after your done.
		ECS::Entity** GetAllEntitiesByTag(uint32_t& arraySize, const std::string& tag) const;
		//gets all entities by layer || remeber to destroy the array after your done.
		ECS::Entity** GetAllEntitiesByLayer(uint32_t& arraySize, const std::string& layer) const;

		//gets all entities by name || remeber to destroy the array after your done.
		Mirror::ECS::Entity** GetAllEntitiesByName(const std::string& name) const;
		//gets all entities by tag || remeber to destroy the array after your done.
		ECS::Entity** GetAllEntitiesByTag(const std::string& tag) const;
		//gets all entities by layer || remeber to destroy the array after your done.
		ECS::Entity** GetAllEntitiesByLayer(const std::string& layer) const;

		//makes a camera
		ECS::Entity* MakeCamera(float width = 800.0f, float height = 600.0f);

		//sets if the scene is active
		inline void IsActive(bool state) { isActive = state; }

		//gets the scene registory
		inline entt::registry& GetReg() { return reg; }
		//gets if the scene is active
		inline bool IsActive() const { return isActive; }
	};
}