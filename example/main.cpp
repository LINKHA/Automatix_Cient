#include <iostream>
#include <coroutine>

struct generator {
    struct promise_type {
        int current_value;

        generator get_return_object() {
            return generator(std::coroutine_handle<promise_type>::from_promise(*this));
        }

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }

        void return_void() {}

        void unhandled_exception() { throw; }
    };

    std::coroutine_handle<promise_type> coroutine;

    explicit generator(std::coroutine_handle<promise_type> coroutine_) : coroutine(coroutine_) {}

    ~generator() {
        if (coroutine) {
            coroutine.destroy();
        }
    }

    bool move_next() {
        coroutine.resume();
        return !coroutine.done();
    }

    int current_value() const {
        return coroutine.promise().current_value;
    }
};

generator generate_range(int start, int end) {
    for (int i = start; i <= end; ++i) {
        co_yield i;
    }
}

int main() {
    generator g = generate_range(1, 5);

    while (g.move_next()) {
        std::cout << g.current_value() << " ";
    }
    return 0;
}
