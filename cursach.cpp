#include "stdafx.h" //подключение нужных библиотек
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
#include <clocale>

using namespace std;
using namespace System;
using namespace System::IO;


#define ENTER 13 //обозначения кнопок
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

int menu(int, char [7][95]); //объявление функций
void maxim(int, struct bs*);
void last(int, struct bs*);
void text_data(char *,char *);
void kolvo(int, struct bs *);
void alfalist(int, struct bs*,struct sp*);
struct sp *vstavka(int, struct bs*,char*,struct sp*);
void listing(int, struct bs*);
void diagram(int, struct bs*,struct sp*);

struct bs {  //шаблон структуры для исходных данных
	char name[20]; //имя
	char rarity[20]; //редкость
	char role[20]; //роль
	int damage; //урон от атаки
	int hp; //количество здоровья
	char data[11]; //дата добавления
};

struct sp { //шаблон двухстороннего списка
	char fio[20];
	int damage;
	struct sp* sled; //указатель на следующий элемент
	struct sp* pred; //указатель на предыдущий элемент
};

int main(array<System::String ^> ^args) //выводит список бойцов, меню на экран
{
	int i,n;
	int k; //количество бойцов в базе
	char dan[7][95]={ //тексты вопросов для меню
		"Количество бойцов редкости \"Сверхредкий\"                     ",
		"Какой боец был добавлен в игру последним ?                   ",
		"Какой боец имеет наибольшее количество урона от атаки ?      ",
		"Алфавитный список всех бойцов                                ",
		"Диаграмма. Процентное соотношение количесва здоровья у бойцов",
		"Первые два бойца с одинаковым уроном и разным здоровьем      ",
		"Выход                                                        "
	};
	char BlankLine[]="                                                               ";
	FILE *in, *out; //открываем файл локально
	struct bs *brawlers; //создаем структуру на осонове шаблона
	struct sp *spisok;
	setlocale(LC_CTYPE,"Russian"); //для поддержки русского языка
	Console::CursorVisible::set(false); //делаем курсор невидимым
	Console::WindowWidth = 110; //установка ширины окна
	Console::WindowHeight = 25; //установка высоты окна
	Console::BufferHeight=Console::WindowHeight; //устанавливаем ширину и высотку окна
	Console::BufferWidth=Console::WindowWidth;
	if((in=fopen("dannie.txt","r"))==NULL){ //попытка открыть файл
		printf("\nФайл не открыт!"); //выводим предупреждение, если файл не открыт
		getch();
		exit(1);
	}
	printf("Список бойцов:\n");
	fscanf(in,"%d",&k); //считываем количество строк файла
	brawlers=(struct bs*)malloc(k*sizeof(struct bs)); //выделяем память
	Console::ForegroundColor=ConsoleColor::Red; //задаем цвета для консоли
	Console::BackgroundColor=ConsoleColor::Yellow;
	Console::Clear(); //очищаем консоль
	for(i=0;i<k;i++) //считываем данные из списка
		fscanf(in,"%s%s%s%d%d%s", brawlers[i].name, brawlers[i].rarity,
			brawlers[i].role, &brawlers[i].damage, &brawlers[i].hp, brawlers[i].data);
	out=fopen("dannie.txt", "w");
	for(i=0;i<k;i++)
		fprintf(out,"%s %s %s %d %d %s \n", brawlers[i].name, brawlers[i].rarity,
			brawlers[i].role, &brawlers[i].damage, &brawlers[i].hp, brawlers[i].data);
	for(i=0;i<k;i++) //выводим данные на экран
		printf("\n%-15s %-15s %-15s %6ld %6ld %s", brawlers[i].name,
			brawlers[i].rarity, brawlers[i].role, brawlers[i].damage,
			brawlers[i].hp, brawlers[i].data);
	getch();
	while(1){ //создаем область для вопросов и окрашиваем ее в цвета
		Console::BackgroundColor=ConsoleColor::Black; //фон консоли
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Yellow; //цвет текста меню
		Console::BackgroundColor=ConsoleColor::Red; //фон текста меню
		Console::CursorLeft=10; //координаты точки откуда будем закрашивать область меню
		Console::CursorTop=4;
		printf(BlankLine); //выводим цветную строку
		for(i=0;i<7;i++){ //размещение вопросов
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf(" %s ",dan[i]);
		}
		Console::CursorLeft=10; //координаты точки, где заканчивается меню
		Console::CursorTop=12;
		printf(BlankLine);
		getch();
		n=menu(7,dan); //связываем выбор вопроса в меню с нужной функцией
		switch(n){
			case 1: kolvo(k, brawlers); break;
			case 2: last(k, brawlers); break;
			case 3: maxim(k, brawlers); break;
			case 4: alfalist(k, brawlers, spisok); break;
			case 5: diagram(k, brawlers, spisok); break;
			case 6: listing(k, brawlers); break;
			case 7: exit(0);
		}
	}
	return 0;
}

void maxim(int k, struct bs* brawler){ //функция поиска бойца с максимальным уроном
	int i=0; struct bs best; //создаём структуру для ответа
	strcpy(best.name,brawler[0].name); //заполнчем её данные первым элементом списка
	best.damage=brawler[0].damage;
	for(i=1;i<k;i++) //меняем данные если нашли больший урон
		if(brawler[i].damage>best.damage){
			strcpy(best.name,brawler[i].name);
			best.damage=brawler[i].damage;
		}
	Console::ForegroundColor=ConsoleColor::Green; //цвета для вывода ответа
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10; //место вывода ответа
	Console::CursorTop=15;
	printf("Максимальный урон %d", best.damage); //текст и ответ
	Console::CursorLeft=10; //продолжение ответа на следующей строке
	Console::CursorTop=16;
	printf("имеет боец %s", best.name);
	getch();
}
int menu(int n, char dan[7][95]) { //функция выбора из меню
	int y1=0,y2=n-1; //номер текущей кнопки и предыдущей
	char c=1;
	while (c!=ESC) { //при нажатии ESC меню закроется
		switch(c) { 
			case DOWN: y2=y1; y1++; break; //при нажатии кнопки DOWN спускаемся к кнопке ниже
			case UP: y2=y1; y1--; break; //при нажатии кнопки UP поднимаемся выше
			case ENTER: return y1+1; //при нажатии кнопки ENTER выбираем вопрос
			case HOME : y2=y1; y1=0; break; //при нажатии кнопки HOME возвращаемся к первой кнопке
			case END: y2=y1; y1=n-1; break; //при нажатии кнопки END спускаемся к нижней кнопке
		}
		if(y1>n-1){ //возвращение к первой кнопке если зашли ниже последней
			y2=n-1;y1=0;
		}
		if(y1<0){ //возвращение к последней кнопке если выше первой
			y2=0;y1=n-1;
		}
		Console::ForegroundColor=ConsoleColor::White; //цвет подсветки текущей кнопки
		Console::BackgroundColor=ConsoleColor::Green;
		Console::CursorLeft=11; //место подсветки
		Console::CursorTop=y1+5;
		printf("%s",dan[y1]); //выводим текст вопроса ещё раз
		Console::ForegroundColor=ConsoleColor::Yellow; //убираем подстветку с предыдущей кнопки
		Console::BackgroundColor=ConsoleColor::Red;
		Console::CursorLeft=11; //местот пропажи подсветки
		Console::CursorTop=y2+5;
		printf("%s",dan[y2]);
		c=getch();
	}
	exit(0);
}

//функция поиска последнего добавленного в игру бойца
void last(int k, struct bs* brawler){
	char s[17]; //дата добавления последнего
	struct bs* best=brawler;
	for(int i=1;i<k;i++) //ищем последнего добавленного
		if(strcmp(brawler[i].data,best->data)>0)
			best=&brawler[i];
	text_data(s,best->data); //переводим дату из исходного вида в понятный
	Console::ForegroundColor=ConsoleColor::Green; //цветь ответа
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10; //место ответа
	Console::CursorTop=15;
	printf("Последний боец %s был добавлен %s года", best->name, s); //вывод ответа
	Console::CursorLeft=10;
	Console::CursorTop=17;
	getch();
}

//функция преобразования даты из одного вида в другой
void text_data(char *s,char *sd){
	char s0[3],month[12][9]={"января","февраля","марта","апреля","мая",
		"июня","июля","августа","сентября","октября","ноября","декабря"}; //месяцы
	strcpy(s,sd+8); //копируем день месяца
	strcat(s," "); ////ставим пробел
	strncpy(s0,sd+5,2); s0[2]=0; //копируем номер месяца в строку s0
	strcat(s,month[atoi(s0)-1]); //переводим номер месяца в текст и добавляем в строку
	strcat(s," "); //делаем ещё пробел
	strncat(s,sd,4); //добавляем год
	return;
}

//функция поиска количества бойцов редкости "Сверхредкий"
void kolvo(int k, struct bs *brawler){
	int rez=0; //переменная-счётчик для будущего результата
	for(int i=0;i<k;i++) //проходимся по списку и считаем нужных бойцов
		if(strcmp(brawler[i].rarity,"Сверхредкий")==0)
			rez++;
	Console::ForegroundColor=ConsoleColor::Green; //цвет ответа
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10; //координаты ответа
	Console::CursorTop=15;
	printf("Всего \"Сверхредких\" бойцов: %d",rez); //вывод овтета
	getch();
}

//Функция вывода алфавитного списка и обратного алфавитного списка
void alfalist(int k, struct bs *brawler, struct sp *spisok){
	int i;
	struct sp *nt, *z; //указатели на соседние элементы списка
	Console::ForegroundColor=ConsoleColor::Yellow; //цвет списка
	Console::BackgroundColor=ConsoleColor::Red;
	Console::Clear();

	if(!spisok) //если список ещё не создан, то создаём
		for(i=0;i<k;i++) //вызываем фунцию добавления элемента в список для каждого
			spisok=vstavka(k, brawler,brawler[i].name,spisok); 

	Console::Clear();
	printf("\n Алфавитный список бойцов              Обратный список бойцов"); //заоловки
	printf("\n==========================            ========================\n");
	for(nt=spisok,z=0; nt!=0; z=nt,nt=nt->sled) //вывод алфавитного списка
		printf("\n %-15s %d",nt->fio,nt->damage);
	Console::CursorTop = 4; //место вывода обратного списка

	for(nt=z; nt!=0; nt=nt->pred){ //вывод обратного списка
		Console::CursorLeft = 40;
		printf("%s %d \n",nt->fio,nt->damage);
	}
	getch();
	return;
}

//фукнция добавления элемента в алфавитный список
struct sp* vstavka(int k, struct bs* brawler, char* fio, struct sp* spisok){
	int i;
	struct sp *nov,*nt,*z=0; //указатель на новый элемент и соседние
	for(nt=spisok; nt!=0 && strcmp(nt->fio,fio)<0; z=nt, nt=nt->sled); //ищем место, куда вставить новый элемент
	if(nt && strcmp(nt->fio,fio)==0) //если такой элемент уже есть то не добавляем
		return spisok;
	nov=(struct sp *) malloc(sizeof(struct sp)); //выделяем память под новый элемент
	strcpy(nov->fio,fio); //копируем данные в новый элемент
	nov->sled=nt; //задаём ссылки на соседние элементы
	nov->pred=z;
	nov->damage=0;
	for(i=0;i<k;i++) //ищем бойца с таким же именем и копируем его урон
		if(strcmp(brawler[i].name,fio)==0)
			nov->damage+=brawler[i].damage;
	if (!z) //если слева соседа не сущесвует, то крайний становится новым
		spisok=nov;
	else //иначе левый список получает ссылку на новый
		z->sled=nov;
	if (nt) //если правый элемент существует, т одаём ему ссылку на новый
		nt->pred=nov;
	return spisok;
}
//Функция поиска первых двух бойцов в списке с равным уроном и разным здоровьем (сложный вопрос)
void listing(int k, struct bs* brawler){
	int i,j,r=0; //счётчики
	Console::ForegroundColor=ConsoleColor::Yellow; //цвета фона и ответа
	Console::BackgroundColor=ConsoleColor::Red;
	Console::Clear();
	printf("\n\r Боец с равным уроном и разным здоровьем"); //текст ответа
	printf("\n\r=========================================\n\r");

	for(i=0; i<k; i++) { //проходимся по каждой паре бойцов и сравниваем их по показателям
		for(j=i; j<k; j++) {
			if (r == 0 && brawler[i].name != brawler[j].name && brawler[i].damage == brawler[j].damage && brawler[i].hp != brawler[j].hp) {
				printf("\n");
				printf("%s и %s имеют одинаковый урон и разное здоровье", brawler[i].name, brawler[j].name); //вывод ответа
				r = 1; //break в данном случае не поможет выйти из обоих циклов
			}
		}
		if (r == 1) break; //если мы нашли ответ то заканчиваем
	}
	getch();
}

//Функция вывода диаграммы дял сравнения урона бойцов
void diagram(int k, struct bs *brawler, struct sp *spisok)
{
	struct sp *nt; //ссылка а соедний элемент
	int len,i,NColor; //длина, номер, цвет
	int damage = 0 ;
	char str1[20];
	char str2[20];
	System::ConsoleColor Color;
	Console::ForegroundColor=ConsoleColor::White; //цвета фона и текста
	Console::BackgroundColor=ConsoleColor::Black;
	Console::Clear();
	for(i=0;i<k;i++) //считаем суммарныый урон всех бойцов
		damage = damage+brawler[i].damage;
	if(!spisok) //если алфавитного спикска ещё нет, то создаём
		for(i=0;i<k;i++)
			spisok=vstavka(k, brawler,brawler[i].name,spisok);
	Color=ConsoleColor::Black; NColor=0; //чтобы цвет у каждой полоски был свой
	for(nt=spisok,i=0; nt!=0; nt=nt->sled,i++){
		sprintf(str1,"%s",nt->fio); //строка имя бойца
		sprintf(str2,"%3.1f%%",(nt->damage*100./damage)); //его урон в соотношении с общим
		Console::ForegroundColor=ConsoleColor::Red; //цвет текста
		Console::BackgroundColor= ConsoleColor::Black;
		Console::CursorLeft=5; Console::CursorTop=i+1; //место вывода
		printf(str1); //вывод данных бойца
		Console::CursorLeft=20;
		printf("%s",str2);
		Console::BackgroundColor=++Color; NColor++; //меняем цвет
		Console::CursorLeft=30;
		for(len=0; len<nt->damage*100/damage; len++) //определяем длину полоски
			printf(" ");
		if(NColor==14){
			Color=ConsoleColor::Black; NColor=0; //если цвета кончились, возвращаемся к певрому
		}
	}
	getch();
	return ;
}
