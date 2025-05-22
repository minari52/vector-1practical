#include "../include/Vector.h"

// деструктор 
Vector::~Vector(){
    delete[] _data;
    _size = 0;
    _capacity = 0;
};


    // конструкторы с параметрами
Vector::Vector(const ValueType* rawArray, const size_t size, float coef)
    : _size(size), _capacity(size),
    _multiplicativeCoef(coef)
{
    if (size > 0){

        _data = new ValueType[_capacity];
        for (size_t i = 0; i < size; i++){
            _data[i] = rawArray[i];
        };
    }
};

    // конструктор копирования
    // \param other - копируемый объект
Vector::Vector(const Vector& other)
    : _size(other._size), _capacity(other._size),
    _multiplicativeCoef(other._multiplicativeCoef)

{
    if (_size == 0){
        _data = nullptr;
        return;
    };

    _data = new ValueType[_size];
    for (size_t i = 0; i < _size; i++ ){
        _data[i] = other._data[i];
    }
};

    // оператор присваивания копирвоания
    // \param other - копируемый объект
Vector& Vector::operator=(const Vector& other)
{
    if (this == &other){
        return *this;
    }


    delete[] _data;
    _size = other._size;
    _capacity = other._size;
    _multiplicativeCoef = other._multiplicativeCoef;

    if (_size > 0){
        _data = new ValueType[_size];
        for (size_t i = 0; 0 < other._size; i++){
            _data[i] = other._data[i];
        };
    }
    else{
        _data = nullptr;
    };
    return *this;
};
    // конструктор перемещения
    // \param other - перемещаемый объект
Vector::Vector(Vector&& other) noexcept
    :_data(other._data), _size(other._size), _capacity(other._capacity),
    _multiplicativeCoef(other._multiplicativeCoef)
{
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
};

    // оператор присваивания перемещением
    // \param other - перемещаемый объект
Vector& Vector::operator=(Vector&& other) noexcept
{
    if (this != &other){
        clear();

        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        _multiplicativeCoef = other._multiplicativeCoef;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    };
    return *this;
};

// ======================================================================================================================
// оператор

    // доступ к элементу по индексу
ValueType& Vector::operator[](size_t idx){
    if (_size <= idx) {std::cout << "индекс не подходит\n";};
    return _data[idx];
};

    // доступ к элементу по индексу
const ValueType& Vector::operator[](size_t idx) const{
    if (_size <= idx) {std::cout << "индекс не подходит\n";};
    return _data[idx];
};

// оператор
// ======================================================================================================================
// итератор

Vector::Iterator::Iterator(ValueType* ptr) : _ptr(ptr) {}

ValueType& Vector::Iterator::operator*(){
    return *_ptr;
};

const ValueType& Vector::Iterator::operator*() const{
    return *_ptr;
};

ValueType* Vector::Iterator::operator->(){
    return _ptr;
};

const ValueType* Vector::Iterator::operator->() const{
    return _ptr;
};
Vector::Iterator Vector::Iterator::operator++(){
    ++_ptr;
    return *this;
};
Vector::Iterator Vector::Iterator::operator++(int){
    Iterator tmp = *this;
    ++_ptr;
    return *this;
};
bool Vector::Iterator::operator==(const Iterator& other) const{
    return _ptr == other._ptr;
};
bool Vector::Iterator::operator!=(const Iterator& other) const{
    return _ptr != other._ptr;
};

    // получить итератор из начала вектора
Vector::Iterator Vector::begin(){
    return Iterator(_data);
};

    // получить итератор на эл-т вектора, следующий за последним
Vector::Iterator Vector::end(){
    return Iterator(_data + _size);
};

// итератор
// ======================================================================================================================
// функция

void Vector::printVec() const{  // HELPERS Вывод вектора
    std::cout << "{ ";
    for (size_t i = 0; i < _size; ++i){
        std::cout << _data[i];
        if (i != _size - 1){
            std::cout << ", ";
        }
    }
    std::cout << " }" << std::endl;
}

void Vector::clear(){
    delete[] _data;
    _data = nullptr;
    _size = 0;
}

size_t Vector::size() const{ // кол-во элементов вектора
    return _size;
};


size_t Vector::capacity() const{ // сколько памяти занимает вектор
    return _capacity;
};

double Vector::loadFactor() const{ // фактор загруженности
    if (_capacity == 0) {return 0.0;};
    return double(_size)/ double(_capacity);
};

void Vector::shrinkToFit(){ // уменьшение памяти до size
    if (_size == _capacity) {return;}
    
    if (_size == 0){
        clear();
    }

    ValueType* new_data = new ValueType[_size];
    for (size_t i = 0; i < _size; i++){
        new_data[i] = _data[i];
    };

    delete[] _data;
    _data = new_data;
    _capacity = _size;

};

void Vector::pushBack(const ValueType& value){ // добавление элемента в конец вектор
    if (_size >= _capacity){
        size_t new_capacity = (_capacity == 0) ? 4 : static_cast<size_t>(_capacity * _multiplicativeCoef);

        ValueType* new_data = new ValueType[new_capacity];

        for (size_t i = 0; i < _size; i++){
            new_data[i] = _data[i];
        }
        delete[] _data;
        _data = new_data;
        _capacity = new_capacity;
    }
    _data[_size] = value;
    ++_size;
};

void Vector::pushFront(const ValueType& value){ // добавление элемента в начало вектора
    if (_size >= _capacity){
        size_t new_capacity = (_capacity == 0) ? 4 : static_cast<size_t>(_capacity * _multiplicativeCoef);

        ValueType* new_data = new ValueType[new_capacity];

        for (size_t i = 0; i < _size; ++i){
            new_data[i + 1] = _data[i];
        }

        delete[] _data;
        _data = new_data;
        _capacity = new_capacity;
    }
    else{
        for (size_t i = _size; i > 0; --i){
            _data[i] = _data[i - 1];
        }    
    }
    _data[0] = value;
    ++_size;
};

void Vector::insert(const ValueType& value, size_t pos){ // вставка элемента value в позицию pos
    if (pos > _size){
        std::cout << "ошибка (pos > _size)\n";
        return;
    }
    if (_size >= _capacity){
        size_t new_capacity = (_capacity == 0) ? 1 : static_cast<size_t>(_capacity * _multiplicativeCoef);

        ValueType* new_data = new ValueType[new_capacity];

        for (size_t i = 0; i < pos; i++){
            new_data[i] = _data[i];
        }

        new_data[pos] = value;

        for (size_t i = pos; i < _size; i++){
            new_data[i + 1] = _data[i];
        }
        delete[] _data;
        _data = new_data;
        _capacity = new_capacity;
    }

    else{
        for (size_t i = _size; i > pos; --i){
            _data[i] = _data[i - 1];
        }
        _data[pos] = value;
    }
    _size++;
};

void Vector::insert(const ValueType* values, size_t size, size_t pos){ // вставка массива элементов типа ValueType в позицию pos
    if (pos > _size){
        std::cout << "ошибка (pos > _size)\n";
        return;
    }
    if (size == 0){
        return;
    }
    if (_size + size >= _capacity){
        size_t new_capacity = std::max(_size + size, static_cast<size_t>(_capacity * _multiplicativeCoef));

        ValueType* new_data = new ValueType[new_capacity];

        for (size_t i = 0; i < pos; ++i){
            new_data[i] = _data[i];
        }

        for (size_t i = 0; i < size; ++i){
            new_data[pos + i] = values[i];
        }

        for (size_t i = pos; i < _size; ++i){
            new_data[i + size] = _data[i];
        }
        delete[] _data;
        _data = new_data;
        _capacity = new_capacity;
    }
    else{
        for (size_t i = pos; pos < _size; ++i){ // сдвигаем от pos до size
            _data[i + size] = _data[i];
        }
        for (size_t i = 0; i < size; ++i){ // вставляем
            _data[pos + i] = values[i];
        }
    }
    _size += size;
};

void Vector::insert(const Vector& vector, size_t pos){ //Вставка содержимого вектора в позицию pos
    if (pos > _size){
        std::cout << "ошибка (pos > _size)\n";
        return;
    }
    if (vector.size() == 0){
        return;
    }
    if (_size + vector.size() >= _capacity){
        size_t tmp_size = _size + vector.size(); //tmp_size

        size_t new_capacity = std::max(tmp_size, static_cast<size_t>(_capacity * _multiplicativeCoef));

        ValueType* new_data = new ValueType[new_capacity];

        for (size_t i = 0; i < pos; ++i){ // копируем до pos
            new_data[i] = _data[i];
        }

        for (size_t i = 0; i < vector.size(); ++i){ // вставляем
            new_data[pos + i] = vector._data[i]; 
        }

        for (size_t i = pos; i < _size; ++i){ // копируем после pos до конца
            new_data[i + vector.size()] = _data[i];
        }
        delete[] _data;
        _data = new_data;
        _capacity = new_capacity;
    }
    else{
        for (size_t i = pos; pos < _size; ++i){ // сдвигаем от pos до size
            _data[i + vector.size()] = _data[i];
        }
        for (size_t i = 0; i < vector.size(); ++i){ // вставляем
            _data[pos + i] = _data[i];
        }
    }
    _size += vector.size();
};

void Vector::popBack(){ // удаление из конца
    if (_size == 0){
        throw std::out_of_range("нельзя удалить элемент из пустого вектора");
    }
    --_size;
}

void Vector::popFront(){ // удаление из начала
    if (_size == 0){
        throw std::out_of_range("нельзя удалить элемент из пустого вектора");
    }

    for (size_t i = 1; i < _size; ++i){
        _data[i - 1] = _data[i];
    }
    --_size;
};


/*!
    \brief удалить элемент(ы) из вектора
    \param pos: позиция начала удаляемого участка
    \param count: количество удаляемых элементов
    если (pos + count) > size, то элементы интервала [pos, size) должны быть удалены
*/
void Vector::erase(size_t pos, size_t count){
    if (pos > _size){
        std::cout << "ошибка (pos > _size)";
        return;
    }
    if (count > _size - pos){ // количество удаляемых жлементов больше их количества ? уменьшаем count
        count = _size - pos;
    }
    if (_size == 0){
        return;
    }

    for (size_t i = pos; i < _size - count; ++i){
        _data[i] = _data[i + count];
    }
    _size -= count;
};

/*!
    \brief Удалить элемент(ы) из вектора
    \param beginPos: позиция начала удаляемого участка
    \param endPos: позиция окончания удаляемого участка
    Все элементы интервала [beginPos, endPos) должны быть удалены:
        [1, 2, 3, 4] -> erase(1, 3) -> [1, 4]
    если endPos > size, то элементы интервала [beginPos, size) должны быть удалены
*/
void Vector::eraseBetween(size_t beginPos, size_t endPos){
    if (_size == 0) {return;}
    if (beginPos > _size || beginPos > endPos){
        std::cout << "ошибка (beginPos > _size || beginPos > endPos)\n";
        return;
    }
    if (endPos > _size){
        endPos = _size;
    }

    size_t count = endPos - beginPos;
    for (size_t i = beginPos; i < _size - count; ++i){
        _data[i] = _data[i + count];
    }
    _size -= count;
};


long long Vector::find(const ValueType& value) const{ // поиск первого вхождения значения
    if (_size == 0){
        std::cout << "ошибка (_size == 0)\n";
        return -1;
    }
    for (size_t i = 0; i < _size; ++i){
        if (_data[i] == value){
            return static_cast<long long>(i);
        }
    }
    std::cout << "элемента нет в векторе\n";
    return -1;
};


void Vector::reserve(size_t capacity){ // Если capacity > _capacity, то выделить новый участок памяти размером capacity и перенести вектор туда, иначе - ничего
    if (capacity > _capacity){
        ValueType* new_data = new ValueType[capacity];
        for (size_t i = 0; i < _size; ++i){
            new_data[i] = _data[i];
        }
        delete[] _data;
        _data = new_data;
        _capacity = capacity;
    }
    else {return;}
};






