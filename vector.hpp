#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#pragma once
namespace usu
{

    template <typename T>

    class vector
    {

      public:
        using size_type = std::size_t;
        using reference = T&;
        using pointer = std::shared_ptr<T[]>;
        using value_type = T;
        using resize_type = std::function<size_type(size_type)>;

        resize_type function = [](size_type currentCapacity) -> size_type
        {
            return currentCapacity * 2;
        };
        vector() :
            capacityTotal(10), sizeTotal(0)
        { // m_data = pointer(new T[capacityTotal]);
            m_data = std::make_shared<T[]>(10);
        }

        vector(size_type size);

        vector(resize_type resize);

        vector(size_type size, resize_type resize);

        vector(std::initializer_list<T> list);

        vector(std::initializer_list<T> list, resize_type resize);

        reference operator[](size_type index);

        void add(T value);
        void remove(size_type index);
        void clear();
        void insert(size_type index, T value);
        size_type size() { return sizeTotal; }
        size_type capacity() { return capacityTotal; }

        class iterator
        {
          public:
            using iterator_category = std::forward_iterator_tag;
            iterator() :
                iterator(nullptr)
            {
            }

            iterator(pointer ptr) :
                m_data(ptr),
                m_pos(0)
            {
            }
            iterator(size_type pos, pointer ptr) :
                m_pos(pos),
                m_data(ptr)
            {
            }

            iterator(const iterator& obj); // Copy Constructable

            iterator(iterator&& obj) noexcept; // Move Constructable

            iterator& operator=(const iterator& rhs); // Copy Assignable

            iterator& operator=(iterator&& rhs); // Move Assignable

            iterator operator++(); // Pre increment

            iterator operator++(int); // Post increment

            bool operator!=(const iterator& rhs) { return m_pos != rhs.m_pos; } // != Operator
            bool operator==(const iterator& rhs) { return m_pos == rhs.m_pos; } // == Operator

            reference operator*() { return m_data[m_pos]; }

          private:
            pointer m_data;
            size_type m_pos;
        };

        iterator begin()
        {
            return iterator(m_data);
        }
        iterator end()
        {
            return iterator(sizeTotal, m_data);
        }

      private:
        pointer m_data;
        size_type sizeTotal;
        size_type capacityTotal;
    };

    template <typename T>
    vector<T>::vector(size_type size) :
        capacityTotal(10), sizeTotal(size)

    {
        if (size > 10)
        {
            capacityTotal = function(size);
        }
        m_data = std::make_shared<T[]>(capacityTotal);
    }

    template <typename T>
    void vector<T>::vector::add(T value)
    {
        if (sizeTotal + 1 > capacityTotal)
        {
            // Code for testing if memory allocation and deallocation removes old data

            /* std::cout << m_data << std::endl;
             auto* ptr = m_data.get();

             std::cout << "Yeah: " << *ptr <<  std::endl;
             std::cout << "Before: " << std::endl;
             for (int i = 0; i < 20; i++)
             {
                 std::cout << m_data[i] << std::endl;
             }*/
            // std::cout << m_data.use_count() << std::endl;

            // Re-allocate new array
            // TODO: rewrite capacity to use custom function given from user, or regular method.
            capacityTotal = function(capacityTotal);
            pointer newArray = std::make_shared<T[]>(capacityTotal);
            for (int i = 0; i < sizeTotal; i++)
            {
                newArray[i] = m_data[i];
            }
            m_data = newArray;
            // std::cout << m_data.use_count() << std::endl;

            // Code for testing if memory allocation and deallocation removes old data

            /*std::cout << "Yeah: " << *ptr << std::endl;
            std::cout << "After: " << std::endl;
            for (int i = 0; i < 20; i++)
            {
                std::cout << m_data[i] << std::endl;
            }*/
        }

        m_data[sizeTotal] = value;
        sizeTotal += 1;
        // if (sizeTotal > capacityTotal)
        //{
        //           capacityTotal *= 2;
        //           for (int i = 0; i < sizeTotal; i++)
        //           {
        //               //tempData[i] = m_data[i];
        //           }
        //           m_data = std::make_shared<T[]>(100);
        // }
    }
    template <typename T>
    vector<T>::vector(resize_type resize) :
        capacityTotal(10),
        sizeTotal(0)
    {
        function = resize;
        m_data = std::make_shared<T[]>(capacityTotal);
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list) :
        capacityTotal(10),
        sizeTotal(0)
    {
        m_data = std::make_shared<T[]>(capacityTotal);

        for (auto i{ list.begin() }; i != list.end(); i++)
        {
            add(*i);
        }
    }
    template <typename T>
    typename vector<T>::reference vector<T>::vector::operator[](vector<T>::size_type value)
    {
        if (value >= sizeTotal || value < 0)
        {
            throw std::range_error("Index out of range.");
        }
        return m_data[value];
    }
    template <typename T>
    vector<T>::vector(size_type size, resize_type resize) :
        sizeTotal(size),
        capacityTotal(10)

    {
        // I can attempt to use constructor delegation here, but function needs to be set to 'resize' before calling the constructor
        // A way we can get around this: Have the function not equal anything, and initialize it in the constructors that need it.
        function = resize;
        if (sizeTotal > 10)
        {
            capacityTotal = function(size);
        }
        m_data = std::make_shared<T[]>(capacityTotal);
    }
    template <typename T>
    vector<T>::vector(std::initializer_list<T> list, resize_type resize) :
        vector(list)
    {
        function = resize;
    }

    template <typename T>
    void vector<T>::vector::remove(size_type index)
    {
        if (index < 0 || index >= sizeTotal)
        {
            throw std::range_error("Index out of range.");
        }
        // Loop through rest of list to set items in correct positions
        // Example: List = [1,2,3,4,5,6,7]
        // remove(3);
        // List = [1,2,3,_,5,6,7]
        // Set the list to: [1,2,3,5,6,7]
        m_data[index] = 0;

        for (auto i{ index + 1 }; i < sizeTotal; i++)
        {
            m_data[i - 1] = m_data[i];
        }
        sizeTotal -= 1;
    }

    template <typename T>
    void vector<T>::vector::clear()
    {
        sizeTotal = 0;
    }

    template <typename T>
    void vector<T>::vector::insert(size_type index, T value)
    {
        if (index < 0 || index > sizeTotal)
        {
            throw std::range_error("Index out of range.");
        }

        // Start at where we are supposed to (the index), and shift everything to the right.
        // Example: List = [1,2,3,4,5,6,7,8,9,10]
        //                 [1,2,3,_,4,5,6,7,8,9,10]
        // insert(3,12);
        // List = [1,2,3,_,4,5,6,7,8,9,10]
        // List = [1,2,3,4,4,5,6,7,8,9,10]
        // List = [1,2,3,12,4,5,6,7,8,9,10]
        // T temp = m_data[index];

        T current = m_data[index];
        if (sizeTotal + 1 > capacityTotal)
        {
            capacityTotal = function(capacityTotal);
            pointer newArray = std::make_shared<T[]>(capacityTotal);
            for (int i = 0; i < sizeTotal; i++)
            {
                newArray[i] = m_data[i];
            }
            m_data = newArray;
        }
        for (auto i{ index }; i < (sizeTotal); i++)
        {
            if (i < sizeTotal - 1)
            {
                T next = m_data[i + 1];
                m_data[i + 1] = current;
                current = next;
            }
            else
            {
                m_data[i + 1] = current;
            }
        }

        m_data[index] = value;
        sizeTotal += 1;
        /*std::cout << "----------" << std::endl;

        for (int j = 0; j < sizeTotal; j++)
        {
            std::cout << m_data[j] << std::endl;
        }*/
    }

    // Copy Assignment (iterator)
    template <typename T>
    typename vector<T>::iterator& vector<T>::iterator::operator=(const iterator& rhs)
    {
        this->m_data = rhs.m_data;
        this->m_pos = rhs.m_pos;
        return *this;
    }

    // Move Assignment (iterator)
    template <typename T>
    typename vector<T>::iterator& vector<T>::iterator::operator=(iterator&& rhs)
    {
        if (this != &rhs)
        {
            std::swap(this->m_data, rhs.m_data);
            std::swap(this->m_pos, rhs.m_pos);
        }
        return *this;
    }

    // Copy constructable (iterator)
    template <typename T>
    vector<T>::iterator::iterator(const iterator& obj)
    {
        this->m_pos = obj.m_pos;
        this->m_data = obj.m_data;
    }

    // Move constructable (iterator)
    template <typename T>
    vector<T>::iterator::iterator(iterator&& rhs) noexcept
    {
        this->m_data = rhs.m_data;
        this->m_pos = rhs.m_pos;

        rhs.m_data = nullptr;
        rhs.m_pos = 0;
    }

    // Pre increment operator (++i)
    template <typename T>
    typename vector<T>::iterator vector<T>::iterator::operator++()
    {
        m_pos++;
        return *this;
    }

} // namespace usu