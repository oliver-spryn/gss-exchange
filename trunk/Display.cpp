#include <iostream>
#include <Windows.h>

#include "Display.h"
#include "DistributeParametersInvalidException.h"

using std::cout;
using std::endl;

/**
 * Chevrolet Camaro:
 * 
 * ++++++++++++++++=========================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~==~~~~~~
 * ++++++++++++++===========================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * +++++++++++======================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * +++++++++================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ++++++===================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ++++================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * +++=================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ===============================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~==~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ==========================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=~=:~~~~=~==?IIIIII?+==~~~~~~~=~=+???I??+=====~~~~$O=~=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ==========================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.:IODDDDDDD88888DDDDDD8D88DDDDD8888DDDDDDD8OOOONNDD$8,~~=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ====================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~===.78DD8OOOOO88OOZ$??++===++=============~~===+7$OOOOO888O8O8D8I,~===~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ====================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.IMMMM8+I7+~~=~=~===+7$Z88DDDDDDDDDDD88OZ$I?+~:::::::::::,?77$$ZZO8DNMD,.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ==================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~,:MMMNM::::~~~7DD$++===++=++==+++++++++=====+====+OZ++IOOO$,,,,,,,~==+?I7$ZO88M8,.~=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ===============~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~==~..MMMM8,,:::~7D?I????????????????????????????????????87????????OZ$:,,,.,,::~==+?7$ZOO8MD,.~~~=~~~=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ===============~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=:.+MMMMZ,,,::=887$777777777777777777777777777777$$7777$$78$$$$77777$I78$7,,....,,,,:~==+?I$$OOOM8.,~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =============~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=~=~~..MMMMM7,.,::INZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOOOZZOZZ8OOOZZZZZZZZZZZDZZ:......,,,,::~==+I7$ZOODN~.,~=~~~=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =============~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:,,MMMMM$..,:~IN888D8DD88DDDDDDDDDD888888888888888888DDDDDDDDD8ODDDDDDDDDDDDD8D8D+7$........,,,,,:~==+?7$ZOO88O..~=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ==========~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=~=~~?~:.~MNMMM8 .,::+N8$7$OMNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNMMMMMMMND8MMNNNNNNNNNNNN8OZZZI,........,,,,,,,:=++?I7$ZOO88888=:,.,:~:ZZ77+=:::~~~==~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~==~~~~:,,.......,.,,$NMDII..ODNMMM...,~=OM7?=~,+7DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMDNMMMMMMN88O8Z~,,,.,..,,:=I$OOOZ77I?+~:,............,,:~=+I$O8O$7=:,?7ZDDD7=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=~~,...,:+?7$7IIIIIIIIIII7777$M:8$~$NMNMMM~...~=?D8?+=~=?77I7MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNDD88D$~::::,,,:+ZOZI~,:,,,,,,,,,,,.,...............,,,,,,,,,,,,::::~=IOD8Z$:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=~~~,..,~I$7III77$ZZZZZ$$$ZZ$$$$77II?==OOO8DDMMMMI..,~=IMM~?ZZ$$$$ZZZDNNNNNNNNDDDDD888OOZZ$$$$7IIII???+===~=~=~~~~~+,:::,:=$O7=::~:::::::::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:::::::~~=?Z8OO?:=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=~~.,+$$$ZOZZ$I+:::::~=I7ZOOOOOOZZZZZ$$777II?+?++++++=++=++~~~=++=~~=8OZIIIIIIII777$$$$$Z$$$$$$77III??+++++======~~~~~:::$?7++==========~~~~~~~~~~~~~~~~~~::::::::::::::::::::::::~~~~~~~==?$MN+OOZ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =====~~~~~~~~~~~~~~~~~~~~~~~~~=~=:.=ZOO?~::::?$OOZZ+,.,,,,,:,,::::~:~~~~~========+++++++????I???II?II7$$$777777IIII7777777IIIIIIIIIIII?I??????????+++++=====~:IIII?????????+++++++++======~~:::::~~~~~~~~~~~~~~~=============+??7ZM$7=I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =====~~~~~~~~~~~~~~~~~~~=~~=~,:?::,,:?ZOOO$:,,,:::::::,,..,:~==+=+++?????IIIIIIIIII777777777Z777777777$$$$$$$7$$$77777777777777IIIIIIIIIIIIIIIIII?I$7I=+~+=+++IIIIIII????????????++++=::,,,,,,,,,,,,,,:~+=+++++++++++++++++++???I7O?+Z,~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =====~~~~~~~~~~~~~~~~~~~~~,::~~IZOOOZ?,~~~~=====:............,,,,~?I77777777777777777777777II777777IIIII777777I77IIIIIIIIIIIIIIIIIIIII??????????II7$I77=II????7?????????+++++++++?+=+=====~~~~~~~~~~~~:~~~=++++++++++++++++++++++?$88IZ,~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =====~~~~~~~~~~~~~~~==~=,::~=ZZOZ?==++???????~,,,,,,,,,:::::~~~=~=++?7IIIIIIIIIII?????????????????????????+++++++++++++++++++++++++++++++++++++++++I$$7+++++++7++++++=+========+????+?++++==~::~=++++++=====++++++++++++++==+=====?Z7O+:,~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =====~~~~~~~~~~~~~~~=~~:.I$ZOO8$7IIIIIIIIII:~~~==+=::,,,,,,,,,,,:7777I7?+++++++============7~===~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~I~~~~~~~~~~~~~~~=++===~:.,.,,,,,,,,,,,,,:===+++===~~:::::::,,,,,,,...=8Z=~:~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~~=NND8MO.ZIIIIII7777????I=:::?MMMMMMMMMMMMM+,,,~==++=~~~~~~~~~~~~~::::+:::::::::::::::::::::::::::,,,:,::::::::,,,,,,,,,,,,,.............,........:...,::::::..,.ZMMMMMMMMMMMMMMI::::~~~~~~:::::ZO$ZII7.,.....~OO+::~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~~,DDMMNM.?+??III777IIII::,MMMMMMMNNNNNNNNMMMMMI,.,.::~::::::::,:,,::,,,,................                          .....   .................... .8=D=8.,,,,...,8MMMMMMMMMMMMMMMMMMMM8,,:,:::::::,:,,,,,,,......~OO=~:=~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~=:8DMNDZI~~=++??IIII?~::MMMMMNNDD88888888DNNMMMM=,,.....~,~::,:~:~=+=.,.................................................................+  ... Z+$?7.......,8MMMMMNNDDD8888DDNNMMMMMMZ,,::,::,::,,,,,,.......,=OO==+~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~=7OMMNNZ,...,.:::~=++=MMMMNDDDDDDNNDDDDDND888NNMMM:,,.................:................................................................  ...  ?+?+I~....,,7MMMMNN8888DDDDDND8888DNMMMMM?:::,:::,,,,,,,,......,+OO=~:~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =======~~~~~~~~~~~~~~:,:~=ZI:....,I????=~:MMMNDDDDND7OZ=...OOZ7DND88DMMM$~:,,,,,,,,,,,,,,,,?,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,~,....,O,$~?....,,MMMMND8DDNDZ?Z...+$?8ND88DNMMMMD:::,,,,,,,,,,,,,,,,,:?OO=~::=~~~~~~~~~~~~~~~~~~~~~~~~~
 * ==========~~~~~~~~~~:,,,~?$I:....,,,,,:~?MMMNDDDD7ZZ8MMO...MMNOOZDN8D8NMMD::,,,:::::::::,,,I,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:::::,.,,,,,N.D.D,,.,:,MMMNN88DDIOZONN  .ZN8OZIDD88DNMMMM::,,,,,,,,,,,,,,,,,,:ZOO7,,~~~~~~~~~~~~~~~~~~~~~~~~~~
 * =============~~~~~~=::,,~I$I:....,,,,,~=MMMDDDDD$ODMMMMM..,MMMMMNOI8DDONMMZ=:::::::::::::::+:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::=,,,,,8:D:N:,,,::MMMND88D8$ZMMMMM. .NMMMMOZI8ND8NMMMO:~:::::::::::~~~~~=?Z8O$,:=~~~~~~~~~~~~~~~~~~~~~~~~~
 * =============~~~~~==::,,~+$7:..,,,,,,~=MMMDDDD8Z8MMMMZ$7+:7O$$DMMM8I8DDOMMM==~~~~~~~~~~~~~~~~:::::::::::::::::::::::::::::::::::::::::::::::::::::::~~~:::?,,,,,,D.8:Z,,,::ZMMND88D$ZZMMMMZO~.,$8MMMM8$7NND8NMMM==~~~~~~~~~~~~~===+7O8OZZ:~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ===============~~~~~+:::?DZ7~,,,,,,,,?+MMM8D88,+MMMMZ$7777$Z77$$MMD?=DD8DMMN===~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=====~~~=~~:::,N:N:M:,,,~~MMMNO8D$Z8MMD77IZ$=+II77MMMNO$8DD8MMM?++========+++++??7Z8DMNI~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ===============~~~~~~~D=?DDZ~,:::::,:?$MMOD8D~,...IDO$8ZO+$ZOZZO?....ODDZNMM++=====================================================~~~::,,,.......,,::~~=:::,..,::~::::::~$MMNODD8...=8$7ZO$7DZ?OZONNO...=DD8DMMMII?????II77$$$$ZZOO8MM~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ==================~~=~$O$?NZ+~~~~~~~+7NMMOD88I==??+,$OOI+8ZO8D=.::,:~+DD8DMM7????++++++++++?7++++++++++++++==~~::,,.....,::~===+++++++==============~~~~~~~~~=====~:,..:~=MMMNO8D:......,?7?8+8.$7:....,,.8DD8MMMZZZZZZZZZZZZZOOOOO87D=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ,,,,,,,,,,,,,,,,,,...,7Z$?NZ?==~====I$MMM8D8O~OMMM8O?O?8,,.Z+77ODDMM:.888DMMD7III???????+=~:I,..,,:::~~~~~~::::,,,,,,,,,,::~~=++??II7777$77$$$$$$$$$$$O77$$$$$$$$$$$$$7777MMMD88D :MM8?+~O,?:.,I.8++IOMM8IO8D8MMMZZZOOOOOOOOOOOO8ODN:,....,,,,,..,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,:,,:~+$++++++++7$MMM8D8O.=MMM888+~,=++.+=ONDDMM,.8D8DMMM777II7777$$$$$$Z$$$$$$$$$$$$7$$7$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$8$$$$$$$$$$$$$ZZZ$$$ZMMMD8D8 .MMMOOZ+:,~==..7ONNDMMO.O8D8MMMOOOOOOOOOOOOO8ODN=,,.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,,I?I78OOZZZZZ$$ZOMMMDDDD..MMMN88=8?ZZ7?I$8NDNMM.:88DDMMMZZZ8$$ZZZZZZZZ$$$$Z$$$$$$$$$$ZZZ$$$$$$$$$$ZZZZZZZZ$$$ZZZZZZZZZZOOOOOZZZZZZZZZZZZZZZZZZZZZZZZZMMMDD88..MMMOOO+~,I:7:=I88DMMM~.D8DDMMMOOOOO8D88D8NI..,.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,.,...:ND88OOOOZZOODMMNDDO= =MMMD$.?I88O$?.ZNNMM$.N88DNMMMZZZZ$$$$$$$$$$$$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZMMMND8OZ.DMM8OD$~Z=Z7O=?Z8MMMD..8DDDMMMDDDDDNNN7,,,.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,,,,,,.,.DO8OOOOOOOMMMNDD8.?8MM$..IN888O~..7MMO.7888DMMMMOOOZZZZZOOOOOOZZZZZZZZZZZZZZZZZZZZOOOOOOOOOOOOOOOO888888888888888888888OOOOOOOOOOOOOOOOOOOOOZMMMNNDD8,.MMMN...O8OOO...DD78+.DODD8NMMMM$ ,...,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,,,,,,,,.,:=$OD8888:::ODD88.I7:~=IMN8DDNM?~::+~7O8DNMMMMMDDDDDDDDDDDDDDDD8DDDDDDDDDDDDDDDDD8DDD88888DDDDDD8DDDDDDD8DDDDD888888888888888888DDDDDDDDDNND,,..ND888.Z87,,:88DDDDD:,.I8I,8ODDN.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,.....NDDNN.,,:MNDO8:?=??7.IMMMMMO++=:ZO88DN.,,..=~~:::,,.........,,,,,.........,,,..,,,,,,,,,.,.,,,,,,.,.,,,,,,,,,,,.....,,,,,,,,,,,.,..,,,.,.,,.+ND888..~~=7MMMMMMMD=~:,.DODDN~,,.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,,::::~~~===++??III$$$$ZNNDDO8:?+=ONNNMNO~:I~D88DNNNNNDDDD888888888OOOOOOOOOOOOOOZZZZZZZZZZZZZZZZZZZZZZZZZZ$$$$$$$$$$$$$$$$$$$$$$$ZZZZZZZZZZZZOOOOOOO8888DDNND88OO+=+NMNDD8OO87=+IOODDN???+++===~~~::::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,::~~===++?II77$$ZZZOO8888DDDDNND8OOD==$Z$Z7:IOOOD8NNNNNNNNNNNNNNNNNNNNNNNNNDNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNDD8ZD:=...,..,+:DO8DDNN88OOOOZ$$$$77II??++==~~~::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,::::~==++?I77$$ZZOOO888DDDDDDNNNMNDND8OOOOOO88DDDNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNMNND8O8O8DDD8OOZDDDNMD8888888OOOZZZ$$$77II??++==~~::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,::::::~~===++??II77$$ZZOO88DNNMMNNNNNNNMMMMMMNNNNDDDD88888888888OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO888888888888888888888888888888888888DDDDDDDDDNNNNNNNNNNMMNNNNNDDNNNNMMND8888OOOZZ$$$777III??++==~~~~:::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 * ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::::::~~~~~:~::::::,:::::::::::,,:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::~~~~~~~~===+??I7$$OODDDNNNNNDD8OZ$7I?++===~~~~::::::::::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::::::::
 * ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:::,,,,,,,,,,::::::::::::::::
 * :::::::::::::::::::::::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:::::::::::::::::::::::::::::::::::::
 * ::::::::::::::::::::::::::::::::::::::::::::::::::::::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::,,,,,,,,,,,,,,,,,,,,,:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::~~~
 * ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::~~~~~~~~~~~~~~~~
 * ~~~~~~~:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                                                                                                                                                                                                                                                       GlassGiant.com
*/

/**
 * This will clear the text in the console.
 *
 * @access public
 * @return void
*/

void Display::clear() {
	system("CLS");
}

/**
 * This function is designed to output colored or highlighted text to the
 * console.
 *
 * @access public
 * @param  string text  The text to display to the console
 * @param  int    color The integer representation of the text or highligher color to use
 * @return void
*/

void Display::coloredText(string text, int color) {
	HANDLE cHandle;
	cHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(cHandle, color);
	cout << text;
	SetConsoleTextAttribute(cHandle, Display::DEFAULT_CONSOLE_COLOR);
}

/**
 * This function will take a vector of strings and lay them out such that each
 * string will have its own "cell" in an evenly-spaced, invisible table that
 * stretches the length of the console window.
 *
 * For example, if these strings are given:
 *  - one
 *  - two
 *  - three
 *  - four
 *
 * they will be laid out like this (except without the table border, which is 
 * show here to demonstrate this function's formatting capibilities):
 *
 * +------------------+------------------+------------------+-----------------+
 * |one               |two               |three             |four             |
 * +------------------+------------------+------------------+-----------------+
 *
 * The align parameter will accept a vector of characters, where each character
 * may have one of the following values:
 *  - L = align left
 *  - C = align center
 *  - R = align right
 *
 * The string of text is matched to the associated align character by its index
 * in each of the vectors.
 *
 * @access public
 * @param  vector<string>                       text  A vector of strings containing text to be aligned
 * @param  vector<char>                         align A vector of characters symbolizing the string's alignment within the respective "cell"
 * @return string                               ret   A string formatted to the parameter's specifications
 * @throws CentralizeParametersInvalidException       Encountered if parameter vector sizes do not match or if an invalid character appears in the align vector
*/

string Display::distribute(vector<string> text, vector<char> align) {
	int length = text.size();
	string ret = "";
	
//Does the size of each vector parameter match?
	if (text.size() != align.size()) {
		string error = "The number of strings in the text parameter (";
		error += text.size();
		error += ") does not match the number of characters in the align parameter (";
		error += align.size();
		error += ").";

		throw DistributeParametersInvalidException(error);
	}

//Calculate the number of characters we can allot per cell with the given input
	int cellW = static_cast<int>(floor(static_cast<double>(Display::CONSOLE_WIDTH) / static_cast<double>(length)));
	int extra = Display::CONSOLE_WIDTH - (cellW * length);
	int leftW = 0;
	int rightW = 0;
	
//Print out each "cell"
	for (int i = 0; i < length; i++) {
		switch(align[i]) {
			case 'L' : 
				ret += text[i];

			//Add any padding needed to the right side of the text
				rightW = 0;
				rightW = cellW - text[i].length();

				for (int k = 0; k < rightW; k++) {
					ret += " ";
				}

			//Usually not all cells will have an even amount of space, so in the final iterations of this loop
			//output an additional space in the last few cells to ensure the the returned string is still
			//Display::CONSOLE_WIDTH characters wide
				if (extra + i >= length) {
					ret += " ";
				}

				break;

			case 'C' : 
			//Add any padding needed to the left side of the text
				leftW = 0;
				leftW = static_cast<int>(floor(static_cast<double>(cellW - text[i].length()) / 2.0));

				for (int j = 0; j < leftW; j++) {
					ret += " ";
				}

				ret += text[i];

			//Add any padding needed to the right side of the text
				rightW = 0;
				rightW = static_cast<int>(ceil(static_cast<double>(cellW - text[i].length()) / 2.0));

				for (int k = 0; k < rightW; k++) {
					ret += " ";
				}

			//Usually not all cells will have an even amount of space, so in the final iterations of this loop
			//output an additional space in the last few cells to ensure the the returned string is still
			//Display::CONSOLE_WIDTH characters wide
				if (extra + i >= length) {
					ret += " ";
				}

				break;

			case 'R' : 
			//Usually not all cells will have an even amount of space, so in the final iterations of this loop
			//output an additional space in the last few cells to ensure the the returned string is still
			//Display::CONSOLE_WIDTH characters wide
				if (extra + i >= length) {
					ret += " ";
				}

			//Add any padding needed to the right side of the text
				leftW = 0;
				leftW = cellW - text[i].length();

				for (int j = 0; j < leftW; j++) {
					ret += " ";
				}

				ret += text[i];
				break;

			default : 
				string error = "An invalid align type wan encountered: ";
				error += align[i];
				error += ". Valid align types include: L (left), C (center), or R (right).";

				throw DistributeParametersInvalidException(error);
				break;
		}
	}
	
	return ret;
}

/**
 * This function will reset the output text color to default.
 *
 * @access public
 * @return void
*/

void Display::resetTextColor() {
	HANDLE cHandle;
	cHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(cHandle, Display::DEFAULT_CONSOLE_COLOR);
}

/**
 * This function is designed to set the text color or highlighter color for all
 * text which will be outputted to the console after this function call.
 *
 * @access public
 * @param  int    color The integer representation of the text or highligher color to use
 * @return void
*/

void Display::setTextColor(int color) {
	HANDLE cHandle;
	cHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(cHandle, color);
}