
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

typedef struct noDesc{
	int id;
	char letra;
	int frequencia;
	struct noDesc* dir;
	struct noDesc* esq;
	struct noDesc* ant;
	struct noDesc* prox;
} NO;
NO* inicio = NULL;
NO* raiz = NULL;
int tamLista = 0;
int localizarId = 0;
NO* auxiliar= NULL;
//após pegar a arvore bastar remontar o arquivo
void descompactar(FILE *entrada,FILE* saida){
	char letra[1];
	//o while estava interferindo com uso de variaveis locais dentro da função por isso tive que usar auxiliar global
	auxiliar = raiz;
	while(fgets(letra,2,entrada) != NULL){
		if(letra[0] == '0'){
			auxiliar = auxiliar->esq;
		}
		if(letra[0] == '1'){
			auxiliar = auxiliar->dir;
		}
		if(auxiliar->esq == NULL && auxiliar->dir == NULL){
			//ao chegar na folha escreve a letra e volta para raiz e reinicia todo processo até decodificar todo arquivo
			fprintf(saida,"%c",auxiliar->letra);
			auxiliar = raiz;
		}
	}

}
//isso "remove" da lista repare que ele não da free no lixo pois o valor removido é colocado na arvore 
//em resumo isso retira o valor da lista que será colocado na arvore mais tarde
NO* removerLista(int id){
	NO* aux = inicio;
	if(inicio->id == id){
		//caso o valor que será removido seja o inicio faz com que o proximo seja o novo inicio
		inicio = inicio->prox;
	}else{
		while(aux->id != id){
			//percorre o vetor atras do ID que será removido
			aux = aux->prox;
		}
	}
	//faz com que os nos anteriores e posteriores ao aux ignorem ele veja que eu não dei free no ponteiro pois ele poderá ser usado no futuro para fazer a arvore
	aux->ant->prox = aux->prox;
	aux->prox->ant = aux->ant;
	tamLista--;
	return aux;
}
NO* addArvore(NO* menor1, NO*menor2){
	NO* novo = malloc(sizeof(NO));
	novo->esq = menor1;
	novo->dir = menor2;
	novo->frequencia = menor1->frequencia + menor2->frequencia;
	novo->id = localizarId;
	localizarId++;
	return novo;
}
//ao final do precesso é necessario criar a raiz da arvore onde essa função é chamada
void criarRaiz(NO* no1,NO* no2){
	NO* novo = malloc(sizeof(NO));
	novo->esq = no1;
	novo->dir = no2;
	novo->frequencia = no1->frequencia+no2->frequencia;
	novo->id = localizarId;// da um ID para o novo no
	localizarId++;//incrementa para que não haja ID repetido
	raiz = novo;//define o no como raiz
}
void addNoLista(NO* novo){
	inicio->ant->prox = novo;
	novo->ant = inicio->ant;
	inicio->ant = novo;
	novo->prox = inicio;
	tamLista++;
}
NO* buscarMenor(){
	NO* aux = inicio;
	NO* menor = inicio; //o menor começa do inicio
	for(int i = 0;i < tamLista ; i++){
		if(aux->frequencia <= menor->frequencia){ //testa se o valor é realmente o menor 
			//ps: veja que se ele for igual ele irá manter o que ja foi pego 
			menor = aux;
		}
		aux = aux->prox;
	}
	return menor;
}
void addnoLista(int frequencia,char letra){
	NO* novo = malloc(sizeof(NO));
	novo->frequencia = frequencia;
	novo->letra = letra;
	novo->id = localizarId;
	novo->esq = NULL;
	novo->dir = NULL;
	if(tamLista == 0){
		inicio = novo;
		novo->prox = novo;
		novo->ant = novo;
	}else{
		inicio->ant->prox = novo;
		novo->ant = inicio->ant;
		inicio->ant = novo;
		novo->prox = inicio;
	}
	tamLista++;
	localizarId++;
};
void percorrerLista(){
	NO* aux = inicio;
	for(int i = 0;i<tamLista;i++){
		printf(" %c ",aux->letra);
		printf(" %d ",aux->frequencia);
		aux = aux->prox;
	}
}
void percorrerArq(FILE* entrada){
	int tam = 0;
	//inicia pegando o tamanho do arquivo
	fscanf(entrada,"%d",&tam);
	for(int i = 0;i < tam ; i++){
		char letra;
		int frequencia;
		fscanf(entrada,"%c",&letra);
		fscanf(entrada,"%d",&frequencia);
		addnoLista(frequencia,letra);
	}
}
//começa a criar a arvore a partir da folha feita pegando os valores com as 2 menores frequencia
void criarArvore(){
	while(tamLista > 2){
		NO* menor1 = buscarMenor();
		removerLista(menor1->id);
		NO* menor2 = buscarMenor();
		removerLista(menor2->id);
		//enquanto a lista for maior que dois ele vai pegando os dois nos com menores frequencias da lista e os remove
		//e adiciona um no na arvore ao mesmo tempo que adciona esse no a lista
		addNoLista(addArvore(menor1,menor2));
	}if(tamLista == 2){
		//ao final cria a raiz com os dois ultimos valores que serão sempre o inicio e o proximo depois dele
		criarRaiz(inicio,inicio->prox);
		inicio->prox = NULL;
		inicio = NULL;
		tamLista = 0;
	}
}
int main(){
	char nomeEntrada[50];
	char nomeSaida[50];
	printf("digite o nome do arquivo a ser descodificado\n");
	scanf("%[^\n]s",nomeEntrada);
	FILE* entrada = fopen(nomeEntrada,"r");
	percorrerArq(entrada);
	criarArvore();
	printf("digite o nome do arquivo que vai sair\n");
	scanf("%s",nomeSaida);
	FILE* saida = fopen(nomeSaida,"w");
	descompactar(entrada,saida);
	fclose(saida);
	fclose(entrada);

	return 0;
}
