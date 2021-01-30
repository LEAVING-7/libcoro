#include "catch.hpp"

#include <coro/coro.hpp>

TEST_CASE("when_all single task with tuple container", "[when_all]")
{
    auto make_task = [](uint64_t amount) -> coro::task<uint64_t> { co_return amount; };

    auto output_tasks = coro::sync_wait(coro::when_all(make_task(100)));
    REQUIRE(std::tuple_size<decltype(output_tasks)>() == 1);

    uint64_t counter{0};
    std::apply([&counter](auto&&... tasks) -> void { ((counter += tasks.return_value()), ...); }, output_tasks);

    REQUIRE(counter == 100);
}

TEST_CASE("when_all multiple tasks with tuple container", "[when_all]")
{
    auto make_task = [](uint64_t amount) -> coro::task<uint64_t> { co_return amount; };

    auto output_tasks = coro::sync_wait(coro::when_all(make_task(100), make_task(50), make_task(20)));
    REQUIRE(std::tuple_size<decltype(output_tasks)>() == 3);

    uint64_t counter{0};
    std::apply([&counter](auto&&... tasks) -> void { ((counter += tasks.return_value()), ...); }, output_tasks);

    REQUIRE(counter == 170);
}

TEST_CASE("when_all single task with vector container", "[when_all]")
{
    auto make_task = [](uint64_t amount) -> coro::task<uint64_t> { co_return amount; };

    std::vector<coro::task<uint64_t>> input_tasks;
    input_tasks.emplace_back(make_task(100));

    auto output_tasks = coro::sync_wait(coro::when_all(input_tasks));
    REQUIRE(output_tasks.size() == 1);

    uint64_t counter{0};
    for (const auto& task : output_tasks)
    {
        counter += task.return_value();
    }

    REQUIRE(counter == 100);
}

TEST_CASE("when_all multple task withs vector container", "[when_all]")
{
    auto make_task = [](uint64_t amount) -> coro::task<uint64_t> { co_return amount; };

    std::vector<coro::task<uint64_t>> input_tasks;
    input_tasks.emplace_back(make_task(100));
    input_tasks.emplace_back(make_task(200));
    input_tasks.emplace_back(make_task(550));
    input_tasks.emplace_back(make_task(1000));

    auto output_tasks = coro::sync_wait(coro::when_all(input_tasks));
    REQUIRE(output_tasks.size() == 4);

    uint64_t counter{0};
    for (const auto& task : output_tasks)
    {
        counter += task.return_value();
    }

    REQUIRE(counter == 1850);
}