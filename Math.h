#pragma once
#include <vector>
#include <stdlib.h>

struct Coords
{
	static int mazeSize;
	int x=0, y=0;
	Coords() {}
	Coords(const int& X, const int& Y) : x(X), y(Y) {}
	bool operator==(const Coords& other) const
	{
		return x == other.x && y == other.y;
	}
};
int GetIndex(const int& X, const int& Y)
{
	return Coords::mazeSize * Y + X;
}
int GetIndex(const Coords& vec)
{
	return Coords::mazeSize * vec.y + vec.x;
}
Coords GetPos(const int& index)
{
	return { index % Coords::mazeSize,index / Coords::mazeSize };
}

int UpIndex(const Coords& vec)
{
	return GetIndex(vec.x, vec.y - 1);
}
int UpIndex(const int& index)
{
	return UpIndex(GetPos(index));
}
Coords UpCoord(const Coords& vec)
{
	return GetPos(UpIndex(vec));
}
Coords UpCoord(const int& index)
{
	return UpCoord(GetPos(index));
}

int DownIndex(const Coords& vec)
{
	return GetIndex(vec.x, vec.y + 1);
}
int DownIndex(const int& index)
{
	return DownIndex(GetPos(index));
}
Coords DownCoord(const Coords& vec)
{
	return GetPos(DownIndex(vec));
}
Coords DownCoord(const int& index)
{
	return DownCoord(GetPos(index));
}

int LeftIndex(const Coords& vec)
{
	return GetIndex(vec.x - 1, vec.y);
}
int LeftIndex(const int& index)
{
	return LeftIndex(GetPos(index));
}
Coords LeftCoord(const Coords& vec)
{
	return GetPos(LeftIndex(vec));
}
Coords LeftCoord(const int& index)
{
	return LeftCoord(GetPos(index));
}

int RightIndex(const Coords& vec)
{
	return GetIndex(vec.x + 1, vec.y);
}
int RightIndex(const int& index)
{
	return RightIndex(GetPos(index));
}
Coords RightCoord(const Coords& vec)
{
	return GetPos(RightIndex(vec));
}
Coords RightCoord(const int& index)
{
	return RightCoord(GetPos(index));
}

bool IsPath(const int& dist)
{
	return dist < INT_MAX-2;
}
bool IsClaimable(int curDist, const int& otherDist)
{
	if (!otherDist || otherDist > ++curDist)
		return true;
	return false;
}
Coords GetJunction(const std::vector<Coords>& junctions, int* maze)
{
	for (const auto& junction : junctions)
	{
		if (!maze[GetIndex(junction)])
			continue;
		int left = maze[LeftIndex(junction)], right = maze[RightIndex(junction)], up = maze[UpIndex(junction)], down = maze[DownIndex(junction)], current = maze[GetIndex(junction)];
		if (IsPath(right) && IsClaimable(current, right))
		{
			return junction;
		}
		else if (IsPath(left) && IsClaimable(current, left))
		{
			return junction;
		}
		else if (IsPath(up) && IsClaimable(current, up))
		{
			return junction;
		}
		else if (IsPath(down) && IsClaimable(current, down))
		{
			return junction;
		}
	}
	return {0,0};
}
Coords GetLower(int* maze, const Coords& cCoords)
{
	Coords lowest = cCoords;
	int iLeft = LeftIndex(cCoords), iRight = RightIndex(cCoords), iUp = UpIndex(cCoords), iDown = DownIndex(cCoords);
	int dLeft = maze[iLeft], dRight = maze[iRight], dUp = maze[iUp], dDown = maze[iDown];
	if (IsPath(dRight) && ++dRight == maze[GetIndex(lowest)])
	{
		lowest = RightCoord(cCoords);
	}
	if (IsPath(dUp) && ++dUp == maze[GetIndex(lowest)])
	{
		lowest = UpCoord(cCoords);
	}
	if (IsPath(dLeft) && ++dLeft == maze[GetIndex(lowest)])
	{
		lowest = LeftCoord(cCoords);
	}
	if (IsPath(dDown) && ++dDown == maze[GetIndex(lowest)])
	{
		lowest = DownCoord(cCoords);
	}
	return lowest;
}
