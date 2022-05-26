/*
 * File: hashtable.cc
 * Author: Joshua Jacobs-Rebhun
 * Date: May 20, 2022
 * 
 * This file implements the table datatype for storing builtin functions and jobs.
 */


#include <hashtable.h>


// constructor and destructor

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
Table<T1,T2>::iterator& Table<T1,T2>::begin()
{
    return Table<T1,T2>::iterator(_table.begin());
}

template<typename T1, typename T2>
Table<T1,T2>::iterator& Table<T1,T2>::end()
{
    return Table<T1,T2>::iterator(_table.end());
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