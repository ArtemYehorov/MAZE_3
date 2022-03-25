#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <ctime>
#include <conio.h>

#pragma comment(lib, "winmm.lib")


using namespace std;

enum MazeObject { HALL, WALL, COIN, ENEMY, BORDER, GRAVE, HEALTHBOX, SPAWNENEMY };
enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, BLACK = 0, PINK = 13 };
enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, Dot = 47 };

const int WIDTH = 40; // ширина лабиринта
const int HEIGHT = 10; // высота лабиринта

int maze[HEIGHT][WIDTH] = {}; // maze - лабиринт по-английски

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

COORD infobox;

int coins = 0; // счётчик собранных монет
int health = 100; // количество очков здоровья главного героя
int Steps = 0;
int Energy = 200;
int stepsEn = 0;

void Spawn()
{
	maze[5][35] = MazeObject::ENEMY;
}

void ShowCoins()
{
	infobox.X = WIDTH + 1;
	infobox.Y = 1;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKYELLOW);
	cout << "COINS: ";
	SetConsoleTextAttribute(h, Color::YELLOW);
	cout << coins << "\n"; // 0
}

void ShowHealth()
{
	infobox.Y = 2;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKRED);
	cout << "HEALTH: ";
	SetConsoleTextAttribute(h, Color::RED);
	cout << health << " \n";

}

void ShowSteps()
{
	infobox.X = WIDTH + 1;
	infobox.Y = 3;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::WHITE);
	cout << "Steps: ";
	SetConsoleTextAttribute(h, Color::WHITE);
	cout << Steps << "\n"; // 0
}

void ShowEnergy()
{
	infobox.X = WIDTH + 1;
	infobox.Y = 4;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << "ENERGY: ";
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << Energy << "  \n"; // 0
}


int main()
{
	// enumeration (перечисление - это набор именованных целочисленных констант)
	// MazeObject - пользовательский (кастомизированный) тип данных


	// настройки шрифта консоли
	CONSOLE_FONT_INFOEX font; // https://docs.microsoft.com/en-us/windows/console/console-font-infoex
	font.cbSize = sizeof(font);
	font.dwFontSize.Y = 70;
	font.FontFamily = FF_DONTCARE;
	font.FontWeight = FW_NORMAL;
	wcscpy_s(font.FaceName, 9, L"Consolas");
	SetCurrentConsoleFontEx(h, 0, &font);

	// скрытие мигающего курсора 
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false; // спрятать курсор
	cursor.dwSize = 1; // 1...100
	SetConsoleCursorInfo(h, &cursor);

	system("title Maze");
	MoveWindow(GetConsoleWindow(), 20, 60, 1850, 900, true);
	// 20 - отступ слева от левой границы рабочего стола до левой границы окна консоли (в пикселях!)
	// 60 - отступ сверху от верхней границы рабочего стола до верхней границы окна консоли
	// 1850 - ширина окна консоли в пикселях
	// 900 - высота окна консоли
	// true - перерисовать окно после перемещения

	srand(time(0));



	// алгоритм заполнения массива
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			maze[y][x] = rand() % 4; // 4 типа объектов в игре

			if (maze[y][x] == MazeObject::ENEMY) // если в лабиринте сгенерился враг
			{
				int probability = rand() % 8; // 0...14, если выпало 0 - враг останется, останется только одна пятая часть врагов
				if (probability != 0) // убираем врага
				{
					maze[y][x] = MazeObject::HALL; // на место врага ставим коридор
				}
			}

			if (maze[y][x] == MazeObject::WALL) // если в лабиринте сгенерировалась стена
			{
				int probability = rand() % 2; // 0...1, если выпало 0 - стена останется, останется только половина стен
				if (probability != 0) // убираем стену
				{
					maze[y][x] = MazeObject::HALL; // на место стены ставим коридор
				}
			}

			if (x == 36 && y == 5)maze[y][x] = MazeObject::SPAWNENEMY;

			if (x == 25 && y == 5 || x == 3 && y == 8 || x == 38 && y == 2) maze[y][x] = MazeObject::HEALTHBOX;

			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) maze[y][x] = MazeObject::BORDER; // белая рамка

			if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2) maze[y][x] = MazeObject::HALL; // вход

			if (x == WIDTH - 1 && y == HEIGHT - 3 ||
				x == WIDTH - 2 && y == HEIGHT - 3 ||
				x == WIDTH - 3 && y == HEIGHT - 3 ||
				x == 35 && y == 5 || x == 34 && y == 5
				|| x == 14 && y == 7 || x == 13 && y == 6) maze[y][x] = MazeObject::HALL; // выход

		}
	}

	// показ лабиринта
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			switch (maze[y][x])
			{
			case MazeObject::HALL: // hall - коридор
				SetConsoleTextAttribute(h, Color::BLACK);
				cout << " ";
				break;

			case MazeObject::WALL: // wall - стена
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				cout << (char)178;
				break;

			case MazeObject::BORDER: // border - рамка
				SetConsoleTextAttribute(h, Color::WHITE);
				cout << (char)178;
				break;

			case MazeObject::COIN: // coin - монетка
				SetConsoleTextAttribute(h, Color::YELLOW);
				cout << ".";
				break;

			case MazeObject::SPAWNENEMY:
				SetConsoleTextAttribute(h, Color::RED);
				cout << "#";
				break;

			case MazeObject::HEALTHBOX:
				SetConsoleTextAttribute(h, 15 * 16 + 12);
				cout << "+";
				break;

			case MazeObject::ENEMY: // enemy - враг
				SetConsoleTextAttribute(h, Color::RED);
				cout << (char)1;
				break;
			}
		}
		cout << "\n";
	}

	/////////////////////////////////////////////////////////////////////
	COORD positionES;
	positionES.X = 36;
	positionES.Y = 5;
	SetConsoleCursorPosition(h, positionES);
	SetConsoleTextAttribute(h, Color::RED);
	cout << "#";

	// размещение главного героя (ГГ)
	COORD position; // координаты нашего персонажа
	position.X = 0;
	position.Y = 2;
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << (char)2;

	/////////////////////////////////////////////////////////////////////
	// информация по всем показателям

	ShowCoins();

	ShowHealth();

	ShowSteps();

	ShowEnergy();

	while (true)
	{
		if (_kbhit()) // если было нажатие на клавиши пользователем
		{
			int code = _getch(); // get character, получение кода нажатой клавиши
			if (code == 224) { // если это стрелка
				code = _getch(); // получить конкретный код стрелки
			}

			ShowCoins();

			ShowHealth();

			ShowSteps();

			ShowEnergy();

			// стирание персонажика в старой позиции
			SetConsoleCursorPosition(h, position);
			if (maze[position.Y][position.X] == MazeObject::GRAVE)
			{
				SetConsoleTextAttribute(h, Color::PINK);
				cout << "X";
			}
			else
			{
				SetConsoleTextAttribute(h, Color::BLACK);
				cout << " ";
			}

			if (Energy <= 0)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/fail.wav", NULL, SND_FILENAME | SND_ASYNC);
				MessageBoxA(0, "Energy = 0!", "LOSE!", 0);
				system("cls");
				exit(0);
			}

			if (code == KeyCode::ENTER)
			{
				if (Energy > 20)
				{

					if (maze[position.Y][position.X + 1] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y][position.X + 1] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X + 1;
						effects.Y = position.Y;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";

					}
					if (maze[position.Y][position.X - 1] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y][position.X - 1] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X - 1;
						effects.Y = position.Y;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";
					}
					if (maze[position.Y + 1][position.X] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y + 1][position.X] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X;
						effects.Y = position.Y + 1;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";
					}
					if (maze[position.Y - 1][position.X] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y - 1][position.X] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X;
						effects.Y = position.Y - 1;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";
					}
					if (maze[position.Y - 1][position.X - 1] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y - 1][position.X - 1] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X - 1;
						effects.Y = position.Y - 1;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";
					}
					if (maze[position.Y - 1][position.X + 1] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y - 1][position.X + 1] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X + 1;
						effects.Y = position.Y - 1;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";
					}
					if (maze[position.Y + 1][position.X + 1] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y + 1][position.X + 1] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X + 1;
						effects.Y = position.Y + 1;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";
					}
					if (maze[position.Y + 1][position.X - 1] == MazeObject::ENEMY)
					{
						PlaySound(L"D:/visualstudio/repos/MAZE_3/Udar.wav", NULL, SND_FILENAME | SND_ASYNC);
						Energy -= 10;
						maze[position.Y + 1][position.X - 1] = MazeObject::GRAVE;
						COORD effects;
						effects.X = position.X - 1;
						effects.Y = position.Y + 1;
						SetConsoleCursorPosition(h, effects);
						SetConsoleTextAttribute(h, Color::PINK);
						cout << "X";
					}
				}
			}
			else if (code == KeyCode::ESCAPE)
			{
				cout << "ESCAPE!\n";
				exit(0);
			}
			else if (code == KeyCode::SPACE)
			{
				if (Energy > 60)
				{
					PlaySound(L"D:/visualstudio/repos/MAZE_3/boom.wav", NULL, SND_FILENAME | SND_ASYNC);
					Energy -= 50;
					int radius = 3;
					for (int y = position.Y - radius; y <= position.Y + radius; y++)
					{
						for (int x = position.X - radius; x <= position.X + radius; x++)
						{
							if (x >= 0 && x <= WIDTH - 1 &&
								y >= 0 && y <= HEIGHT - 1 &&
								maze[y][x] == MazeObject::ENEMY)
							{
								maze[y][x] = MazeObject::GRAVE;
								COORD effects;
								effects.X = x;
								effects.Y = y;
								SetConsoleCursorPosition(h, effects);
								SetConsoleTextAttribute(h, Color::PINK);
								cout << "X";
							}
						}
					}
				}
			}
			else if (code == KeyCode::LEFT
				&& maze[position.Y][position.X - 1] != MazeObject::WALL
				&& maze[position.Y][position.X - 1] != MazeObject::BORDER
				&& maze[position.Y][position.X - 1] != MazeObject::SPAWNENEMY)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/shag.wav", NULL, SND_FILENAME | SND_ASYNC);
				position.X--;
				Steps++;
				Energy--;
				stepsEn++;
				if (Steps % 25 == 0) Spawn();
			}
			else if (code == KeyCode::RIGHT // если ГГ собрался пойти направо
				&& maze[position.Y][position.X + 1] != MazeObject::WALL
				&& maze[position.Y][position.X + 1] != MazeObject::BORDER
				&& maze[position.Y][position.X + 1] != MazeObject::SPAWNENEMY)
				// и при этом в лабиринте на той же строке (где ГГ) и
				// немного (на одну ячейку) правее на 1 столбец от ГГ
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/shag.wav", NULL, SND_FILENAME | SND_ASYNC);
				position.X++;
				Steps++;
				Energy--;
				stepsEn++;
				if (Steps % 25 == 0) Spawn();
			}
			else if (code == KeyCode::UP
				&& maze[position.Y - 1][position.X] != MazeObject::WALL
				&& maze[position.Y - 1][position.X] != MazeObject::BORDER
				&& maze[position.Y - 1][position.X] != MazeObject::SPAWNENEMY)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/shag.wav", NULL, SND_FILENAME | SND_ASYNC);
				position.Y--;
				Steps++;
				Energy--;
				stepsEn++;
				if (Steps % 25 == 0) Spawn();
			}
			else if (code == KeyCode::DOWN
				&& maze[position.Y + 1][position.X] != MazeObject::WALL
				&& maze[position.Y + 1][position.X] != MazeObject::BORDER
				&& maze[position.Y + 1][position.X] != MazeObject::SPAWNENEMY)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/shag.wav", NULL, SND_FILENAME | SND_ASYNC);
				position.Y++;
				Steps++;
				Energy--;
				stepsEn++;
				if (Steps % 25 == 0) Spawn();
			}

			// показ ГГ в новой позиции
			SetConsoleCursorPosition(h, position);
			SetConsoleTextAttribute(h, Color::BLUE);
			cout << (char)2;

			////////////////////////////////////////////////////////////////
			// пересечение с элементами массива
			if (position.Y == HEIGHT - 3 &&
				position.X == WIDTH - 1)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/win.wav", NULL, SND_FILENAME | SND_ASYNC);
				MessageBoxA(0, "you find exit!", "WIN!", 0);
				system("cls");
				exit(0);
				// main(); // для запуска сначала, но на другой рандомной локации
			}

			// пересечение с монетками
			// если в лабиринте по позиции ГГ (под ним) находится монетка
			if (maze[position.Y][position.X] == MazeObject::COIN)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/Monet.wav", NULL, SND_FILENAME | SND_ASYNC);
				coins++; // на одну монетку собрали больше
				Energy += 10;
				ShowCoins();
				maze[position.Y][position.X] = MazeObject::HALL; // убираем монетку из лабиринта

			}

			int EnemyLost = 0;

			for (int y = 0; y < HEIGHT; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					if (maze[y][x] == MazeObject::ENEMY)
					{
						EnemyLost++;
					}
				}
			}
			if (EnemyLost == 0)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/win.wav", NULL, SND_FILENAME | SND_ASYNC);
				int answer = MessageBoxA(0, "All ENEMY are dead!\n\nwant to play again?", "WIN!", MB_YESNO);
				system("cls");
				if (answer == IDYES)
				{
					main();
				}
				else
				{
					exit(0);
				}
			}

			int COINLOST = 0;

			for (int y = 0; y < HEIGHT; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					if (maze[y][x] == MazeObject::COIN)
					{
						COINLOST++;
					}
				}
			}
			if (COINLOST == 0)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/win.wav", NULL, SND_FILENAME | SND_ASYNC);
				int answer = MessageBoxA(0, "All coins collected!\n\nwant to play again?", "WIN!", MB_YESNO);
				system("cls");
				if (answer == IDYES)
				{
					main();
				}
				else
				{
					exit(0);
				}
			}

			if (maze[position.Y][position.X] == MazeObject::HEALTHBOX)
			{
				PlaySound(L"D:/visualstudio/repos/MAZE_3/Hill.wav", NULL, SND_FILENAME | SND_ASYNC);
				health = 100;
				ShowHealth();
				maze[position.Y][position.X] = MazeObject::HALL;
			}

			// пересечение с врагами
			// если в лабиринте по позиции ГГ (под ним) находится враг
			if (maze[position.Y][position.X] == MazeObject::ENEMY)
			{
				health -= 20;
				Energy -= 10;
				ShowHealth();
				maze[position.Y][position.X] = MazeObject::GRAVE; // убираем врага из лабиринта


				if (health <= 0)
				{
					PlaySound(L"D:/visualstudio/repos/MAZE_3/fail.wav", NULL, SND_FILENAME | SND_ASYNC);
					int answer = MessageBoxA(0, "health is over!\n\nwant to play again?", "FAIL!", MB_YESNO);
					system("cls");
					if (answer == IDYES)
					{
						main();
					}
					else
					{
						exit(0);
					}
				}
			}

		}
		else // нажатия не было, двигаем врагов
		{
			Sleep(15);
			///////////////////////////////////////////////////////////////
			// движение врагов




			COORD enemy_positions[100]; // массив который хранит координаты врагов
			int enemy_count = 0; // реальное количество врагов на данный момент после шага ГГ



			// перебор всех ячеек которые есть в лабиринте
			for (int y = 0; y < HEIGHT; y++) // перебор строк
			{
				for (int x = 0; x < WIDTH; x++) // перебор столбцов
				{
					// если очередная ячейка - это враг
					if (maze[y][x] == MazeObject::ENEMY)
					{
						enemy_positions[enemy_count].X = x;
						enemy_positions[enemy_count].Y = y;
						enemy_count++;
					}
				}
			}

			// перебор всех врагов
			for (int i = 0; i < enemy_count; i++)
			{
				int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
				if (r == LEFT &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::SPAWNENEMY)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X - 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObject::ENEMY;
				}
				else if (r == RIGHT &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::SPAWNENEMY)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X + 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObject::ENEMY;
				}

				else if (r == UP &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALL &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::SPAWNENEMY)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y - 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObject::ENEMY;
				}
				else if (r == DOWN &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALL &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::SPAWNENEMY)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y + 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObject::ENEMY;
				}
			}
		}
	}
}