#include <typeinfo>
#include <typeindex>
#include <map>
#include <functional>
// Base - базовый класс иерархии
// Result - тип возвращаемого значения мультиметода
// Commutative - флаг, который показывает, что
// мультиметод коммутативный (т.е. f(x,y) = f(y,x)).
using pair_type = std::pair<std::type_index, std::type_index>;

template<class Base, class Result, bool Commutative>
struct Multimethod2
{
    void addImpl(const std::type_info & t1, const std::type_info & t2, std::function<Result (Base *, Base *)> f )
    {
        auto type_pair = pair_type(std::type_index(t1), std::type_index(t2));
        funcs[type_pair] = f;
    }
    bool check_class(Base * a, Base * b) const
    {
        std::type_index a_index{typeid(*a)};
        std::type_index b_index{typeid(*b)};
        auto iter = funcs.find(pair_type(std::move(a_index), std::move(b_index)));
        if(iter != funcs.end())
        {
            return true;
        }
        return false;
    }
    bool hasImpl(Base * a, Base * b) const
    {
        bool flag = check_class(a, b);
        if(Commutative && !flag)
        {
            flag = check_class(b, a);
        }
        return flag; 
    }
    Result call(Base * a, Base * b) const
    {
        std::type_index a_index{typeid(*a)};
        std::type_index b_index{typeid(*b)};
        if(!check_class(a, b) && Commutative)
        {
            return funcs.find(pair_type(std::move(b_index), std::move(a_index)))->second(b, a);
        }
        return funcs.find(pair_type(std::move(a_index), std::move(b_index)))->second(a, b);
    }
    std::map<pair_type, std::function<Result (Base *, Base *)>> funcs;
};