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
        // using rawPtr = T[]*;
        using pointer = std::shared_ptr<T[]>;
        using value_type = T;
        using resize_type = std::function<size_type(size_type)>;
        /*[](size_type currentCapacity) -> size_type {
            return currentCapacity * 2;
        };*/

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
        size_type size() { return sizeTotal; }
        size_type capacity() { return capacityTotal; }

        class iterator
        {
          public:
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

          private:
            pointer m_data;
            size_type m_pos;
            // Does this work now??
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

        for (auto i{ index + 1}; i < sizeTotal; i++)
        {
            m_data[i - 1] = m_data[i];
        }
        sizeTotal -= 1;

    }

} // namespace usu