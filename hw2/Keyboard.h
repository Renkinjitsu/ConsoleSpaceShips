#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <vector>

class Keyboard
{
public:
	enum Key
	{
		ESC = 27,
		ENTER = '\n',

		NUM0 = '0',
		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9,

		A = 'a',
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
	};

private:
	static const size_t SUPPORTED_KEYS = 128; //All ASCII characters are between 0 & 127

	bool _pressedKeys[SUPPORTED_KEYS];

	void resetPressedKeys();
public:

	Keyboard();
	~Keyboard();

	void update();

	bool isPressed(Key key) const;
};

#endif /* KEYBOARD_H_ */
