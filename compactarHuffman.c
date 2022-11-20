#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct no{
	int id;	
	int frequencia;
	char bin[30];
	char letra;
	struct no* dir;
	struct no* esq;
	struct no* pai;
	struct no* prox;
	struct no* ant;
} NO;
int localizadorId = 0;
int tamLista = 0;
NO* inicio = NULL;
NO* raiz = NULL;

//inverte string minha ja que a ideia é ler a partir da folha até a raiz depois inverter a ordem do valor binario criado
void inverter(char palavra[]){
	int tam = strlen(palavra);
	for(int i = 0; i < tam/2; i++) {
	   char aux = palavra[i]; //armazena o character inicial
	   palavra[i] = palavra[tam-1-i]; //Troca o character da ponta oposta -1 pois o ultimo valor da string é '\0'
	   palavra[tam-1-i] = aux; //o ultimo recebe o primeiro
	}
}
//ao final do precesso é necessario criar a raiz da arvore onde essa função é chamada
void criarRaiz(NO* no1,NO* no2){
	NO* novo = malloc(sizeof(NO));
	novo->esq = no1;
	novo->dir = no2;
	no1->pai = novo;
	no2->pai = novo;
	novo->frequencia = no1->frequencia+no2->frequencia;
	//o pai da raiz é NULL
	novo->pai = NULL;
	novo->id = localizadorId;// da um ID para o novo no
	localizadorId++;//incrementa para que não haja ID repetido
	raiz = novo;//define o no como raiz
}
//essa função cria o nó na arvore que liga os valores com 2 menores frequencia e retorna ele para que seja adcionado a lista
NO* addArvore(NO* menor1, NO*menor2){
	NO* novo = malloc(sizeof(NO));
	novo->esq = menor1;
	novo->dir = menor2;
	//eu ultilizei o conceito de pai na arvore para pegar o binario
	menor1->pai = novo;
	menor2->pai = novo;
	novo->frequencia = menor1->frequencia + menor2->frequencia;
	novo->id = localizadorId;
	localizadorId++;
	return novo;
}
//adiciona um NO sem letra na lista no caso as junções da arvore a pós se ligarem aos 2 menores devem entrar na lista para presseguir
//o processo
void addNoLista(NO* novo){
	inicio->ant->prox = novo;
	novo->ant = inicio->ant;
	inicio->ant = novo;
	novo->prox = inicio;
	tamLista++;
}
//adiciona um nó na lista e adciona um ID para identifica-lo e incrementa o ID para que os valores n se repitam
void addLista(char letra,int frequencia){
	NO* novo = malloc(sizeof(NO));
	novo->letra = letra;
	strcpy(novo->bin,"");
	novo->id = localizadorId;
	novo->frequencia = frequencia;
	//para que não de erro na arvore e as letras serão folhas então esq e dir é NULL
	novo->esq = NULL;
	novo->dir = NULL;
	if(tamLista == 0){
		inicio = novo;
		novo->ant = novo;
		novo->prox = novo;
	}else{
		inicio->ant->prox = novo;
		novo->ant = inicio->ant;
		inicio->ant = novo;
		novo->prox = inicio;
	}
	tamLista++;
	localizadorId++;
}
//isso "remove" da lista repare que ele não da free no lixo pois o valor removido é colocado na arvore 
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
	//faz com que os nos anteriores e posteriores ao aux ignorem ele veja que eu não dei free no ponteiro pois ele será usado na arvore
	aux->ant->prox = aux->prox;
	aux->prox->ant = aux->ant;
	tamLista--;
	return aux;
}
//busca na lista encadeada a letra retornando 1 se não achar e 0 se achar
int buscarPalavra(char letra){
	NO* aux = inicio;
	for(int i = 0;i<tamLista;i++){
		if (aux->letra == letra){
			return 0;
		}
		aux = aux->prox;
	}
	return 1;
}
//percorre a palavra e põe na lista os caracteres que não são repetidos 
void percorrerPalavra(char palavra[]){
	char letra;
	for(int i = 0;i < strlen(palavra);i++){
		letra = palavra[i];
		//contador de frequencia
		int contador = 0;
		for(int j = 0;j<strlen(palavra);j++){
			if(palavra[j] == letra){
				//se ao percorrer a palavra a letra reapareceu incrementa
				contador++;
			}
		}
		//testa se a letra ja está na lista encadeada circular não estiver adciona ela
		//função buscar palavra está acima
		if(buscarPalavra(letra) == 1){
			addLista(letra,contador);
		}
	}
}
//essa função perccore a lista buscando os menores valores dela
NO* buscarMenor(){
	NO* aux = inicio;
	NO* menor = inicio; //o menor começa do inicio
	for(int i = 0;i < tamLista ; i++){
		if(aux->frequencia <= menor->frequencia){ //testa se o valor é realmente o menor 
			menor = aux;
		}
		aux = aux->prox;
	}
	return menor;
}
//começa a criar a arvore a partir da folha feita pegando os valores com as 2 menores frequencia
void criarArvore(){
	while(tamLista > 2){
		NO* menor1 = buscarMenor();
		removerLista(menor1->id);
		NO* menor2 = buscarMenor();
		removerLista(menor2->id);
		//enquanto a lista for maior que dois ele vai pegando os dois nos com menores frequencias da lista e os remove
		//e adiciona um no na arvore ao mesmo tempo que adciona esse no a lista e prossegue 
		addNoLista(addArvore(menor1,menor2));
	}if(tamLista == 2){
		//ao final cria a raiz com os dois ultimos valores que serão sempre o inicio e o proximo depois dele
		criarRaiz(inicio,inicio->prox);
	}
}
//percorre a arvore criando o codigo binario de cada letra
void guardarHuffman(NO* aux ,NO* aux2){
	//aux: variavel quer irá a percorrer o array
	//aux2:guarda a variavel inicial que vai guardar o valor binario
	if(aux->pai != NULL){
		//guardo o valor binario criado a cada caminho feito
		//adcionar 2 apenas porque ele espera que seja 2 strings
		if(aux->pai->esq == aux){
			strncat(aux2->bin," 0 ",2);
		}
		if(aux->pai->dir == aux){
			strncat(aux2->bin," 1 ",2);
		}
		guardarHuffman(aux->pai,aux2);
	}else{
		//ao final o binario gerado vem ao contrario por isso é necessario inverte-lo
		inverter(aux2->bin);
	}
}
void guardaBin(NO* aux){
	//percorre o vetor atras das folhas que guardam as letras
    if(aux->esq != NULL){
    	guardaBin(aux->esq);
    }
    if(aux->dir != NULL){
    	guardaBin(aux->dir);
    }
    if(aux->dir == NULL && aux->esq == NULL){
    	//manda 2 aux um para percorrer o outro para guardar o binario
    	guardarHuffman(aux,aux);
    	//printa o binario gerado
    }
}
//busca o NO equivalente a letra
void buscarNo(char letra,NO* aux,FILE* saida){
	if(aux->esq != NULL){
    	buscarNo(letra,aux->esq,saida);
    }
    if(aux->dir != NULL){
    	buscarNo(letra,aux->dir,saida);
    }
    if(aux->esq == NULL && aux->dir == NULL && aux->letra == letra){
    	//escreve o valor binario referente a letra
    	fwrite(aux->bin,1,strlen(aux->bin),saida);
    }
}
//escreve os dados necessarios para reconstruir a arvore
void escrever_cabecalho(FILE* saida){
	NO* aux = inicio;
	for(int i = 0;i<tamLista;i++){
		fprintf(saida,"%c",aux->letra);
		fprintf(saida,"%d",aux->frequencia);
		aux = aux->prox;
	}
}
//busca a letra na arvore e escreve seu binario
void escrever_arvore_corpo(FILE* entrada,FILE* saida){
	char letra[1];
	//como eu ja tinha pego o arquivo e percorrido ele todo tive que chamar ele pro começo atravez do rewind
	rewind(entrada);
    fprintf(saida,"\n");
	while(fgets(letra,2,entrada) != NULL){
		buscarNo(letra[0],raiz,saida);
	}
}
//pega o todo arquivo enviado e trasnforma em um vetor de caracteres
void pegandoArquivo(FILE* arq,char palavra[]){
	char auxTexto[1];
	while(fgets(auxTexto,2,arq) != NULL){
		strncat(palavra,auxTexto,1);
	}
}
void pos_ordem(NO* aux){
	if(aux->esq != NULL){
		printf("\ntem espaco para esquerda\n");
    	pos_ordem(aux->esq);
    }
    if(aux->dir != NULL){
		printf("\ntem espaco para a direita\n");
    	pos_ordem(aux->dir);
    }
    if(aux->esq == NULL && aux->dir == NULL){
    	//escreve o valor binario referente a letra
    	printf("\nachei uma folha\n");
    }
}
int main(){
	char palavra[1000] = "";
	char nomeEntrada[50];
	char nomeSaida[50];
	printf("digite o nome do arquivo que será comprimido\n");
	scanf("%[^\n]s",nomeEntrada);
	FILE* entrada = fopen(nomeEntrada,"r");
	if(entrada != NULL){
		pegandoArquivo(entrada,palavra);
		percorrerPalavra(palavra);
		printf("digite o nome do arquivo de saida\n");
		scanf("%s",nomeSaida);
		FILE* saida = fopen(nomeSaida,"wb");
		fprintf(saida, "%d",tamLista);
		escrever_cabecalho(saida);
		criarArvore();
		guardaBin(raiz);
		escrever_arvore_corpo(entrada,saida);
		fclose(entrada);
		fclose(saida);
	}else{
		printf("nao foi possivel ler o arquivo");
	}
	return 0;
}