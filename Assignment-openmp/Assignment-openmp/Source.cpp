#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

void applyKernel(vector<vector<int>>& image, vector<vector<int>>& kernel, vector<vector<int>>& result) {
    int rows = image.size();
    int cols = image[0].size();
    int kSize = kernel.size();
    int kCenter = kSize / 2;

    for (int i = kCenter; i < rows - kCenter; ++i) {
        for (int j = kCenter; j < cols - kCenter; ++j) {
            int sum = 0;
            for (int m = 0; m < kSize; ++m) {
                for (int n = 0; n < kSize; ++n) {
                    sum += image[i + m - kCenter][j + n - kCenter] * kernel[m][n];
                }
            }
            result[i - kCenter][j - kCenter] = sum;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " input_image.txt mask_file.txt output_image.txt\n";
        return 1;
    }

    ifstream inputImageFile(argv[1]);
    ifstream maskFile(argv[2]);
    ofstream outputImageFile(argv[3]);

    if (!inputImageFile || !maskFile || !outputImageFile) {
        cerr << "Error opening files.\n";
        return 1;
    }

    int rows, cols;
    inputImageFile >> rows >> cols;

    vector<vector<int>> image(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            inputImageFile >> image[i][j];
        }
    }

    int kSize;
    maskFile >> kSize;
    vector<vector<int>> kernel(kSize, vector<int>(kSize));
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            maskFile >> kernel[i][j];
        }
    }

    vector<vector<int>> result(rows - kSize + 1, vector<int>(cols - kSize + 1));

    auto start = chrono::steady_clock::now();

    applyKernel(image, kernel, result);

    auto end = chrono::steady_clock::now();
    double executionTime = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0;

    outputImageFile << result.size() << " " << result[0].size() << "\n";
    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[0].size(); ++j) {
            outputImageFile << result[i][j] << " ";
        }
        outputImageFile << "\n";
    }

    cout << "Execution time: " << executionTime << " seconds." << endl;

    return 0;
}
