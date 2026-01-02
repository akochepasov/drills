#include <mutex>
#include <thread>
#include <chrono>
#include <gtest/gtest.h>

static std::mutex cas_mutex;
template<typename T>
bool compare_and_swap_raw(T *a, T expected, T desired) {
	std::lock_guard<std::mutex> lock(cas_mutex);

    T orig_a = *a;
    if (orig_a != expected)
        return false;

    *a = desired;
    return true;
}


TEST(CAS, raw) {
    uint64_t N = 1'000'000;
	uint64_t sum = 0;
    int threads_count = 10;

	std::mutex start_mtx;
	std::condition_variable start_cv;
	bool start_flag = false;
    std::atomic<int> ready_count = 0;

	std::vector<std::thread> threads;
    for (int t = 0; t < threads_count; t++) {
        threads.emplace_back([t, N, threads_count, &sum, &start_mtx, &start_flag, &start_cv, &ready_count]() {
            ready_count++;
            {
                std::unique_lock<std::mutex> lock(start_mtx);
                start_cv.wait(lock, [&]() { return start_flag; });

//                std::cout << "Thread " << t << " started, Range: [" << t * N / threads_count << ", " << (t + 1) * N / threads_count << ")\n";
            }

            for (int i = t * N / threads_count; i < (t + 1) * N / threads_count; i++) {
                int64_t expected, desired;
                do {
                    expected = sum;
                    desired = expected + i;
                } while (!compare_and_swap_raw((int64_t*)&sum, expected, desired));
            }
        });
	}

    while (ready_count.load() < threads_count) {
        std::this_thread::yield();
    }

    {
        std::unique_lock<std::mutex> lock(start_mtx);
        start_flag = true;
        start_cv.notify_all();
//        std::cout << "All threads started\n";
    }

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (auto& thread : threads) {
        thread.join();
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Duration: " << duration.count() << " seconds" << std::endl;

	EXPECT_EQ(sum, N * (N - 1) / 2);
}

// Same, with STL std::atomic_compare_exchange_strong
TEST(CAS, stl) {
    uint64_t N = 1'000'000;
    uint64_t sum = 0;
    int threads_count = 10;

    std::mutex start_mtx;
    std::condition_variable start_cv;
    bool start_flag = false;
    std::atomic<int> ready_count = 0;

    std::vector<std::thread> threads;
    for (int t = 0; t < threads_count; t++) {
        threads.emplace_back([t, N, threads_count, &sum, &start_mtx, &start_flag, &start_cv, &ready_count]() {
            ready_count++;
            {
                std::unique_lock<std::mutex> lock(start_mtx);
                start_cv.wait(lock, [&]() { return start_flag; });

//                std::cout << "Thread " << t << " started, Range: [" << t * N / threads_count << ", " << (t + 1) * N / threads_count << ")\n";
            }

            for (int i = t * N / threads_count; i < (t + 1) * N / threads_count; i++) {
                int64_t expected, desired;
                do {
                    expected = sum;
                    desired = expected + i;
                } while (!std::atomic_compare_exchange_strong((std::atomic<int64_t>*)&sum, &expected, desired));
            }
            });
    }

    while (ready_count.load() < threads_count) {
        std::this_thread::yield();
    }

    {
        std::unique_lock<std::mutex> lock(start_mtx);
        start_flag = true;
        start_cv.notify_all();
//        std::cout << "All threads started\n";
    }

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (auto& thread : threads) {
        thread.join();
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Duration: " << duration.count() << " seconds" << std::endl;

    EXPECT_EQ(sum, N * (N - 1) / 2);
}
