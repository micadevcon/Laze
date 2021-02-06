#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
using namespace std;

int main();
void start(char** map, int weight, int height, int bomb, int vision, int row, int colomn, char modelWall, char modelBomb, char modelVision, char modelPersona, int modelPass, int modelEnd);
// функции для открытия существующего уровня
void open_level(int* height, int* weight, char *name)
{ // открытие файла с картой и замер ее высоты и длины
	FILE* F;
	do
	{
		cout << "\n введите желаемый уровень:(1-1)" << endl;
		cin >> name;
		fopen_s(&F, name, "r");
		if (F == NULL)
			cout << "Неизвестный уровень, введите существующий уровень!" << endl;
		else
			fclose(F);
	} while (F == NULL);


	char mass[5000];
	(*height) = 0;
	fopen_s(&F, name, "r");
	fgets(mass, 5000, F);
	(*weight) = strlen(mass)-1 ;//1-начальный пробел
	fclose(F);


	fopen_s(&F, name, "r");
	while (fgetc(F) != EOF)
	{
		fgets(mass, 5000, F);
		(*height)++;
	}
	fclose(F);
}
void copy_map(char** map, int height, int weight, char name[50])

{ //копирование файла из файла в массив 
	FILE* F;
	fopen_s(&F, name, "r");


	for (int i = 0; i < (height); i++)
	{
		fgetc(F);//убирание пробела 
		map[i] = new char[weight];//задаем столбцы
		fgets(map[i], weight, F);
		fgetc(F);//убирание чего-то
	}


}
void data_level(char name[50], int* row, int* colomn, int* vision, int* bomb)
{
	char bug[50];
	FILE* F;
	strcat_s(name, 50, "_data");
	fopen_s(&F, name, "r");
	if (F == NULL)
	{
		cout << "Отсуствует или поврежден файл с уровнем, нажмите любую клавишу для продолжения" << endl;
		_getch();
		main();
	}

	fscanf_s(F, "%s", bug, 50);
	fscanf_s(F, "%i", row);

	fscanf_s(F, "%s", bug, 50);
	fscanf_s(F, "%i", colomn);

	fscanf_s(F, "%s", bug, 50);
	fscanf_s(F, "%i", vision);

	fscanf_s(F, "%s", bug, 50);
	fscanf_s(F, "%i", bomb);

}
//функции для случайной генерации
void backRoom(char** map, int height, int weight, int* row, int* colomn, const char wall, const char pass)
//возвращает координаты назад на свободный проход
// за один раз может не возвратить из-за шанса, поэтому на один возврат может потребоваться несколько итераций цикла в createMap
{
	int chance = rand() % 4;
	switch (chance)   // по две клетки в одном направлении за прыжок
	{
	case 0:
		if (*colomn + 2 < weight && map[*row][*colomn + 2] == pass && map[*row][*colomn + 1] == pass)//право
		{
			*colomn = *colomn + 2;
		}


	case 1:
		if (*row + 2 < height && map[*row + 2][*colomn] == pass && map[*row + 1][*colomn] == pass)//низ
		{
			*row = *row + 2;
		}
		break;
	case 2:
		if (*colomn - 2 >= 2 && map[*row][*colomn - 2] == pass && map[*row][*colomn - 1] == pass)//влево
		{
			*colomn = *colomn - 2;
		}

	case 3:
		if (*row - 2 >= 2 && map[*row - 2][*colomn] == pass && map[*row - 1][*colomn] == pass)//вверх
		{
			*row = *row - 2;
		}
		break;
	}
}
void koordEndMap(char** map, int height, int weight, const char wall, const char pass, int* x, int* y)
{ //перемещает координаты на ближайшую стенку, которую можно разрушить
	int row;
	int colomn;

	for (int i = 2; i < height; i = i + 2)
	{
		for (int j = 2; j < weight; j = j + 2)
		{
			row = i;
			colomn = j;
			if (map[row][colomn] == pass)
			{
				if (colomn + 2 < weight - 2 && map[row][colomn + 2] == wall && map[row][colomn + 1] == wall)//право
				{
					*x = row;
					*y = colomn;
				}

				else if (row + 2 < height - 2 && map[row + 2][colomn] == wall && map[row + 1][colomn] == wall)//низ
				{
					*x = row;
					*y = colomn;
				}

				else if (colomn - 2 >= 0 && map[row][colomn - 2] == wall && map[row][colomn - 1] == wall)//влево
				{
					*x = row;
					*y = colomn;
				}

				else if (row - 2 >= 0 && map[row - 2][colomn] == wall && map[row - 1][colomn] == wall)//вверх
				{
					*x = row;
					*y = colomn;
				}
			}
		}
	}
}
bool endMap(char** map, int height, int weight, const char wall, const char pass, int x = 2, int y = 2, bool endRoom = 0)//x=2 y=2 начало проверки без ! и первого слоя стен
{ //проверка на создание проходов, можно ли добавить еще проход на карте, при endRoom=1 проверка происходит только определенных координат
	int row;
	int colomn;

	for (int i = x; i < height; i = i + 2)
	{
		for (int j = y; j < weight; j = j + 2)
		{
			row = i;
			colomn = j;
			if (map[row][colomn] == pass)
			{
				if (colomn + 2 < weight - 2 && map[row][colomn + 2] == wall && map[row][colomn + 1] == wall)//право
				{
					return 0;
				}

				else if (row + 2 < height - 2 && map[row + 2][colomn] == wall && map[row + 1][colomn] == wall)//низ
				{
					return 0;
				}

				else if (colomn - 2 >= 0 && map[row][colomn - 2] == wall && map[row][colomn - 1] == wall)//влево
				{
					return 0;
				}

				else if (row - 2 >= 0 && map[row - 2][colomn] == wall && map[row - 1][colomn] == wall)//вверх
				{
					return 0;
				}
			}
			if (endRoom)
				return 1;
		}
	}
	return 1;//если для всех элементов массива отсуствует ход	
}
void createBonusMap(char** map, int height, int weight, const int startRow, const int startColomn, const char border, const char wall, const char pass, const char exit, const char persona, const char bomb, const char vision)
{
	//.....вход(создание персонажа)
	map[startRow][startColomn] = persona;


	//.....создание выхода

	int prwall = 0;//количество проходов к потенциальному выходу
	int bestWall = 5;// наименьшее количество проходов, которое смогли найти, 5-ка должна сразу смениться при проверке 
	int coord = 5; //координата лучшего выхода

	for (int i = 2, j = weight - 3; i < height - 2; i++)
	{
		//если есть проход, увеличиваем счетчик
		if (map[i][j - 1] == pass)
			prwall++;
		if (i < height - 2 && map[i + 1][j] == pass)
			prwall++;
		if (i > 1 && map[i - 1][j] == pass)
			prwall++;

		//если проходы есть и их меньше, чем было до этого, меняем лучший выход 
		if (prwall < bestWall && prwall != 0)
		{
			coord = i;
			bestWall = prwall;
		}
		prwall = 0;
	}
	map[coord][weight - 3] = exit;//создаем выход в лучшем месте


	//.......создание бонусов(бомб и увеличение видимости)

	int bonus = (weight + height) / 10;// всего бонусов на карте

	while (bonus != 0)
	{
		int chance = rand() % 2;//бомба или видимость

		int row = (2 + rand() % ((height - 5)));//случайные координаты
		int colomn = (2 + rand() % (weight - 5));

		if (map[row][colomn] == pass)
		{
			if (chance == 1)
				map[row][colomn] = bomb;
			else
				map[row][colomn] = vision;
			bonus--;
		}
	}


}
void createMap(char** map, int height, int weight, const char border, const char wall, const char pass, const int startRow, const int startColomn)
{

	for (int i = 0; i < height; i++)
		for (int j = 0; j < weight; j++)
		{
			map[i][j] = wall;//все поле стенка, вдальнейшем будут создаваться проходы
		}

	for (int i = 0, j = 0; j < weight; j++)
		map[i][j] = border;//граница лабиринта сверху

	for (int i = 0, j = weight - 1; i < height; i++)
		map[i][j] = border;//граница лабиринта справа

	for (int i = height - 1, j = weight - 1; j > 0; j--)
		map[i][j] = border;//граница лабиринта снизу

	for (int i = height - 1, j = 0; i > 0; i--)
		map[i][j] = border;//граница лабиринта слева

	int row = startRow;//начальные координаты 
	int colomn = startColomn;
	map[row][colomn] = pass;
	//int CH = 0;
	while (!endMap(map, height, weight, wall, pass))
	{
		int chance = rand() % 4;
		switch (chance)   // по две клетки в одном направлении за прыжок
		{
		case 0:
			if (colomn + 2 < weight - 2 && map[row][colomn + 2] == wall && map[row][colomn + 1] == wall)
				// создается проход вправо на 2кл, weight-2 из-за 2-ух стенок
			{
				map[row][colomn + 1] = pass;
				map[row][colomn + 2] = pass;
				colomn = colomn + 2;
			}
			break;

		case 1:
			if (row + 2 < height - 2 && map[row + 2][colomn] == wall && map[row + 1][colomn] == wall) //низ
			{
				map[row + 1][colomn] = pass;
				map[row + 2][colomn] = pass;
				row = row + 2;
				//CH = 0;
			}
			break;
		case 2:
			if (colomn - 2 >= 2 && map[row][colomn - 2] == wall && map[row][colomn - 1] == wall)  //влево
			{
				map[row][colomn - 1] = pass;
				map[row][colomn - 2] = pass;
				colomn = colomn - 2;
				//CH = 0;
			}
			break;
		case 3:
			if (row - 2 >= 2 && map[row - 2][colomn] == wall && map[row - 1][colomn] == wall)    //вверх
			{
				map[row - 1][colomn] = pass;
				map[row - 2][colomn] = pass;
				row = row - 2;
				//CH = 0;
			}
			break;
		}

		if (endMap(map, height, weight, wall, pass, row, colomn, true))
			//если нельзя создать новый проход ,то вернуться назад по пройденному пути
		{
			koordEndMap(map, height, weight, wall, pass, &row, &colomn);
			//backRoom(map, height, weight, &row, &colomn, wall, pass); //случайно возвращает в сторону без стен
		}
		/*if (CH >= 100000)
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

			//НЕ РАБОТАЕТ Сh в коде временно
			map[row][colomn]='2';

			coutMap(map, height, weight);
		}

		CH++;*/
	}
}
void randStartCoord(int weight, int height, int bomb, int vision,  char modelWall, char modelBomb, char modelVision, char modelPersona, char modelPass, int modelEnd, char modelBorder)
{
	int startRow; //начальная позиция по строке
	int startColomn; //начальная позиция по столбцу

	srand(time(0));
	do
	{
		startRow = (2 + rand() % ((height - 5)));
		startColomn = (2 + rand() % ((weight - 5) / 2));
	} while (startRow % 2 == 1 || startColomn % 2 == 1);


	char** map = new char* [height];
	for (int i = 0; i < height; i++)
	{
		map[i] = new char[weight];//задаем столбцы
	}
	createMap(map, height, weight, modelBorder, modelWall, modelPass, startRow, startColomn); //создание карты без всех бонусов(выход=бонус)
	createBonusMap(map, height, weight, startRow, startColomn, modelBorder, modelWall, modelPass, modelEnd, modelPersona, modelBomb, modelVision);
	start(map, weight, height, bomb, vision, startRow, startColomn, modelWall, modelBomb, modelVision, modelPersona, modelPass, modelEnd);
	// нанесение бонусов на существующую карту
}
//функции, для работы самой игры
void write(SHORT x, SHORT y, char model)
{
	HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE); //Дескриптор устройства стандартного вывода
	if (cons != INVALID_HANDLE_VALUE)
	{
		SetConsoleCursorPosition(cons, { y, x });
		cout <<model;

	}
}
void writeMap(char** map, const int height, const int weight, int row, int colomn, int vision)
{
	for (int i = row - vision; i < row + vision + 1; i++ )// вывод ограниченного поля
	{
		
		for (int b = colomn - vision; b < colomn + vision + 1; b++) //+1 из-за того, что знак < 
		{
			if (i >= 0 && i < height  && b >= 0 && b < weight) //отображение только символов в игровом поле -1, так как кол-во строк на 1 больше
			{
				write(i+1, b, map[i][b]);
			}

		}
	}

	if (false)//подключить для отображения пройденного пути
	{
		for (int i = 0, koordX = 0; i < height; i++, koordX++)//вывод всего поля
		{
			for (int b = 0, koordY = 0; b < weight - 1; b++, koordY++)
			{
				if ((i<row - vision || i > row + vision || b < colomn - vision || b > colomn + vision)) //отображение только символов в игровом поле -1, так как кол-во строк на 1 больше
				{
					if (map[i][b] != ' ')
						write(koordX, koordY, ' ');
				}
			}
		}
	}
	
}
void win()
{
	system("chcp 1251&&cls");
	cout << "{\\_/}";
	cout << "\n(•-•)";
	cout << "\n You win! " << endl;
	cout << "\n  Нажмите <enter> для продолжения" << endl;
	cout << "\n        ____¶0_____";
	cout << "\n	____¶¶¶0_____";
	cout << "\n	_____¶¶¶00_____";
	cout << "\n	_____.¶¶¶000_____";
	cout << "\n	______¶¶¶¶000_____";
	cout << "\n	______.¶¶¶¶0000_____";
	cout << "\n	_______¶¶¶¶¶0000_____";
	cout << "\n	_______.¶¶¶¶¶00000_____";
	cout << "\n	________¶¶¶¶¶000000______";
	cout << "\n	________.¶¶¶¶¶0000000_______";
	cout << "\n	_________¶¶¶¶¶¶0000000________";
	cout << "\n	_________.¶¶¶¶¶¶0000000______";
	cout << "\n	__________¶¶¶¶¶¶¶¶000000_____";
	cout << "\n	___________o¶¶¶¶¶¶oooo_____";
	cout << "\n	_________________¶oooo_____";
	cout << "\n	__________________.¶ooo_____";
	cout << "\n	___________________.¶ooo_____";
	cout << "\n	____________________.¶ooo_____";
	cout << "\n	_____________________.¶ooo_____";
	cout << "\n	______________________.¶ooo______";
	cout << "\n	_____________________.¶oo__.¶oo____";
	cout << "\n	____________________.¶oo.____¶¶oo_____";
	cout << "\n	_____________________.¶oo___.¶oo_____";
	cout << "\n	_______________________.¶00000_______";
	


	int num;
	while (true) //gльзователь должен нажать enter
	{
		num = _getch();
		if (num == 13) //enter
		{
			main();
		}
	}

}	//функция вызывается при победе(подобранная 'e')
void move_go(char** map, int* row, int* colomn, int dublicate_row, int dublicate_colomn, int modelPersona,int modelPass) {
	
	map[*row][*colomn] = modelPass;
	*row = dublicate_row;
	*colomn = dublicate_colomn;
	map[dublicate_row][dublicate_colomn] = modelPersona;
}
void move_flag(char** map, int* row, int* colomn, int* vision, int* bomb, int dublicate_row, int dublicate_colomn,int modelPersona,int modelWall,int modelPass,int modelBomb,int modelVision, int modelEnd,int height)
{
	if (map[dublicate_row][dublicate_colomn] == modelPass) //проверка на стену и бомбы
	{
		move_go(map, row, colomn, dublicate_row, dublicate_colomn, modelPersona, modelPass);
	}
	else if (map[dublicate_row][dublicate_colomn] == modelWall && *bomb > 0) //проверка на стену и бомбы
	{
		(*bomb)--;
		move_go(map, row, colomn, dublicate_row, dublicate_colomn, modelPersona, modelPass);
	}
	else if (map[dublicate_row][dublicate_colomn] == modelBomb) // на одну бомбу больше
	{
		(*bomb)++;
		move_go(map, row, colomn, dublicate_row, dublicate_colomn, modelPersona, modelPass);
	}
	else if (map[dublicate_row][dublicate_colomn] == modelVision)// видимость на 1 блок становится больше
	{
		(*vision)++;
		move_go(map, row, colomn, dublicate_row, dublicate_colomn, modelPersona, modelPass);
	}
	else if (map[dublicate_row][dublicate_colomn] == modelEnd) //проверка на победу (символ "e")
	{
		for (int i = 0; i < height; i++)
		{
			delete[] map[i];
		}
		delete[] map;
		win(); // запуск окна "победа"
	}
		
}
void start(char**map, int weight,int height, int bomb,int vision,int row,int colomn,char modelWall,char modelBomb,char modelVision,char modelPersona, int modelPass, int modelEnd) //игра
{
	system("chcp 866&&cls");
	//char** map = new char* [height];
	system("cls");
	system("title Лабиринт");
	cout << "bomb: " << bomb << endl;

	int flagFull = 0;//для читов
	int num;
	int dublicateBomb = bomb;//для корректного отображения бомб на больших картах
	while (true)
	{	

		
			

		//использование читов(при нажатии на 0 показывается весь лабиринт)
		if (flagFull == 1) //поднимается по кнопке
		{
			flagFull = 2;

			for (int i = 0; i < height; i++)//вывод всего поля
			{
				for (int b = 0; b < weight ; b++)
				{
					write(i+1, b, map[i][b]);
				}
				cout << "\n";
			}
			cout << "\n\n\n";
		}
		
		//вывод ограниченного поля зрения
		writeMap(map, height, weight, row, colomn, vision);
		



		num = _getch();
		int dublicate_row = row;
		int dublicate_colomn = colomn;
		switch (num)
		{
		case 72:		//вверх
			dublicate_row = row - 1;
			move_flag(map, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn, modelPersona, modelWall, modelPass, modelBomb, modelVision, modelEnd, height);
			break;

		case 80:					//вниз
			dublicate_row = row + 1;
			move_flag(map, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn, modelPersona, modelWall, modelPass, modelBomb, modelVision, modelEnd, height);
			break;

		case 77:					//вправо
			dublicate_colomn = colomn + 1;
			move_flag(map, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn, modelPersona, modelWall, modelPass, modelBomb, modelVision, modelEnd, height);
			break;

		case 75:					//лево
			dublicate_colomn = colomn - 1;
			move_flag(map, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn, modelPersona, modelWall, modelPass, modelBomb, modelVision, modelEnd, height);
			break;

		case 48:
			if (flagFull == 0)
				flagFull = 1;
			else
			{
				flagFull = 0;
				system("cls");
				cout << "bomb: " << bomb << endl;
			}
				break;

		case 27: //backspace
			cout << "\nВыход из игры " << endl;
			exit(0);
		}
		//отображение бомб
		if (bomb != dublicateBomb)
		{
			dublicateBomb = bomb;
			if (48 + bomb > 57)
			{
				write(0, 5, 62);
				write(0, 6, 57);
			}
			else
			{
				write(0, 5, ' ');
				write(0, 6, 48 + bomb);
			}
		}
		
	}
}

int main()
{
	//удаление курсора
	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = false;
	CCI.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CCI);

	system("chcp 1251&&cls");
	system("cls"); //кроме русского, еще необходима для очистки, при переходе из других меню
	system("title Лабиринт");

	cout << "\n\n  В  свойствах консоли необходимо выставить шрифт !!lucida console!! и поставить максимальный буфер экрана";
	cout << "\n\n  Игра представляет собой лабиринт, из которого нужно выбраться.";
	cout << "\n\n  Управление в игре осуществляется с помощью стрелок.";
	cout << "\n  На уровне можно найти : \n  e - Выход из лабиринта\n  + -В инвентаре появляется бомба.";
	cout << "С помощью нее можно уничтожить любую стену.\n  * - Увеличивает видимость вокруг героя.";
	cout << "\n\n  Нажмите <enter> для начала игры или <ecsape> для выхода из игры";

	int height;
	int weight;
	int startRow; //начальная позиция по строке
	int startColomn; //начальная позиция по столбцу
	int vision;// количество видимых клеток вокруг ГГ (вокруг 'X')
	int bomb;
	const char modelPass = ' ';
	const char modelEnd = 176; 
	const char modelWall=219;
	const char modelPersona = 2;
	const char modelBomb=15;
	const char modelVision=23;
	const char modelBorder = 253;//конец уровня		

	//начальные координаты должны быть четными!
	
	while (true)
	{
		int num;
		num = _getch();
		if (num == 13) //enter
			break;
		else if (num == 27) //backspace
		{
			cout << "\nВыход из игры " << endl;
			exit(0);
		}
	}
	system("cls");
	cout << "> Открыть уже существующий уровень" << endl;
	cout << "  Сгенерировать новый уровень " << endl;
	int mode = 1;
	while (true)
	{
		//72 80
		int num;
		
		num = _getch();
		if (num == 72) //вверх
		{
			mode = 1;
			write(0, 0, '>');
			write(1, 0, ' ');
		}
		if (num == 80) //вниз
		{
			mode = 2;
			write(0, 0, ' ');
			write(1, 0, '>');
		}
			
		if (num == 13) //enter
		{
			if (mode == 1)
			{
				char name[50];
				open_level(&height, &weight, name);
				char** map = new char* [height];
				copy_map(map, height, weight, name);
				data_level(name, &startRow, &startColomn, &vision, &bomb);
				start(map,weight, height,bomb,vision,startRow,startColomn, modelWall,modelBomb,modelVision, modelPersona, modelPass, modelEnd);

			}
				
			else if (mode == 2)
			{
				system("cls");
				cout << "Выбор сложности" << endl;
				cout << "> Простой(height=11, weight=15, vision = 3, bomb = 2)" << endl;
				cout << "  Легкий (height=21, weight=31, vision = 3, bomb = 2) " << endl;
				cout << "  Средний(height=31, weight=41, vision = 2, bomb = 2)" << endl;
				cout << "  Сложный(height=51, weight=51, vision = 2, bomb = 1)" << endl;
				cout << "  Очень сложный(height=71,weight=71, vision=2, bomb=0)" << endl;
				cout << "  Настраиваемый" << endl;
				int mode = 1;
				while (true)
				{
					
					int num;
					
					num = _getch();
					if (num == 72) //вверх
						if(mode!=1)
							mode--;
					if (num == 80) //вниз
						if (mode != 6)
							mode++;
					if (num == 13) //enter
					{

						switch (mode)
						{
							
							
						case 1:
							height=11;
							weight=15;
							vision=3;// количество видимых клеток вокруг ГГ (вокруг 'X')
							bomb=2;
							randStartCoord(weight, height, bomb, vision, modelWall, modelBomb, modelVision, modelPersona, modelPass, modelEnd, modelBorder);
							break;
						case 2:
							height = 21;
							weight = 31;
							vision = 3;// количество видимых клеток вокруг ГГ (вокруг 'X')
							bomb = 2;
							randStartCoord(weight, height, bomb, vision, modelWall, modelBomb, modelVision, modelPersona, modelPass, modelEnd, modelBorder);
							break;
						case 3:
							height = 31;
							weight = 41;
							vision = 2;// количество видимых клеток вокруг ГГ (вокруг 'X')
							bomb = 2;
							randStartCoord(weight, height, bomb, vision, modelWall, modelBomb, modelVision, modelPersona, modelPass, modelEnd, modelBorder);
							break;
						case 4:
							height = 51;
							weight = 51;
							vision = 2;// количество видимых клеток вокруг ГГ (вокруг 'X')
							bomb = 1;
							randStartCoord(weight, height, bomb, vision, modelWall, modelBomb, modelVision, modelPersona, modelPass, modelEnd, modelBorder);
							break;
						case 5:
							height = 71;
							weight = 71;
							vision = 2;// количество видимых клеток вокруг ГГ (вокруг 'X')
							bomb = 0;
							randStartCoord(weight, height, bomb, vision, modelWall, modelBomb, modelVision, modelPersona, modelPass, modelEnd, modelBorder);
							break;
						case 6:
							//вызов доп меню;
							system("cls");
							cout << "weight";
							cin >> weight;

							cout << "height";
							cin >> height;

							cout << "bomb";
							cin >> bomb;

							cout << "vision";
							cin >> vision;
							randStartCoord(weight, height, bomb, vision, modelWall, modelBomb, modelVision, modelPersona, modelPass, modelEnd, modelBorder);
							break;
						}

					}
					if (num == 27) //backspace
					{
						cout << "\nВыход из игры " << endl;
						exit(0);
					}

					write(mode, 0, '>');

					if (mode != 6)
						write(mode+1, 0, ' ');
					
					if(mode!=1)
						write(mode - 1, 0, ' ');

				}
			}
		}

		if (num == 27) //backspace
		{
			cout << "\nВыход из игры " << endl;
			exit(0);
		}
	}
}
