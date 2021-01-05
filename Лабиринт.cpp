#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
using namespace std;

int main();
void write(SHORT x, SHORT y, char model)
{
	HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE); //Дескриптор устройства стандартного вывода
	if (cons != INVALID_HANDLE_VALUE)
	{
		SetConsoleCursorPosition(cons, { y, x });
		cout <<model;

	}
}
void win()
{
	setlocale(0, "");
	system("cls");
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
void open_level(int* height, int* weight, char name[50])
{ // открытие файла с картой и замер ее высоты и длины
	char mass[500];

	FILE* F;
	(*height) = 0;
	fopen_s(&F, name, "r");
	fgets(mass, 500, F);
	(*weight) = strlen(mass) - 1;//1-начальный пробел
	fclose(F);


	fopen_s(&F, name, "r");
	while (fgetc(F) != EOF)
	{
		fgets(mass, 500, F);
		(*height)++;
	}
	fclose(F);
}
void copy_map(char** karta, int height, int weight, char name[50])

{ //копирование файла из файла в массив 
	FILE* F;
	fopen_s(&F, name, "r");


	for (int i = 0; i < (height); i++)
	{
		fgetc(F);//убирание пробела 
		karta[i] = new char[weight];//задаем столбцы
		fgets(karta[i], weight, F);
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
void move_go(char** karta, int* row, int* colomn, int dublicate_row, int dublicate_colomn) {
	karta[*row][*colomn] = ' ';
	*row = dublicate_row;
	*colomn = dublicate_colomn;
	karta[dublicate_row][dublicate_colomn] = 'X';
}
void move_flag(char** karta, int* row, int* colomn, int* vision, int* bomb, int dublicate_row, int dublicate_colomn)
{
	if (karta[dublicate_row][dublicate_colomn] == ' ') //проверка на стену и бомбы
	{
		move_go(karta, row, colomn, dublicate_row, dublicate_colomn);
	}
	else if (karta[dublicate_row][dublicate_colomn] == '|' && *bomb > 0) //проверка на стену и бомбы
	{
		(*bomb)--;
		move_go(karta, row, colomn, dublicate_row, dublicate_colomn);
	}
	else if (karta[dublicate_row][dublicate_colomn] == '+') // на одну бомбу больше
	{
		(*bomb)++;
		move_go(karta, row, colomn, dublicate_row, dublicate_colomn);
	}
	else if (karta[dublicate_row][dublicate_colomn] == '*')// видимость на 1 блок становится больше
	{
		(*vision)++;
		move_go(karta, row, colomn, dublicate_row, dublicate_colomn);
	}
	else if (karta[dublicate_row][dublicate_colomn] == 'e') //проверка на победу (символ "e")
		win(); // запуск окна "победа"
}

void start() //игра
{
	FILE* F;
	int height = 0, weight = 0;
	char name[50];
	int row = 1; //начальная позиция по строке
	int colomn = 1; //начальная позиция по столбцу
	int vision = 1;// количество видимых клеток вокруг ГГ (вокруг 'X')
	int bomb = 0;
	int flagFull = 0;//для читов
	int flagBomb = 0;//для изменения надписи бомб
	int num;
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



	open_level(&height, &weight, name);
	char** karta = new char* [height];
	copy_map(karta, height, weight, name);
	data_level(name, &row, &colomn, &vision, &bomb);
	system("cls");
	setlocale(0, "");
	system("title Лабиринт");
	while (true)
	{
		if (bomb != 0)
			flagBomb = 1;
		
		if (flagFull == 1) //поднимается по кнопке
		{
			flagFull = 2;
			for (int i = 0; i < height; i++)//вывод всего поля
			{
				for (int b = 0; b < weight - 1; b++)
				{
					write(i, b, karta[i][b]);
					//cout << karta[i][b] << " ";
				}
				cout << "\n";
			}
			cout << "\n\n\n";
		}
		for (int i = row - vision; i < row + vision + 1; i++)// вывод ограниченного поля
		{
			cout << "\n";
			for (int b = colomn - vision; b < colomn + vision + 1; b++) //+1 из-за того, что знак < 
			{
				if (i >= 0 && i < weight - 1 && b >= 0 && b < height) //отображение только символов в игровом поле -1, так как кол-во строк на 1 больше
				{
					write(i, b, karta[i][b]);
					//cout << karta[i][b] << "  ";// "   "--- расстояние между клетками 
				}
			}
			cout << "\n";
		}
		
		for (int i = 0; i < height; i++)//вывод всего поля
		{
			for (int b = 0; b < weight - 1; b++)
			{
				if ((i<row - vision || i > row + vision || b < colomn - vision || b > colomn + vision) && flagFull==0) //отображение только символов в игровом поле -1, так как кол-во строк на 1 больше
				{
					if(karta[i][b]!=' ')
					write(i, b, ' ');
				}
			}
		}

		num = _getch();
		int dublicate_row = row;
		int dublicate_colomn = colomn;
		switch (num)
		{
		case 72:		//вверх
			dublicate_row = row - 1;
			move_flag(karta, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn);
			break;

		case 80:					//вниз
			dublicate_row = row + 1;
			move_flag(karta, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn);
			break;

		case 77:					//вправо
			dublicate_colomn = colomn + 1;
			move_flag(karta, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn);
			break;

		case 75:					//лево
			dublicate_colomn = colomn - 1;
			move_flag(karta, &row, &colomn, &vision, &bomb, dublicate_row, dublicate_colomn);
			break;

		case 48:
			if (flagFull == 0)
				flagFull = 1;
			else
			{
				flagFull = 0;
				system("cls");
			}
				break;

		case 27: //backspace
			cout << "\nВыход из игры " << endl;
			exit(0);
		}
		if (flagBomb != 0)
			cout << "\n\t\t\t\t" << "количество доступных бомб: " << bomb << endl;
		else if(flagBomb == 0)
			;

	}
}

int main()
{
	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = false;
	CCI.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CCI);//удаление курсора

	setlocale(0, "");//корректное отображение точечных шрифтов
	system("cls"); //кроме русского, еще необходима для очистки, при переходе из других меню
	system("title Лабиринт");
	cout << "\n\n  Игра представляет собой лабиринт, из которого нужно выбраться.";
	cout << "\n\n  Управление в игре осуществляется с помощью стрелок.";
	cout << "\n  На уровне можно найти : \n  e - Выход из лабиринта\n  + -В инвентаре появляется бомба.";
	cout << "С помощью нее можно уничтожить любую стену.\n  * - Увеличивает видимость вокруг героя.";
	cout << "\n\n  Нажмите <enter> для начала игры или <ecsape> для выхода из игры";
	int num;
	while (true)
	{
		num = _getch();
		if (num == 13) //enter
		{
			start(); //запуск игры

		}
		if (num == 27) //backspace
		{
			cout << "\nВыход из игры " << endl;
			exit(0);
		}
	}
}
