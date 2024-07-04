#ifndef CACHE_HPP
#define CACHE_HPP

#include <cstdint>
#include <exception>
#include <vector>

class cache {
    uint64_t _n_sets;
    uint64_t** _lines;
    uint64_t _n_ways;
    uint64_t _line_size;
    uint64_t _bit_mask;
    uint64_t _set_idx_shift = 0;

    struct node {
        node* prev;
        node* next;
        uint64_t tag;
    };

    struct set {
        node* head = NULL;
        node* tail = NULL;
        uint64_t size = 0;
    };

    set* _sets;
public:
    /**
     * @brief Constroi uma nova cache
     * 
     * @param cache_size tamanho da cache, em bytes
     * @param line_size  tamanho de cada linha da cache
     * @param n_ways     numero de vias (n_ways == cache_size: mapeamento direto; n_ways == 1: totalmente associativo)
     */
    cache(uint64_t cache_size, uint64_t line_size, uint64_t n_ways);

    /**
     * @brief tentar acessar uma posicao da memoria via a cache
     * 
     * @param addr endereco da memoria principal
     * @return true: cache hit;
     * @return false: cache miss
     */
    bool access(uint64_t addr);

    /**
     * @brief imprime um conjunto
     * 
     * @param set indice do conjunto
     */
    void print_set(uint64_t set);

    /**
     * @brief retorna o valor da tag de um endereco
     * 
     * @param addr endereco
     * @return uint64_t -- valor da tag
     */
    uint64_t get_tag_num(uint64_t addr);

    /**
     * @brief retorna o indice do conjunto ao qual "addr" pertencera
     * 
     * @param addr endereco
     * @return uint64_t -- indice do conjunto
     */
    uint64_t get_set_num(uint64_t addr);

    /**
     * @brief retorna o indice da palavra dentro da linha de cache
     * 
     * @param addr endereco
     * @return uint64_t -- indice da palavra
     */
    uint64_t get_word_num(uint64_t addr);

    /**
     * @brief Destrutor do objeto cache
     * 
     */
    ~cache();
};

#endif