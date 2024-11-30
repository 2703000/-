#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// 矩形结构体
typedef struct
{
	int length;
	int width;
	int innerShapeRow;			// 内部形状的行位置
	int innerShapeCol;			// 内部形状的列位置
	int innerShapeRowMonster;	// 内部形状的行位置
	int innerShapeColMonster;	// 内部形状的列位置
	int innerShapeRowNewChar;	// 新字符的行位置
	int innerShapeColNewChar;	// 新字符的列位置
} Rectangle;

// 用户状态结构体
typedef struct
{
	int health;					// 用户血量
} Player;

// 怪物状态结构体
typedef struct
{
	int Mhealth;				// 怪物血量
} Monster;

// 新字符状态结构体
typedef struct
{
	int Nhealth;				// 新字符血量
} NewChar;

// 绘制矩形及内部形状
void drawRectangle(Rectangle rect, Player player, Monster monster, NewChar newChar)
{
	for (int i = 0; i < rect.length; i++)
	{
		for (int j = 0; j < rect.width; j++)
		{
			if (i == 0 || i == rect.length - 1 || j == 0 || j == rect.width - 1)
			{
				printf("界");
			}
			else if (i == rect.innerShapeRow && j == rect.innerShapeCol)
			{
				if (player.health > 0)
					printf("机");
				else
					printf("  ");	// 如果血量为0，则不显示字符
			}
			else if (i == rect.innerShapeRowMonster && j == rect.innerShapeColMonster)
			{
				if (monster.Mhealth > 0)
					printf("敌");
				else
					printf("  ");	// 如果血量为0，则不显示字符
			}
			else if (i == rect.innerShapeRowNewChar && j == rect.innerShapeColNewChar)
			{
				if (newChar.Nhealth > 0)
					printf("弹");
				else
					printf("  ");	// 如果血量为0，则不显示字符
			}
			else
			{
				printf("  ");
			}
		}
		printf("\n");
	}
}

// 获取用户输入的字符（非阻塞方式）
int getch(void)
{
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	int ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

int fight()
{
	Rectangle rect = {
		.length = 50,
		.width = 15,
		.innerShapeRow = 45,
		.innerShapeCol = 7,
		.innerShapeRowMonster = 14,
		.innerShapeColMonster = 7,
		.innerShapeRowNewChar = 0,
		.innerShapeColNewChar = 7
	};

	Player player = { 20 };
	Monster monster = { 15 };
	NewChar newChar = { 1 };

	drawRectangle(rect, player, monster, newChar);

	while (player.health > 0 && monster.Mhealth > 0 && newChar.Nhealth > 0)
	{
		int ch = getch();
		if ((ch == 'a' || ch == 'A') && rect.innerShapeCol > 1)
		{
			rect.innerShapeCol--;
		}
		else if ((ch == 'd' || ch == 'D') && rect.innerShapeCol < rect.length - 2)
		{
			rect.innerShapeCol++;
		}

		// 新字符向下移动
		if (rect.innerShapeRowNewChar < rect.length - 1)
		{
			rect.innerShapeRowNewChar++;
		}

		// 检查新字符是否到达底部
		if (rect.innerShapeRowNewChar == rect.length - 1)
		{
			newChar.Nhealth = 0;	// 扣除所有血量
		}

		printf("\033[H\033[J");	// 清屏
		drawRectangle(rect, player, monster, newChar);
	}

	if (player.health <= 0)
	{
		printf("Game Over! Your health has run out.\n");
	}
	else if (monster.Mhealth <= 0)
	{
		printf("Congratulations! You defeated the monster.\n");
	}

	return 0;
}

void printWithDelay(const char *text)
{
	printf("%s\n", text);
	usleep(50000);				// 延迟1秒
}

int main()
{
	printWithDelay("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");
	printWithDelay("MMMMMM  MMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM");
	printWithDelay("MMMMMM  MMMMMMM  MMMM  MMMMMMMMMMMMMMMMMMM");
	printWithDelay("MMMMMM  MMMMMMMMMMMM  MMMMM  MMMMM  MMMMMM");
	printWithDelay("MMMM    MMMMMMMMMMM  MMMMMMMMMMMMMMMMMMMMM");
	printWithDelay("MMMMMM  MMMMMMMMM  MMMMMMMMMMMMMMMMMMMMMMM");
	printWithDelay("MMMMMM  MMMMMMM  MMMMMMM               MMM");
	printWithDelay("MMMMMM  MMMM  MMMMMMMMMMMMMMMMMMMMMMMMMMMM");
	printWithDelay("MMMMMMLEMONADE_NINGYOUMMMMMMMMMMMMMMMMMMMM");

	printf("\033[H\033[J");
	fight();
	return 0;
}
