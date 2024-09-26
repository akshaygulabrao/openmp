#include <iostream>
#include <vector>
#include <omp.h>

static long num_steps = 100;
double step;

int main() {
    std::cout << "Serial Mode" << std::endl;

    double pi = 0.0;
    double sum = 0.0;
    double x = 0.0;
    int i = 0;
    step = 1.0/(double) num_steps;
    double start = omp_get_wtime();
    for(i = 0; i < num_steps; i++){
        x = (i + 0.5)*step;
        sum = sum + 4.0/(1.0 + x*x);
    }
    pi = step * sum;
    double end = omp_get_wtime();
    std::cout << "Time Taken: " << end - start << " Value: " << pi << std::endl;
    std::cout << "Parallel Mode" << std::endl;
    x = 0.0;
    i = 0;
    start = omp_get_wtime();
    int n_threads = omp_get_num_threads();
    std::vector<double> partial_sum(20,0);
    for(i = 0; i < num_steps; i+= n_threads){
        #pragma omp parallel
        {
            int ID = omp_get_thread_num();
            x = (i + ID + 0.5)*step;            
            partial_sum[ID % n_threads] += 4.0 / (1.0 + x*x);  
        }
    }
    sum = 0;
    for(i = 0; i < n_threads; i++){
        sum += partial_sum[i];
    }
    pi = step * sum;
    end = omp_get_wtime();
    std::cout << "Time Taken: " << end - start << " Value: " << pi << std::endl;
    return 0;
}