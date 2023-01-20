#include <bits/stdc++.h>
using namespace std;

#define MAX 100
#define initial 1
#define visited 2
#define finished 3

int n=3; /*Number of vertices in the graph */
int adj[MAX][MAX];
void create_graph();

int state[MAX];

vector<pair<bool, pair<char, int>>> info;

void store_in_array(char temp[])
{
    char variable;
    bool read;
    int transaction_number;
    stringstream ss;
    for (int i = strlen(temp) - 1; i >= 0; i--)
    {
        if (temp[i] == '(' || temp[i] == ')')
            continue;
        else if (temp[i] == 'w' || temp[i] == 'r')
        {
            if (temp[i] == 'w')
                read = false;
            else
                read = true;
        }
        else if (isdigit(temp[i]))
        {
            ss << temp[i];
        }
        else
        {
            variable = temp[i];
        }
    }
    ss >> transaction_number;

    info.push_back(make_pair(read, make_pair(variable, transaction_number)));
}

void split_input(char str[])
{
    char temp[8];
    int word_number = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
        {
            temp[word_number] = '\0';
            word_number = 0;
            store_in_array(temp);
        }
        else
        {
            temp[word_number++] = str[i];
        }
    }
}

void DFS(int v)
{
    int i;
    state[v] = visited;

    for (i = 0; i < n; i++)
    {
        if (adj[v][i] == 1)
        {
            if (state[i] == initial)
                DFS(i);
            else if (state[i] == visited)
            {
                printf("\nNot serializable\n");
                exit(1);
            }
        }
    }
    state[v] = finished;
}

void DF_Traversal()
{
    int v;

    for (v = 0; v < n; v++)
        state[v] = initial;

    DFS(0);

    for (v = 0; v < n; v++)
    {
        if (state[v] == initial)
            DFS(v);
    }
    printf("\nSerializable\n");
}

void make_graph()
{
    int max_edges, origin, destin;

    for (int i = 0; i < info.size(); i++)
    {
        bool read = true;
        for (int j = i + 1; j < info.size(); j++)
        {
            if (info[i].second.first == info[j].second.first)
            {
                if (!info[i].first || !info[j].first)
                    read = false;
                if (!read)
                {
                    adj[info[i].second.second][info[j].second.second] = 1;
                }
            }
        }
    }
}

int main()
{
    char input[] = "r1(x) r3(y) w3(x) r1(y) w1(x) ";
    //n = 3;
    split_input(input);
    make_graph();
    DF_Traversal();
}