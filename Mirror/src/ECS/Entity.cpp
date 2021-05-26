#include <Mirror\ECS\Entity.hpp>

void Mirror::ECS::Entity::Destroy()
{
	if (!reg)
	{
		printf("Smok Warning: NULL Reg || Reg is NULL can not destroy entity!\n");
		return;
	}

	if (!reg->valid(entity))
	{
		printf("Smok Warning: NULL Entity || Entity is NULL can not destroy entity!\n");
		return;
	}

	reg->destroy(entity);
	reg = nullptr;
	entity = entt::null;
}

void Mirror::ECS::Entity::SetName(const std::string& _name)
{
	name = _name;
	GetComponent<Mirror::ECS::Comp::EntityData>()->name = _name;
}

void Mirror::ECS::Entity::SetTag(const std::string& _tag)
{
	tag = _tag;
	GetComponent<Mirror::ECS::Comp::EntityData>()->tag = _tag;
}

void Mirror::ECS::Entity::SetLayer(const std::string& _layer)
{
	layer = _layer;
	GetComponent<Mirror::ECS::Comp::EntityData>()->layer = _layer;
}

void Mirror::ECS::Entity::SetIsActive(bool _isActive)
{
	isActive = _isActive;
	GetComponent<Mirror::ECS::Comp::EntityData>()->isActive = _isActive;
}