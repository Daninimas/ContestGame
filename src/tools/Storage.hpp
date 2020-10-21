#pragma once

#include <iterator>
#include <stdint.h>
#include <vector>
#include <unordered_map>
using namespace std;

template <class T>
class Storage {
public:

    using iterator = typename vector<T>::iterator;
    using const_iterator = typename vector<T>::const_iterator;

    //////////////////////////////////////////////////
    //              THE RULE OF THE 3               //
    //////////////////////////////////////////////////

    explicit Storage(const size_t max_cap) {
        inner_vector.reserve(max_cap);
        inner_map.reserve(max_cap);
    };
    Storage(const Storage<T> &s) = delete;
    ~Storage() = default;

    //////////////////////////////////////////////////
    //                  ITERATORS                   //
    //////////////////////////////////////////////////

    iterator begin() noexcept {
        return inner_vector.begin();
    }
    iterator end() noexcept {
        return inner_vector.end();
    }
    const_iterator begin() const noexcept {
        return inner_vector.begin();
    }
    const_iterator end() const noexcept {
        return inner_vector.end();
    }
    const_iterator cbegin() const noexcept {
        return inner_vector.cbegin();
    }
    const_iterator cend() const noexcept {
        return inner_vector.cend();
    }
    /*iterator find (const uint32_t &id) {
        return inner_map.find(id);
    }
    const_iterator find (const uint32_t &id) const {
        return inner_map.find(id);
    }*/

    //////////////////////////////////////////////////
    //                  CAPACITY                    //
    //////////////////////////////////////////////////

    size_t size() const noexcept {
        return inner_vector.size();
    }
    size_t sizeMap() const noexcept {
        return inner_map.size();
    }
    size_t capacity() const noexcept {
        return inner_vector.capacity();
    }

    bool empty() const noexcept {
        return inner_vector.empty();
    }

    size_t count (const int id ) const noexcept {
        return inner_map.count(id);
    }

    bool exists (const int id) const noexcept {
        if (inner_map.find(id) != inner_map.end())
            return true;
        return false;
    }

    //////////////////////////////////////////////////
    //               ELEMENT ACCESS                 //
    //////////////////////////////////////////////////

    vector<T> &getVector() noexcept {
        return inner_vector;
    }

    T &operator[](const size_t index) noexcept {
        return inner_vector[index];
    }

    T &at(const size_t index) {
        // if exists
        //if (exists(index))
        return inner_vector[index];
    }

    size_t &operator[](const int id) noexcept {
        return inner_map[id];
    }

    size_t &at(const int id) {
        return inner_map.at(id);
    }

    T &front() noexcept {
        return inner_vector.front();
    }

    T &back() noexcept {
        return inner_vector.back();
    }

    //////////////////////////////////////////////////
    //                  MODIFIERS                   //
    //////////////////////////////////////////////////

    void emplace(const int id) noexcept {
        if (inner_vector.size() < inner_vector.capacity()) {
            if (!inner_map.count(id)) {
                inner_map.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(inner_vector.size()));
                inner_vector.emplace_back(id);
            } //else
                //throw "";
        }//else
            //terminate();
    }

    void erase(const int id) noexcept {
        if (inner_map.count(id)) {
            size_t index = inner_map[id];
            // Me guardo el id del elemento ultimo, que se va a mover para luego cambiarlo en el mapa, esto lo hago porque si en algun momento el elemento movido es el mismo que el borrado, dara un core dumped, ya que se intenta acceder al id de un elemento borrado
            // Al final borro la posicion del vector al final para que no intente acceder en la linea de inner_map[inner_vector[index].id] = index; a una posicion no existente
            inner_vector[index] = inner_vector.back();
            inner_map[inner_vector[index].id] = index;
            inner_map.erase(id);
            inner_vector.pop_back();
        }
    }

    void clear() noexcept {
        inner_vector.clear();
        inner_map.clear();
    }

private:
    vector<T> inner_vector;
    unordered_map<int, size_t> inner_map;
};
