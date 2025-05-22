#ifndef MyVector_H
#define MyVector_H


#include <iostream>
#include <memory>
#include <algorithm>
#include <limits>

template <typename T>
class MyVector{
    public: 
        typedef size_t size_type;              // Tipas dydžiui saugoti
        typedef T value_type;                  // Tipas vektoriaus elementui
        typedef T& reference;                  // Nuoroda į elementą
        typedef const T& const_reference;      // Konstanta nuoroda
        typedef T* iterator;                   // Paprastas iteratorius
        typedef const T* const_iterator;       // Konstanta iteratorius

    //KONSTRUKTORIAI
        MyVector() {create();} // Tuščias vektorius
        explicit MyVector(size_type n, const T& t = T{}) { create (n,t); } // Vektorius su n reikšmių // MyVector<int> v(3, 5); // {5,5,5}
        //copy constructor
        MyVector(const MyVector& v) { create(v.begin(), v.end()); } 
        //range constructor
        template <class InputIterator>  
        MyVector (InputIterator first, InputIterator last) { create(first,last); }
        //move constructor
        MyVector (MyVector&& v) {
            create();
            swap(v);
            v.uncreate();
        }
        //initializer list constructor // MyVector<int> v = {1, 2, 3};
        MyVector(const std::initializer_list<T> il) { create(il.begin(), il.end()); }

    // Destruktorius
        ~MyVector() {uncreate();}

    // operator = 
        //copy assignment
        MyVector& operator = (const MyVector& other) {
            if (this != &other) {
                uncreate();
                create(other.begin(), other.end());
            }
            return *this;
        };

        //move assignment    
        MyVector& operator = (MyVector&& other) noexcept {
            if (this != &other) {
                // Atlaisvina esamus resursus
                uncreate();
                // Sukeisti rodykles su pradiniu vektoriumi
                std::swap(dat, other.dat);
                std::swap(avail, other.avail);
                std::swap(limit, other.limit);
            }
            return *this;
        }

    // Iteratoriai
        iterator begin() {return dat;} // Pradžios iteratorius
        const_iterator begin() const {return dat;}
        iterator end() {return avail;} // Pabaigos iteratorius
        const_iterator end() const {return avail;}

    // talpa
        size_type size() const {return avail-dat;} // Esamas dydis ( v.size();)
        size_type max_size() const {return std::numeric_limits<size_type>::max();}  // Max dydis (v.max_size();)
        void resize(size_type sz) { // Pakeisti dydį (be reikšmės) (v.resize(10);)
            if (sz < size()) {
                iterator it = dat + sz;
                while (it != avail) {
                    alloc.destroy(it++);
                }
                avail = dat + sz;
            }
            else if (sz > capacity()) {
                grow(sz);
                std::uninitialized_fill(avail, dat + sz, value_type());
                avail = dat + sz;
            }
            else if (sz > size()) {
                std::uninitialized_fill(avail, dat + sz, value_type());
                avail = dat + sz;
            }
        }
        void resize(size_type sz, const value_type& value) { // Pakeisti dydį (su reikšme) (v.resize(5, 7);)
            if (sz > capacity()) {
                grow(sz);
            }
            
            if (sz > size()) {
                insert(end(), sz - size(), value);
            } else if (sz < size()) {
                avail = dat + sz;
            }
        }

        size_type capacity() const {return limit-dat;}  // Kiek capacity turi
        bool empty() const noexcept { return size() == 0;} // Ar vektorius tuscias
        void reserve (size_type n) { // rezervuoja vieta
            if (n > capacity()) {
                grow(n);
            }
        }
        void shrink_to_fit(){ // Sumazina talpa iki dydzio
            if (limit > avail) 
            limit = avail;
        }

    // Elemento access
        T& operator[] (size_type n) {return dat[n];} // Prieiga be ribų tikrinimo
        const T& operator[] (size_type n) const {return dat[n];} // cout << v[1];
        reference at (size_type n) { // tikrina ribas
            if (n >= size() || n < 0)
                throw std::out_of_range("Index out of range");
            return dat[n];
        }
        const_reference at (size_type n) const {
            if (n >= size() || n < 0)
                throw std::out_of_range("Index out of range");
            return dat[n];
        }
        reference front() { // Pirmas elementas
            return dat[0];
        };
        const_reference front() const {
            return dat[0];
        }
        reference back() {
            return dat[size() - 1]; // Paskutinis elementas
        }
        const_reference back() const {
            return dat[size() - 1];
        }
        value_type* data() noexcept { // Pointeris į duomenis
            return dat;
        }
        const value_type* data() const noexcept {
            return dat;
        }

    // Modifikatoriai
        template <class InputIterator>  
        void assign (InputIterator first, InputIterator last) { // Priskiria iš intervalo
            uncreate();
            create(first, last);
        }
        void assign (size_type n, const value_type& val) { // Priskiria n reikšmių (v.assign(4, 99);)
            uncreate();
            create(n, val);
        }
        void assign (std::initializer_list<value_type> il) { // Priskiria iš sąrašo (v.assign({1,2,3});)
            uncreate();
            create(il);
        }
        void push_back (const value_type& t) { // Prideda elementą
            if (avail==limit) 
                grow();
            unchecked_append(t);
        }
        void push_back (value_type&& val) { // Prideda (move)
            if (avail == limit)
                grow();
            unchecked_append(val);
        }
        void pop_back() { // Pasalina paskutini elementa
            if (avail != dat)
                alloc.destroy(--avail);
        }
        iterator insert(iterator pos, const T &value) { // Įterpia vieną elementą į nurodytą poziciją
            size_type index = pos - begin();
            size_type numNewElements = 1; // Kadangi įterpiame vieną elementą

            // Tikrinama ar dydi reik keist
            if (size() + numNewElements > capacity()) {
                reserve((size() + numNewElements) * 2);
            }

            // Perkelia elementus, kad atsirastų vietos naujam elementui
            std::move_backward(dat + index, avail, avail + numNewElements);
            // Insert nauja elementa
            dat[index] = value;
            // Update dydi
            avail += numNewElements;

            return dat + index; // Grazinti iteratoriu, rodanti i iterpta elementa
}
        iterator insert(iterator pos, size_type count, const T& value) { // Įterpia count reikšmių
            size_type index = pos - dat;
            if (size() + count > capacity()) {
            reserve(size() + count);
        }

        pos = dat + index;
        std::move_backward(pos, avail, avail + count);
        std::uninitialized_fill(pos, pos + count, value);
        avail += count;

        return pos;
}
        iterator erase(iterator position) { // Pašalina vieną elementą ( v.erase(v.begin()+3);)
            if (position < dat || position > avail) {
                throw std::out_of_range("Index'as isejo is riibu");
            }
            std::move(position + 1, avail, position);
            alloc.destroy(avail - 1);
            --avail;

            return position;
        }
        iterator erase(iterator first, iterator last) { // Pašalina intervalą (v.erase(v.begin()+2, v.begin()+5);)
            iterator new_available = std::uninitialized_copy(last, avail, first);

            iterator it = avail;
            while (it != new_available) {
                alloc.destroy(--it);
            }

            avail= new_available;
            return last;
        }

        void swap(MyVector& x) { // Sukeičia (swap)
            std::swap(dat, x.dat);
            std::swap(avail, x.avail);
            std::swap(limit, x.limit);
        }
        void clear() noexcept {  // Pašalina visus duomenis
            uncreate();
        }

    //RELATION OPERATORS
        bool operator== (const MyVector<T>& other) const { // Tikrina ar vektoriai lygūs
            if (size() != other.size()) {
                return false;
            }

            return std::equal(begin(), end(), other.begin());
        }
        bool operator!= (const MyVector<T>& other) const { // Nelygumo operatorius
            return !(*this == other);
        }
        bool operator < (const MyVector<T> & other) const { // Maziau
            return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }
        bool operator <= (const MyVector<T> & other) const { //Maziau arba lygu
            return !(other < *this);
        }
        bool operator > (const MyVector<T> & other) const { // Daugiau
            return std::lexicographical_compare(other.begin(), other.end(), begin(), end());
        }
        bool operator >= (const MyVector<T> & other) const { //Daugiau arba lygu
            return !(other > *this);
        }

        void swap (MyVector<T>& x, MyVector<T>& y) { // swap
            std::swap(x,y);
        }

        static int reallocations; 
        static void resetReallocations() { reallocations = 0; }// Nustato 0
        static int getReallocations() { return reallocations; } // Grazina kiek perskirstymu buvo


    private:
        iterator dat; //Prad. pointeris
        iterator avail; // Pirmas laisvas elementas
        iterator limit; // RIba pabaigos
        std::allocator<T> alloc; //atminties valdymas
        void create() {dat = avail = limit = nullptr;} // Tuscias prad.
        void create (size_type n, const T& val) { //sukuroia n elementu su reiksme
            dat = alloc.allocate(n);
            limit = avail = dat + n;
            std::uninitialized_fill(dat, limit, val);
        }
        void create(const_iterator i, const_iterator j) { // Sukuria iš intervalo
            dat = alloc.allocate(j - i);
            limit = avail = std::uninitialized_copy(i, j, dat);
        }
        void uncreate(){ // Atlaisvina
            if (dat) {
                iterator it = avail;
                while (it != dat) {
                    alloc.destroy(--it);
                }
            alloc.deallocate(dat, limit - dat);
            }
            dat = limit = avail = nullptr;
        }
        void grow(size_type new_capacity = 1) { //Didina capacity
            ++reallocations;
            size_type new_size = std::max(new_capacity, 2 * capacity());
            iterator new_data = alloc.allocate(new_size);
            iterator new_avail = std::uninitialized_copy(dat, avail, new_data);
            uncreate();
            dat = new_data;
            avail = new_avail;
            limit = dat + new_size;
        }
        void unchecked_append(const T& val) {
            alloc.construct(avail++, val);
        }
};

template <typename T>
int MyVector<T>::reallocations = 0;


#endif // MyVector_H