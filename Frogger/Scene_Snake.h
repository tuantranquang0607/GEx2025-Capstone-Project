//
// Created by David Burchill on 2023-09-27.
//

#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include "Scene.h"
#include "GameEngine.h"


class Scene_Snake : public Scene {

	sPtrEntt				_player{ nullptr };
	sf::FloatRect           _worldBounds;

	int						_lives{ 1 };
	int						_score{ 0 };
	int						_scoreTotal{ 0 };
	int						_scoredHeights[11] = {};

	bool					_isFinish = false;
	bool					_isComplete = false;
	bool                    _drawTextures{ true };
	bool                    _drawAABB{ false };
	bool                    _drawCam{ false };


	//systems
	void                    sMovement(sf::Time dt);
	void                    sCollisions();
	void                    sUpdate(sf::Time dt);
	void	                onEnd() override;
	void					sAnimation(sf::Time dt);
	void					drawScore();
	void					getScore();
	void					drawGameOver();
	void					drawWin();



	// helper functions
	void                    init(const std::string& path);
	void                    loadLevel(const std::string& path);
	void                    spawnPlayer(sf::Vector2f pos);
	void                    playerMovement(sf::Time dt);
	void                    adjustPlayerPosition();
	void	                registerActions();
	/*void                    checkIfDead(sPtrEntt e);*/

public:
	Scene_Snake(GameEngine* gameEngine, const std::string& levelPath);

	void		update(sf::Time dt) override;
	void		sDoAction(const Command& action) override;
	void		sRender() override;

};
