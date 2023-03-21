#include <bits/stdc++.h>
using namespace std;
#define SIZE 500
#define MAX 4
struct
{
    int x, y;
    int center;
} value[SIZE];
int center_value[MAX][2];

void read_from_file()
{
    ifstream MyFile("filename.txt");
    for (int i = 0; i < SIZE; i++)
    {
        MyFile >> value[i].x >> value[i].y;
    }
}

void get_random_number()
{
    int heigest[2] = {value[0].x, value[0].y}, lowest[2] = {value[0].x, value[0].y};
    for (int i = 1; i < SIZE; i++)
    {
        if (value[i].x > heigest[0])
            heigest[0] = value[i].x;
        else if (value[i].x < lowest[0])
            lowest[0] = value[i].x;
        if (value[i].y > heigest[1])
            heigest[1] = value[i].y;
        else if (value[i].y > lowest[1])
            lowest[1] = value[i].y;
    }

    cout << "Number of random: ";
    for (int i = 0; i < SIZE; i++)
    {
        center_value[i][0] = lowest[0] + (rand() % heigest[0]);
        center_value[i][1] = lowest[1] + (rand() % heigest[1]);
        cout << center_value[i][0] << " " << center_value[i][1] << endl;
    }
}

int main()
{
    read_from_file();
    get_random_number();
}
