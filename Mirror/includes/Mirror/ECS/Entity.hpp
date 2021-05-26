#pragma once

//defines the Entity for ECS

#include <entt\entt.hpp>
#include <entt\entity\registry.hpp>
#include <string>

#include <Mirror\ECS\Components\EntityData.hpp>

namespace Mirror::ECS
{
	class Entity
	{
		//vars
	private:

		entt::registry* reg = nullptr; //the scene reg
		entt::entity entity{ entt::null };

		std::string name = "Entity";
		std::string tag = "Default";
		std::string layer = "Default";
		bool isActive = true;

		//the entities childeren

		//methods
	public:

		//Constructor
		Entity() {}
		//Constructor
		Entity(entt::registry* _reg, entt::entity& id, std::string _name = "Entity", std::string _tag = "Default", std::string _layer = "Default", bool _isActive = true /*childeren*/)
		: reg(_reg), entity(id), name(_name), tag(_tag), layer(_layer), isActive(_isActive)
		{
			//checks if it needs a Entity Data
			if(!reg->any_of<Comp::EntityData>(entity))
				reg->emplace_or_replace<Comp::EntityData>(entity, name, tag, layer, isActive);
		}

		//destroys the entity
		void Destroy();

		//sets the name
		void SetName(const std::string& _name);
		//sets the tag
		void SetTag(const std::string& _tag);
		//sets the layer
		void SetLayer(const std::string& _layer);
		//sets if the entity is active
		void SetIsActive(bool _isActive);

		//gets the name
		inline std::string GetName() const { return name; }
		//gets the tag
		inline std::string GetTag() const { return tag; }
		//gets the layer
		inline std::string GetLayer() const { return layer; }
		//gets if the entity is active
		inline bool IsActive() const { return isActive; }
		//gets the entity id
		inline entt::entity GetId() const { return entity; }
		//gets the scene registory
		inline entt::registry* GetRegistry() const { return reg; }

		//checks if the entity has the component
		template<typename Comp>
		bool HasComponent()
		{
			if (!reg)
			{
				printf("Smok Warning: NULL Reg || Reg is NULL can not check if entity %s has comp!\n", name.c_str());
				return false;
			}

			if (!reg->valid(entity))
			{
				printf("Smok Warning: NULL entity id || id is NULL can not check if entity %s has comp!\n", name.c_str());
				return false;
			}

			return reg->any_of<Comp>(entity);
		}

		//gets a compoent
		template<typename Comp>
		Comp* GetComponent()
		{
			if (!reg)
			{
				printf("Smok Warning: NULL Reg || Reg is NULL can not get component!\n");
				return nullptr;
			}

			if (!reg->valid(entity))
			{
				printf("Smok Warning: NULL Entity || Entity %s is NULL can not get component!\n", name.c_str());
				return nullptr;
			}

			if (!HasComponent<Comp>())
			{
				printf("Smok Warning: NULL Comp || Entity %s does not contain the desired component!\n", name.c_str());
				return nullptr;
			}

			return &reg->get<Comp>(entity);
		}

		//adds a component
		template<typename Comp, typename... Args>
		Comp* AddComponent(Args&&... args)
		{
			if (!reg)
			{
				printf("Smok Warning: NULL Reg || Reg is NULL can not add component to %s!\n", name.c_str());
				return nullptr;
			}

			if (!reg->valid(entity))
			{
				printf("Smok Warning: NULL Entity || Entity is NULL can not add component to %s!\n", name.c_str());
				return nullptr;
			}

			return &reg->emplace_or_replace<Comp>(entity, std::forward<Args>(args)...);
		}

		//removes a compoent
		template<typename Comp>
		void RemoveComponent()
		{
			if (!reg)
			{
				printf("Smok Warning: NULL Reg || Reg is NULL can remove component from %s!\n", name.c_str());
				return;
			}

			if (!reg->valid(entity))
			{
				printf("Smok Warning: NULL Entity || Entity is NULL can not remove component from %s!\n", name.c_str());
				return;
			}

			if (!HasComponent<Comp>())
			{
				printf("Smok Warning: NULL Comp || Entity %s does not have the component to remove!\n", name.c_str());
				return;
			}

			return &reg->remove<Comp>(entity);
		}

		//gets the entities childeren
		//adds a entity as a child
		//removes a entity as a child

		//gets all childeren with a component
		//removes all childeren with a compoent
	};
}