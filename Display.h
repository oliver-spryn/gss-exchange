#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Display {
public : 
	static void clear();
	static void coloredText(string text, int color);
	static string distribute(vector<string> text, vector<char> align);
	static void resetTextColor();
	static void setTextColor(int color);

	static const int CONSOLE_HEIGHT = 25;
	static const int CONSOLE_WIDTH = 80;
	static const int DEFAULT_CONSOLE_COLOR = 7;
};

enum colors {
	black = 0,
	darkBlue = 1,
	darkGreen = 2,
	darkAqua, darkCyan = 3,
	darkRed = 4,
	darkPurple = 5, darkPink = 5, darkMagenta = 5,
	darkYellow = 6,
	darkWhite = 7,
	gray = 8,
	blue = 9,
	green = 10,
	aqua = 11, cyan = 11,
	red = 12,
	purple = 13, pink = 13, magenta = 13,
	yellow = 14,
	white = 15
};

#endif