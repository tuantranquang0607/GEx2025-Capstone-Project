#pragma once

#include <SFML/System/Time.hpp>

class State
{
	public:
		State() {};
		virtual ~State() {};

		virtual void Init() = 0;
		virtual void ProcessInput() = 0;
		virtual void Update(sf::Time delltaTime) = 0;
		virtual void Draw() = 0;
		virtual void Pause() {};
		virtual void Start() {};
};

