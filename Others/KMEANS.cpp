#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;


struct Coordinate {
    double x, y;
};


double distance(Coordinate a, Coordinate b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}


void assign_clusters(vector<Coordinate>& coordinates, vector<Coordinate>& centers, vector<int>& clusters) {
    for (int i = 0; i < coordinates.size(); i++) {
        double min_distance = numeric_limits<double>::max();
        int closest_center = 0;
        for (int j = 0; j < centers.size(); j++) {
            double d = distance(coordinates[i], centers[j]);
            if (d < min_distance) {
                min_distance = d;
                closest_center = j;
            }
        }
        clusters[i] = closest_center;
    }
}


void update_centers(vector<Coordinate>& coordinates, vector<Coordinate>& centers, vector<int>& clusters) {
    for (int i = 0; i < centers.size(); i++) {
        double sum_x = 0, sum_y = 0;
        int count = 0;
        for (int j = 0; j < coordinates.size(); j++) {
            if (clusters[j] == i) {
                sum_x += coordinates[j].x;
                sum_y += coordinates[j].y;
                count++;
            }
        }
        if (count > 0) {
            centers[i].x = sum_x / count;
            centers[i].y = sum_y / count;
        }
    }
}


void k_means(vector<Coordinate>& coordinates, int k) {
    vector<Coordinate> centers(k);
    for (int i = 0; i < k; i++) {
        centers[i].x = coordinates[i].x;
        centers[i].y = coordinates[i].y;
    }
    vector<int> clusters(coordinates.size());
    bool converged = false;
    while (!converged) {
        assign_clusters(coordinates, centers, clusters);
        Coordinate old_centers[k];
        copy(centers.begin(), centers.end(), old_centers);
        update_centers(coordinates, centers, clusters);
        converged = true;
        for (int i = 0; i < k; i++) {
            if (distance(old_centers[i], centers[i]) > 0.0001) {
                converged = false;
                break;
            }
        }
    }
    

    //print output
    for(int j=0;j<3;j++){
        cout<<"Cluster is: "<<j<<endl;
        for (int i = 0; i < coordinates.size(); i++) {
            if(clusters[i] == j)
                cout << coordinates[i].x << " " << coordinates[i].y << " " << clusters[i] << endl;
        }
    }
}

int main() {
    ifstream infile("coordinates.txt"); // Open the input file
    vector<Coordinate> coordinates;
    double x, y;
    while (infile >> x >> y) { // Read the coordinates from the file
        Coordinate c = {x, y};
        coordinates.push_back(c);
    }
    int k = 3; // Set the number of clusters
    k_means(coordinates, k); // Perform K-means clustering
    return 0;
}
