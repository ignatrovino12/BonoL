#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;
int CIRCLE_RADIUS = 50;
int CIRCLE_SIDES = 100;
const int SQUARE_NUM = 4;
const int CHARACTER_SIZE = 40;
const int CHARACTER_SIZE2 = 30;
const int TEXT_OUTLINE = 5;

float RECT_WIDTH = 100;
float RECT_HEIGHT = 100;
float PADDING = 2;
const float TOP_LEFT_X = 200;
const float TOP_LEFT_Y = 180;

int EXITED = 0;
bool INVALID = 0;
bool CANUNDO = 0;
bool GAMEOVER = 0;
bool EASYGAME = 0;
bool HARDGAME = 0;
bool BOTCOLOUR = 1; //0 BLUE 1 RED
bool NEWTIME = 0;
int SAMECOLOR = 0;
bool MUSICSTART = 0;
bool EngSelected = 1, RoSelected = 0, TimeSelected = 0;
bool TIMEMODE = 1;
bool BlitzSelected = 0, CasualSelected = 0, ColorSelected = 0;
int Released = 0, Pressed = 0;

int a[5][5], b[5][5], c[5][5];
int colorsint1, colorsint2;

sf::RenderWindow window(sf::VideoMode(800, 800), "BonoL", sf::Style::Titlebar | sf::Style::Close);
sf::Music music;
sf::Event event;
sf::Color color1;
sf::Color color2;
sf::Time t1;
sf::Time t2;

enum cell_value
{
	FREE,
	RED,
	BLUE,
	NEUTRAL,
	NEUTRAL_SELECTED
};

pair<float, float> get_position(int i, int j)
{
	return { TOP_LEFT_X + PADDING + i * (RECT_WIDTH + PADDING), TOP_LEFT_Y + PADDING + j * (RECT_HEIGHT + PADDING) };
}

pair<float, float> get_table_size()
{
	return { SQUARE_NUM * RECT_WIDTH + (SQUARE_NUM + 1) * PADDING, SQUARE_NUM * RECT_HEIGHT + (SQUARE_NUM + 1) * PADDING };
}

void StartMenu();

void Atribab()
{
	for (int i = 0; i < SQUARE_NUM; i++)
		for (int j = 0; j < SQUARE_NUM; j++)
		{
			b[i][j] = a[i][j];
			if (b[i][j] == NEUTRAL_SELECTED)
				b[i][j] = NEUTRAL;
		}
}

void Atribac() {
	int i, j;
	for (i = 0; i < SQUARE_NUM; i++)
		for (j = 0; j < SQUARE_NUM; j++)
		{
			a[i][j] = c[i][j];

		}

}

void AtribColors()
{
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			a[i][j] == FREE;
	a[0][0] = a[3][3] = NEUTRAL;
	a[1][0] = a[2][0] = a[2][1] = a[2][2] = BLUE;
	a[1][1] = a[1][2] = a[1][3] = a[2][3] = RED;
}

int NS_EXIST()
{
	bool OK = 0;
	for (int i = 0; i < SQUARE_NUM; i++)
		for (int j = 0; j < SQUARE_NUM; j++)
			if (a[i][j] == NEUTRAL_SELECTED)
				OK = 1;
	return OK;

}

int VerifBLUEFREE(int x) {
	if (x == FREE || x == BLUE) return 1;
	return 0;
}

int VerifREDFREE(int x) {
	if (x == FREE || x == RED) return 1;
	return 0;
}

int CalculMutari(int tur, int x[5][5]) {
	int sus, stg, drp, jos, i, j, i2, sum = 0, max, min;

	if (tur == 1) {
		for (i = 0; i < SQUARE_NUM; i++)
			for (j = 0; j < SQUARE_NUM; j++) {

				if (VerifBLUEFREE(x[i][j]) == 1) {
					sus = stg = drp = jos = 0;

					//CALCUL STANGA DREAPTA
					if (i - 2 >= 0)min = i - 2;
					else if (i - 1 >= 0)min = i - 1;
					else min = i;

					if (i + 2 <= 3)max = i + 2;
					else if (i + 1 <= 3)max = i + 1;
					else max = i;

					for (i2 = min; i2 <= max; i2++) {
						if (VerifBLUEFREE(x[i2][j]) == 1) {
							if (i2 < i) stg++;
							else if (i2 > i) drp++;
						}

					}
					if ((i == 2 || i == 3) && VerifBLUEFREE(x[i - 2][j]) == 1 && stg == 1) stg = 0;
					if ((i == 0 || i == 1) && VerifBLUEFREE(x[i + 2][j]) == 1 && drp == 1) drp = 0;

					//CALCUL SUS JOS
					if (j - 2 >= 0)min = j - 2;
					else if (j - 1 >= 0)min = j - 1;
					else min = j;

					if (j + 2 <= 3)max = j + 2;
					else if (j + 1 <= 3)max = j + 1;
					else max = j;

					for (i2 = min; i2 <= max; i2++) {
						if (VerifBLUEFREE(x[i][i2]) == 1) {
							if (i2 < j) sus++;
							else if (i2 > j) jos++;
						}
					}

					if ((j == 2 || j == 3) && VerifBLUEFREE(x[i][j - 2]) == 1 && sus == 1) sus = 0;
					if ((j == 0 || j == 1) && VerifBLUEFREE(x[i][j + 2]) == 1 && jos == 1) jos = 0;

					//CALCUL SUMA
					if (sus == 2) {
						if (stg == 2) sum = sum + 2;
						else if (stg == 1) sum = sum + 1;

						if (drp == 2) sum = sum + 2;
						else if (drp == 1) sum = sum + 1;
					}

					else if (sus == 1) {
						if (stg == 2) sum = sum + 1;
						if (drp == 2) sum = sum + 1;
					}

					if (jos == 2) {
						if (stg == 2) sum = sum + 2;
						else if (stg == 1) sum = sum + 1;

						if (drp == 2) sum = sum + 2;
						else if (drp == 1) sum = sum + 1;
					}

					else if (jos == 1) {
						if (stg == 2) sum = sum + 1;
						if (drp == 2) sum = sum + 1;
					}


				}
			}

	}

	else if (tur == 6) {
		for (i = 0; i < SQUARE_NUM; i++)
			for (j = 0; j < SQUARE_NUM; j++) {

				if (VerifREDFREE(x[i][j]) == 1) {
					sus = stg = drp = jos = 0;

					//CALCUL STANGA DREAPTA
					if (i - 2 >= 0)min = i - 2;
					else if (i - 1 >= 0)min = i - 1;
					else min = i;

					if (i + 2 <= 3)max = i + 2;
					else if (i + 1 <= 3)max = i + 1;
					else max = i;

					for (i2 = min; i2 <= max; i2++) {
						if (VerifREDFREE(x[i2][j]) == 1) {
							if (i2 < i) stg++;
							else if (i2 > i) drp++;
						}

					}
					if ((i == 2 || i == 3) && VerifREDFREE(x[i - 2][j]) == 1 && stg == 1) stg = 0;
					if ((i == 0 || i == 1) && VerifREDFREE(x[i + 2][j]) == 1 && drp == 1) drp = 0;

					//CALCUL SUS JOS
					if (j - 2 >= 0)min = j - 2;
					else if (j - 1 >= 0)min = j - 1;
					else min = j;

					if (j + 2 <= 3)max = j + 2;
					else if (j + 1 <= 3)max = j + 1;
					else max = j;

					for (i2 = min; i2 <= max; i2++) {
						if (VerifREDFREE(x[i][i2]) == 1) {
							if (i2 < j) sus++;
							else if (i2 > j) jos++;
						}
					}

					if ((j == 2 || j == 3) && VerifREDFREE(x[i][j - 2]) == 1 && sus == 1) sus = 0;
					if ((j == 0 || j == 1) && VerifREDFREE(x[i][j + 2]) == 1 && jos == 1) jos = 0;

					//CALCUL SUMA
					if (sus == 2) {
						if (stg == 2) sum = sum + 2;
						else if (stg == 1) sum = sum + 1;

						if (drp == 2) sum = sum + 2;
						else if (drp == 1) sum = sum + 1;
					}

					else if (sus == 1) {
						if (stg == 2) sum = sum + 1;
						if (drp == 2) sum = sum + 1;
					}

					if (jos == 2) {
						if (stg == 2) sum = sum + 2;
						else if (stg == 1) sum = sum + 1;

						if (drp == 2) sum = sum + 2;
						else if (drp == 1) sum = sum + 1;
					}

					else if (jos == 1) {
						if (stg == 2) sum = sum + 1;
						if (drp == 2) sum = sum + 1;
					}


				}
			}
	}
	return sum - 1;
}

void Turnshow(int tur, sf::RenderWindow& window)
{

	sf::RectangleShape rectangle(sf::Vector2f(RECT_WIDTH, RECT_WIDTH));
	sf::Font font;
	sf::Text text;
	text.setFont(font);
	sf::RectangleShape Skip(sf::Vector2f(150, 75));
	sf::RectangleShape Undo(sf::Vector2f(150, 75));
	sf::IntRect Back(650, 730, 150, 100);
	sf::RectangleShape Back2(sf::Vector2f(200, 100));

	sf::IntRect Skip2(425, 650, 100, 50);
	sf::IntRect Undo2(275, 650, 100, 50);

	if (!font.loadFromFile("font/font.otf")) { // adresa font
		std::cout << "Error font";
		system("pause");
	} // verificare font

	//PATRAT COLT
	if (tur <= 4)
	{
		rectangle.setFillColor(color1);
		rectangle.setPosition(WINDOW_WIDTH - RECT_WIDTH, 0);
		window.draw(rectangle);
	}
	else if (tur == 5 || tur == 10)
	{
		if (tur == 10) rectangle.setFillColor(color2);
		else rectangle.setFillColor(color1);

		rectangle.setPosition(WINDOW_WIDTH - RECT_WIDTH, 0);
		window.draw(rectangle);
		rectangle.setFillColor(sf::Color::Yellow);
		rectangle.setPosition(WINDOW_WIDTH - RECT_WIDTH / 2, 0);
		window.draw(rectangle);

		//SKIP BUTTON
		if (NS_EXIST() == 0 && GAMEOVER == 0)
		{
			Skip.setFillColor(sf::Color::Black);
			Skip.setOutlineColor(sf::Color{ 255, 215, 0 });
			Skip.setOutlineThickness(3);
			Skip.setPosition(405, 650);
			window.draw(Skip);
			if (EngSelected == 1) text.setString("SKIP");
			else text.setString("SARI");
			text.setFillColor(sf::Color{ 255, 215, 0 });
			text.setCharacterSize(1.3 * CHARACTER_SIZE);
			text.setPosition(Skip2.left + (Skip2.width - text.getLocalBounds().width) / 2 + 5, Skip2.top + (Skip2.height - text.getLocalBounds().height) / 2);
			window.draw(text);
		}
	}
	else
	{
		rectangle.setFillColor(color2);
		rectangle.setPosition(WINDOW_WIDTH - RECT_WIDTH, 0);
		window.draw(rectangle);
	}

	//UNDO BUTTON
	if ((tur == 5 || tur == 10 || tur == 1 || tur == 6) && NS_EXIST() == 0 && CANUNDO == 1 && GAMEOVER == 0) {
		Undo.setFillColor(sf::Color::Black);
		Undo.setOutlineColor(sf::Color{ 255, 215, 0 });
		Undo.setOutlineThickness(3);
		Undo.setPosition(255, 650);
		window.draw(Undo);
		if (EngSelected == 1)
		{
			text.setString("UNDO");
			text.setCharacterSize(1.3 * CHARACTER_SIZE);
		}
		else {
			text.setString("SCHIMBA");
			text.setCharacterSize(CHARACTER_SIZE);
		}
		text.setFillColor(sf::Color{ 255, 215, 0 });
		text.setPosition(Undo2.left + (Undo2.width - text.getLocalBounds().width) / 2 + 5, Undo2.top + (Undo2.height - text.getLocalBounds().height) / 2);
		window.draw(text);
	}

	// BACK BUTTON
	Back2.setPosition(650, 730);
	Back2.setOutlineThickness(5);
	Back2.setFillColor(sf::Color::Black);
	Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
	if (EngSelected == 1) {
		text.setString("BACK");
		text.setCharacterSize(1.65 * CHARACTER_SIZE);
		text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 720);
	}
	else {
		text.setString("INAPOI");
		text.setCharacterSize(1.3 * CHARACTER_SIZE);
		text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 730);
	}
	text.setFillColor(sf::Color{ 255, 215, 0 });
	window.draw(Back2);
	window.draw(text);

	//GAME OVER
	if (GAMEOVER == 1) {
		text.setCharacterSize(CHARACTER_SIZE2);
		if (EngSelected == 1) {
			text.setString("GAME OVER");
		}
		else text.setString("JOC TERMINAT");
		text.setPosition((WINDOW_WIDTH - text.getLocalBounds().width) / 2, 50);
		if (tur >= 6) text.setFillColor(color1);
		else if (tur <= 5)  text.setFillColor(color2);
		window.draw(text);


		if (tur <= 5) {
			text.setFillColor(color2);

			if (EASYGAME == 1 || HARDGAME == 1) {
				if (EngSelected == 1) {
					if (BOTCOLOUR == 0) text.setString("THE PLAYER WON");
					else text.setString("THE BOT WON");
				}
				else {
					if (BOTCOLOUR == 0) text.setString("JUCATORUL A CASTIGAT");
					else text.setString("BOT-UL A CASTIGAT");
				}
			}
			else {
				if (EngSelected == 1)  text.setString("2ND PLAYER WON");
				else text.setString("AL DOILEA JUCATOR A CASTIGAT");
			}

		}
		else if (tur >= 6) {
			text.setFillColor(color1);

			if (EASYGAME == 1 || HARDGAME == 1) {
				if (EngSelected == 1) {
					if (BOTCOLOUR == 1) text.setString("THE PLAYER WON");
					else text.setString("THE BOT WON");
				}
				else {
					if (BOTCOLOUR == 1) text.setString("JUCATORUL A CASTIGAT");
					else text.setString("BOT-UL A CASTIGAT");
				}
			}

			else {
				if (EngSelected == 1) 	text.setString("1ST PLAYER WON");
				else text.setString("PRIMUL JUCATOR A CASTIGAT");
			}


		}
		text.setPosition((WINDOW_WIDTH - text.getLocalBounds().width) / 2, 83);
		text.setCharacterSize(CHARACTER_SIZE2);
		window.draw(text);
	}

	// MUTARE INVALIDA
	else if (INVALID == 1) {
		if (EngSelected == 1) text.setString("INVALID MOVE");
		else text.setString("MUTARE INVALIDA");
		text.setCharacterSize(CHARACTER_SIZE2);
		text.setPosition((WINDOW_WIDTH - text.getLocalBounds().width) / 2, TOP_LEFT_Y + get_table_size().second + 20);
		text.setFillColor(sf::Color::Red);
		window.draw(text);
	}

	//NR MUTARI POSIBILE
	if (tur == 1 || tur == 6) {
		if (EngSelected == 1) text.setString("POSSIBLE MOVES");
		else text.setString("MUTARI POSIBILE");
		text.setCharacterSize(CHARACTER_SIZE2);
		text.setPosition((WINDOW_WIDTH - text.getLocalBounds().width) / 2 - 20, TOP_LEFT_Y - 50);
		if (tur == 1) text.setFillColor(color1);
		else if (tur == 6)  text.setFillColor(color2);
		text.setCharacterSize(CHARACTER_SIZE2);
		window.draw(text);
		int spacing = text.getLocalBounds().width;

		text.setPosition((WINDOW_WIDTH - text.getLocalBounds().width) / 2 - 20 + spacing + 15, TOP_LEFT_Y - 50);
		int nr = CalculMutari(tur, a);
		text.setString(std::to_string(nr));
		if (tur == 1) text.setFillColor(color1);
		else if (tur == 6)  text.setFillColor(color2);
		text.setCharacterSize(CHARACTER_SIZE2);
		window.draw(text);
	}

	//TIME MODE
	if (TIMEMODE == 1) {

		//TIME PLAYER 1
		text.setPosition(0, 0);
		if (EngSelected == 1) text.setString("TIME LEFT ");
		else text.setString("TIMP RAMAS ");
		text.setFillColor(color1);
		text.setCharacterSize(CHARACTER_SIZE2);
		window.draw(text);
		int spacinginc = text.getLocalBounds().width + 1;

		float near1 = round(((float)t1.asSeconds()) * 10) / 10;

		text.setPosition(spacinginc, 0);
		text.setString(std::to_string((int)near1));
		window.draw(text);

		int spacing = text.getLocalBounds().width + 1 + spacinginc;
		text.setPosition(spacing, 0);
		text.setString('.');
		window.draw(text);

		near1 = (int)(near1 * 10) % 10;
		text.setPosition(text.getLocalBounds().width + spacing, 0);
		text.setString(std::to_string((int)near1));
		window.draw(text);

		//TIME PLAYER 2
		text.setPosition(0, 50);
		if (EngSelected == 1) text.setString("TIME LEFT ");
		else text.setString("TIMP RAMAS ");
		text.setFillColor(color2);
		window.draw(text);
		spacinginc = text.getLocalBounds().width + 1;

		near1 = round(((float)t2.asSeconds()) * 10) / 10;

		text.setPosition(spacinginc, 50);
		text.setString(std::to_string((int)near1));
		window.draw(text);

		spacing = text.getLocalBounds().width + 1 + spacinginc;
		text.setPosition(spacing, 50);
		text.setString('.');
		window.draw(text);

		near1 = (int)(near1 * 10) % 10;
		text.setPosition(text.getLocalBounds().width + spacing, 50);
		text.setString(std::to_string((int)near1));
		window.draw(text);
	}

}

void UndoMove(int& tur)
{
	int ctur = 0;
	for (int i = 0; i < SQUARE_NUM; i++)
		for (int j = 0; j < SQUARE_NUM; j++)
			a[i][j] = b[i][j];

	if (tur == 5) ctur = 1;
	else if (tur == 10) ctur = 6;
	else if (tur == 6) ctur = 5;
	else if (tur == 1) ctur = 10;
	tur = ctur;
}

int Validmove(int tur, int x[5][5]) {
	int vi[4] = { 0,0,0,0 }, vj[4] = { 0,0,0,0 }, p = 0, i, j;
	bool OK = 1, ID = 0;

	if (tur == 5) {
		for (j = 0; j < 4; j++)
			for (i = 0; i < 4; i++)
			{
				if (x[i][j] == BLUE) {
					vi[p] = i; vj[p] = j;
					p++;
				}

			}
	}

	if (tur == 10) {
		for (j = 0; j < 4; j++)
			for (i = 0; i < 4; i++)
			{
				if (x[i][j] == RED) {
					vi[p] = i; vj[p] = j;
					p++;
				}

			}
	}

	//CAZ1
	if (vj[0] == vj[1]) {
		if (vi[0] + 1 != vi[1])   OK = 0;

		if (vj[2] == vj[0]) {
			if (vi[1] + 1 != vi[2]) OK = 0;
			if (vj[3] != vj[0] + 1) OK = 0;
			else if (vi[3] != vi[0] && vi[3] != vi[2]) OK = 0;
		}

		else if (vj[2] == vj[0] + 1) {
			if (vi[2] != vi[0] && vi[2] != vi[1]) OK = 0;
			else if (vj[3] != vj[2] + 1 || vi[3] != vi[2]) OK = 0;
		}

		else OK = 0;
	}

	//CAZ2
	else if (vi[0] == vi[1]) {
		if (vj[0] + 1 != vj[1])   OK = 0;

		if (vj[2] == vj[1]) {
			if (vi[2] != vi[1] + 1) OK = 0;
			else if (vi[3] != vi[2] + 1 || vj[3] != vj[2]) OK = 0;
		}

		else if (vj[2] = vj[1] + 1) {
			if (vi[2] == vi[1]) { if (vi[3] != vi[2] + 1 || vj[3] != vj[2])OK = 0; }

			else if (vi[2] + 1 == vi[1]) { if (vi[3] != vi[2] + 1 || vj[3] != vj[2])OK = 0; }

			else OK = 0;
		}

		else OK = 0;

	}

	//CAZ3
	else if (vj[1] == vj[0] + 1 && vi[1] + 2 == vi[0]) {
		if (vj[1] != vj[2] || vi[1] + 1 != vi[2])OK = 0;
		else if (vj[3] != vj[2] || vi[2] + 1 != vi[3])OK = 0;
	}

	else OK = 0;

	//CAZ MATRICE IDENTICA CA LA INCEPUT
	for (j = 0; j < 4; j++)
		for (i = 0; i < 4; i++) if (x[i][j] != b[i][j]) ID = 1;

	if (ID == 0) OK = 0;
	
	return OK;
}

void ClearElem(int tur)
{
	if (tur == 1)
	{
		Atribab();
		for (int i = 0; i < SQUARE_NUM; i++)
			for (int j = 0; j < SQUARE_NUM; j++)
				if (a[i][j] == BLUE)
				{
					a[i][j] = FREE;
				}

	}
	else if (tur == 6)
	{
		Atribab();
		for (int i = 0; i < SQUARE_NUM; i++)
			for (int j = 0; j < SQUARE_NUM; j++)
				if (a[i][j] == RED)
				{
					a[i][j] = FREE;
				}

	}
	else if (tur == 5 || tur == 10) { Atribab(); }
}

int VerifColor(int tur)
{
	if (tur >= 1 && tur <= 4)
		return 2;
	else if (tur >= 6 && tur <= 9)
		return 1;
}

void BOTEASY(int& tur) {
	int i, j, k, l;
	enum cell_value cv = FREE;

	Atribab();
	if (tur == 1) cv = BLUE;
	else if (tur == 6) cv = RED;

	for (i = 0; i < SQUARE_NUM; i++)
		for (j = 0; j < SQUARE_NUM; j++)
		{
			c[i][j] = a[i][j];
			if (c[i][j] == cv) c[i][j] = FREE;
		}

	for (i = 0; i < 16; i++) {
		if (c[i % 4][i / 4] == FREE) c[i % 4][i / 4] = cv;
		else continue;
		for (j = i + 1; j < 16; j++) {
			if (c[j % 4][j / 4] == FREE) c[j % 4][j / 4] = cv;
			else continue;

			for (k = j + 1; k < 16; k++) {
				if (c[k % 4][k / 4] == FREE) c[k % 4][k / 4] = cv;
				else continue;

				for (l = k + 1; l < 16; l++) {
					if (c[l % 4][l / 4] == FREE)
					{
						c[l % 4][l / 4] = cv;
						if (tur == 1) { if (Validmove(5, c) == 1) { goto jump; } }
						else if (tur == 6) { if (Validmove(10, c) == 1) { goto jump; } }
						c[l % 4][l / 4] = FREE;
					}
					else continue;

				}
				c[k % 4][k / 4] = FREE;
			}
			c[j % 4][j / 4] = FREE;

		}
		c[i % 4][i / 4] = FREE;
	}

jump:

	if (i == 16 && j == 16 && k == 16 && l == 16) GAMEOVER = 1;
	else {
		for (i = 0; i < SQUARE_NUM; i++)
			for (j = 0; j < SQUARE_NUM; j++) if (c[i][j] == NEUTRAL) { c[i][j] = FREE; goto jump2; }

	jump2:

		int random;
		random = rand() % 16;
		bool OK = 0;
		while (OK == 0) {
			if (c[random % 4][random / 4] == FREE) { c[random % 4][random / 4] = NEUTRAL; OK = 1; }
			else random = rand() % 16;
		}

		Atribac();
		if (tur == 1) tur = 6;
		else if (tur == 6) tur = 1;
	}

	CANUNDO = 0;

}

void BOTHARD(int& tur) {
	int i, j, k, l, ifin, jfin, kfin, lfin, minmutari = 150, mut, pozneut1 = -1, pozneut2, pozneutfin = 0;
	bool OKN = 0, TIME = 0;
	enum cell_value cv = FREE;


	Atribab();
	if (tur == 1) cv = BLUE;
	else if (tur == 6) cv = RED;


	for (i = 0; i < SQUARE_NUM; i++)
		for (j = 0; j < SQUARE_NUM; j++)
		{
			c[i][j] = a[i][j];
			if (c[i][j] == cv) c[i][j] = FREE;

			if (c[i][j] == NEUTRAL) {
				if (pozneut1 == -1) pozneut1 = i + j * 4;
				else pozneut2 = i + j * 4;
			}
		}

	for (i = 0; i < 16; i++) {
		if (c[i % 4][i / 4] == FREE) c[i % 4][i / 4] = cv;
		else continue;
		for (j = i + 1; j < 16; j++) {
			if (c[j % 4][j / 4] == FREE) c[j % 4][j / 4] = cv;
			else continue;

			for (k = j + 1; k < 16; k++) {
				if (c[k % 4][k / 4] == FREE) c[k % 4][k / 4] = cv;
				else continue;

				for (l = k + 1; l < 16; l++) {
					if (c[l % 4][l / 4] == FREE)
					{

						c[l % 4][l / 4] = cv;

						if (tur == 1) {
							if (Validmove(5, c) == 1) {
								mut = CalculMutari(6, c);
								if (mut < minmutari) {
									minmutari = mut;
									ifin = i; jfin = j; kfin = k; lfin = l;
								}
							}
						}

						else if (tur == 6) {
							if (Validmove(10, c) == 1) {
								mut = CalculMutari(1, c);
								if (mut < minmutari) {
									minmutari = mut;
									ifin = i; jfin = j; kfin = k; lfin = l;
								}
							}
						}

					}
					else continue;

					c[l % 4][l / 4] = FREE;

				}
				c[k % 4][k / 4] = FREE;
			}
			c[j % 4][j / 4] = FREE;

		}
		c[i % 4][i / 4] = FREE;
	}



	if (minmutari == 150) GAMEOVER = 1;
	else {
		c[ifin % 4][ifin / 4] = c[jfin % 4][jfin / 4] = c[kfin % 4][kfin / 4] = c[lfin % 4][lfin / 4] = cv;

		pozneutfin = pozneut1;

		if (minmutari > 0) {

			c[pozneut1 % 4][pozneut1 / 4] = FREE;
			for (i = 0; i < 16 && minmutari>0; i++) {
				if (c[i % 4][i / 4] == FREE) {
					c[i % 4][i / 4] = NEUTRAL;

					if (tur == 6)mut = CalculMutari(1, c);
					else mut = CalculMutari(6, c);


					if (mut <= minmutari) { minmutari = mut; OKN = 0; pozneutfin = i; }
					c[i % 4][i / 4] = FREE;


				}
			}
			c[pozneut1 % 4][pozneut1 / 4] = NEUTRAL;


			c[pozneut2 % 4][pozneut2 / 4] = FREE;
			for (j = 0; j < 16 && minmutari>0; j++) {
				if (c[j % 4][j / 4] == FREE) {
					c[j % 4][j / 4] = NEUTRAL;

					if (tur == 6)mut = CalculMutari(1, c);
					else mut = CalculMutari(6, c);

					if (mut <= minmutari) { minmutari = mut; OKN = 1; pozneutfin = j; }
					c[j % 4][j / 4] = FREE;

				}
			}
			c[pozneut2 % 4][pozneut2 / 4] = NEUTRAL;

			if (OKN == 0) c[pozneut1 % 4][pozneut1 / 4] = FREE;
			else c[pozneut2 % 4][pozneut2 / 4] = FREE;
			c[pozneutfin % 4][pozneutfin / 4] = NEUTRAL;
		}

		Atribac();

		if (tur == 1) tur = 6;
		else if (tur == 6) tur = 1;
	}
	CANUNDO = 0;
	if (CalculMutari(tur, a) == 0) GAMEOVER = 1;
}

void RenderTable()
{
	sf::RectangleShape background(sf::Vector2f(get_table_size().first, get_table_size().second));
	sf::RectangleShape rectangle(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));
	sf::CircleShape circle(CIRCLE_RADIUS, CIRCLE_SIDES);

	background.setFillColor(sf::Color::White);
	background.setPosition(TOP_LEFT_X, TOP_LEFT_Y);
	window.draw(background);
	for (int i = 0; i < SQUARE_NUM; i++)
		for (int j = 0; j < SQUARE_NUM; j++)
		{
			pair<float, float> poz = get_position(i, j);
			if (a[i][j] == FREE)
			{
				rectangle.setFillColor(sf::Color::Black);
				rectangle.setPosition(poz.first, poz.second);
				window.draw(rectangle);
			}
			else if (a[i][j] == RED)
			{
				rectangle.setFillColor(color2);
				rectangle.setPosition(poz.first, poz.second);
				window.draw(rectangle);
			}
			else if (a[i][j] == BLUE)
			{
				rectangle.setFillColor(color1);
				rectangle.setPosition(poz.first, poz.second);
				window.draw(rectangle);
			}
			else if (a[i][j] == NEUTRAL)
			{
				rectangle.setFillColor(sf::Color::Black);
				rectangle.setPosition(poz.first, poz.second);
				circle.setPosition(poz.first, poz.second);
				circle.setFillColor(sf::Color::Yellow);
				window.draw(rectangle);
				window.draw(circle);
			}
			else if (a[i][j] == NEUTRAL_SELECTED)
			{
				rectangle.setFillColor(sf::Color::Black);
				rectangle.setPosition(poz.first, poz.second);
				circle.setPosition(poz.first, poz.second);
				circle.setFillColor(sf::Color::Green);
				window.draw(rectangle);
				window.draw(circle);
			}
		}
}

void DrawGameMenu(int EngSelected, int RoSelected)
{
	sf::IntRect Play(250, 320, 300, 100);
	sf::IntRect HowToPlay(25, 450, 350, 80);
	sf::IntRect WhatIs(425, 450, 350, 80);
	sf::IntRect Quit(250, 560, 300, 100);
	sf::IntRect Settings(700, 700, 100, 100);
	sf::RectangleShape Play2(sf::Vector2f(300, 100));
	sf::RectangleShape HowToPlay2(sf::Vector2f(350, 80));
	sf::RectangleShape WhatIs2(sf::Vector2f(350, 80));
	sf::RectangleShape Quit2(sf::Vector2f(300, 100));
	sf::RectangleShape Settings2(sf::Vector2f(100, 100));

	sf::Texture imageSource;
	sf::Sprite imageSprite;

	if (!imageSource.loadFromFile("titlu2.jpg"))
		cout << "NU";
	imageSprite.setTexture(imageSource);
	imageSprite.setScale(0.4, 0.5);
	imageSprite.setPosition(-15, 0);
	window.draw(imageSprite);

	sf::Font font;
	sf::Text text;

	if (!font.loadFromFile("font/font.otf"))
	{
		std::cout << "Error font";
		system("pause");
	}
	text.setFont(font);

	if (EngSelected == 1)
	{
		Play2.setPosition(250, 330);
		Play2.setOutlineThickness(5);
		Play2.setFillColor(sf::Color::Black);
		Play2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("PLAY");
		text.setCharacterSize(2 * CHARACTER_SIZE);
		text.setPosition(Play.left + (Play.width - text.getLocalBounds().width) / 2, 330);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Play2);
		window.draw(text);

		HowToPlay2.setPosition(25, 460);
		HowToPlay2.setOutlineThickness(5);
		HowToPlay2.setFillColor(sf::Color::Black);
		HowToPlay2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("HOW TO PLAY");
		text.setCharacterSize(4 * CHARACTER_SIZE / 3);
		text.setPosition(HowToPlay.left + (HowToPlay.width - text.getLocalBounds().width) / 2, 465);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(HowToPlay2);
		window.draw(text);

		WhatIs2.setPosition(425, 460);
		WhatIs2.setOutlineThickness(5);
		WhatIs2.setFillColor(sf::Color::Black);
		WhatIs2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setCharacterSize(6 * CHARACTER_SIZE / 5);
		text.setString("WHAT IS BONOL?");
		text.setPosition(WhatIs.left + (WhatIs.width - text.getLocalBounds().width) / 2, 465);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(WhatIs2);
		window.draw(text);

		Quit2.setPosition(250, 570);
		Quit2.setOutlineThickness(5);
		Quit2.setFillColor(sf::Color::Black);
		Quit2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("QUIT");
		text.setCharacterSize(2 * CHARACTER_SIZE);
		text.setPosition(Quit.left + (Quit.width - text.getLocalBounds().width) / 2, 570);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Quit2);
		window.draw(text);
	}

	else if (RoSelected == 1)
	{
		Play2.setPosition(250, 330);
		Play2.setOutlineThickness(5);
		Play2.setFillColor(sf::Color::Black);
		Play2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("JOACA");
		text.setCharacterSize(2 * CHARACTER_SIZE);
		text.setPosition(Play.left + (Play.width - text.getLocalBounds().width) / 2, 330);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Play2);
		window.draw(text);

		HowToPlay2.setPosition(25, 460);
		HowToPlay2.setOutlineThickness(5);
		HowToPlay2.setFillColor(sf::Color::Black);
		HowToPlay2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("CUM SE JOACA");
		text.setCharacterSize(4 * CHARACTER_SIZE / 3);
		text.setPosition(HowToPlay.left + (HowToPlay.width - text.getLocalBounds().width) / 2, 465);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(HowToPlay2);
		window.draw(text);

		WhatIs2.setPosition(425, 460);
		WhatIs2.setOutlineThickness(5);
		WhatIs2.setFillColor(sf::Color::Black);
		WhatIs2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setCharacterSize(6 * CHARACTER_SIZE / 5);
		text.setString("CE ESTE BONOL?");
		text.setPosition(WhatIs.left + (WhatIs.width - text.getLocalBounds().width) / 2, 465);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(WhatIs2);
		window.draw(text);

		Quit2.setPosition(250, 570);
		Quit2.setOutlineThickness(5);
		Quit2.setFillColor(sf::Color::Black);
		Quit2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("IESI");
		text.setCharacterSize(2 * CHARACTER_SIZE);
		text.setPosition(Quit.left + (Quit.width - text.getLocalBounds().width) / 2, 570);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Quit2);
		window.draw(text);
	}


	if (!imageSource.loadFromFile("settings.png"))
		cout << "NU";
	imageSprite.setTexture(imageSource);
	imageSprite.setScale(0.2, 0.2);
	imageSprite.setPosition(700, 700);
	window.draw(imageSprite);
}

int DrawSettings(int EngSelected, int RoSelected, int& VolumeSet)
{
	sf::IntRect English(50, 200, 300, 150);
	sf::RectangleShape English2(sf::Vector2f(300, 150));
	sf::IntRect Romanian(450, 200, 300, 150);
	sf::RectangleShape Romanian2(sf::Vector2f(300, 150));
	sf::IntRect Back(650, 730, 150, 100);
	sf::RectangleShape Back2(sf::Vector2f(200, 100));

	sf::Font font;
	sf::Text text;

	if (!font.loadFromFile("font/font.otf"))
	{
		std::cout << "Error font";
		system("pause");
	}
	text.setFont(font);

	int SPACING = 310;

	if (EngSelected == 1)
	{
		English2.setPosition(50, 200);
		English2.setOutlineThickness(5);
		English2.setFillColor(sf::Color::Black);
		English2.setOutlineColor(sf::Color{ 255, 215, 0 });
		window.draw(English2);
		Back2.setPosition(650, 730);
		Back2.setOutlineThickness(5);
		Back2.setFillColor(sf::Color::Black);
		Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("BACK");
		text.setCharacterSize(1.65 * CHARACTER_SIZE);
		text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 720);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Back2);
		window.draw(text);
		text.setString("VOLUME:");
		text.setCharacterSize(1.5 * CHARACTER_SIZE);
		text.setPosition(50, 450);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(text);
	}

	if (RoSelected == 1)
	{
		Romanian2.setPosition(450, 200);
		Romanian2.setOutlineThickness(5);
		Romanian2.setFillColor(sf::Color::Black);
		Romanian2.setOutlineColor(sf::Color{ 255, 215, 0 });
		window.draw(Romanian2);
		Back2.setPosition(650, 730);
		Back2.setOutlineThickness(5);
		Back2.setFillColor(sf::Color::Black);
		Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("INAPOI");
		text.setCharacterSize(1.3 * CHARACTER_SIZE);
		text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 730);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Back2);
		window.draw(text);
		text.setString("VOLUM:");
		text.setCharacterSize(1.5 * CHARACTER_SIZE);
		text.setPosition(50, 450);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(text);
	}

	sf::Texture imageSource;
	sf::Sprite imageSprite;

	if (!imageSource.loadFromFile("uk.png"))
		cout << "NU";
	imageSprite.setTexture(imageSource);
	imageSprite.setScale(0.25, 0.25);
	imageSprite.setPosition(50, 200);
	window.draw(imageSprite);

	if (!imageSource.loadFromFile("ro.png"))
		cout << "NU";
	imageSprite.setTexture(imageSource);
	imageSprite.setScale(0.25, 0.25);
	imageSprite.setPosition(450, 200);
	window.draw(imageSprite);

	SPACING = 310;

	for (int i = 1; i <= VolumeSet; i++)
	{
		sf::RectangleShape VolumeSquare(sf::Vector2f(35, 35));
		sf::IntRect VolumeSquareClick(SPACING, 475, 35, 35);

		VolumeSquare.setPosition(SPACING, 475);
		VolumeSquare.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(VolumeSquare);
		SPACING += 45;
		if (event.mouseButton.button == sf::Mouse::Left)
			if (VolumeSquareClick.contains(event.mouseButton.x, event.mouseButton.y) == 1)
			{
				VolumeSet = i;
				music.setVolume(i);
			}


	}
	SPACING = 310;
	for (int i = 1; i <= 10; i++)
	{
		sf::IntRect VolumeSquareClick(SPACING, 475, 35, 35);
		sf::RectangleShape VolumeSquareOutline(sf::Vector2f(35, 35));

		if (event.mouseButton.button == sf::Mouse::Left)
			if (VolumeSquareClick.contains(event.mouseButton.x, event.mouseButton.y) == 1)
				VolumeSet = i;

		VolumeSquareOutline.setPosition(SPACING, 475);
		VolumeSquareOutline.setOutlineColor(sf::Color{ 45, 41, 46 });
		VolumeSquareOutline.setOutlineThickness(5);
		VolumeSquareOutline.setFillColor(sf::Color::Transparent);
		window.draw(VolumeSquareOutline);
		SPACING += 45;
	}
	return VolumeSet;
}

int DrawGameMode(int EngSelected, int RoSelected, int PvpPressed, int PvePressed, int& player1selected)
{
	sf::IntRect Back(650, 730, 150, 100);
	sf::RectangleShape Back2(sf::Vector2f(200, 100));
	sf::IntRect Pvp(200, 630, 400, 100);
	sf::RectangleShape Pvp2(sf::Vector2f(400, 100));
	sf::IntRect Pve(200, 70, 400, 100);
	sf::RectangleShape Pve2(sf::Vector2f(400, 100));
	sf::IntRect Easy(80, 200, 300, 400);
	sf::RectangleShape Easy2(sf::Vector2f(300, 400));
	sf::IntRect Hard(420, 200, 300, 400);
	sf::RectangleShape Hard2(sf::Vector2f(300, 400));
	sf::RectangleShape Line(sf::Vector2f(300, 0));
	sf::RectangleShape Line2(sf::Vector2f(300, 0));
	sf::RectangleShape ColorBlock2(sf::Vector2f(100, 100));
	sf::IntRect Skip(405, 650, 150, 75);
	sf::IntRect Undo(255, 650, 150, 75);
	sf::Font font;
	sf::Text text;

	if (!font.loadFromFile("font/font.otf"))
	{
		std::cout << "Error font";
		system("pause");
	}
	text.setFont(font);

	Pvp2.setPosition(200, 630);
	Pvp2.setOutlineThickness(5);
	Pvp2.setFillColor(sf::Color::Black);
	Pvp2.setOutlineColor(sf::Color{ 255, 215, 0 });
	text.setString("VS. PLAYER");
	text.setCharacterSize(2 * CHARACTER_SIZE);
	text.setPosition(Pvp.left + (Pvp.width - text.getLocalBounds().width) / 2, 630);
	text.setFillColor(sf::Color{ 255, 215, 0 });
	window.draw(Pvp2);
	window.draw(text);

	Pve2.setPosition(200, 70);
	Pve2.setOutlineThickness(5);
	Pve2.setFillColor(sf::Color::Black);
	Pve2.setOutlineColor(sf::Color{ 255, 215, 0 });
	text.setString("VS. AI");
	text.setCharacterSize(2 * CHARACTER_SIZE);
	text.setPosition(Pve.left + (Pve.width - text.getLocalBounds().width) / 2, 70);
	text.setFillColor(sf::Color{ 255, 215, 0 });
	window.draw(Pve2);
	window.draw(text);

	if (EngSelected == 1)
	{
		Back2.setPosition(650, 730);
		Back2.setOutlineThickness(5);
		Back2.setFillColor(sf::Color::Black);
		Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("BACK");
		text.setCharacterSize(1.65 * CHARACTER_SIZE);
		text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 720);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Back2);
		window.draw(text);
	}

	else if (RoSelected == 1)
	{
		Back2.setPosition(650, 730);
		Back2.setOutlineThickness(5);
		Back2.setFillColor(sf::Color::Black);
		Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("INAPOI");
		text.setCharacterSize(1.3 * CHARACTER_SIZE);
		text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 730);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Back2);
		window.draw(text);
	}

	if (PvePressed == 1)
	{
		Easy2.setPosition(80, 200);
		Easy2.setOutlineThickness(5);
		Easy2.setFillColor(sf::Color::Black);
		Easy2.setOutlineColor(sf::Color{ 255, 215, 0 });
		if (EngSelected == 1) text.setString("EASY");
		else text.setString("USOR");
		text.setCharacterSize(CHARACTER_SIZE);
		text.setPosition(Easy.left + (Easy.width - text.getLocalBounds().width) / 2, 570 - text.getLocalBounds().height);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		Line.setPosition(80, 560 - text.getLocalBounds().height);
		Line.setOutlineThickness(2.5);
		Line.setFillColor(sf::Color::Black);
		Line.setOutlineColor(sf::Color{ 255, 215, 0 });
		window.draw(Easy2);
		window.draw(text);
		window.draw(Line);
		Hard2.setPosition(420, 200);
		Hard2.setOutlineThickness(5);
		Hard2.setFillColor(sf::Color::Black);
		Hard2.setOutlineColor(sf::Color{ 255, 215, 0 });
		if (EngSelected == 1)  text.setString("HARD");
		else text.setString("GREU");
		text.setCharacterSize(CHARACTER_SIZE);
		text.setPosition(Hard.left + (Hard.width - text.getLocalBounds().width) / 2, 570 - text.getLocalBounds().height);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		Line2.setPosition(420, 560 - text.getLocalBounds().height);
		Line2.setOutlineThickness(2.5);
		Line2.setFillColor(sf::Color::Black);
		Line2.setOutlineColor(sf::Color{ 255, 215, 0 });
		window.draw(Hard2);
		window.draw(text);
		window.draw(Line2);
		sf::Texture imageSource;
		if (!imageSource.loadFromFile("brain.PNG"))
			cout << "NU";
		sf::Sprite imageSprite;
		imageSprite.setTexture(imageSource);
		imageSprite.setScale(0.7, 0.7);
		imageSprite.setPosition(380, 220);
		window.draw(imageSprite);
		imageSprite.setTexture(imageSource);
		imageSprite.setScale(0.3, 0.3);
		imageSprite.setPosition(150, 300);
		window.draw(imageSprite);

		if (Easy.contains(event.mouseButton.x, event.mouseButton.y) == 1)
		{
			EASYGAME = 1;
			TIMEMODE = 0;
			color1 = sf::Color::Blue;
			color2 = sf::Color::Red;
			PvpPressed = 1;
			TimeSelected = 1;
			player1selected = 3;
		}

		if (Hard.contains(event.mouseButton.x, event.mouseButton.y) == 1)
		{
			HARDGAME = 1;
			TIMEMODE = 0;
			color1 = sf::Color::Blue;
			color2 = sf::Color::Red;
			PvpPressed = 1;
			TimeSelected = 1;
			player1selected = 3;
		}
	}

	if (PvpPressed == 1)
	{
		int SPACINGX = 200, SPACINGY = 350;
		sf::Color colors[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan };
		sf::RectangleShape Player(sf::Vector2f(400, 50));
		sf::IntRect Start(200, 235, 400, 50);

		if (TimeSelected == 1)
		{
			if (player1selected == 0)
			{
				Player.setPosition(200, 235);
				Player.setOutlineThickness(5);
				Player.setFillColor(sf::Color::Black);
				Player.setOutlineColor(sf::Color{ 255, 215, 0 });
				text.setString("CHOOSE PLAYER 1 COLOR");
				// text.setString("CULOAREA PRIMULUI JUCATOR:");
				text.setCharacterSize(0.9 * CHARACTER_SIZE);
				text.setPosition(200 + (400 - text.getLocalBounds().width) / 2, 235);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(Player);
				window.draw(text);
			}
			for (int i = 0; i < 6; i++)
			{
				ColorBlock2.setPosition(SPACINGX, SPACINGY);
				ColorBlock2.setOutlineThickness(5);
				ColorBlock2.setFillColor(colors[i]);
				ColorBlock2.setOutlineColor(sf::Color{ 255, 215, 0 });
				if (player1selected == 0)
				{
					if (event.type == sf::Event::MouseButtonPressed)
						if (event.mouseButton.button == sf::Mouse::Left)
							if (event.mouseButton.x >= SPACINGX && event.mouseButton.x <= SPACINGX + 100 && event.mouseButton.y >= SPACINGY && event.mouseButton.y <= SPACINGY + 100)
							{
								color1 = colors[i];
								colorsint1 = i;
								player1selected = 1;
								ColorSelected = 1;
							}
				}
				else if (player1selected == 1 && ColorSelected == 1)
				{
					if (event.type == sf::Event::MouseButtonPressed)
						if (event.mouseButton.x >= SPACINGX && event.mouseButton.x <= SPACINGX + 100 && event.mouseButton.y >= SPACINGY && event.mouseButton.y <= SPACINGY + 100)
						{
							color2 = colors[i];
							colorsint2 = i;
							if (colorsint1 == colorsint2)
								SAMECOLOR = 1;
							else
								player1selected = 2;
						}
					if (SAMECOLOR == 1)
					{
						text.setString("COLOR ALREADY CHOSEN");
						text.setCharacterSize(0.9 * CHARACTER_SIZE);
						text.setPosition(200 + (400 - text.getLocalBounds().width) / 2, 290);
						text.setFillColor(sf::Color::Red);
						window.draw(text);
					}
					Player.setPosition(200, 235);
					Player.setOutlineThickness(5);
					Player.setFillColor(sf::Color::Black);
					Player.setOutlineColor(sf::Color{ 255, 215, 0 });
					text.setString("CHOOSE PLAYER 2 COLOR");
					text.setCharacterSize(0.9 * CHARACTER_SIZE);
					text.setPosition(200 + (400 - text.getLocalBounds().width) / 2, 235);
					text.setFillColor(sf::Color{ 255, 215, 0 });
					window.draw(Player);
					window.draw(text);
					color2 = colors[i];
					colorsint2 = i;
				}
				if (player1selected == 2)
				{
					Player.setPosition(200, 235);
					Player.setOutlineThickness(5);
					Player.setFillColor(sf::Color::Black);
					Player.setOutlineColor(sf::Color{ 255, 215, 0 });
					text.setString("START");
					text.setCharacterSize(0.9 * CHARACTER_SIZE);
					text.setPosition(200 + (400 - text.getLocalBounds().width) / 2, 235);
					text.setFillColor(sf::Color{ 255, 215, 0 });
					window.draw(Player);
					window.draw(text);
					if (event.type == sf::Event::MouseButtonPressed)
						if (event.mouseButton.button == sf::Mouse::Left)
							if (Start.contains(event.mouseButton.x, event.mouseButton.y) == 1)
								player1selected = 3;
				}
				if (player1selected == 3)
				{
					AtribColors();

					int tur = 1;
					bool PRESSED = 0;

					sf::RectangleShape rectangle(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));
					sf::CircleShape circle(CIRCLE_RADIUS, CIRCLE_SIDES);
					pair<float, float> table_size = get_table_size();
					sf::RectangleShape background(sf::Vector2f(table_size.first, table_size.second));
					sf::View view = window.getDefaultView();

					sf::Clock clock;
					sf::Clock clockmod;
					sf::Time zerosec = sf::seconds(0);

					if (TIMEMODE == 1 && BlitzSelected == 1)
					{
						t1 = sf::seconds(30);
						t2 = sf::seconds(30);
					}
					else if (TIMEMODE == 1 && CasualSelected == 1)
					{
						t1 = sf::seconds(180);
						t2 = sf::seconds(180);
					}

					INVALID = 0;
					CANUNDO = 0;

					int x9 = -1, y9 = -1;
					while (window.isOpen())
					{
						//TIME MODE
						if (TIMEMODE == 1 && GAMEOVER == 0)
						{
							sf::Time timpul = clockmod.getElapsedTime();
							if (tur <= 5) { t1 = t1 - timpul; }
							else { t2 = t2 - timpul; }
							clockmod.restart();
							//cout << "t1:" << t1.asSeconds() << ' ' << "t2:" << t2.asSeconds() << '\n';
							if (t1.asSeconds() <= zerosec.asSeconds() || t2.asSeconds() <= zerosec.asSeconds()) {
								GAMEOVER = 1;

							}
						}


						sf::Event event;
						while (window.pollEvent(event))
						{
							if (event.type == sf::Event::Closed)
							{
								window.close();
								break;
							}



							if (event.type == sf::Event::MouseButtonPressed && ((HARDGAME == 0 && EASYGAME == 0) || ((BOTCOLOUR == 0 && tur != 1) || (BOTCOLOUR == 1 && tur != 6)) || GAMEOVER==1 ))
							{
								if (Back.contains(event.mouseButton.x, event.mouseButton.y) == 1)
								{
									EASYGAME = 0;
									HARDGAME = 0;
									GAMEOVER = 0;
									TimeSelected = 0;
									BlitzSelected = 0;
									CasualSelected = 0;

									for (int i = 0; i <= 3; i++)
										for (int j = 0; j <= 3; j++)
											a[i][j] = FREE;

									StartMenu();


								}
								if (event.mouseButton.button == sf::Mouse::Left && GAMEOVER == 0)
								{
									//SKIP
									if (Skip.contains(event.mouseButton.x, event.mouseButton.y) == 1)
									{
										if (NS_EXIST() == 0)
										{
											if (tur == 5)
											{
												tur++;
												Atribab();
											}
											else if (tur == 10)
											{
												tur = 1;
												Atribab();
											}
											PRESSED = 0;
											CANUNDO = 1;
										}
									}

									//UNDO
									else if (Undo.contains(event.mouseButton.x, event.mouseButton.y) == 1)
									{
										if (CANUNDO == 1 && NS_EXIST() == 0 && (tur == 10 || tur == 5))
										{
											UndoMove(tur);
											CANUNDO = 0;
											PRESSED = 0;
										}
										else if (CANUNDO == 1 && NS_EXIST() == 0 && (tur == 6 || tur == 1))
										{
											UndoMove(tur);
											CANUNDO = 0;
											PRESSED = 1;
										}
									}


									else if (event.mouseButton.x > TOP_LEFT_X + PADDING && event.mouseButton.x <= TOP_LEFT_X + 4 * RECT_WIDTH + 4 * PADDING && event.mouseButton.y > TOP_LEFT_Y && event.mouseButton.y <= TOP_LEFT_Y + 4 * RECT_HEIGHT + 4 * PADDING && GAMEOVER == 0)
									{
										if (PRESSED == 0)
										{
											int x0 = (event.mouseButton.x - TOP_LEFT_X - (event.mouseButton.x / RECT_WIDTH) * PADDING) / RECT_WIDTH;
											int y0 = (event.mouseButton.y - TOP_LEFT_Y - (event.mouseButton.y / RECT_HEIGHT) * PADDING) / RECT_HEIGHT;

											if (a[x0][y0] == FREE || ((tur == 1 || tur == 6) && VerifColor(tur) == a[x0][y0]))
											{
												ClearElem(tur);
												if (tur <= 4)
													a[x0][y0] = BLUE;
												else if (tur >= 6 && tur <= 9)
													a[x0][y0] = RED;
												else if (NS_EXIST() == 1)
												{
													a[x0][y0] = NEUTRAL;
													a[x9][y9] = FREE;
												}


												tur++;

												if (tur > 10)
													tur -= 10;
												if (tur % 5 == 0)
													PRESSED = 1;

											}
											if (tur == 6 || tur == 1 || tur == 5 || tur == 10) CANUNDO = 1;
											if (tur == 5 || tur == 10) { if (Validmove(tur, a) == 0) { UndoMove(tur); PRESSED = 0; CANUNDO = 0; INVALID = 1; } }
											else INVALID = 0;


										}

									}

								}

								else if (event.mouseButton.button == sf::Mouse::Left && GAMEOVER == 1)
								{
									if (Back.contains(event.mouseButton.x, event.mouseButton.y) == 1)
									{
										EASYGAME = 0;
										HARDGAME = 0;
										GAMEOVER = 0;

										for (int i = 0; i <= 3; i++)
											for (int j = 0; j <= 3; j++)
												a[i][j] = FREE;

										StartMenu();


									}
								}

								else if (event.mouseButton.button == sf::Mouse::Right)
								{
									if (NS_EXIST() == 0)
									{
										x9 = (event.mouseButton.x - TOP_LEFT_X - (event.mouseButton.x / RECT_WIDTH) * PADDING) / RECT_WIDTH;
										y9 = (event.mouseButton.y - TOP_LEFT_Y - (event.mouseButton.y / RECT_HEIGHT) * PADDING) / RECT_HEIGHT;
										if ((tur == 5 || tur == 10) && a[x9][y9] == NEUTRAL)
										{
											PRESSED = 0;
											a[x9][y9] = NEUTRAL_SELECTED;
										}
									}
								}
							}
						}

						if (tur == 1 || tur == 6) {
							if (CalculMutari(tur, a) == 0) GAMEOVER = 1;
						}

						RenderTable();

						if (EASYGAME == 1 || HARDGAME == 1) {

							if (tur == 1 && BOTCOLOUR == 0) {
								if (NEWTIME == 0) { NEWTIME = 1; clock.restart(); }
								sf::Time elapsed = clock.getElapsedTime();
								sf::Time timp = sf::seconds(0.3);


								if (elapsed.asSeconds() >= timp.asSeconds()) {
									NEWTIME = 0;
									if (EASYGAME == 1) BOTEASY(tur);
									if (HARDGAME == 1) BOTHARD(tur);

								}

							}
							if (tur == 6 && BOTCOLOUR == 1) {

								if (NEWTIME == 0) { NEWTIME = 1; clock.restart(); }
								sf::Time elapsed = clock.getElapsedTime();
								sf::Time timp = sf::seconds(0.3);


								if (elapsed.asSeconds() >= timp.asSeconds()) {
									NEWTIME = 0;
									if (EASYGAME == 1) BOTEASY(tur);
									if (HARDGAME == 1) BOTHARD(tur);


								};

							}
						}

						Turnshow(tur, window);
						window.display();
						window.clear();
					}
				}

				//PATRATE PT CULORI
				if ((i + 1) % 3 == 0)
				{
					SPACINGX = 200;
					SPACINGY += 150;
					if (i == 5)
						SPACINGX += 150;
				}
				else
					SPACINGX += 150;
				window.draw(ColorBlock2);
			}
		}

		else if (TimeSelected == 0)
		{
			sf::IntRect Blitz2(100, 220, 250, 100);
			sf::RectangleShape Blitz(sf::Vector2f(250, 100));
			Blitz.setOutlineColor(sf::Color{ 255, 215, 0 });
			Blitz.setOutlineThickness(5);
			Blitz.setFillColor(sf::Color::Black);
			Blitz.setPosition(100, 220);
			text.setString("Blitz");
			text.setCharacterSize(2 * CHARACTER_SIZE);
			text.setPosition(Blitz2.left + (Blitz2.width - text.getLocalBounds().width) / 2, 220);
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(Blitz);
			window.draw(text);

			sf::IntRect Casual2(450, 220, 250, 100);
			sf::RectangleShape Casual(sf::Vector2f(250, 100));
			Casual.setOutlineColor(sf::Color{ 255, 215, 0 });
			Casual.setOutlineThickness(5);
			Casual.setFillColor(sf::Color::Black);
			Casual.setPosition(450, 220);
			text.setString("Casual");
			text.setCharacterSize(2 * CHARACTER_SIZE);
			text.setPosition(Casual2.left + (Casual2.width - text.getLocalBounds().width) / 2, 220);
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(Casual);
			window.draw(text);

			sf::IntRect NoTime2(275, 480, 250, 100);
			sf::RectangleShape NoTime(sf::Vector2f(250, 100));
			NoTime.setOutlineColor(sf::Color{ 255, 215, 0 });
			NoTime.setOutlineThickness(5);
			NoTime.setFillColor(sf::Color::Black);
			NoTime.setPosition(275, 480);
			text.setString("Timeless");
			text.setCharacterSize(1.5 * CHARACTER_SIZE);
			text.setPosition(NoTime2.left + (NoTime2.width - text.getLocalBounds().width) / 2, 490);
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(NoTime);
			window.draw(text);


			if (Blitz2.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) == 1)
			{
				sf::RectangleShape Hover(sf::Vector2f(600, 100));
				sf::IntRect Hover2(100, 350, 600, 100);
				Hover.setPosition(100, 350);
				Hover.setFillColor(sf::Color::Black);
				Hover.setOutlineColor(sf::Color{ 255, 215, 0 });
				Hover.setOutlineThickness(5);
				window.draw(Hover);
				text.setString("For experienced players!");
				text.setCharacterSize(0.8 * CHARACTER_SIZE);
				text.setPosition(Hover2.left + (Hover2.width - text.getLocalBounds().width) / 2, 360);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
				int SPACING = text.getLocalBounds().height;
				text.setString("Each player has 30 seconds to beat their opponent.");
				text.setCharacterSize(0.6 * CHARACTER_SIZE);
				text.setPosition(Hover2.left + (Hover2.width - text.getLocalBounds().width) / 2, 350 + SPACING + 25);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
			}
			else if (Casual2.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) == 1)
			{
				sf::RectangleShape Hover(sf::Vector2f(600, 100));
				sf::IntRect Hover2(100, 350, 600, 100);
				Hover.setPosition(100, 350);
				Hover.setFillColor(sf::Color::Black);
				Hover.setOutlineColor(sf::Color{ 255, 215, 0 });
				Hover.setOutlineThickness(5);
				window.draw(Hover);
				text.setString("For a more casual experience.");
				text.setCharacterSize(0.8 * CHARACTER_SIZE);
				text.setPosition(Hover2.left + (Hover2.width - text.getLocalBounds().width) / 2, 360);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
				int SPACING = text.getLocalBounds().height;
				text.setString("Each player has 3 minutes to beat their opponent.");
				text.setCharacterSize(0.6 * CHARACTER_SIZE);
				text.setPosition(Hover2.left + (Hover2.width - text.getLocalBounds().width) / 2, 350 + SPACING + 25);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
			}
			else if (NoTime2.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) == 1)
			{
				sf::RectangleShape Hover(sf::Vector2f(600, 100));
				sf::IntRect Hover2(100, 350, 600, 100);
				Hover.setPosition(100, 350);
				Hover.setFillColor(sf::Color::Black);
				Hover.setOutlineColor(sf::Color{ 255, 215, 0 });
				Hover.setOutlineThickness(5);
				window.draw(Hover);
				text.setString("Feel free to learn the game mechanics!");
				text.setCharacterSize(0.8 * CHARACTER_SIZE);
				text.setPosition(Hover2.left + (Hover2.width - text.getLocalBounds().width) / 2, 360);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
				int SPACING = text.getLocalBounds().height;
				text.setString("There is no time limit.");
				text.setCharacterSize(0.6 * CHARACTER_SIZE);
				text.setPosition(Hover2.left + (Hover2.width - text.getLocalBounds().width) / 2, 350 + SPACING + 25);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
			}
			if (event.type == sf::Event::MouseButtonPressed && PvpPressed == 1)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (Blitz2.contains(event.mouseButton.x, event.mouseButton.y) == 1)
					{
						BlitzSelected = 1;
						TIMEMODE = 1;
						TimeSelected = 1;

					}
					else if (Casual2.contains(event.mouseButton.x, event.mouseButton.y) == 1)
					{
						CasualSelected = 1;
						TIMEMODE = 1;
						TimeSelected = 1;
					}
					else if (NoTime2.contains(event.mouseButton.x, event.mouseButton.y) == 1)
					{
						TIMEMODE = 0;
						TimeSelected = 1;
					}
				}
			}
		}
	}

	return player1selected;
}

void DrawHowToPlay(int EngSelected, int RoSelected, int NextPressed)
{
	sf::IntRect Back(600, 700, 200, 100);
	sf::RectangleShape Back2(sf::Vector2f(200, 100));
	sf::IntRect Next(650, 370, 100, 50);
	sf::RectangleShape Next2(sf::Vector2f(100, 50));
	sf::IntRect Previous(50, 370, 100, 50);
	sf::RectangleShape Previous2(sf::Vector2f(100, 50));

	sf::Font font;
	sf::Text text;

	if (!font.loadFromFile("font/font.otf"))
	{
		std::cout << "Error font";
		system("pause");
	}
	text.setFont(font);

	int i = 0, j = 0, t = 0, r = 0;
	char line2[100][1000], line4[100][1000], line5[100][1000], line[100][1000];
	int SPACING = 100;

	ifstream fin("howtoplay.txt");
	ifstream fin2("howtoplay2.txt");
	ifstream fin4("howtoplayRO.txt");
	ifstream fin5("howtoplay2RO.txt");

	while (fin.getline(line[++i], 10000));
	while (fin2.getline(line2[++j], 10000));
	while (fin4.getline(line4[++t], 10000));
	while (fin5.getline(line5[++r], 10000));

	if (NextPressed == 0)
	{
		SPACING = 100;

		if (EngSelected == 1)
		{
			for (int k = 1; k <= i; k++)
			{
				text.setString(line[k]);
				text.setCharacterSize(6 * CHARACTER_SIZE / 9);
				text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, SPACING);
				SPACING += text.getLocalBounds().height + 10;
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
			}
			Back2.setPosition(650, 730);
			Back2.setOutlineThickness(5);
			Back2.setFillColor(sf::Color::Black);
			Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
			text.setString("BACK");
			text.setCharacterSize(1.65 * CHARACTER_SIZE);
			text.setPosition(655, 720);
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(Back2);
			window.draw(text);
		}

		else if (RoSelected == 1)
		{
			for (int k = 1; k <= t; k++)
			{
				text.setString(line4[k]);
				text.setCharacterSize(6 * CHARACTER_SIZE / 9);
				text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, SPACING);
				SPACING += text.getLocalBounds().height + 10;
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
				Back2.setPosition(650, 730);
				Back2.setOutlineThickness(5);
				Back2.setFillColor(sf::Color::Black);
				Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
				text.setString("INAPOI");
				text.setCharacterSize(1.3 * CHARACTER_SIZE);
				text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2 + 25, 730);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(Back2);
				window.draw(text);
			}
		}
		Next2.setPosition(650, 370);
		Next2.setOutlineThickness(5);
		Next2.setFillColor(sf::Color::Black);
		Next2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString(">");
		text.setCharacterSize(2 * CHARACTER_SIZE);
		text.setPosition(Next.left + (Next.width - text.getLocalBounds().width) / 2, 335);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Next2);
		window.draw(text);
	}

	if (NextPressed == 1)
	{
		SPACING = 100;
		if (EngSelected == 1)
		{
			for (int k = 1; k <= j; k++)
			{
				text.setString(line2[k]);
				text.setCharacterSize(6 * CHARACTER_SIZE / 9);
				text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, SPACING);
				SPACING += text.getLocalBounds().height + 10;
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
				Back2.setPosition(650, 730);
				Back2.setOutlineThickness(5);
				Back2.setFillColor(sf::Color::Black);
				Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
				text.setString("BACK");
				text.setCharacterSize(1.65 * CHARACTER_SIZE);
				text.setPosition(655, 720);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(Back2);
				window.draw(text);
			}
		}
		else if (RoSelected == 1)
		{
			for (int k = 1; k <= r; k++)
			{
				text.setString(line5[k]);
				text.setCharacterSize(6 * CHARACTER_SIZE / 9);
				text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, SPACING);
				SPACING += text.getLocalBounds().height + 10;
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(text);
				Back2.setPosition(650, 730);
				Back2.setOutlineThickness(5);
				Back2.setFillColor(sf::Color::Black);
				Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
				text.setString("INAPOI");
				text.setCharacterSize(1.3 * CHARACTER_SIZE);
				text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2 + 25, 730);
				text.setFillColor(sf::Color{ 255, 215, 0 });
				window.draw(Back2);
				window.draw(text);
			}
		}
		Previous2.setPosition(50, 370);
		Previous2.setOutlineThickness(5);
		Previous2.setFillColor(sf::Color::Black);
		Previous2.setOutlineColor(sf::Color{ 255, 215, 0 });
		text.setString("<");
		text.setCharacterSize(2 * CHARACTER_SIZE);
		text.setPosition(Previous.left + (Previous.width - text.getLocalBounds().width) / 2, 335);
		text.setFillColor(sf::Color{ 255, 215, 0 });
		window.draw(Previous2);
		window.draw(text);

	}
}

void DrawWhatIsBonoL(int EngSelected, int RoSelected)
{
	sf::IntRect Back(650, 730, 150, 100);
	sf::RectangleShape Back2(sf::Vector2f(200, 100));

	sf::Font font;
	sf::Text text;

	if (!font.loadFromFile("font/font.otf"))
	{
		std::cout << "Error font";
		system("pause");
	}
	text.setFont(font);

	char line3[100][1000], line6[100][1000];
	int q = 0, w = 0;
	int SPACING = 100;

	ifstream fin3("WhatIs.txt");
	ifstream fin6("WhatIsRO.txt");
	while (fin3.getline(line3[++q], 10000));
	while (fin6.getline(line6[++w], 10000));

	if (EngSelected == 1)
	{
		for (int k = 1; k <= q; k++)
		{
			text.setString(line3[k]);
			text.setCharacterSize(6 * CHARACTER_SIZE / 9);
			text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, SPACING);
			SPACING += text.getLocalBounds().height + 10;
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(text);
			Back2.setPosition(650, 730);
			Back2.setOutlineThickness(5);
			Back2.setFillColor(sf::Color::Black);
			Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
			text.setString("BACK");
			text.setCharacterSize(1.65 * CHARACTER_SIZE);
			text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 720);
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(Back2);
			window.draw(text);
		}
	}
	else if (RoSelected == 1)
	{
		for (int k = 1; k <= w; k++)
		{
			text.setString(line6[k]);
			text.setCharacterSize(6 * CHARACTER_SIZE / 9);
			text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, SPACING);
			SPACING += text.getLocalBounds().height + 10;
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(text);
			Back2.setPosition(650, 730);
			Back2.setOutlineThickness(5);
			Back2.setFillColor(sf::Color::Black);
			Back2.setOutlineColor(sf::Color{ 255, 215, 0 });
			text.setString("INAPOI");
			text.setCharacterSize(1.3 * CHARACTER_SIZE);
			text.setPosition(Back.left + (Back.width - text.getLocalBounds().width) / 2, 730);
			text.setFillColor(sf::Color{ 255, 215, 0 });
			window.draw(Back2);
			window.draw(text);
		}

	}
}

void StartMenu()
{

	bool EXITED = 0;
	bool FocusSettings = 0, FocusMenu = 1, FocusGameMode = 0, FocusWhatIsBonol = 0, FocusHowToPlay = 0, FocusBotOptions = 0;

	sf::IntRect Play(250, 320, 300, 100);
	sf::IntRect HowToPlay(25, 450, 350, 80);
	sf::IntRect WhatIs(425, 450, 350, 80);
	sf::IntRect Quit(250, 560, 300, 100);
	sf::IntRect Settings(700, 700, 100, 100);
	sf::RectangleShape Play2(sf::Vector2f(300, 100));
	sf::RectangleShape HowToPlay2(sf::Vector2f(350, 80));
	sf::RectangleShape WhatIs2(sf::Vector2f(350, 80));
	sf::RectangleShape Quit2(sf::Vector2f(300, 100));
	sf::RectangleShape Settings2(sf::Vector2f(100, 100));
	sf::RectangleShape background(sf::Vector2f(800, 800));
	sf::IntRect Back(650, 730, 150, 100);
	sf::RectangleShape Back2(sf::Vector2f(200, 100));
	sf::IntRect Pvp(200, 630, 400, 100);
	sf::RectangleShape Pvp2(sf::Vector2f(400, 100));
	sf::IntRect Pve(200, 70, 400, 100);
	sf::RectangleShape Pve2(sf::Vector2f(400, 100));
	sf::IntRect Easy(80, 200, 300, 400);
	sf::RectangleShape Easy2(sf::Vector2f(300, 400));
	sf::IntRect Hard(420, 200, 300, 400);
	sf::RectangleShape Hard2(sf::Vector2f(300, 400));
	sf::RectangleShape Line(sf::Vector2f(300, 0));
	sf::RectangleShape Line2(sf::Vector2f(300, 0));
	sf::RectangleShape ColorBlock2(sf::Vector2f(100, 100));
	sf::IntRect Next(650, 370, 100, 50);
	sf::RectangleShape Next2(sf::Vector2f(100, 50));
	sf::IntRect Previous(50, 370, 100, 50);
	sf::RectangleShape Previous2(sf::Vector2f(100, 50));
	sf::IntRect English(50, 200, 300, 150);
	sf::RectangleShape English2(sf::Vector2f(300, 150));
	sf::IntRect Romanian(450, 200, 300, 150);
	sf::RectangleShape Romanian2(sf::Vector2f(300, 150));

	if (MUSICSTART == 0) {
		if (!music.openFromFile("music.ogg"))
			cout << "error music"; // error
	}

	if (MUSICSTART == 0) {
		music.setLoop(true);
		music.setVolume(50);
		music.play();
		MUSICSTART = 1;
	}

	sf::Font font;
	sf::Text text;

	if (!font.loadFromFile("font/font.otf"))
	{
		std::cout << "Error font";
		system("pause");
	}
	text.setFont(font);

	int PvePressed = 0, PvpPressed = 0, NextPressed = 0;

	int SPACING = 100;
	int done = 0, done2 = 0, done3 = 0;
	int i = 0, j = 0, q = 0, w = 0, r = 0, t = 0;
	int OldX = 450;
	int VolumeSet = 10;
	bool VolumePressed = 0;
	int player1selected = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (FocusMenu == 1)
					{
						if (Quit.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							EXITED = 1;
							window.close();
							break;
						}
						else if (Play.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							FocusGameMode = 1;
							FocusMenu = 0;
							break;
						}
						else if (HowToPlay.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							FocusHowToPlay = 1;
							FocusMenu = 0;
							break;
						}
						else if (WhatIs.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							FocusWhatIsBonol = 1;
							FocusMenu = 0;
							break;
						}
						else if (Settings.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							FocusSettings = 1;
							FocusMenu = 0;
						}
					}

					else if (FocusSettings == 1)
					{
						if (Back.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							FocusSettings = 0;
							FocusMenu = 1;
						}
						else if (English.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							EngSelected = 1;
							RoSelected = 0;
						}
						else if (Romanian.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							EngSelected = 0;
							RoSelected = 1;
						}
					}

					else if (FocusGameMode == 1)
					{
						if (Pvp.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							PvpPressed = 1;
							if (PvePressed == 1)
								PvePressed = !PvePressed;
						}
						else if (Pve.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							PvePressed = 1;
							if (PvpPressed == 1)
								PvpPressed = !PvpPressed;
						}
						else if (Back.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							FocusGameMode = 0;
							FocusMenu = 1;
						}
					}

					else if (FocusHowToPlay == 1)
					{
						if ((Back.contains(event.mouseButton.x, event.mouseButton.y) == 1) && NextPressed == 0)
						{
							FocusHowToPlay = 0;
							FocusMenu = 1;
							break;
						}
						else if (Next.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							NextPressed = 1;
							break;
						}
						if (Previous.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							NextPressed = 0;
							break;
						}
						else if ((Back.contains(event.mouseButton.x, event.mouseButton.y) == 1) && NextPressed == 1)
						{
							NextPressed = 0;
							FocusHowToPlay = 0;
							FocusMenu = 1;
							break;
						}
					}

					else if (FocusWhatIsBonol == 1)
					{
						if (Back.contains(event.mouseButton.x, event.mouseButton.y) == 1)
						{
							FocusWhatIsBonol = 0;
							FocusMenu = 1;
							break;
						}
					}
				}
			}

			sf::Texture imageSource;
			sf::Sprite imageSprite;

			if (FocusMenu == 1)
				DrawGameMenu(EngSelected, RoSelected);

			if (FocusSettings == 1)
				VolumeSet = DrawSettings(EngSelected, RoSelected, VolumeSet);

			if (FocusGameMode == 1)
				player1selected = DrawGameMode(EngSelected, RoSelected, PvpPressed, PvePressed, player1selected);

			if (FocusHowToPlay == 1)
				DrawHowToPlay(EngSelected, RoSelected, NextPressed);

			if (FocusWhatIsBonol == 1)
				DrawWhatIsBonoL(EngSelected, RoSelected);



			window.display();
			window.clear();
		}
	}
}

int main()
{
	StartMenu();
	return 0;
}