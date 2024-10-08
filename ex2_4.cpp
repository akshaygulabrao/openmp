#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#define NUM_THREADS 8

long double calculate_pi_serial(long num_steps) {
    long double step = 1.0L / static_cast<long double>(num_steps);
    long double sum = 0.0L;
    for (long i = 0; i < num_steps; i++) {
        long double x = (static_cast<long double>(i) + 0.5L) * step;
        sum += 4.0L / (1.0L + x * x);
    }
    return step * sum;
}

long double calculate_pi_parallel(long num_steps) {
    long double step = 1.0L / static_cast<long double>(num_steps);
    std::vector<long double> partial_sums(NUM_THREADS, 0.0L);

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int id = omp_get_thread_num();
        for (long i = id; i < num_steps; i += NUM_THREADS) {
            long double x = (static_cast<long double>(i) + 0.5L) * step;
            partial_sums[id] += 4.0L / (1.0L + x * x);
        }
    }

    long double sum = 0.0L;
    for (int i = 0; i < NUM_THREADS; i++) {
        sum += partial_sums[i];
    }
    return step * sum;
}

#if 0
long double calculate_pi_atomic(long num_steps) {
    long double step = 1.0L / static_cast<long double>(num_steps);
    long double sum = 0.0L;

    #pragma omp parallel num_threads(NUM_THREADS)
    {
            int ID = omp_get_thread_num();
            for (long i = ID; i < num_steps; i += NUM_THREADS) {
                long double x = (static_cast<long double>(i) + 0.5L) * step;
                #pragma omp atomic
                sum += 4.0L / (1.0L + x * x);
            }
    }

    return step * sum;
}
#endif

long double calculate_pi_for_reduce(long num_steps) {
    long double step = 1.0L / static_cast<long double>(num_steps);
    long double sum = 0.0L;

    #pragma omp parallel for reduction(+:sum)
    {
            for (long i = 0; i < num_steps; i++) {
                long double x = (static_cast<long double>(i) + 0.5L) * step;
                sum += 4.0L / (1.0L + x * x);
            }
    }

    return step * sum;
}

int main() {
    int n = 37;
    std::vector<long> num_steps_vec;
    for (int i = 15; i <= n; i++) {
        num_steps_vec.push_back(static_cast<long>(std::pow(2, i)));
    }
    std::vector<double> serial_times(n-14, 0);
    std::vector<double> parallel_times(n-14, 0);
    #if 0
    std::vector<double> atomic_times(n-14, 0);
    #endif
    std::vector<double> for_reduce_times(n-14, 0);
    
    // Open a file for writing results
    std::ofstream outfile("pi_calculation_results.csv");
    outfile << std::setprecision(15); // Set precision for floating-point output
    outfile << "Num_Steps,Serial_Time,Parallel_Time,For_Reduce_Time,Serial_Pi,Parallel_Pi,For_Reduce_Pi\n";
    
    std::cout << "Num Steps,Serial Time,Parallel Time,For Reduce Time,Serial Pi,Parallel Pi,For Reduce Pi\n";

    for (int num_steps_it = 0; num_steps_it < num_steps_vec.size(); num_steps_it++) {
        long num_steps = num_steps_vec[num_steps_it];
        
        double start, end;
        long double pi_serial, pi_parallel, pi_for_reduce;
        #if 0
        long double pi_atomic;
        #endif

        // Serial execution
        start = omp_get_wtime();
        pi_serial = calculate_pi_serial(num_steps);
        end = omp_get_wtime();
        serial_times[num_steps_it] = end - start;

        // Parallel execution
        start = omp_get_wtime();
        pi_parallel = calculate_pi_parallel(num_steps);
        end = omp_get_wtime();
        parallel_times[num_steps_it] = end - start;

        #if 0
        // Atomic execution
        start = omp_get_wtime();
        pi_atomic = calculate_pi_atomic(num_steps);
        end = omp_get_wtime();
        atomic_times[num_steps_it] = end - start;
        #endif

        // For Reduce execution
        start = omp_get_wtime();
        pi_for_reduce = calculate_pi_for_reduce(num_steps);
        end = omp_get_wtime();
        for_reduce_times[num_steps_it] = end - start;

        if (std::isnan(pi_serial) || std::isnan(pi_parallel) || std::isnan(pi_for_reduce) || 
            std::isinf(pi_serial) || std::isinf(pi_parallel) || std::isinf(pi_for_reduce)) {
            std::cout << "Invalid result detected for num_steps = " << num_steps << std::endl;
            break;
        }

        // Output results to console and file
        std::cout << num_steps << ","
                  << serial_times[num_steps_it] << ","
                  << parallel_times[num_steps_it] << ","
                  << for_reduce_times[num_steps_it] << ","
                  << std::setprecision(15) << pi_serial << ","
                  << std::setprecision(15) << pi_parallel << ","
                  << std::setprecision(15) << pi_for_reduce << "\n";

        // Write results to file
        outfile << num_steps << ","
                << serial_times[num_steps_it] << ","
                << parallel_times[num_steps_it] << ","
                << for_reduce_times[num_steps_it] << ","
                << std::setprecision(15) << pi_serial << ","
                << std::setprecision(15) << pi_parallel << ","
                << std::setprecision(15) << pi_for_reduce << "\n";
    }

    outfile.close();
    std::cout << "Results have been written to pi_calculation_results.csv" << std::endl;

    return 0;
}