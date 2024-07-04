#include <stdio.h>

int main(){
	long int endereco;
	long int word;
	long int linha;
	long int tag;
	
	/* Considerando uma memória de 16 MB (enderecos de 24 bits de tamanho) */
	/* Considerando um bloco da cache com 4 words (2 bits de tamanho) */
	/* Considerando uma cache com 64 K words de tamanho, sendo 16 K linhas (14 bits de tamanho) */
	/* Considerando uma TAG com 8 bits */
	
	printf("\nDigite um endereco (em decimal) de uma memoria de 16MB: ");
	scanf("%ld", &endereco);
	
	/* word = endereco AND 000000000000000000000011 */
	word = endereco & 3;
	
	/* linha = endereco AND 000000001111111111111100 */
	linha = endereco & 65532;
	linha = linha >> 2; /* deslocar 2 bits para a direita! */
	
	/* tag = endereco AND 111111110000000000000000 */
	tag = endereco & 16711680;
	tag = tag >> 16; /* deslocar 16 bits para a direita! */
	
	printf("\n\nPosicao da word no bloco: %ld", word);
	printf("\nLinha ocupada na cache: %ld", linha);
	printf("\nValor da TAG: %ld \n", tag);

	return(0);
}
