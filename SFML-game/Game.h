#pragma once

#include "UI.h"
#include <functional>
#include "KeyMapper.h"

class Game : public UI
{
public:
	Game() = default;
	~Game() = default;

	// Todo: frame/tick needs some kind of merge
	bool frame() override;
	//void tick();

	void init() override;
	
	void pause() override;
	void resume() override;
	void toggle();
	//void draw() override;
	
	bool pausable = false;
	KeyMapper keyMapper;

private:
	//sf::Text pauseText;

/*	//Functions
	void HandleKeyInput();

	//int framespertick = 0; // <-- Used for measuring performance

	//Class object (should be) initialized by App

	sftools::Chronometer clock;
	Map *map = nullptr;
	std::map<std::string, TileLayer*> layerMap;
	//std::map<int, ObjectLayer*> spriteMap; todo: group "sub"-objects
	*/
};
