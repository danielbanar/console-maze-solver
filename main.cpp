#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <thread>
#include "math.h"

#define MAZE_SIZE 16
#define WALL INT_MAX
#define MAZE_FINISH INT_MAX-1
#define MAZE_START INT_MAX-2

#define IS_PATH(_VEC)          (maze[_VEC] < INT_MAX - 2)

int Coords::mazeSize = MAZE_SIZE;
//Maps
int maze[MAZE_SIZE * MAZE_SIZE];



//NOTE: Start must be on bottom line and finish on top line
const char* mazeStart = 
{
"#######F########"
"# ##### ###### #"
"# #  ## ## # # #"
"# #     ## #   #"
"# # ### ## # # #"
"# # ### ## # # #"
"# # ### ## # # #"
"# # ###      # #"
"# #     # ## # #"
"# # ###      # #"
"# #    ####### #"
"# ####         #"
"# #    ####### #"
"# # ####       #"
"#        #######"
"####S##########" 
};
std::vector<Coords> junctions;
using namespace std::literals::chrono_literals;


int main()
{
	//Initialize console
	char screenBuffer[MAZE_SIZE*MAZE_SIZE];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
	SetConsoleActiveScreenBuffer(hConsole);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 32;
	cfi.dwFontSize.Y = 32;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(hConsole, false, &cfi);
	DWORD dwBytesWritten = 0;
	//Init end
	bool solved = false;
	//Convert characters to int
	for (unsigned int i = 0; i < strlen(mazeStart); i++)
	{
		if (mazeStart[i] == ' ') //Empty block
			maze[i] = 0;
		else if (mazeStart[i] == '#') //Wall
			maze[i] = WALL;
		else if (mazeStart[i] == 'S') //Start
			maze[i] = MAZE_START;
		else if (mazeStart[i] == 'F') //Start
			maze[i] = MAZE_FINISH;
	}
	Coords startCoords, curCoords, finishCoords;
	for (unsigned int i = 0; i < strlen(mazeStart); i++)
	{
		if (maze[i] == MAZE_START)
		{
			startCoords = curCoords = UpCoord(i);
			maze[GetIndex(curCoords)] = 1;
			junctions.push_back(startCoords);
		}
		if (maze[i] == MAZE_FINISH)
		{
			finishCoords = DownCoord(i);
			junctions.push_back(finishCoords);
		}
	}

	while (!solved)
	{
		solved = true;
		for (unsigned int i = 0; i < strlen(mazeStart); i++)
		{
			if (!maze[i])
			{
				solved = false;
			}
		}
		if (!solved)
		{
			int paths = 0;
			int left = maze[LeftIndex(curCoords)], right = maze[RightIndex(curCoords)], up = maze[UpIndex(curCoords)], down = maze[DownIndex(curCoords)], current = maze[GetIndex(curCoords)];
			IsPath(right) ? paths++ : 0;
			IsPath(up) ? paths++ : 0;
			IsPath(left) ? paths++ : 0;
			IsPath(down) ? paths++ : 0;
			if (paths >= 3)
			{
				bool exists = false;
				for (Coords& it : junctions)
					if (it == curCoords)
						exists = true;
				if (!exists)
					junctions.push_back(curCoords);
			}
			if (IsPath(right) && IsClaimable(current, right))
			{
				maze[RightIndex(curCoords)] = ++current;
				curCoords = RightCoord(curCoords);
			}
			else if (IsPath(up) && IsClaimable(current, up))
			{
				maze[UpIndex(curCoords)] = ++current;
				curCoords = UpCoord(curCoords);
			}
			else if (IsPath(left) && IsClaimable(current, left))
			{
				maze[LeftIndex(curCoords)] = ++current;
				curCoords = LeftCoord(curCoords);
			}
			else if (IsPath(down) && IsClaimable(current, down))
			{
				maze[DownIndex(curCoords)] = ++current;
				curCoords = DownCoord(curCoords);
			}
			else
			{
				//Jump to junction with free or high
				curCoords = GetJunction(junctions, maze);
				if (curCoords == Coords(0, 0))
					MessageBoxA(0, solved ? "yaah" : "naah", 0, 0x00000000L);
			}
		}
	}
	//Convert int to characters
	for (unsigned int i = 0; i < strlen(screenBuffer); i++)
	{
		if (maze[i] == 0) //Empty block
			screenBuffer[i] = ' ';
		else if (maze[i] == WALL) //Wall
			screenBuffer[i] = 219;
		else if (maze[i] == MAZE_START) //Start
			screenBuffer[i] = 'S';
		else if (maze[i] == MAZE_FINISH) //Start
			screenBuffer[i] = 'F';
		else
			screenBuffer[i] = '.';
	}
	curCoords = finishCoords;
	screenBuffer[GetIndex(curCoords)] = '#';
	while (maze[GetIndex(curCoords)] != 1)
	{
		curCoords = GetLower(maze, curCoords);
		screenBuffer[GetIndex(curCoords)] = '#';
	}
	WriteConsoleOutputCharacterA(hConsole, screenBuffer, MAZE_SIZE*MAZE_SIZE, {0 ,0}, &dwBytesWritten);
	std::this_thread::sleep_for(60s);
}