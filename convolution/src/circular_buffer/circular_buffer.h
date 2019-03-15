
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

template <typename T>
class CircularBuffer {
public:
  CircularBuffer(unsigned int arr_size);
  ~CircularBuffer();

  int size();
  void push(T element);
  T atIndex(int index);

private:
  T* arr;
  unsigned int arr_size;
  unsigned int zeroIndex = 0;
  unsigned int writeIndex = 0;
  bool filled = false;
};

template <class T>
CircularBuffer<T>::CircularBuffer(unsigned int arr_size) {
  arr = new T[arr_size];
  this->arr_size = arr_size;
}

template <class T>
CircularBuffer<T>::~CircularBuffer() {
  delete arr;
}

template <class T>
int CircularBuffer<T>::size() {
  return arr_size;
}

template <class T>
void CircularBuffer<T>::push(T element) {
  arr[writeIndex] = element;
  if(filled) {
    zeroIndex = (zeroIndex + 1) % arr_size;
  } else if(writeIndex == arr_size - 1) {
    filled = true;
  }
  writeIndex = (writeIndex + 1) % arr_size;
}

template <class T>
T CircularBuffer<T>::atIndex(int index) {
  if(index < 0 || index >= arr_size) {
    throw "IndexError: Index out of range.";
  } else {
    return arr[(zeroIndex + index) % arr_size];
  }
}

#endif
