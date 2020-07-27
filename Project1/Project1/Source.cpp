#include <iostream>
#include <thread>
#include <ctime>
#include <mutex>
#include <vector>
#include <conio.h>

#define SIZE 15

using namespace std;

int Map[SIZE][SIZE] = { 0 };
int endX = SIZE - 2;
int endY = SIZE - 2;

bool stopflag = false;

mutex Mutex;
mutex ballMutex;


class Ball {

public:
	Ball() {
		//ResetDir();
		//DirX = (rand() % 3) - 1;//-1~1
		//DirY = (rand() % 3) - 1;//-1~1

		//Mutex.unlock();
		while (DirX == 0 && DirY == 0) {
			DirX = (rand() % 3) - 1;//-1~1
			DirY = (rand() % 3) - 1;//-1~1
		}

		x = 2 + rand() % (SIZE - 3);
		y = 2 + rand() % (SIZE - 3);
		//cout << "位置: (" << x << " , " << y << " )" << endl;
	}

	~Ball() {
		cout << "解構" << endl;
		Map[x][y] = 0;
	}

	void ResetDir() {
		//lock_guard<mutex> mLock(Mutex);
		
		//Mutex.lock();

		
		DirX = (rand() % 3) - 1;//-1~1
		DirY = (rand() % 3) - 1;//-1~1

		//Mutex.unlock();
		while (DirX == 0 && DirY == 0) {
			DirX = (rand() % 3) - 1;//-1~1
			DirY = (rand() % 3) - 1;//-1~1
		}
		//DirX = DirX * -1;
		//DirY = DirY * -1;
	
	}
	void Move() {

		Map[x][y] = 0;
		
		x = x + DirX;
		y = y + DirY;

		Map[x][y] = 2;
	}
	void update() 
	{
		//ballMutex.lock();
		if (Map[x + DirX][y + DirY] == 1) {
			//撞牆
			ResetDir();
			
		}
		else {
			Move();
		}
		//ballMutex.unlock();
		//cout << "方向 : (" << DirX << " , "<<DirY << ")" << endl;
	}
	int x;
	int y;

	int DirX =0;
	int DirY =0;
};
vector<Ball*> BallVector;

void printMap() {
	int i, j;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++)
			switch (Map[i][j])
			{
			case 0: cout << "  "; break;
			case 1: cout << "█"; break;
			case 2: cout << "◇";
			}
		printf("\n");
	}

}
void clear() {
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	//std::cout << "\x1B[2J\x1B[H";
	system("cls");
}

//update遊戲畫面
void update() {

		while (stopflag!=true)
		{	
			clear();
			// Call our method
			Mutex.lock();
			for (int i = 0; i < BallVector.size(); i++) {
				
				BallVector[i]->update();
				//cout << i << endl;
				
			}
			Mutex.unlock();
			printMap();
			

			// Wait 5 minutes
			this_thread::sleep_for(std::chrono::milliseconds(1));//600ms = 1秒
		
		}

}

void KeyListener() {

	char commandKey='0';
	while (stopflag != true)
	{
		//cin >> commandKey;
		commandKey = _getch();
		if (commandKey == 's') 
		{
			stopflag = true;
		}
		if (commandKey == 'd')
		{
			Mutex.lock();
			cout << "清除所有ball" << endl;
			for (int i = 0; i < BallVector.size(); i++) {
				delete BallVector[i];
			}
			BallVector.clear();
			Mutex.unlock();
		}
		if (commandKey == 'n')
		{
			//Mutex.lock();

			cout << "加入一個ball" << endl;
			
			
			
		/*
			//測試選項 加入1000顆球做壓力測試
			for (int i = 0; i < 1000; i++) 
			{
				Ball* ball = new Ball();
				//Ball ball;
				BallVector.push_back(ball);
			}
		*/
			Ball* ball = new Ball();
			//Ball ball;
			BallVector.push_back(ball);
			
			//BallVector.push_back(ball);

			//Mutex.unlock();
		}
		commandKey = '0';
	}
	
}

void setupMap() {
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			if (row == 0 || col == SIZE - 1 || row == SIZE - 1 || col == 0) {
				Map[row][col] = 1;
			}

		}
	}
}

int main() {
	srand(time(NULL));
	setupMap();

	thread Listener(KeyListener);
	//update();
	
	thread GameLoop(update);

	Listener.join();
	GameLoop.join();
	return 0;
}