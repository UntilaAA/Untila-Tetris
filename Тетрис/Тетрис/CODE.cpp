#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
#include <sstream>
#include <conio.h>
using namespace sf;
using namespace std;

const int H = 20;//высота пол€
const int W = 10;//ширина пол€
int field[H][W] = { 0 };//поле

struct Point//структура определ€юща€ точку на двумерной плоскости
{int x, y;}
//переменные которые хран€т в себе одну фигуру(в каждой фигуре 4 блока)
a[4], // состо€ние измен€ющеес€ 
b[4],//состо€ние которое не измен€етс€ 
c[4];

int figures[7][4] =//массив хран€щий фигуры, их расположение в пр€моугольнике(рис.1)
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()//проверка на выход из пределов пол€
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= W || a[i].y >= H) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

		return 1;
}

int game()
{
	setlocale(0, "rus");
srand(time(0));//инициализаци€ генератора случайных чисел

RenderWindow window(VideoMode(320, 480), "Tetris");
window.setFramerateLimit(100);

Texture bloks,map,pole;
bloks.loadFromFile("images/bloks.png");
map.loadFromFile("images/map.png");
Sprite B(bloks), M(map), BB(bloks);

Font font;//шрифт
font.loadFromFile("20db.ttf");
Text gameover("", font, 30);
Text text("", font, 15);
Text text1("", font, 15);
text1.setColor(Color::Black);
text.setColor(Color::Black);
gameover.setColor(Color::Black);
gameover.setStyle(Text::Underlined);

int p(0);//переменна€ счета score

int n = rand() % 7;
int T = rand() % 7;
for (int i = 0; i < 4; i++)//создает самую первую фигуру в a
{
	a[i].x = figures[T][i] % 2;
	a[i].y = figures[T][i] / 2;
}

int dx = 0; bool rotate = 0; int colorNum = 1 + rand() % 7;

float timer(0), // “аймер, считает врем€ прошедшее с запуска программы
delay(0.4); // „ерез сколько секунд будет падать блок (частота падени€)

Clock C;

while (window.isOpen())
{
	float time = C.getElapsedTime().asSeconds(); // ¬озвращает врем€, прошедшее с запуска часов (в секундах)
	C.restart(); // ѕерезапускает часы
	timer += time;

	Event T;//событие
	while (window.pollEvent(T))//пока имеютс€ событи€ в очереди
	{
		if (T.type == Event::Closed)//закрытие окна
			window.close();

		if (T.type == Event::KeyPressed)//нажатие клавиш
			if (T.key.code == Keyboard::Up) rotate = true;
			else if (T.key.code == Keyboard::Left) dx = -1;
			else if (T.key.code == Keyboard::Right) dx = 1;
	}

	if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;//ускор€ет падение

     // ƒвижение ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 4; i++)
	{
		b[i] = a[i];
		a[i].x += dx;
	}
	if (check() == 0)
		for (int i = 0; i<4; i++)
			a[i] = b[i];

	// ¬ращение ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (rotate)
	{
		Point p = a[1]; //центр вращени€
		for (int i = 0; i < 4; i++)
		{
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}
		if (check() == 0)
			for (int i = 0; i < 4; i++)
				a[i] = b[i];
	}

	//ѕадение фигурок//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 4; i++)//создает фигуру в c
	{
		c[i].x = figures[n][i] % 2;
		c[i].y = figures[n][i] / 2;
	}
	if (timer > delay)
	{
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].y += 1;
		}
		if (check() == 0)
		{
			for (int i = 0; i < 4; i++)
				field[b[i].y][b[i].x] = colorNum;

			colorNum = 1 + rand() % 7;
			n = rand() % 7;

			for (int i = 0; i < 4; i++)//создает фигуру в a
			{
				a[i].x = c[i].x;
				a[i].y = c[i].y;
			}

		}

		timer = 0;
	}

	//ѕроверка на линии////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int LL(H - 1);//последн€€ лини€(Last Line)
	for (int i = H - 1; i > 0; i--)
	{
		int count(0);
		for (int j = 0; j < W; j++)
		{
			if (field[i][j])count++;

			field[LL][j] = field[i][j];
		}
		if (count < W) LL--;


	}

	dx = 0; rotate = 0; delay = 0.4; p++;

	//–исование////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	window.clear(Color::White);
	window.draw(M);

	for (int i = 0; i < 4; i++)//показывает следующую фигурку
	{
		BB.setTextureRect(IntRect(145, 0, 18, 18));
		BB.setPosition(c[i].x * 18, c[i].y * 18);
		BB.move(234, 50);
		window.draw(BB);

	}

	for (int i = 0; i < H; i++)//расстановка фигур на поле(рисование фигур, которые уже упали)
		for (int j = 0; j < W; j++)
		{
			if (field[i][j] == 0) continue;
			B.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
			B.setPosition(j * 18, i * 18);
			B.move(28, 31);
			window.draw(B);
		}

	if (check() == 0)//конец игры
	{
		gameover.setString("GAME OVER");
		gameover.setPosition(26, 220);
		window.draw(text);
		window.draw(gameover);
		window.display();
		system("pause");
	}

	for (int i = 0; i < 4; i++)//рисует фигуру, каждый раз расставл€€ спрайт S
	{

		B.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
		B.setPosition(a[i].x * 18, a[i].y * 18);//умножаем на 18, т.к. каждый блок размера 18 на 18
		B.move(28, 31);//то, откуда будут начинатьс€ фигуры
		window.draw(B);

	}

	//¬ывод текста на экран
	ostringstream Score;
	Score << (p / 100);
	text.setString("Score: " + Score.str());
	text1.setString("Next:");
	text.setPosition(27, 0);
	text1.setPosition(225,30);
	window.draw(text);
	window.draw(text1);
	window.display();
}
return 0;
}
int main()
{
	game();
}