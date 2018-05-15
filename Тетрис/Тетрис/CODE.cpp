#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
#include <sstream>
#include <conio.h>
using namespace sf;
using namespace std;

const int H = 20;//������ ����
const int W = 10;//������ ����
int field[H][W] = { 0 };//����

struct Point//��������� ������������ ����� �� ��������� ���������
{int x, y;}
//���������� ������� ������ � ���� ���� ������(� ������ ������ 4 �����)
a[4], // ��������� ������������ 
b[4],//��������� ������� �� ���������� 
c[4];

int figures[7][4] =//������ �������� ������, �� ������������ � ��������������(���.1)
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()//�������� �� ����� �� �������� ����
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= W || a[i].y >= H) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

		return 1;
}

int game()
{
	setlocale(0, "rus");
srand(time(0));//������������� ���������� ��������� �����

RenderWindow window(VideoMode(320, 480), "Tetris");
window.setFramerateLimit(100);

Texture bloks,map,pole;
bloks.loadFromFile("images/bloks.png");
map.loadFromFile("images/map.png");
Sprite B(bloks), M(map), BB(bloks);

Font font;//�����
font.loadFromFile("20db.ttf");
Text gameover("", font, 30);
Text text("", font, 15);
Text text1("", font, 15);
text1.setColor(Color::Black);
text.setColor(Color::Black);
gameover.setColor(Color::Black);
gameover.setStyle(Text::Underlined);

int p(0);//���������� ����� score

int n = rand() % 7;
int T = rand() % 7;
for (int i = 0; i < 4; i++)//������� ����� ������ ������ � a
{
	a[i].x = figures[T][i] % 2;
	a[i].y = figures[T][i] / 2;
}

int dx = 0; bool rotate = 0; int colorNum = 1 + rand() % 7;

float timer(0), // ������, ������� ����� ��������� � ������� ���������
delay(0.4); // ����� ������� ������ ����� ������ ���� (������� �������)

Clock C;

while (window.isOpen())
{
	float time = C.getElapsedTime().asSeconds(); // ���������� �����, ��������� � ������� ����� (� ��������)
	C.restart(); // ������������� ����
	timer += time;

	Event T;//�������
	while (window.pollEvent(T))//���� ������� ������� � �������
	{
		if (T.type == Event::Closed)//�������� ����
			window.close();

		if (T.type == Event::KeyPressed)//������� ������
			if (T.key.code == Keyboard::Up) rotate = true;
			else if (T.key.code == Keyboard::Left) dx = -1;
			else if (T.key.code == Keyboard::Right) dx = 1;
	}

	if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;//�������� �������

     // �������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 4; i++)
	{
		b[i] = a[i];
		a[i].x += dx;
	}
	if (check() == 0)
		for (int i = 0; i<4; i++)
			a[i] = b[i];

	// �������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (rotate)
	{
		Point p = a[1]; //����� ��������
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

	//������� �������//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 4; i++)//������� ������ � c
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

			for (int i = 0; i < 4; i++)//������� ������ � a
			{
				a[i].x = c[i].x;
				a[i].y = c[i].y;
			}

		}

		timer = 0;
	}

	//�������� �� �����////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int LL(H - 1);//��������� �����(Last Line)
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

	//���������////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	window.clear(Color::White);
	window.draw(M);

	for (int i = 0; i < 4; i++)//���������� ��������� �������
	{
		BB.setTextureRect(IntRect(145, 0, 18, 18));
		BB.setPosition(c[i].x * 18, c[i].y * 18);
		BB.move(234, 50);
		window.draw(BB);

	}

	for (int i = 0; i < H; i++)//����������� ����� �� ����(��������� �����, ������� ��� �����)
		for (int j = 0; j < W; j++)
		{
			if (field[i][j] == 0) continue;
			B.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
			B.setPosition(j * 18, i * 18);
			B.move(28, 31);
			window.draw(B);
		}

	if (check() == 0)//����� ����
	{
		gameover.setString("GAME OVER");
		gameover.setPosition(26, 220);
		window.draw(text);
		window.draw(gameover);
		window.display();
		system("pause");
	}

	for (int i = 0; i < 4; i++)//������ ������, ������ ��� ���������� ������ S
	{

		B.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
		B.setPosition(a[i].x * 18, a[i].y * 18);//�������� �� 18, �.�. ������ ���� ������� 18 �� 18
		B.move(28, 31);//��, ������ ����� ���������� ������
		window.draw(B);

	}

	//����� ������ �� �����
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