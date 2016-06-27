#include <algorithm>
#include <atomic>
#include <deque>
#include <string>
#include <vector>
#include <sstream>

#include "test_utils.h"
#include "spsc_ring_buffer.h"
//#include "ya_contest_sim.h"

namespace SPSCRingBufferTests {

#if defined(dump_field)
#   error "already defined"
#else
#   define dump_field(field) out << (#field) << "=" << (field)
#endif

    struct TestOpts {
        size_t n_elements = 10;
        size_t n_additions = 100000;

        std::string to_string() const {
            std::ostringstream out;
            dump_field(n_elements) << ", ";
            dump_field(n_additions);
            return out.str();
        }
    };

    struct NumHash {
        size_t operator()(size_t n) const {
            return n;
        }
    };

    struct TestContext {
        const TestOpts opts;
        spsc_ring_buffer<size_t> buffer;
        Barrier barrier_start;

        TestContext(const TestOpts& o)
            : opts(o)
            , buffer(o.n_elements)
            , barrier_start(2)
        {}
    };

    void do_test(const TestOpts& opts) {
        std::cout << "Testing with parameters: " << opts.to_string() << std::endl;

        std::vector<std::thread> threads;
        TestContext ctx(opts);

        std::cout << "Running workers ... " << std::endl;
        threads.emplace_back([&ctx] {
            ctx.barrier_start.wait();

            for (size_t i = 1; i <= ctx.opts.n_additions + ctx.opts.n_elements; ++i) {
                while (!ctx.buffer.enqueue(i)) {
                    std::this_thread::yield();
                }
                cdebug("added " << i);
            }
        });

        threads.emplace_back([&ctx] {
            ctx.barrier_start.wait();

            for (size_t i = 1; i <= ctx.opts.n_additions; ++i) {
                size_t result = -1;
                while (!ctx.buffer.dequeue(result)) {
                    std::this_thread::yield();
                }
                cdebug("checking " << i);
                test_assert(result == i, "expected to get " << i << " << as next result, got " << result);
            }
        });

        std::cout << "Done" << std::endl;

        std::cout << "Joining workers ... " << std::endl;
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        for (size_t i = 1; i <= ctx.opts.n_elements; ++i) {
            size_t result = -1;
            test_assert(ctx.buffer.dequeue(result), "could not get result");
            cdebug("checking " << i);
            test_assert(result == i + ctx.opts.n_additions, "expected to get " << i + ctx.opts.n_additions << " << as next result, got " << result);
        }

        std::cout << "Done" << std::endl;
        std::cout << "OK" << std::endl;
    }

    void run_tests(int argc, char* argv[]) {
        TestOpts opts;
        read_opts(argc, argv, opts.n_elements, opts.n_additions);
        do_test(opts);
    }
}

int main(int argc, char* argv[]) {
    SPSCRingBufferTests::run_tests(argc, argv);
}
