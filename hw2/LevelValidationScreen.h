#ifndef LEVEL_VALIDATION_SCREEN_H_
#define LEVEL_VALIDATION_SCREEN_H_

#include <vector>
#include <string>

#include "GameScreenBuilder.h"
#include "Screen.h"

class LevelValidationScreen : public Screen
{
private:
	static const unsigned MAX_SUPPORTED_FILES;

	std::vector<std::string> _fileNames;
	std::vector<std::string> _options;

	GameScreenBuilder _builder;

public:
	LevelValidationScreen();
	~LevelValidationScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* LEVEL_VALIDATION_SCREEN_H_ */
