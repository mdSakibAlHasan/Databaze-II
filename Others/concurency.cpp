#include <bits/stdc++.h>
using namespace std;
#define MAX_TRAN 10
#define VARIABLE 2
char vari_data[VARIABLE] = {'x', 'y'};

void store_in_array(char temp[], int tran[VARIABLE][2], int *current_variable, int num)
{
    cout << temp << endl;
    int index = 0, i;
    char x = temp[4];
    for (i = 0; i < VARIABLE; i++)
    {
        if (x == vari_data[i])
        {
            index = i;
            break;
        }
    }

    if (temp[0] == 'r')
    {
        tran[num++][0] = 0;
        tran[num++][1] = temp[1] - '0';
        cout << " " << temp[1] << " ";
    }
    else
    {
        tran[num++][0] = 1;
        tran[num++][1] = temp[1] - '0';
    }
}

void split_input(char str[], int tran[VARIABLE][2], int num)
{
    // cout << num[0] << num[0] << endl;
    bool read = false;
    char temp[8];
    int word_number = 0, current_variable = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        // cout << x << " ";
        if (str[i] == ' ')
        {
            temp[word_number] = '\0';
            word_number = 0;
            store_in_array(temp, tran, &current_variable, num);
        }
        else
        {
            temp[word_number++] = str[i];
        }
    }
    // char *ptr = strtok(str, ",");

    // while (ptr != NULL)
    // {
    //     store_in_array(temp, tran, &current_variable, num);
    //     ptr = strtok(NULL, ",");
    // }
}

int main()
{
    char input[] = "r2(x) r3(y) w3(x) r1(y) w2(x) w1(y) w1(x) ";
    int transaction[VARIABLE][2];
    int number_of_transaction = 0, total;
    split_input(input, transaction, &number_of_transaction);

    for (int i = 0; i < number_of_transaction; i++)
    {
        cout << transaction[i][0] << " " << transaction[i][1] << " --- ";
    }
}