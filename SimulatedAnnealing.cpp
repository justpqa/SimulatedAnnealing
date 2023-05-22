#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

// Problem statement:
// Given N points in the plane, find the shortest tour from 1 points to all other points and back to first point

// use floats for the array of points in order to deal with the length
float calcTotalDist(float** arr, int size) {
    float total = 0.0;
    for (int i = 1; i < size; i++) {
        total += std::sqrt(std::pow((arr[i][0] - arr[i-1][0]), 2) + std::pow((arr[i][1] - arr[i-1][1]), 2));
    }
    total += std::sqrt(std::pow((arr[size-1][0] - arr[0][0]), 2) + std::pow((arr[size-1][1] - arr[0][1]), 2));
    return total;
}

void randomPermute(float** points, int size) {
    // number of permute pair
    int num_permute = size / 4;
    int first, second;
    float* temp;
    for (int i = 0; i < num_permute; i++) {
        first = rand() % size;
        second = rand() % size;
        temp = points[first];
        points[first] = points[second];
        points[second] = temp;
    }
}

float simulatedAnnealing(float** points, int size, int numIters, float alpha) {
    // 0.8 <= alpha <= 0.9
    float minTourLength = calcTotalDist(points, size);
    // float** minTour = points;
    float t0 = 1.0;
    float temp = t0;
    float currTourLength;
    float prob;
    float nega_cost;
    float acceptance;

    for (int i = 0; i < numIters; i++) {
        randomPermute(points, size);
        currTourLength = calcTotalDist(points, size);
        if (currTourLength < minTourLength) {
            minTourLength = currTourLength;
            // minTour = points;
        } else {
            prob = (float) rand()/RAND_MAX;
            nega_cost = minTourLength - currTourLength;
            acceptance = std::exp(nega_cost / temp);
            if (prob >= acceptance) {
                minTourLength = currTourLength;
                // minTour = points;
            }
        }
        temp *= alpha; 
    }

    return minTourLength; // can calculate minTourLength later
}

int main() {

    // read the number of points
    int num_points;
    int read_n;
    bool start_points = false;
    std::ifstream inputfile("test.txt");
    while ((!start_points)) {
        while (inputfile >> read_n) {
            num_points = read_n;
            start_points = !start_points;
        }
    } 

    // read all the points in the list
    float x, y;
    float** points = new float*[num_points];
    int i = 0;
    while (inputfile >> x >> y) {
        points[i] = new float[2];
        points[i][0] = x;
        points[i][1] = y;
        i++;
    }
    inputfile.close();

    for (i = 0; i < num_points; i++) {
        std::cout << points[i][0] << " " << points[i][1] << std::endl;
    }

    float result;
    result = simulatedAnnealing(points, num_points, 100, 0.8);
    std::cout << result << std::endl;

    // Delete the array after using
    for (i = 0; i < num_points; i++) {
        delete[] points[i];
    }

    delete[] points;

    return 0;
}