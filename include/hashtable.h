
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <map>


template <typename T1, typename T2> 
class Table
{
private:
    std::map<T1, T2> _table;

public:

    // implements the iterator for the class
    class iterator: public std::iterator<
                            std::bidirectional_iterator_tag,
                            std::pair<T1, T2> >
    {
    private:
        std::map::iterator _current;
    
    public:
        iterator(std::map::iterator start) : _current(start) {}

        // overloaded operators
        iterator& operator++() {++_current; return *this}
        iterator& operator--() {--_current; return *this}
        std::map::iterator& operator->() {return _current}
        bool operator==(iterator& other) {return _current->first == other->first && _current->second == other->second}
        bool operator!=(iterator& other) {return !(_current->first == other->first && _current->second == other->second)}
    };

    iterator& begin();
    iterator& end();



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


#endif