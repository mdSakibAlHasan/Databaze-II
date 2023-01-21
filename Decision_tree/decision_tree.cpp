#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define MAX 600
#define COL 4
struct row_info
{
    char class_name;
    int attribute_info[4];
};
row_info values[626];

struct node
{
    int attribute_number, child_number, internal_class_name;
    char class_name;
    bool leaf = false;
    node **child;
    vector<row_info> row;
};

double find_attribute_probability(int num, int index, node *r)
{
    int result = 0;
    for (int i = 0; i < r->row.size(); i++)
    {
        if (r->row[i].attribute_info[index] == num)
            result++;
    }
    return (double)result;
}

double find_class_probability(char ch, node *r)
{
    int result = 0;
    for (int i = 0; i < r->row.size(); i++)
    {
        if (r->row[i].class_name == ch)
            result++;
    }
    return (double)result;
}

double find_probability_value(int num, char ch, int index, node *r)
{
    int result = 0;
    for (int i = 0; i < r->row.size(); i++)
    {
        if (r->row[i].class_name == ch && r->row[i].attribute_info[index] == num)
            result++;
    }

    return (double)result;
}

int sorting_colum_number;
void sortAttribute(node *r, int column)
{
    sorting_colum_number = column;
    std::sort(r->row.begin(), r->row.end(), [](const row_info &a, const row_info &b)
              { return a.attribute_info[sorting_colum_number] < b.attribute_info[sorting_colum_number]; });
}

void calculate_gain_value(node *row_information)
{
    set<int> attribute_value;
    set<char> class_value;
    double total_row = (double)(row_information->row.size()), heighest_gain_value = 0, t1, t2, t3;
    double entropy = 0, row_entropy;

    for (int p = 0; p < row_information->row.size(); p++) // class unique value
    {
        class_value.insert(row_information->row[p].class_name);
    }

    for (auto &num2 : class_value)
    {
        t1 = find_class_probability(num2, row_information);
        t2 = (t1 / total_row) * (log2(t1 / total_row));
        entropy -= t2;
    }

    for (int i = 0; i < COL; i++)
    {
        for (int p = 0; p < row_information->row.size(); p++) // attribute unique value
        {
            attribute_value.insert(row_information->row[p].attribute_info[i]);
        }
        row_entropy = 0;
        for (auto &num1 : attribute_value)
        {
            double gain_value = 0;

            for (auto &num2 : class_value)
            {
                t2 = find_attribute_probability(num1, i, row_information);
                t1 = (find_probability_value(num1, num2, i, row_information) / t2);
                if (t1 == 0)
                    continue;
                t3 = log2(t1);
                gain_value -= (t1 * t3);
            }
            t2 = (t2 / total_row);
            row_entropy += (t2 * gain_value);
        }
        if ((entropy - row_entropy) > heighest_gain_value)
        {
            heighest_gain_value = (entropy - row_entropy);
            row_information->attribute_number = i;
            row_information->child_number = attribute_value.size();
        }
        attribute_value.clear();
    }

    sortAttribute(row_information, row_information->attribute_number);
}

bool checkSameClass(node *children)
{
    set<char> temp;
    for (int i = 0; i < children->row.size(); i++)
    {
        temp.insert(children->row[i].class_name);
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
int counter2 = 0;
void addChildren(node *children)
{
    if (checkSameClass(children))
    {
        children->leaf = true;
        children->class_name = children->row[0].class_name;
        cout << children->row[0].class_name;
        cout << " " << children->row.size() << " ++++    " << endl;
        if (children->row.size() != 1)
            counter2 += children->row.size();
        return;
    }

    children->leaf = false;
    calculate_gain_value(children);

    children->child = new node *[children->child_number];
    node *new_child = new node();
    new_child->internal_class_name = children->row[0].attribute_info[children->attribute_number];
    children->child[0] = new_child;
    int child_index = 0, first_value = children->row[0].attribute_info[children->attribute_number];
    for (int i = 0; i < children->row.size(); i++)
    {
        if (children->row[i].attribute_info[children->attribute_number] == first_value)
        {
            new_child->row.push_back(children->row[i]);
        }
        else
        {
            addChildren(new_child);
            new_child = new node();
            new_child->internal_class_name = children->row[i].attribute_info[children->attribute_number];
            children->child[++child_index] = new_child;
            new_child->row.push_back(children->row[i]);
            first_value = children->row[i].attribute_info[children->attribute_number];
        }
    }
    addChildren(new_child);
    children->row.clear(); // clear parents data only child data are remaining
}

struct node *create_tree()
{
    node *root = new node();
    root->leaf = false;
    for (int i = 0; i < MAX; i++)
    {
        root->row.push_back(values[i]);
    }
    calculate_gain_value(root);

    root->child = new node *[root->child_number];
    node *children = new node();
    root->child[0] = children;
    children->internal_class_name = root->row[0].attribute_info[root->attribute_number];
    int child_index = 0, first_value = root->row[0].attribute_info[root->attribute_number];
    for (int i = 0; i < root->row.size(); i++)
    {
        if (root->row[i].attribute_info[root->attribute_number] == first_value)
        {
            children->row.push_back(root->row[i]);
        }
        else
        {
            addChildren(children);
            children = new node();
            root->child[++child_index] = children;
            children->internal_class_name = root->row[i].attribute_info[root->attribute_number];
            children->row.push_back(root->row[i]);
            first_value = root->row[i].attribute_info[root->attribute_number];
        }
    }
    addChildren(children);
    root->row.clear();
    cout << endl
         << "P: " << root << " ";
    for (int i = 0; i < root->child_number; i++)
        cout << root->child[i] << " ";
    return root;
}

void load_data()
{
    char temp;
    int a, b, c;
    ifstream myFile("balance-scale.data");
    for (int i = 0; i < 625; i++)
    {
        myFile >> values[i].class_name;
        for (int k = 0; k < 4; k++)
        {
            myFile >> temp >> values[i].attribute_info[k];
        }
    }
}

char find_decision(node *level_data, row_info test_data)
{
    char ch;
    int i=0;
    if (level_data->leaf)
    {
        ch = level_data->class_name;
    }
    else
    {
        for (i = 0; i < level_data->child_number; i++)
        {
            if (level_data->child[i]->internal_class_name == test_data.attribute_info[level_data->attribute_number])
            {
                ch = find_decision(level_data->child[i], test_data);
                break;
            }
        }
        if(i==level_data->child_number)
            ch = find_decision(level_data->child[i-1], test_data);
    }
    cout << "-1-";
    
    // find_decision(level_data->child[0], test_data);
    return ch;
}

void testing(node *root, row_info test_data)
{
    char ch = find_decision(root, test_data);
    cout << ch << " ";
    if (ch == test_data.class_name)
    {
        cout << "Match" << endl;
    }
    else
    {
        cout << "Not Match" << endl;
    }
}

int main()
{
    load_data();
    node *root = create_tree();
    cout << " root: " << root << endl;
    cout << "Total:  " << counter2 << endl;

    for (int i = 600; i < 625; i++)
    {
        testing(root, values[i]);
    }

    return 0;
}