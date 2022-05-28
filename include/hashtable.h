
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <map>
#include <iterator>


template <typename T1, typename T2> 
class Table
{
private:
    std::map<T1, T2> _table;

public:

    // implements the iterator for the class

    typename std::map<T1,T2>::iterator begin();
    typename std::map<T1,T2>::iterator end();



    // constructor and destructor
    Table();
    ~Table();

    // data access
    T2 get(T1 key);
    T2 operator[](T1 key);

    // modifiers
    void insert(T1 key, T2 value);
    void remove(T1 key);
    T2 extract(T1 key);
};



template<typename T1, typename T2>
Table<T1, T2>::Table()
{

}

template<typename T1, typename T2>
Table<T1, T2>::~Table()
{

}


// iterator access methods

template<typename T1, typename T2>
typename std::map<T1,T2>::iterator Table<T1,T2>::begin()
{
    return _table.begin();
}

template<typename T1, typename T2>
typename std::map<T1,T2>::iterator Table<T1,T2>::end()
{
    return _table.end();
}



// element access functions. Both of these are equivalent

template<typename T1, typename T2>
T2 Table<T1, T2>::get(T1 key)
{
    return _table[key];
}


template<typename T1, typename T2>
T2 Table<T1, T2>::operator[](T1 key)
{
    return _table[key];
}


// modifier functions

template<typename T1, typename T2>
void Table<T1, T2>::insert(T1 key, T2 value)
{
    _table.insert({key, value});
}


template<typename T1, typename T2>
void Table<T1, T2>::remove(T1 key)
{
    _table.erase(key);
}


template<typename T1, typename T2>
T2 Table<T1, T2>::extract(T1 key)
{
    T2 value = _table[key];
    _table.erase(key);
    return value;
}


#endif