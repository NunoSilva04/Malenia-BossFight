/**********************************************************************************
*   File: n_vector.h
* 
*   Description: 
*       Dynamic array implementation. 
*                     
*   Constraints: 
*       pop_first() is very slow for very big arrays. Uses O(N) complexity.
* 
*   See: 
*       docs/core/n_vector.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __N_VECTOR_H__
#define __N_VECTOR_H__

#include <cstdlib>
#include <iostream>

namespace Core{

    template<typename T>
    class n_vector{
        public:
            n_vector(){ 
                size = 0;
                capacity = init_size;
                data = new T[capacity];
            }

            ~n_vector(){ 
                delete[] data;
            }

            //Copy constructor
            n_vector(const n_vector& other){
                this->capacity = other.capacity;
                this->size = other.size;
                this->data = new T[other.capacity];

                for(size_t i = 0; i < other.size; i++){
                    this->data[i] = other.data[i];
                }
            }
            
            //Copy assignment
            n_vector& operator=(const n_vector& other){
                if(this == &other) return *this;

                T *this_data = new T[other.capacity];
                for(size_t i = 0; i < other.size; i++){
                    this_data[i] = other.data[i];
                }
                delete[] this->data;

                this->data = this_data;
                this->size = other.size;
                this->capacity = other.capacity;

                return *this;
            }
            
            void print(void){
                for(size_t i = 0; i < size; i++){
                    std::cout << "id: " << i << " with value " << data[i] << '\n'; 
                }

                return;
            }

            void push_back(const T value){
                if(size == capacity){
                    capacity *= 2;
                    T *temp_data = new T[capacity];
                    for(size_t i = 0; i < size; i++){
                        temp_data[i] = data[i];
                    }
                    delete[] data;
                    data = temp_data;
                }

                data[size] = value;
                size += 1;
            
                return;
            }

            bool pop_first(void){
                if(size <= 0){
                    return false;
                }

                size_t new_size = size - 1;
                T *temp_data = new T[capacity];
                for(size_t i = 0; i < new_size; i++){
                    temp_data[i] = data[i + 1];
                }
                
                delete[] data;
                data = temp_data;
                size = new_size;

                return true;
            }

            bool pop_first(T *popped_value){
                if(size <= 0){
                    return false;
                }
                *popped_value = data[0];

                size_t new_size = size - 1;
                T *temp_data = new T[capacity];
                for(size_t i = 0; i < new_size; i++){
                    temp_data[i] = data[i + 1];
                }
                
                delete[] data;
                data = temp_data;
                size = new_size;

                return true;
            }

            void empty_vector(void){
                capacity = init_size;
                size = 0;
                delete[] data;
                data = new T[capacity];

                return;
            }

            bool is_empty(void){
                if(size <= 0){
                    return true;
                }

                return false;
            }

            size_t vector_size(void) const{
                return this->size;
            }

            size_t vector_capacity(void) const{
                return this->capacity;
            }

            T vector_data_id(size_t id) const{
                if(id >= size) return T{};

                return data[id];
            }

        private:
            T *data;
            size_t size;
            size_t capacity;
            static constexpr size_t init_size = 32; 
    };
}

#endif