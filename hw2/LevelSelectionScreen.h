#ifndef LEVEL_SELECTION_SCREEN_H_
#define LEVEL_SELECTION_SCREEN_H_

#include "MenuScreen.h"

#include <string>

class LevelSelectionScreen : public MenuScreen
{
public:
	enum LoadType
	{
		LOAD_NEW_GAME,
		LOAD_SAVED_GAME
	};

private:
	static const unsigned MAX_SUPPORTED_FILES;

	std::vector<std::string> _fileNames;
	std::vector<std::string> _options;
	LevelSelectionScreen::LoadType _loadType;

	static const std::string & getLevelName(unsigned id, const std::vector<std::string> & levels);

public:
	LevelSelectionScreen(LevelSelectionScreen::LoadType loadType);
	~LevelSelectionScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* LEVEL_SELECTION_SCREEN_H_ */
