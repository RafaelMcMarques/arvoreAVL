#include "avl.h"
#include "fila.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//substitua por seus dados
struct aluno* getAluno1()
{
    struct aluno* retorno = malloc(sizeof(struct aluno));
    if(!retorno)
        exit(1);
    retorno->nome = malloc(sizeof("Rafael Munhoz da Cunha Marques"));//sizeof conta o \0
	if(!retorno->nome)
		exit(1);
    retorno->nomeDinf = malloc(sizeof("rmcm22"));
    if(!retorno->nomeDinf)
        exit(1);
    
    strcpy(retorno->nome, "Rafael Munhoz da Cunha Marques");
    strcpy(retorno->nomeDinf, "rmcm22");
    retorno->grr = 20224385;

    return retorno;
}

struct aluno* getAluno2()
{
    struct aluno* retorno = malloc(sizeof(struct aluno));
    if(!retorno)
        exit(1);
    retorno->nome = malloc(sizeof("Nicolas Costa da Silva"));//sizeof conta o \0
	if(!retorno->nome)
		exit(1);
    retorno->nomeDinf = malloc(sizeof("ncs22"));
    if(!retorno->nomeDinf)
        exit(1);
    
    strcpy(retorno->nome, "Nicolas Costa da Silva");
    strcpy(retorno->nomeDinf, "ncs22");
    retorno->grr = 20221228;
    
    return retorno;
}

void imprimirDadosAlunos()
{
    struct aluno* aluno = getAluno1();
    
    printf("Trabalho de %s\n", aluno->nome);
    printf("Login Dinf %s\n", aluno->nomeDinf);
    printf("GRR %u\n\n", aluno->grr);

    free(aluno->nome);
    free(aluno->nomeDinf);
    free(aluno);

    aluno = getAluno2();
    if(!aluno)
        return;
    
    printf("...E... \n\n");
    printf("Trabalho de %s\n", aluno->nome);
    printf("Login Dinf %s\n", aluno->nomeDinf);
    printf("GRR %u\n\n", aluno->grr);

    free(aluno->nome);
    free(aluno->nomeDinf);
    free(aluno);
}

//retorna novo nodo com chave passada
struct nodo* criaNodo(int chave)
{
    struct nodo* novo = malloc(sizeof(struct nodo));
    if (!novo) 
        exit(1);
    novo->chave = chave;
    novo->pai = NULL;
    novo->fe = NULL;
    novo->fd = NULL;
    novo->h = 1;
    return novo;
}

//define altura do nodo como altura de seu filho mais alto + 1
void consertaAltura(struct nodo* nodo)
{
    if ((nodo->fd == NULL) && (nodo->fe == NULL)) nodo->h = 1;

    else if ((nodo->fd != NULL) ^ (nodo->fe != NULL)) {
        if (nodo->fd != NULL) nodo->h = nodo->fd->h + 1;
        else nodo->h = nodo->fe->h + 1;
    }

    else if (nodo->fd->h > nodo->fe->h) nodo->h = nodo->fd->h + 1;
    else nodo->h = nodo->fe->h + 1;
}

//Rotacao para a esquerda entre p e seu filho direito
void rotarEsquerda (struct nodo** raiz, struct nodo* p) 
{
	struct nodo* x = p->fd;
	
	p->fd = x->fe;
	if(x->fe != NULL)
		x->fe->pai = p;
	x->fe = p;
	x->pai = p->pai;
	p->pai = x;
	if(*raiz == p)
		*raiz = x;
    else {
        if(x->pai->fe == p) x->pai->fe = x;
        else x->pai->fd = x;
    }

    consertaAltura(p);
    consertaAltura(x);
}

//rotacao para a direita entre p e seu filho esquerdo
void rotarDireita (struct nodo** raiz, struct nodo* p) 
{
	struct nodo* x = p->fe;
	
	p->fe = x->fd;
	if(x->fd != NULL)
		x->fd->pai = p;
	
	x->fd = p;
	x->pai = p->pai;
	p->pai = x;
	if(*raiz == p)
		*raiz = x;
    else {
        if(x->pai->fe == p) x->pai->fe = x;
        else x->pai->fd = x;
    }

    consertaAltura(p);
    consertaAltura(x);
}

//Retorna o fator de balanco de um nodo
int calculaBalanco(struct nodo* nodo)
{
    int hDir = 0;
    int hEsq = 0;
    if (nodo->fd != NULL) hDir = nodo->fd->h;
    if (nodo->fe != NULL) hEsq = nodo->fe->h;
    return hDir - hEsq;
}

//Balancea um nodo p fazendo as rotacoes necessarias
void balanceaNodo(struct nodo** raiz, struct nodo* p)
{
    if (p == NULL) return;
    int balancoP = calculaBalanco(p);
    //desbalanceado para a direita
    if (balancoP == 2) {
        struct nodo* q = p->fd;
        int balancoQ = calculaBalanco(q);
        //filho direito de q esta mais alto
        if (balancoQ == 1)
            rotarEsquerda(raiz, p);

        //filho esquerdo de q eh o que esta mais alto 
        else {
            rotarDireita(raiz, q);
            rotarEsquerda(raiz, p);
        }
    }

    //desbalanceado para a esquerda
    else if (balancoP == -2) {
        struct nodo* q = p->fe;
        int balancoQ = calculaBalanco(q);
        //filho esquerdo de q esta mais alto
        if (balancoQ == -1)
            rotarDireita(raiz, p);

        //filho direito de p eh o que esta mais alto
        else {
            rotarEsquerda(raiz, q);
            rotarDireita(raiz, p);
        }
    }
}

//Sobe a arvore recursivamente a partir de um nodo consertando as alturas e balanceando
void ajustaAVL(struct nodo** raiz, struct nodo* nodo)
{
    if (nodo == NULL) return;
    consertaAltura(nodo);
    int balanco = calculaBalanco(nodo);
    if ((balanco == 2) || (balanco == -2))
        balanceaNodo(raiz, nodo);
    ajustaAVL(raiz, nodo->pai);
}

//insere nodo na arvore com a chave passada, retorna NULL caso chave ja esteja na arvore
struct nodo* inserir(struct nodo** raiz, int chave)
{
    struct nodo* novo = criaNodo(chave);

    if (*raiz == NULL) {
        *raiz = novo;
        return novo;
    }

    struct nodo* atual = *raiz;
    struct nodo* pai = NULL;
    while (atual != NULL) {
        pai = atual;
        if (chave > atual->chave) atual = atual->fd;
        else if (chave < atual->chave) atual = atual->fe;
        else {
            printf("Falha ao inserir.\n");
            free(novo);            
            return NULL;
        }
    }
    novo->pai = pai;
    if (novo->chave > pai->chave) 
        pai->fd = novo;
    else pai->fe = novo;

    ajustaAVL(raiz, novo);
    return novo;
}

//Coloca novo nodo no lugar do nodo removido na arvore mantendo-a balanceada
void transplanta(struct nodo **raiz, struct nodo* removido, struct nodo* novo)
{
    if (removido->pai == NULL) *(raiz) = novo;
    else {
        if (removido->pai->fe == removido) removido->pai->fe = novo;
        else removido->pai->fd = novo;
    }

    if (novo != NULL)
        novo->pai = removido->pai;  
    ajustaAVL(raiz, removido->pai);
}

//retorna nodo com a maior chave da subarvore passada
struct nodo* maximo(struct nodo* raiz)
{
    struct nodo* max = raiz;
    while (max->fd != NULL) max = max->fd;
    return max;
}

//exclui nodo com a chave passada da arvore, retorna 0 caso chave nao for encontrada
int excluir(struct nodo** raiz, int chave)
{
    struct nodo* removido = buscar(*raiz, chave);
    struct nodo* substituto;
    if (removido == NULL){
   	printf("Falha ao remover %d.\n",chave);
    	return 0;
    }

    if (removido->fe == NULL) {
        substituto = removido->fd;
        transplanta(raiz, removido, substituto);
    } else if (removido->fd == NULL) {
        substituto = removido->fe;
        transplanta(raiz, removido, substituto);
    } else {
        substituto = maximo(removido->fe);
        if (substituto != removido->fe){
            transplanta(raiz, substituto, substituto->fe);
            substituto->fe = removido->fe;
            substituto->fe->pai = substituto;
        }
        substituto->fd = removido->fd;
        substituto->fd->pai = substituto;
        consertaAltura(substituto);
        transplanta(raiz, removido, substituto);
    }    
    free(removido);
    return 1;
}

//retorna nodo da arvore com a chave passada, NULL caso nao ache
struct nodo* buscar(struct nodo* nodo, int chave)
{
    struct nodo *atual = nodo;
    while(atual != NULL) {
        if (atual->chave == chave) return atual;
        if (chave > atual->chave) atual = atual->fd;
        else atual = atual->fe;
    }
    return NULL;
}

void emOrdemRecursivo(struct nodo *raiz)
{
    if (raiz == NULL) return;
    emOrdemRecursivo(raiz->fe);
    printf("%d\t", raiz->chave);
    emOrdemRecursivo(raiz->fd);
}

struct nodo* imprimirEmOrdem(struct nodo* raiz)
{
    if(raiz == NULL) {
    	printf("Arvore vazia");
    	return NULL;
    }	

    printf("Imprimindo em ordem\n");
    emOrdemRecursivo(raiz);
    printf("\n");
    return NULL;
}

struct nodo* imprimirEmLargura(struct nodo* raiz)
{
    if(raiz == NULL) {
    	printf("Arvore vazia");
    	return NULL;
    }	

    printf("Imprimindo em largura\n");
    lista* fila = criaLista();
    struct nodo* nodoArvore;
    struct nodoL* nodoPilha;
    int nivel = 0;

    printf("[%d]\t%d(%d)\t", nivel, raiz->chave, calculaBalanco(raiz));
    if(raiz->fe != NULL) enfilera(fila, raiz->fe);
    if(raiz->fd != NULL) enfilera(fila, raiz->fd);
    int anterior = raiz->chave;
    while (!ehVazia(fila)) {
        nodoPilha = desenfilera(fila);
        nodoArvore = nodoPilha->nodo;
        free(nodoPilha);

        if (nodoArvore->chave < anterior) {
            nivel++;
            printf("\n[%d]\t", nivel);
        }
        printf("%d(%d)\t", nodoArvore->chave, calculaBalanco(nodoArvore));
        if(nodoArvore->fe != NULL) enfilera(fila, nodoArvore->fe);
        if(nodoArvore->fd != NULL) enfilera(fila, nodoArvore->fd);
        anterior = nodoArvore->chave;
    }
    printf("\n");
    free(fila);
    return NULL;
}

void liberaArvore(struct nodo* raiz)
{
	if(raiz == NULL)
		return;
	liberaArvore(raiz->fe);
	liberaArvore(raiz->fd);
	free(raiz);
    raiz = NULL;
}
