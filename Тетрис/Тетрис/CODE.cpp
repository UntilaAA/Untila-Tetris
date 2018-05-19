#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
#include <sstream>
#include <conio.h>
using namespace sf;
using namespace std;

const int H = 20;//высота поля
const int W = 10;//ширина поля
int field[H][W] = { 0 };//поле

struct Point//структура определяющая точку на двумерной плоскости
{int x, y;}
//переменные которые хранят в себе одну фигуру(в каждой фигуре 4 блока)
a[4],// выпадающий блок
b[4],//состояние которое не изменяется 
c[4];// необходима для того, чтобы реализовать алгоритм показывающий Next

int figures[7][4] =//массив хранящий фигуры, их расположение в прямоугольнике(рис.1)
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	0,1,2,3, // O
};

bool check()//проверка на выход из пределов поля
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= W || a[i].y >= H) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

		return 1;
}

int game()
{
	srand(time(0));//инициализация генератора случайных чисел

	RenderWindow window(VideoMode(320, 480), "Tetris");//создание окна
	window.setFramerateLimit(100);//ограничение фпс

	Texture bloks, map, go;
	bloks.loadFromFile("images/bloks.png");
	map.loadFromFile("images/map.png");
	go.loadFromFile("images/go.png");
	Sprite B(bloks), M(map), BB(bloks), GO(go);

	Font font;
	font.loadFromFile("20db.ttf");

	Text text("", font, 15);// текст Score
	Text text1("", font, 15);// текст Next
	text1.setColor(Color::Black);
	text.setColor(Color::Black);

	int p(0);//переменная счета score

	int n = rand() % 7;
	int R = rand() % 7;//необходим только для отрисовки первой фигурки

	for (int i = 0; i < 4; i++)//создает самую первую фигуру в a, инициализация переменной
	{
		a[i].x = figures[R][i] % 2;
		a[i].y = figures[R][i] / 2;
	}

	int dx = 0; bool rotate = 0; int colorNum = 1 + rand() % 7;

	float timer(0), // Таймер, считает время прошедшее с запуска программы
		delay(0.4); // Через сколько секунд будет падать блок (частота падения)

	Clock C;

	while (window.isOpen())
	{
		float time = C.getElapsedTime().asSeconds(); // Возвращает время, прошедшее с запуска часов (в секундах)
		C.restart(); // Перезапускает часы
		timer += time;

		Event event;//событие
		while (window.pollEvent(event))//пока имеются события в очереди
		{
			if (event.type == Event::Closed)//закрытие окна
				window.close();

			if (event.type == Event::KeyPressed)//нажатие клавиш
				if (event.key.code == Keyboard::Up) rotate = true;
				else if (event.key.code == Keyboard::Left) dx = -1;
				else if (event.key.code == Keyboard::Right) dx = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();//закрытие окна по нажанию на Escape
		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;//ускоряет падение

		// Движение ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (check() == 0)
		{
			for (int i = 0; i < 4; i++) { a[i] = b[i]; }
		}

		// Вращение ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (rotate)
		{
			Point p = a[1]; //центр вращения
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

		//Падение фигурок//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < 4; i++)//создает фигуру в c(Next)
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

				for (int i = 0; i < 4; i++)//создает фигуру в a, которая была показана в Next
				{
					a[i].x = c[i].x;
					a[i].y = c[i].y;
				}
			}
			timer = 0;
		}

		//Проверка на линии////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int LL(H - 1);//последняя линия(Last Line)
		for (int i = H - 1; i >= 0; i--)
		{
			int count(0);
			for (int j = 0; j < W; j++)
			{
				if (field[i][j])count++;

				field[LL][j] = field[i][j];
			}
			if (count < W) { LL--; }
		}


		dx = 0; rotate = 0; delay = 0.4; p++;

		//Рисование////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		window.clear(Color::White);
		window.draw(M);

		if (check() == 0)//проверка на конец игры
		{
			window.draw(GO);
			text.setPosition(75, 235);
			window.draw(text);
			window.display();
			if (Keyboard::isKeyPressed(Keyboard::Space))//новая игра если нажат пробел
			{
				p = 0;//переменная счета score
				 n = 0;
				for (int i = 0; i < 4; i++)//создает самую первую фигуру в a
				{
					a[i].x = 0;
					a[i].y = 0;
					b[i].x = 0;
					b[i].y = 0;
					c[i].x = 0;
					c[i].y = 0;
				}
				for (int i = 0; i < H; i++)
				{
					for (int j = 0; j < W; j++) { field[i][j] = 0; }

				}
				int dx = 0; bool rotate = 0; int colorNum = 0;

				float timer(0), // Таймер, считает время прошедшее с запуска программы
					delay(0); // Через сколько секунд будет падать блок (частота падения)
				int R = rand() % 7;
				for (int i = 0; i < 4; i++)//создает самую первую фигуру в a
				{
					a[i].x = figures[R][i] % 2;
					a[i].y = figures[R][i] / 2;
				}
				if (check() == 0)
				{
					check() == !check();
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();//Закрыть игру
		}
		else//если не конец игры, то рисуем дальше
		{
			for (int i = 0; i < 4; i++)//показывает следующую фигурку
			{
				BB.setTextureRect(IntRect(145, 0, 18, 18));//выбирает серый кубик
				BB.setPosition(c[i].x * 18, c[i].y * 18);
				BB.move(234, 50);//где будет рисоваться
				window.draw(BB);
			}

			for (int i = 0; i < H; i++)//расстановка фигур на поле(рисование фигур, которые уже упали)
			{
				for (int j = 0; j < W; j++)
				{
					if (field[i][j] == 0) continue;
					B.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
					B.setPosition(j * 18, i * 18);
					B.move(28, 31);
					window.draw(B);
				}
			}

			for (int i = 0; i < 4; i++)//рисует фигуру, каждый раз расставляя спрайт S
			{
				B.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
				B.setPosition(a[i].x * 18, a[i].y * 18);//умножаем на 18, т.к. каждый блок размера 18 на 18
				B.move(28, 31);//то, откуда будут начинаться фигуры
				window.draw(B);
			}

			//Вывод текста на экран
			ostringstream Score;
			Score << (p / 100);
			text.setString("Score: " + Score.str());
			text1.setString("Next:");
			text.setPosition(27, 0);
			text1.setPosition(225, 30);
			window.draw(text);
			window.draw(text1);
			window.display();
		}
	}
	return 0;
}

int main()
{game();}