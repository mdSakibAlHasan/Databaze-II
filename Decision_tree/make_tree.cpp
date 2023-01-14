#include <bits/stdc++.h>
using namespace std;

#define MAX 625
#define COL 4

struct row_info
{
    char class_name;
    int attribute_info[4];
} row[MAX];

struct node
{
    int attribute_number, atrribute_value;
    int start_index, end_index;
    char class_name;
    struct node *leftChild;
    struct node *rightChild;
    struct node *parents;
};

double find_class_probability(int num, int index, node *row_information)
{
    int result = 0;
    for (int i = row_information->start_index; i <= row_information->end_index; i++)
    {
        if (row[i].attribute_info[index] == num)
            result++;
    }
    return (double)result;
}

double find_attribute_probability(char ch, node *row_information)
{
    int result = 0;
    for (int i = row_information->start_index; i <= row_information->end_index; i++)
    {
        if (row[i].class_name == ch)
            result++;
    }
    return (double)result;
}

double find_probability_value(int num, char ch, int index, node *row_information)
{
    int result = 0;
    for (int i = row_information->start_index; i <= row_information->end_index; i++)
    {
        if (row[i].class_name == ch && row[i].attribute_info[index] == num)
            result++;
    }

    return (double)result;
}

int sorting_colum_number;
bool compareAttribute(row_info a, row_info b)
{
    return (a.attribute_info[sorting_colum_number] < b.attribute_info[sorting_colum_number]);
}

void sortAttribute(node *row_information, int column)
{
    sorting_colum_number = column;
    sort(row + row_information->start_index, row + row_information->end_index, compareAttribute);
}

int getIndex(int start, int end,int index,int value)
{
    while (start <= end)
    {
        if (row[start].attribute_info[index] > value)
            return start - 1;
        start++;
    }

    return start;
}

int calculate_gain_value(node *row_information)
{
    set<int> attribute_value;
    set<char> class_value;
    double total_row = (double)(row_information->end_index - row_information->start_index + 1), heighest_gain_value = 0, t1, t2, t3;
    int high_attribute, high_value;

    for (int p = row_information->start_index; p <= row_information->end_index; p++) // class unique value
    {
        class_value.insert(row[p].class_name);
    }

    for (int i = 0; i < COL; i++)
    {
        for (int p = row_information->start_index; p <= row_information->end_index; p++) // attribute unique value
        {
            attribute_value.insert(row[p].attribute_info[i]);
        }

        for (auto &num1 : attribute_value)
        {
            double gain_value = 0;
            for (auto &num2 : class_value)
            {
                t1 = (find_probability_value(num1, num2, i, row_information) / total_row);
                if (t1 == 0.0)
                    continue;
                t2 = (find_class_probability(num1, i, row_information) / total_row) * (find_attribute_probability(num2, row_information) / total_row);
                t3 = log2(t1 / t2);

                gain_value += (t1 * t3);
            }
            // cout << gain_value << "---" << num1 << endl;
            if (gain_value >= heighest_gain_value)
            {
                heighest_gain_value = gain_value;
                high_attribute = i;
                high_value = num1;
            }
        }
        attribute_value.clear();
    }

    if (heighest_gain_value == 0)
    {
        high_attribute = 3;
        high_value = row[row_information->start_index].attribute_info[3];
    }

    row_information->attribute_number = high_attribute;
    row_information->atrribute_value = high_value;
    sortAttribute(row_information, high_attribute);
    for (int k = row_information->start_index; k <= row_information->end_index; k++)
    {
        if (row[k].attribute_info[high_attribute] > high_value)
            return (k - 1);
    }

    //return getIndex(row_information->start_index, row_information->end_index,high_attribute,);

    return row_information->start_index;
}

bool checkSameClass(int start_index, int end_index)
{
    set<char> temp;
    for (int i = start_index; i <= end_index; i++)
    {
        temp.insert(row[i].class_name);
    }

    if (temp.size() == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void addChildren(node *children, int start_index, int end_index)
{
    children->start_index = start_index;
    children->end_index = end_index;

    if (checkSameClass(start_index, end_index) || start_index >= end_index)
    {
        cout << start_index << "-" << end_index << " atr " << children->attribute_number << "-" << children->atrribute_value << " ** " << row[start_index].class_name << row[end_index].class_name << endl;
        if (start_index != end_index)
            cout << (end_index - start_index) << " ++++ " << endl;
        return;
    }

    int last_index;
    last_index = calculate_gain_value(children);
    // cout << column_value << " " << root->attribute_number << " * " << root->atrribute_value << endl;

    struct node *leftChild = (struct node *)malloc(sizeof(struct node));
    leftChild->parents = children;
    struct node *rightChild = (struct node *)malloc(sizeof(struct node));
    rightChild->parents = children;
    children->leftChild = leftChild;
    children->rightChild = rightChild;
    addChildren(leftChild, children->start_index, last_index);
    addChildren(leftChild, last_index + 1, children->end_index);
}

void create_tree()
{
    struct node *root = (struct node *)malloc(sizeof(struct node));
    root->start_index = 0;
    root->end_index = MAX - 1;
    int last_index;
    last_index = calculate_gain_value(root);
    // cout << column_value << " " << root->attribute_number << " * " << root->atrribute_value << endl;

    struct node *leftChild = (struct node *)malloc(sizeof(struct node));
    leftChild->parents = root;
    struct node *rightChild = (struct node *)malloc(sizeof(struct node));
    rightChild->parents = root;
    root->leftChild = leftChild;
    root->rightChild = rightChild;
    addChildren(leftChild, 0, last_index);
    addChildren(leftChild, last_index + 1, MAX - 1);
    // cout << column_number << " " << column_value << endl;
}

void load_data()
{
    char temp;
    int a, b, c;
    ifstream myFile("balance-scale.data");
    for (int i = 0; i < MAX; i++)
    {
        myFile >> row[i].class_name;
        for (int k = 0; k < 4; k++)
        {
            myFile >> temp >> row[i].attribute_info[k];
        }
    }
}

int main()
{
    load_data();
    create_tree();
}