#ifndef LEVEL_SELECTION_SCREEN_H_
#define LEVEL_SELECTION_SCREEN_H_

#include "MenuScreen.h"

class LevelSelectionScreen : public MenuScreen
{
private:
private:
	static const unsigned MAX_SUPPORTED_FILES;

	std::vector<std::string> _fileNames;
	std::vector<std::string> _options;

public:
	LevelSelectionScreen();
	~LevelSelectionScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* LEVEL_SELECTION_SCREEN_H_ */
