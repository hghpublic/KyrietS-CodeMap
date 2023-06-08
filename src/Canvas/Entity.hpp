#pragma once

#include "entt/entt.hpp"
#include "Canvas.hpp"
#include "Components.hpp"


class Entity
{
public:
	Entity() = default;
	Entity(const Entity&) = default;
	Entity(entt::entity handle) 
		: Entity(handle, &Canvas::Get()) {}
	Entity(entt::entity handle, Canvas* canvas)
		: m_EntityHandle(handle), m_Canvas(canvas)
	{
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		assert(!HasComponent<T>());
		T& component = m_Canvas->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename T, typename... Args>
	void AttachScript(Args&&... args)
	{
		if (HasComponent<Components::NativeScript>())
		{
			GetComponent<Components::NativeScript>().Bind<T>(std::forward<Args>(args)...);
		}
		else
		{
			AddComponent<Components::NativeScript>().Bind<T>(std::forward<Args>(args)...);
		}
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& component = m_Canvas->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	void AddChild(Entity child)
	{
		assert(HasComponent<Components::Hierarchy>());
		assert(child.HasComponent<Components::Hierarchy>());
		assert(child.GetComponent<Components::Hierarchy>().Parent == entt::null);

		auto& childHierarchy = child.GetComponent<Components::Hierarchy>();
		auto& parentHierarchy = GetComponent<Components::Hierarchy>();
		parentHierarchy.Children.push_back(child);
		childHierarchy.Parent = *this;
	}

	template<typename T>
	T& GetComponent() const
	{
		assert(HasComponent<T>());
		return m_Canvas->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent() const
	{
		return m_Canvas->m_Registry.any_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_Canvas->m_Registry.remove<T>(m_EntityHandle);
	}

	void Destroy()
	{
		m_Canvas->ScheduleEntityForDestruction(m_EntityHandle);
	}

	operator bool() const { return m_EntityHandle != entt::null; }
	operator entt::entity() const { return m_EntityHandle; }
	operator uint32_t() const { return (uint32_t)m_EntityHandle; }

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Canvas == other.m_Canvas;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

protected:
	entt::entity m_EntityHandle = entt::null;
	Canvas* m_Canvas = nullptr;
};
