#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <math.h>
using namespace std;
vector<int> random_co_or;
int n,k;

struct co_or{
    double x;
    double y;
    int custer_num;
};

void find_initial_random_co_or(int n, int k)
{
    set<int> rand_num;

    srand(time(0));

    while(rand_num.size()!=k)
    {
        int new_random_number = rand() % (n);
        rand_num.insert(new_random_number);
    }

    for (const auto& element : rand_num)
    {
        random_co_or.push_back(element);
    }

    for (int i = 0; i < k; i++) {
        cout << random_co_or[i] << " ";
    }
    cout << endl;
}
int main()
{


   cout<<"Number of Co-ordinates: ";
   cin>>n;

   cout<<"Number of Cluster: ";
   cin>>k;

   co_or co_ordinates[n];

   for(int i=0;i<n;i++)
   {
        double x_co, y_co;
        cin>>x_co>>y_co;

        co_ordinates[i].x=x_co;
        co_ordinates[i].y=y_co;
   }



   find_initial_random_co_or(n,k);


   for(int i=0;i<n;i++)
   {
        vector<double> distances;
        for(int j=0;j<k;j++)
        {
            double x_dis= pow((co_ordinates[i].x-co_ordinates[random_co_or[k]].x),2);
            double y_dis= pow((co_ordinates[i].y-co_ordinates[random_co_or[k]].y),2);
            double d=sqrt(x_dis+y_dis);
            cout<<d<<endl;
            distances.push_back(d);
        }

        double mini = distances[0];
        int mini_index;

        for(int j=0;j<k;j++)
        {
            if(distances[j]<mini)
                {
                    mini=distances[j];
                    mini_index=j;
                }
        }
        cout<<mini_index;
        co_ordinates[i].custer_num=mini_index+1;
   }

   for(int i=0;i<n;i++)
   {
        cout<<"("<<co_ordinates[i].x<<","<<co_ordinates[i].y<<")"<<"  "<<co_ordinates[i].custer_num<<endl;
   }

}

/*
1 1
1.5 2
3 4
5 7
3.5 5
4.5 5
4.5 5
3.5 4.5
*/