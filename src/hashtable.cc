/*
 * File: hashtable.cc
 * Author: Joshua Jacobs-Rebhun
 * Date: May 20, 2022
 * 
 * This file implements the table datatype for storing builtin functions and jobs.
 */


#include <hashtable.h>


Table::Table()
{

}


Table::~Table()
{

}


void Table::insert(T1 key, T2 value)
{
    _table.insert({key, value});
}


T2 Table::get(T1 key)
{
    return _table[key];
}


void Table::remove(T1 key)
{
    _table.erase(key);
}


T2 Table::extract(T1 key)
{
    T2 value = _table[key];
    _table.erase(key);
    return value;
}