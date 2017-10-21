#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */


typedef struct {
	int i;
	int j;
}Ponto;

typedef struct{
	Ponto *inicial;
	Ponto *final;
}Caminho;

typedef struct{
	int V; // quantidade de v?rtices
	int A; // quantidade de arcos
	int **adj; //matriz de adjacencia
}TDigrafo;


typedef struct{
	int first, last;
	int N;
	Ponto *elementos;
}TFila;

// Declaração das funções
//Funções do Digrafo
TDigrafo * Init( int V );
void show(TDigrafo *D);
void showMatriz(TDigrafo *D);
void libera(TDigrafo *D);

//Funções da Fila
TFila * InitFila( int N );
void insereFila(TFila *F, Ponto *elemento);
Ponto retiraFila(TFila *F);
int vazia(TFila *F);
int cheia(TFila *F);
void showFila(TFila *fila);

//Nossas funções
void menorCaminho(TDigrafo *D,Caminho *C);
void preencheVizinho(TDigrafo* digrafo, TFila* fila,  int i, int j);
Ponto menorVizinho(TDigrafo *D, Ponto p);


//Variaveis Globais
Ponto *pontoInicial;
Ponto *pontoFinal;

int main(){

	char ch;
	int n;
	TDigrafo *digrafo;

	int i = 0;
	int j = 0;
	
	int iF = 0;
	int jF = 0;

	char url[]="entrada.txt";
    FILE *arq;

    arq = fopen(url, "r");
    if(arq == NULL){

        printf("Erro, nao foi possivel abrir o arquivo\n");
    }
    else{

    	//Lendo n = tamanho da Matriz
        char qtd[256];
        n = atoi(fgets(qtd, 256, arq));
        //printf("%d\n",n);
   	    digrafo = Init(n);


        while((ch = fgetc(arq))!= EOF ){

        	if(ch=='\n'){
        		j = 0;
        		i++;
			}

        	//Com 0 o caminho estï¿½ livro, portando insere na matriz
 		    if(ch=='0'){
				digrafo->adj[i][j] = 0;
 		    	j++;
            }

            //Com 9 o caminho estï¿½ bloqueado, portando nï¿½o ï¿½ uma opï¿½ï¿½o passar aqui Obs:Estou usando 9, pois tratarei para usar -1 depois
            else if(ch=='-'){
				digrafo->adj[i][j] = -1;
            	j++;
			}

        }


    }

    fclose(arq);

    show(digrafo);

	//Receber input do usuario e verficar validade dos pontos
	int valido = 0;
	while(!valido) {
    	printf ("\nDigite i e j do ponto inicial: \n");
    	scanf ("%d",&i);
		scanf ("%d",&j);
		
    	printf ("\nDigite i e j do ponto final: \n");
    	scanf ("%d",&iF);
    	scanf ("%d",&jF);
    	
    	if(i == iF && j == jF) {
    		printf("Pontos invalidos. Certifique-se de escolher pontos diferentes.\n");
		}
		else if(i < 0 || j < 0 || iF < 0 || jF < 0){
			printf("Pontos invalidos. Certifique-se de escolher pontos maiores que zero.\n");
		}
    	else if(digrafo->adj[i][j] != -1 && digrafo->adj[iF][jF] != -1) {
    		valido = 1;
		}
		else {
			printf("Pontos invalidos. Certifique-se de escolher pontos diferentes e que nao correspondem a paredes.\n");
		}
	}
	
    //Criando caminho com os pontos inicial e final fornecidos pelo usuario
    //Receber input do usuario
	pontoInicial = (Ponto *)malloc(sizeof(Ponto));
	pontoInicial->i = i;
	pontoInicial->j = j;
    
    pontoFinal = (Ponto *)malloc(sizeof(Ponto));
    pontoFinal->i = iF;
    pontoFinal->j = jF;
    
    Caminho *caminho;	 
	caminho = (Caminho *)malloc(sizeof(Caminho));
    caminho->inicial = pontoInicial;
    caminho->final = pontoFinal;


    

    //Busca do menor caminho entre os pontos dados
    menorCaminho(digrafo, caminho);

	
	
	libera(digrafo);

	return 0;
}

//Preenche os vizinhos e coloca eles na fila
void preencheVizinho(TDigrafo* digrafo, TFila* fila,  int i, int j) {
	
		
	int ponto = digrafo->adj[i][j];
    ponto++;
    
    Ponto *vizinho;
	vizinho = (Ponto*)malloc(sizeof(Ponto));
    
	//Vizinho de cima
	if(i > 0 ) { //Condição para evitar as bordas
		if(digrafo->adj[i - 1][j] == 0) { //Condição para evitar os blocos com -1
        	digrafo->adj[i - 1][j] = ponto;
        	vizinho->i = i - 1;
	    	vizinho->j = j;
     		insereFila(fila, vizinho);
    	}
	}

	//Vizinho da direita
    if((j+1) < digrafo->V){
        if(digrafo->adj[i][j + 1] == 0) {
            digrafo->adj[i][j + 1] = ponto;
            vizinho->i = i;
			vizinho->j = j + 1;
			insereFila(fila, vizinho);
		}
	}
	//Vizinho de baixo
    if((i+1) < digrafo->V){
        if(digrafo->adj[i + 1][j] == 0) {
            digrafo->adj[i + 1][j] = ponto;
            vizinho->i = i + 1;
			vizinho->j = j;
			insereFila(fila, vizinho);
		}
	}

	//Vizinho da esquerda
    if(j > 0) {
        if(digrafo->adj[i][j - 1] == 0){
            digrafo->adj[i][j - 1] = ponto;
            vizinho->i = i;
			vizinho->j = j - 1;
			insereFila(fila, vizinho);
		}
	}
}

//Verifica qual o caminho vizinho a ser seguido
Ponto menorVizinho(TDigrafo *D, Ponto p) {
	int menor = D->V * D->V;
	int i = p.i;
	int j = p.j;
	
	Ponto *menorVizinho;
	menorVizinho = (Ponto*)malloc(sizeof(Ponto));
	menorVizinho->i = -1;
	menorVizinho->j = -1;
	
	//Vizinho de cima
	if(i > 0){
		if(D->adj[p.i - 1][p.j] != -1 && D->adj[p.i - 1][p.j] == D->adj[p.i][p.j] - 1) {
			menor = D->adj[p.i - 1][p.j];
			menorVizinho->i = p.i - 1;
			menorVizinho->j = p.j;
		}
	}
	
	//Vizinho da direita
	if((j + 1) < D->V){
		if(D->adj[p.i][p.j + 1] != -1 && D->adj[p.i][p.j + 1] == D->adj[p.i][p.j] - 1) { 
			menor = D->adj[p.i][p.j + 1];
			menorVizinho->i = p.i;
			menorVizinho->j = p.j + 1;
		}
	}
	
	//Vizinho de baixo
	if((i + 1) < D->V){	
		if(D->adj[p.i + 1][p.j] != -1 && D->adj[p.i + 1][p.j] == D->adj[p.i][p.j] - 1) { 
			menor = D->adj[p.i + 1][p.j];
			menorVizinho->i = p.i + 1;
			menorVizinho->j = p.j;
		}
	}
	
	//Vizinho da esquerda
	if(j > 0){	
		if(D->adj[p.i][p.j - 1] != -1 && D->adj[p.i][p.j - 1] == D->adj[p.i][p.j] - 1) { 
			menor = D->adj[p.i][p.j - 1];
			menorVizinho->i = p.i;
			menorVizinho->j = p.j - 1;
		}
	}
	
	return *menorVizinho;
}

void menorCaminho(TDigrafo *D,Caminho *C){

	//Guardar pontos final e inicial em uma variavel para facilitar acesso
	Ponto *inicial;
	Ponto *final;
	
    inicial = (Ponto *)malloc(sizeof(Ponto));
    final = (Ponto *)malloc(sizeof(Ponto));
    
	inicial = C->inicial;
	final = C->final;

	//Criar fila
	TFila *fila = InitFila(10);

	//Prenche os vizinhos da casa inicial com 1 
	preencheVizinho(D, fila, inicial->i, inicial->j);
	
	//Iniciar ciclo de verificacao dos vizinhos
	while(!vazia(fila)) {
		//Vizinho a ser retirado e preenchido
		Ponto vizinho = retiraFila(fila);
		preencheVizinho(D, fila, vizinho.i, vizinho.j);
	}
	
	
	//Verificar se o caminho chegou ate o destino final
	if(D->adj[final->i][final->j] != 0) {

		printf("\nChegou no destino");

		//Setar Inicial como 0 novamente para fazer a conta do caminho
		D->adj[inicial->i][inicial->j] = 0;
		
		//Tracar caminho
		printf("\ni = %d e j = %d", final->i, final->j);
		
		Ponto *vizinhoMenor;
		vizinhoMenor = (Ponto*)malloc(sizeof(Ponto));
		*vizinhoMenor = menorVizinho(D, *final);
		printf("\ni = %d e j = %d", vizinhoMenor->i, vizinhoMenor->j);
		
		while(D->adj[vizinhoMenor->i][vizinhoMenor->j] != 0){
			*vizinhoMenor = menorVizinho(D, *vizinhoMenor);
			printf("\ni = %d e j = %d", vizinhoMenor->i, vizinhoMenor->j);
		}
	} 
	else {
		
		printf("\nNao chegou no destino");
		
		//Setar Inicial como 0 novamente para printar
		D->adj[inicial->i][inicial->j] = 0;
	}
	
	
	
	show(D);
	
	
	
}

/*-------------------Funções para Fila----------------*/
// implementação da Fila não circular
TFila * InitFila( int N ){
	TFila *novaFila;
	
	novaFila = (TFila *) calloc( 1, sizeof(TFila));
	novaFila->elementos = (Ponto *) calloc(N, sizeof(Ponto));
	novaFila->first = novaFila->last = 0;
	novaFila->N = N;
	return novaFila;
}

void insereFila(TFila *F, Ponto *elemento){
	if( !cheia( F )){
		F->elementos[F->last] = *elemento;
		F->last = (F->last + 1)%F->N;
	}
	else{
		printf("fila cheia\n");
		exit(1);
	}
}

Ponto retiraFila(TFila *F){
	Ponto elemento;
	elemento = F->elementos[F->first];
	F->first = (F->first + 1)%F->N;
	return elemento;
}

int vazia(TFila *F){
	return F->first == F->last;
}

int cheia(TFila *F){
	//return (F->last+1 == F->N && F->first == 0 ) ||
	//	   (F->last+1 == F->first);	
	return (F->last + 1)%F->N==F->first; 
}

void showFila(TFila *fila) {
	//Mostra a fila para teste
	int i = 0;
	for(i = 0; i < 10; i++) {
		printf("\ni = %d e j = %d", fila->elementos[i].i, fila->elementos[i].j);
	}
}

/*-------------------Funções para Digrafo----------------*/
// Fun??o cria e devolve uma nova estrutura TDigrafo,
// inicializa os atributos da estrutura,
// cria uma matriz de matriz de adjac?ncia para V v?rtices
// e zera os valores para arcos.
TDigrafo * Init( int V ){
	TDigrafo *D;
	int i;
	// aloca uma estrutura do tipo TDigrafo
	// http://www.cplusplus.com/reference/cstdlib/calloc/
	//printf("tamanho estrutura %d",sizeof(TDigrafo*));
	D = (TDigrafo *) calloc(1, sizeof(TDigrafo) );

	// D = new TDigrafo( V )
	// atribui o numero de vertices para a estrtura
	D->V = V;
	// zero numero arcos
	D->A = 0;
	// aloca matriz de adjacencia
	//printf("\n%d ",D->adj);
	D->adj = (int ** )calloc(V,sizeof(int*));
	for( i = 0; i < V;i++){
		D->adj[i]= (int * )calloc(V,sizeof(int));
		//printf("\n%x ",D->adj[i]);
	}
	return D;

}

/*
Para cada v?rtice v do digrafo, este m?todo imprime,
em uma linha, todos os v?rtices adjacentes a v.
*/
void show(TDigrafo *D){
	int v,w;

	for( v = 0; v < D->V; v++){ // linha
		printf("\n%d:",v);
		for(w=0; w < D->V; w++ ) // coluna
	        if(D->adj[v][w] == -1 || D->adj[v][w] == -2 || D->adj[v][w] == -3){
	        	printf("%d ", D->adj[v][w]);
			}
			else{
				printf(" %d ", D->adj[v][w]); //Apenas para ficar bonito no show
			}
	}
	
}

void showMatriz(TDigrafo *D){
	int v,w;

	for( v = 0; v < D->V; v++){ // linha
		printf("\n%d:",v);
		for(w=0; w < D->V; w++ ) // coluna
			if( D->adj[v][w] == 1){
				printf(" %d",0);
			}else if(D->adj[v][w] > 1){
				printf(" %d",1);
			}else{
				printf(" %c",'x'); //-1
			}
	}
}
/*
Fun??o desaloca a mem?ria utilizado pelo d?grafo
e pela matriz de adjac?ncia.
*/
void libera(TDigrafo *D) {

	int i;
	for(i=0;i<D->V;i++)
		free(D->adj[i]); // desloca linha

	// desaloca matriz
	free( D->adj );

	// desaloca digrafo
	free( D );

}



