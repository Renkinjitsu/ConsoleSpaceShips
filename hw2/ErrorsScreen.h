#ifndef ERRORS_SCREEN_H_
#define ERRORS_SCREEN_H_

#include <string>
#include <vector>

#include "Screen.h"

class ErrorsScreen : public Screen
{
private:
	std::vector<std::string> _errors;
	unsigned _width;

	unsigned _startX;
	unsigned _startY;

	void updateStartPoint();

public:
	ErrorsScreen();
	~ErrorsScreen();

	void append(const std::string & error);

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* ERRORS_SCREEN_H_ */
