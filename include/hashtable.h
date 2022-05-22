
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <map>


template <typename T1, typename T2> 
class Table
{
private:
    std::map<T1, T2> _table;

public:

    // constructor and destructor
    Table();
    ~Table();

    // modifiers
    void insert(T1 key, T2 value);
    T2 get(T1 key);
    void remove(T1 key);
    T2 extract(T1 key);

    std::map<T1, T2>& get_table();
};


#endif