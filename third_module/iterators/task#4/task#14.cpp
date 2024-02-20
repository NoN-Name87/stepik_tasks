#include <vector>
#include <list>

template<class T>
class VectorList
{
private:
    using VectT  = std::vector<T>;
    using ListT = std::list<VectT>;

public:
    using value_type = T;

    VectorList() = default;
    VectorList(VectorList const &) = default;
    VectorList(VectorList &&) = default;

    VectorList & operator=(VectorList &&)     = default;
    VectorList & operator=(VectorList const &) = default;

    // метод, который будет использоваться для заполнения VectorList
    // гарантирует, что в списке не будет пустых массивов
    template<class It>
    void append(It p, It q); // определена снаружи
/*  {
        if (p != q)
            data_.push_back(VectT(p,q));
    } 
*/

    bool empty() const { return size() == 0; } 

    // определите метод size
    size_t size() const 
    {
        size_t vect_list_sz{0};
        for(const auto & i : data_)
        {
            vect_list_sz += i.size();
        }
        return vect_list_sz;
    }

    struct const_iterator : std::iterator<std::bidirectional_iterator_tag, const T>
    {
        typename ListT::const_iterator ls_it;
        typename VectT::const_iterator vec_it;
        const ListT * list_; 
        const_iterator() = default;
        const_iterator(const const_iterator &) = default;
        const_iterator(size_t n, const ListT * list) : list_(list)
        {
            if(n == 0)
            {
                ls_it = list->cbegin();
                vec_it = ls_it->cbegin();
            }
            else if(n == list->size())
            {
                ls_it = --list->cend();
                vec_it = ls_it->cend();
            }     
        }
        typename const_iterator::pointer operator->() const
        {
            return &*vec_it;
        }
        typename const_iterator::reference operator*() const
        {
            return *vec_it;
        }
        const_iterator & operator++()
        {
            if(ls_it != --list_->cend() && vec_it + 1 == ls_it->cend())
            {
                ls_it++;
                vec_it = ls_it->cbegin();
            }
            else
            {
                vec_it++;
            }
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator iter = *this;
            operator++();
            return iter;
        }
        const_iterator & operator--()
        {
            if(vec_it != ls_it->cbegin())
            {
                --vec_it;
            }
            else if(ls_it != list_->cbegin())
            {
                ls_it--;
                vec_it = ls_it->cend() - 1;
            }
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator iter = *this;
            operator--();
            return iter;
        }
        bool operator==(const const_iterator & iter) const
        {
            return (iter.ls_it == ls_it && iter.vec_it == vec_it);
        }
        bool operator!=(const const_iterator & iter) const
        {
            return !operator==(iter);
        }      
    };

    const_iterator begin() const { return !empty()? const_iterator(0, &data_) : const_iterator(); }
    const_iterator end() const { return !empty()? const_iterator(data_.size(), &data_) : const_iterator(); }
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    const_reverse_iterator rbegin() const { return static_cast<const_reverse_iterator>(end());}
    const_reverse_iterator rend() const { return static_cast<const_reverse_iterator>(begin()); }

private:
    ListT data_;
};