#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
#include <sstream>
using namespace sf;
using namespace std;

const int H = 20;//высота поля
const int W = 10;//ширина поля
int field[H][W] = { 0 };//поле

struct Point//структура определяющая точку на двумерной плоскости
{int x, y;}
//переменные которые хранят в себе одну фигуру(в каждой фигуре 4 блока)
a[4], // состояние изменяющееся 
b[4];//состояние которое не изменяется 

int figures[7][4] =//массив хранящий фигуры, их расположение в прямоугольнике(рис.1)
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()//проверка на выход из пределов поля
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= W || a[i].y >= H) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

		return 1;
}

int main()
{
	setlocale(0, "rus");
	srand(time(0));//инициализация генератора случайных чисел

	//Убирает консоль
	/*HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);*/

	RenderWindow window(VideoMode(234, 480), "Tetris");

	Texture bloks,map;
	bloks.loadFromFile("images/bloks.png");
	map.loadFromFile("images/map.png");
	
	Sprite B(bloks), M(map);

	

	int dx = 0; 
	bool rotate = 0; 
	int colorNum = 1;

	float timer(0), // Таймер, считает время прошедшее с запуска программы
		delay(0.4); // Через сколько секунд будет падать блок (частота падения)

	Clock C;

	while (window.isOpen())
	{
		float time = C.getElapsedTime().asSeconds(); // Возвращает время, прошедшее с запуска часов (в секундах)
		C.restart(); // Перезапускает часы
		timer += time;

		Event T;//событие
		while (window.pollEvent(T))//пока имеются события в очереди
		{
			if (T.type == Event::Closed)//закрытие окна
				window.close();

			if (T.type == Event::KeyPressed)//нажатие клавиш
				if (T.key.code == Keyboard::Up) rotate = true;
				else if (T.key.code == Keyboard::Left) dx = -1;
				else if (T.key.code == Keyboard::Right) dx = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;//ускоряет падение

		// Движение ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (check()==0)
			for (int i = 0; i<4; i++)
				a[i] = b[i];

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
			if (check()==0)
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
		}

		//Падение фигурок//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}
			if (check()==0)
			{
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)//создает фигуру в a
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		//Проверка на линии////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int LL(H - 1);//последняя линия(Last Line)
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
		
		dx = 0; rotate = 0; delay = 0.3;

		//Рисование////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		window.clear(Color::White);
		window.draw(M);

		for (int i = 0; i < H; i++)//расстановка фигур на поле(рисование фигур, которые уже упали)
			for (int j = 0; j < W; j++)
			{
				if (field[i][j] == 0) continue;
				B.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				B.setPosition(j * 18, i * 18);
				B.move(28, 31); 
				window.draw(B);
			}

		for (int i = 0; i < 4; i++)//рисует фигуру, каждый раз расставляя спрайт S
		{
			
			B.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			B.setPosition(a[i].x * 18, a[i].y * 18);//умножаем на 18, т.к. каждый блок размера 18 на 18
			B.move(28, 31); //то, откуда будут начинаться фигуры
			window.draw(B);		
		}
		
		window.display();
	}
	return 0;
}