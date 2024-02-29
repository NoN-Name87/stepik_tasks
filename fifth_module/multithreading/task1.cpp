#include <vector>
#include <iterator>
#include <utility>

template<class Iter, typename T, typename U>
auto map_reduce(Iter p, Iter q, T f1, U f2, size_t threads) -> decltype(f2(f1(*p), f1(*p)))
{
    using ResType = decltype(f2(f1(*p), f1(*p)));
    std::vector<std::future<ResType>> results(threads);
    size_t iter_len = std::distance(p, q);
    size_t num_part = iter_len / threads;

    auto func = [&f1, &f2](Iter beg, Iter end)
    {
        ResType result = f1(*beg);
        while(++beg != end)
            result = f2(result, f1(*beg));
        return result;       
    };

    auto curr = p;
    for(auto & i : results)
    {
        auto next = curr;
        size_t distance = std::distance(next, q);
        distance == num_part + iter_len % threads ?  
        std::advance(next, distance) : std::advance(next, num_part);
        i = std::async(std::launch::async, func, curr, next);
        std::advance(curr, num_part);
    }

    ResType total_result = results[0].get();
    for(size_t i = 1; i < threads; i++)
    {
        total_result = f2(total_result, results[i].get());
    }
    return total_result;
}