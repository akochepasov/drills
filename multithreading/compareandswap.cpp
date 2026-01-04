//#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h> // For InterlockedCompareExchange

#include <mutex>
#include <thread>
#include <chrono>
#include <gtest/gtest.h>

template<typename T>
bool compare_and_swap_raw(std::mutex &cas_mutex, T *a, T expected, T desired) {
	std::lock_guard lock(cas_mutex);

    T orig_a = *a;
    if (orig_a != expected)
        return false;

    *a = desired;
    return true;
}


class SpinLockCAS {
    private:
        std::mutex cas_mutex;
        int32_t _spinValue = 0;
        bool _available = true;
    public:
		SpinLockCAS() : _spinValue(0), _available(true) {}
		SpinLockCAS(int32_t spin_) : _spinValue(spin_), _available(true) {}

        void lock() {
            while (!compare_and_swap_raw(cas_mutex, &_available, true, false));
        }

        bool try_lock() {
            auto spin = _spinValue;
            while (!compare_and_swap_raw(cas_mutex, &_available, true, false) && --spin) _mm_pause();

            auto timed_out = (spin == 0);
            if (timed_out) {
                std::cout << "Spinlock timed out\n";
            }
            return !timed_out;
        }

        void unlock() {
            _available = true;
        }
};

TEST(SpinLock, CAS) {
    int threads_count = 100;
    std::vector<std::thread> threads;

    std::mutex start_mtx;
    std::condition_variable start_cv;
    bool start_flag = false;
    std::atomic<int> ready_count{ threads_count };

    // Lock under test
    SpinLockCAS spinLock(1000);

    for (int t = 0; t < threads_count; t++) {
        threads.emplace_back([=, &start_mtx, &start_flag, &start_cv, &ready_count, &spinLock]() {
            ready_count--;
            {   
                start_cv.wait(std::unique_lock(start_mtx), [&]() { return start_flag; });
//                std::cout << "Thread " << t << " started\n";
            }

//            spinLock.lock();
            while (!spinLock.try_lock());
            //{
            //    std::this_thread::yield();
            //}

            spinLock.unlock();
         });
    }

    while (ready_count) {
        std::this_thread::yield();
    }

    {
//        std::cout << "All threads started\n";
        std::unique_lock lock(start_mtx);
        start_flag = true;
        start_cv.notify_all();
    }

    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    for (auto& t : threads) {
        t.join();
    }

    typedef milliseconds ms;
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> duration = end - start;
    std::cout << "Duration: " << duration_cast<ms>(duration).count() << " ms" << std::endl;
}


TEST(CAS, raw) {
    uint64_t N = 1'000'000;
	uint64_t sum = 0;
    int threads_count = 10;

	std::mutex start_mtx;
	std::condition_variable start_cv;
	bool start_flag = false;
    std::atomic<int> ready_count{ threads_count };

    std::mutex cas_mutex;
    std::vector<std::thread> threads;
    for (int t = 0; t < threads_count; t++) {
        threads.emplace_back([=, &start_mtx, &start_flag, &start_cv, &ready_count, &sum, &cas_mutex]() {
            ready_count--;
            {
                std::unique_lock<std::mutex> lock(start_mtx);
                start_cv.wait(lock, [&]() { return start_flag; });
//                std::cout << "Thread " << t << " started, Range: [" << t * N / threads_count << ", " << (t + 1) * N / threads_count << ")\n";
            }

            for (int i = t * N / threads_count; i < (t + 1) * N / threads_count; i++) {
                int64_t expected;
                do {
                    expected = sum;
                } while (!compare_and_swap_raw(cas_mutex, (int64_t*)&sum, expected, expected + i));
                //} while (!std::atomic_compare_exchange_strong((std::atomic<int64_t>*) & sum, &expected, expected + i));  // 10% faster
                //} while (InterlockedCompareExchange((uint64_t*)&sum, expected + i, expected) != expected); // 2x faster
            }
        });
	}

    while (ready_count.load()) {
        std::this_thread::yield();
    }

    {
//        std::cout << "All threads started\n";
        std::unique_lock<std::mutex> lock(start_mtx);
        start_flag = true;
        start_cv.notify_all();
    }

    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    for (auto& thread : threads) {
        thread.join();
    }

    typedef milliseconds ms;
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> duration = end - start;
    std::cout << "Duration: " << duration_cast<ms>(duration).count() << " ms" << std::endl;

	EXPECT_EQ(sum, N * (N - 1) / 2);
}

