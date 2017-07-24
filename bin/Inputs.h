#ifndef __INPUTS_H__
#define __INPUTS_H__

#include <vector>
class Inputs {
public:
    static void Update();
    static void UpdateWheel(int delta);
    static void KeyPressed(const sf::Keyboard::Key &code);
    static void KeyReleased(const sf::Keyboard::Key &code);
    static void ButtonPressed(const sf::Mouse::Button &code);
    static void ButtonReleased(const sf::Mouse::Button &code);
	enum Key {
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
        L_SHIFT,
		ACCEPT,
		CANCEL,
		ENTER,
		SELECT,
        CONTROL,
        TAB,
        A,
        D,
        Q,
        SPACE,
        N0,
        N1,
        N2,
        N3,
        N4,
        N5,
        N6,
        N7,
        N8,
        N9,
        ESC,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		ADD,
		SUB,
		M,
		NUM_KEYS
	};
    enum Button {
        M_LEFT = 0,
        M_RIGHT,
        NUM_BUTTONS
    };

	/**
		Returns true if the key k is beeing held down
		@param k, the key to check
	*/
    static bool KeyDown(Inputs::Key k);

	/**
		Returns true if the key k is not beeing held down
		@param k, the key to check
	*/
    static bool KeyUp(Inputs::Key k);

	/**
		Returns true if the key k has been pressed in the last update
		@param k, the key to check
	*/
    static bool KeyHit(Inputs::Key k);

	/**
		Returns true if the key k has been released in the last update
		@param k, the key to check
	*/
    static bool KeyBreak(Inputs::Key k);


    /**
		Returns true if the key k is beeing held down
		@param k, the key to check
	*/
    static bool MouseDown(Inputs::Button k);

    /**
        Returns true if the key k is not beeing held down
        @param k, the key to check
    */
    static bool MouseUp(Inputs::Button k);

    /**
        Returns true if the key k has been pressed in the last update
        @param k, the key to check
    */
    static bool MouseHit(Inputs::Button k);

    /**
        Returns true if the key k has been released in the last update
        @param k, the key to check
    */
    static bool MouseBreak(Inputs::Button k);

    static int GetWheel();
private:
	/**
		Contains the translation between Inputs::Key and sf::Keyboard::Key
	*/
	static const sf::Keyboard::Key _keyMap[NUM_KEYS];
    static const sf::Mouse::Button _mouseMap[NUM_BUTTONS];
    static int wheelDelta;
    static bool _lastKeyState[NUM_KEYS];
    static bool _currentKeyState[NUM_KEYS];
    static bool _lastButtonState[NUM_BUTTONS];
    static bool _currentButtonState[NUM_BUTTONS];
};

#endif
