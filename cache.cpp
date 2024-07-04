#include "cache.hpp"
#include <string>
#include <iostream>

cache::cache(uint64_t cache_size, uint64_t line_size, uint64_t n_ways) {
	_line_size = line_size;
	uint64_t n_lines = cache_size/line_size; // a quantidade de linhas sera o tamanho da cache dividido pelo tamanho da linha
	if (cache_size % line_size != 0) {
		throw std::string("Cache nao eh divisivel pelo tamanho das linhas");
	}
		
	_n_ways = n_ways;
	_n_sets = n_lines/n_ways; // a quantidade de conjuntos sera a quantidade de linhas dividida pelo numero de vias
	if (n_lines % n_ways != 0) {
		throw std::string("Cache nao eh divisivel pela quantidade de vias");
	}

	// alocar conjuntos de linhas
	_sets = new set[_n_sets];

	_bit_mask = -_n_sets*_line_size;
	while (!(1 & (line_size >> _set_idx_shift))) {
		_set_idx_shift++;
	}
}

bool cache::access(uint64_t addr) {
	uint64_t set = (addr & ~_bit_mask) >> _set_idx_shift; // determinar qual conjunto este endereço pertence
	uint64_t tag = addr & _bit_mask;                      // anular os bits que nao correspondem a tag (o complemento de 2 deste produto eh a mascara)

	if (!_sets[set].size) {        // se o conjunto esta vazio
		node* new_node = new node; // criar um novo no e inicializa-lo
		new_node->prev = NULL;
		new_node->next = NULL;
		new_node->tag = tag;
		_sets[set].head = new_node;
		_sets[set].tail = new_node;
		_sets[set].size++;
	} else {
		// se o conjunto nao esta vazio, procura um no com a tag correspondente
		node* nav = _sets[set].head;
		while (nav != NULL) {                        // navegar na lista
			if (nav->tag == tag) {                   // se encontrar e...
				if (nav == _sets[set].head) {
				} else if (nav == _sets[set].tail) { // se esta na cauda
					_sets[set].tail->next = _sets[set].head;            // colocar a cauda na cabeca
					_sets[set].head = _sets[set].tail;
					_sets[set].tail = _sets[set].tail->prev;
					_sets[set].tail->next = NULL;
					_sets[set].head->prev = NULL;
					_sets[set].head->next->prev = _sets[set].head;
				} else {                         // se nao esta nem na cabeca nem na cauda
					nav->prev->next = nav->next; // trocar os ponteiros dos vizinhos e colocar na cabeca
					nav->next->prev = nav->prev;
					nav->prev = NULL;
					nav->next = _sets[set].head;
					_sets[set].head->prev = nav;
					_sets[set].head = nav;
				}
				return true;
			}
			nav = nav->next;
		}

		// se nao foi encontrada a tag, e o conjunto esta cheio
		node* new_node;
		if (_sets[set].size == _n_ways) {     // retirar o no da cauda, e inserir um novo na cabeca
			new_node = _sets[set].tail;       // reservar este endereco para ser trocado
			new_node->prev->next = NULL;      // fazer o penultimo apontar para o ptr nulo
			_sets[set].tail = new_node->prev; // fazer a nova cabeca ser o penultimo
			new_node->tag = tag;              // trocar a tag para a nova
			new_node->prev = NULL;            // o anterior sera o ptr nulo
			new_node->next = _sets[set].head; // o proximo sera a cabeca
			_sets[set].head->prev = new_node; // o anterior da cabeca sera o novo no
			_sets[set].head = new_node;       // e a cabeca sera agora o novo no
		} else {                              // mas se o conjunto ainda nao estiver cheio
			new_node = new node;              // criar um novo no
			new_node->tag = tag;              // copiar a tag
			new_node->prev = NULL;            // o anterior sera o ptr nulo
			new_node->next = _sets[set].head; // o proximo sera a antiga cabeca
			_sets[set].head->prev = new_node; // a antiga cabeca tera como seu anterior o novo no
			_sets[set].head = new_node;       // o novo no se torna a cabeca
			_sets[set].size++;                // aumentar o tamanho
		}
	}
	return false; // cache miss
}

void cache::print_set(uint64_t set) {
	if (set >= _n_sets) {
		throw "indice invalido!, esta cache tem apenas " + std::to_string(_n_sets) + " conjuntos";
	}

	std::cout << "Set " << set << ": ";

	node* nav = _sets[set].head;
	if (nav == NULL) {
		std::cout << "empty.\n";
	} else {
		std::cout << nav->tag;
		nav = nav->next;
		while (nav != NULL) {
			std::cout << " <-> " << nav->tag;
			nav = nav->next;
		}
		std::cout << '\n';
	}
}

uint64_t cache::get_tag_num(uint64_t addr) {
    return addr & _bit_mask;
}

uint64_t cache::get_set_num(uint64_t addr) {
	return (addr & ~_bit_mask) >> _set_idx_shift;
}

uint64_t cache::get_word_num(uint64_t addr) {
    return addr & ~-_line_size;
}

cache::~cache()
{ // desalocar listas
    node* nav;
	node* aux;
	for (uint64_t set = 0; set < _n_sets; set++) { // para cada conjunto
		nav = _sets[set].head;                     // comecar a navegar sua lista
		while (nav != NULL) {                      // enquanto nao se chegamos no final
			aux = nav->next;                       // guardamos o valor do anterior
			delete nav;                            // desalocamos memoria
			nav = aux;                             // o atual vira o proximo
		}
	}

	delete[] _sets; // desalocar o array de conjuntos
}