#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define MAX 500
#define COL 42
#define MAX_DATA 1000
#define START 600
struct row_info
{
    string class_name;
    string attribute_info[42];
};
row_info values[MAX_DATA], all_values[MAX_DATA];
int rand_col[5][42] = {0}, tree_index = 0;

struct node
{
    int attribute_number, child_number;
    string class_name, internal_class_name; // Which catagory it divided of parents values
    bool leaf = false;
    node **child;
    vector<row_info> row;
};

double find_attribute_probability(string num, int index, node *r)
{
    int result = 0;
    for (int i = 0; i < r->row.size(); i++)
    {
        if (r->row[i].attribute_info[index] == num)
            result++;
    }
    return (double)result;
}

double find_class_probability(string ch, node *r)
{
    int result = 0;
    for (int i = 0; i < r->row.size(); i++)
    {
        if (r->row[i].class_name == ch)
            result++;
    }
    return (double)result;
}

double find_probability_value(string num, string ch, int index, node *r)
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
    set<string> attribute_value;
    set<string> class_value;
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
        if (rand_col[tree_index][i] == 1)
            continue;
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
    set<string> temp;
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
        // cout << children->row[0].class_name;
        // cout << " " << children->row.size() << " ++++    " << endl;
        // if (children->row.size() != 1)
        //     counter2 += children->row.size();
        return;
    }

    children->leaf = false;
    calculate_gain_value(children);

    children->child = new node *[children->child_number];
    node *new_child = new node();
    new_child->internal_class_name = children->row[0].attribute_info[children->attribute_number];
    children->child[0] = new_child;
    int child_index = 0;
    string first_value = children->row[0].attribute_info[children->attribute_number];
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
    children->internal_class_name = root->row[0].attribute_info[root->attribute_number]; // Which catagory it divided
    int child_index = 0;
    string first_value = root->row[0].attribute_info[root->attribute_number];
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
    // cout << endl
    //      << "P: " << root << " ";
    // for (int i = 0; i < root->child_number; i++)
    //     cout << root->child[i] << " ";
    return root;
}

void create_random_col()
{
    srand(1209);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int temp = rand() % 42;
            rand_col[i][temp] = 1;
        }
    }

    // for (int i = 0; i < 5; i++)
    // {
    //     for (int j = 0; j < 42; j++)
    //     {
    //         cout << rand_col[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}

void load_data(int seed)
{
    shuffle(all_values, all_values + START, default_random_engine(seed));
    for (int i = 0; i < MAX_DATA; i++)
    {

        for (int k = 0; k < COL; k++)
        {
            values[i].attribute_info[k] = all_values[i].attribute_info[k];
        }
        values[i].class_name = all_values[i].class_name;
    }
}

void load_all_data()
{
    char temp;
    int a, b, c;
    ifstream myFile("new.txt");
    for (int i = 0; i < MAX_DATA; i++)
    {

        for (int k = 0; k < 42; k++)
        {
            myFile >> all_values[i].attribute_info[k];
        }
        myFile >> all_values[i].class_name;
    }

    // for (int i = 0; i < MAX_DATA; i++)
    // {

    //     for (int k = 0; k < 8; k++)
    //     {
    //         cout << i << " " << values[i].attribute_info[k] << " ";
    //     }
    //     cout << values[i].class_name << endl;
    // }

    shuffle(all_values, all_values + MAX_DATA, default_random_engine(120));
}

string find_decision(node *level_data, row_info test_data)
{
    string ch;
    int i = 0;
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
        if (i == level_data->child_number)
            ch = find_decision(level_data->child[i - 1], test_data);
    }
    // cout << "-1-";

    // find_decision(level_data->child[0], test_data);
    return ch;
}

bool testing(node *root, row_info test_data)
{
    string ch = find_decision(root, test_data);
    // cout << ch << " ";
    if (ch == test_data.class_name)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void print_sub_tree(node *root, string str)
{
    string new_str = str + "          ";
    bool first = true;
    int children_index = 0;
    for (int i = 0; i < root->child_number; i++)
    {
        // cout<<root->child[i]->internal_class_name<<" "

        cout << str << root->child[i]->internal_class_name;

        if (root->child[i]->leaf)
        {
            cout << ":" << root->child[i]->class_name << endl;
            // print_sub_tree(root->child[i],new_str);
        }
        else
        {
            cout << endl;
            print_sub_tree(root->child[i], new_str);
        }
    }
    cout << endl;
}

void print_tree(node *root)
{
    string str;
    cout << "Tree print here:" << endl;
    // cout << root->attribute_number << ": atr " << root->child_number << ": class " << root->internal_class_name << endl;
    // for (int i = 0; i < root->child_number; i++)
    // {
    //     cout << root->child[i]->internal_class_name << endl;
    // }
    // cout << "end------------" << endl;
    print_sub_tree(root, str);
}

int main()
{
    node **roots = new node *[5];
    create_random_col();
    load_all_data();
    // load_data();
    for (int i = 0; i < 5; i++)
    {
        load_data(i);
        roots[i] = create_tree();
        cout << roots[i] << endl;
    }
    // node *root = create_tree();
    // cout << " root: " << root << endl;
    // cout << "Total:  " << counter2 << endl;
    int match = 0, disMatch = 0;

    int co[5];
    for (int i = START; i < MAX_DATA; i++)
    {
        int a = 0, r = 0;
        for (int k = 0; k < 5; k++)
        {
            if (testing(roots[k], values[i]))
                a++;
            else
                r++;
        }
        co[a]++;
        if (a > r)
            match++;
        else
            disMatch++;

        tree_index++;
    }

    cout << co[0] << " " << co[1] << " " << co[2] << " " << co[3] << " " << co[4] << endl;
    cout << match << " " << disMatch << endl;
    double probability = (double)match / (double)(match + disMatch);
    cout << "Probability is: " << probability * 100 << endl;

    // print_tree(root);

    return 0;
}