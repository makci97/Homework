#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

const int HOW_FAR = 100;

class semaphore
{
public:
    semaphore(size_t c): count(c) {}
    semaphore(const semaphore& other) = delete;

    void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while(count == 0)
        {
            signal_cv.wait(lock);
        }
        count -= 1;
    }

    void signal()
    {
        std::lock_guard<std::mutex> lock(mtx);
        count += 1;
        signal_cv.notify_one();
    }

private:
    std::mutex mtx;
    std::condition_variable signal_cv;
    std::atomic<size_t> count;
};

enum leg {left, right};

class robot_semaphore
{
public:
    robot_semaphore(): _left(1), _right(0) {}

    void step(leg l)
    {
        semaphore& this_leg = (l == left ? _left : _right);
        semaphore& other_leg = (l == left ? _right : _left);

        other_leg.wait();
        std::cout << ((l == left)? "left": "right") << std::endl;
        this_leg.signal();
    }

private:
    semaphore _left;
    semaphore _right;
};

class robot_cv
{
public:
    robot_cv(): _who_next(right) {}

    void step(leg l)
    {
        std::unique_lock<std::mutex> lock(_mtx);

        while(_who_next.load() != l)
        {
            _cv.wait(lock);
        }

        std::cout << ((l == left)? "left": "right") << std::endl;
        _who_next.store((l == left)? right: left);
        _cv.notify_one();
    }

private:
    std::mutex _mtx;
    std::condition_variable _cv;
    std::atomic<leg> _who_next;
};

template<typename T>
void step_func(T &robot, leg my_favorite_leg)
{
    for(int i = 0; i < HOW_FAR; ++i)
    {
        robot.step(my_favorite_leg);
    }
}

int main()
{
    std::cout << "Robot with semaphore" << std::endl;

    robot_semaphore i_am_robot;
    std::thread right_leg(step_func<robot_semaphore>, std::ref(i_am_robot), right);
    std::thread left_leg(step_func<robot_semaphore>, std::ref(i_am_robot), left);

    right_leg.join();
    left_leg.join();


    std::cout << "Robot with cv" << std::endl;

    robot_cv i_am_robot_too;
    right_leg = std::thread(step_func<robot_cv>, std::ref(i_am_robot_too), right);
    left_leg = std::thread(step_func<robot_cv>, std::ref(i_am_robot_too), left);

    right_leg.join();
    left_leg.join();

    return 0;
}
