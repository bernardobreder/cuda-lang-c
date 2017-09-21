#ifndef BREDER_INTHASHMAP_H
#define BREDER_INTHASHMAP_H

/**
 * Registra um novo tipo de hashmap
 */
typedef struct b_inthashmap_t b_inthashmap_t;

/**
 * Cria um mapa com as propriedades padrões.
 *
 * Paramentros:
 * - freeValueFunc função opcional que libera da memoria o valor
 */
struct b_inthashmap_t* b_inthashmap_new_default(void (*freeValueFunc)(void*));

/**
 * Cria um mapa com as propriedades personalizada.
 *
 * Paramentros:
 * - initialCapactity valor da capacidade inicial
 * - loadFactor fator de carga de novos espaços internamente
 * - freeValueFunc função opcional que libera da memoria o valor
 */
struct b_inthashmap_t* b_inthashmap_new_custom(int initialCapacity,
		float loadFactor, void (*freeValueFunc)(void*));

/**
 * Recupera um valor no mapa. Não é possível passar uma chave nula.
 * Caso não ache a chave, será retornado 0.
 *
 * Paramentros:
 * - self estrutura de mapa
 * - key chave a ser buscada
 */
void* b_inthashmap_get(struct b_inthashmap_t* self, int key);

/**
 * Adiciona um novo elemento no mapa. Não é possível cadastrar uma chave nula.
 * Caso o elemento já exista, será retorna o elemento antigo no parametro do método.
 * Caso ocorra algum erro de alocação de memória, será retornado um valor diferente de zero.
 *
 * Paramentros:
 * - self estrutura de mapa
 * - key chave a ser registrada
 * - value valor a ser registrado
 * - oldValue espaço opcional para ser armazenado o valor antigo da chave já existente
 */
int b_inthashmap_put(struct b_inthashmap_t* self, int key, void* value,
		void** oldValue);

/**
 * Remove um elemento do mapa
 *
 * Paramentros:
 * - self estrutura de mapa
 * - key chave a ser buscada
 */
void* b_inthashmap_remove(struct b_inthashmap_t* self, int key);

/**
 * Retorna o tamanho do mapa
 *
 * Paramentros:
 * - self estrutura de mapa
 */
int b_inthashmap_size(struct b_inthashmap_t* self);

/**
 * Limpa o mapa removendo todos os elementos
 *
 * Paramentros:
 * - self estrutura de mapa
 */
void b_inthashmap_clear(struct b_inthashmap_t* self);

/**
 * Test the inthashmap library
 */
void b_inthashmap_test() ;

#endif
