template<int a, int b>
struct Plus
{
    static constexpr int value = a + b;
};

template<int a, int b>
struct Minus
{
    static constexpr int value = a - b;
};

template<typename T>
class Quantity
{
public:
    Quantity() : value_{0}{};
    explicit Quantity(double value) : value_(value) {}
    double value() const { return value_; }
    double & value() { return value_; }
    const Quantity<T> operator*(double val) const 
    {
        return Quantity<T>(value_ * val);
    }
    const Quantity<T> operator/(double val) const
    {
        return Quantity<T>(value_ / val);
    }
    const Quantity<T> operator+(const Quantity<T> & right)
    {
        return Quantity<T>(value_ + right.value());
    } 
    const Quantity<T> operator-(const Quantity<T> & right)
    {
        return Quantity<T>(value_ - right.value());
    } 
private:
    double value_;
};

template<typename T, typename U>
const auto operator*(const Quantity<T> & left, const Quantity<U> & right) -> decltype(Quantity<typename Zip<T, U, Plus>::type>())
{
    return Quantity<typename Zip<T, U, Plus>::type>(left.value() * right.value());   
}

template<typename T, typename U>
const auto operator/(const Quantity<T> & left, const Quantity<U> & right) -> decltype(Quantity<typename Zip<T, U, Minus>::type>())
{
    return Quantity<typename Zip<T, U, Minus>::type>(left.value() / right.value());
}

template<typename T>
const Quantity<T> operator*(double val, const Quantity<T> & right)
{
    return Quantity<T>(val * right.value());
}

template<typename T>
const auto operator/(double val, const Quantity<T> & right) -> decltype(Quantity<typename Zip<Dimension<>, T, Minus>::type>())
{
    return Quantity<typename Zip<Dimension<>, T, Minus>::type>(val / right.value());
}