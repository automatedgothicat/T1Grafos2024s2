#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ler arquivo da topologia
// selecionar vertices inicial e final
// apresentar o percurso na tela

#define infinito 1000000

typedef struct {
    int vert;
    int peso;
    struct aresta *proxVertice;
} aresta;

typedef struct {
    aresta *cab;
} cabLista;

typedef struct { 
    int vertices; 
    int arestas; 
    cabLista *adj; 	
} grafo;

grafo *criaGrafo (int numvertices) {
	int i;

	grafo *g = (grafo *)malloc(sizeof(grafo)); 
	g->vertices = numvertices;
	g->arestas = 0;
	g->adj = (cabLista *)malloc(numvertices*sizeof(cabLista)); 

	for (i = 0; i < numvertices; i++){
		g->adj[i].cab=NULL;
	}

	return(g);
}

aresta *criaAresta(int vertice, int peso){
    aresta *temp = (aresta *)malloc(sizeof(aresta));
    temp->vert = vertice;
    temp->peso = peso;
    temp->proxVertice = NULL;

    return(temp);
}

bool ligarAresta(grafo *g, int vertOrigem, int vertFim, int peso){
    if(!g)
        return false;
    
    aresta *nova = criaAresta(vertFim,peso);
    nova->proxVertice = g->adj[vertOrigem].cab;
    g->adj[vertOrigem].cab = nova;
    g->arestas++;

    return true;
}

int mapeiaVertice(char vertice) {
    return vertice - 'A';  // Supondo que os vértices sejam 'A', 'B', 'C', etc.
}

void printGrafo(grafo *gr){
	printf("Vertices: %d. Arestas: %d. \n",gr->vertices,gr->arestas);
	int i;
	
	for(i = 0; i < gr->vertices; i++){
        printf("Vertice %d: ",i);
		aresta *ad = gr->adj[i].cab;
			while(ad){ 
				printf("v%d(%d) ",ad->vert,ad->peso);
				ad = ad->proxVertice;
			}
		printf("\n");	
	}
}

void Dijkstra(grafo *g, int vertInicial, int vertFinal){
    const int totalVert = g->vertices;
    int i;
    int distancia[totalVert];
    int anterior[totalVert];
    bool visitado[totalVert];

    for(i = 0; i < totalVert; i++){
        distancia[i] = infinito;
        visitado[i] = false;
        anterior[i] = -1;
    }
    distancia[vertInicial] = 0;

    int visita, ultimo;
    for(i = 0; i < totalVert; i++){
        int menorDist = infinito;
        int ultimo = -1;

        for(visita = 0; visita < totalVert; visita++){
            if(!visitado[visita] && distancia[visita] < menorDist){
                menorDist = distancia[visita];
                ultimo = visita;
            }
        }
        
        if(ultimo == -1)
            break;

        visitado[ultimo] = true;

        aresta *vizinho = g->adj[ultimo].cab;
        int vertId, pesoCaminho;

        while (vizinho) {
            vertId = vizinho->vert;
            pesoCaminho = vizinho->peso;

            if (!visitado[vertId] && distancia[ultimo] + pesoCaminho < distancia[vertId]) {
                distancia[vertId] = distancia[ultimo] + pesoCaminho;
                anterior[vertId] = ultimo;
            }

            vizinho = vizinho->proxVertice;
        }
    }

    if(distancia[vertFinal] == infinito){
        printf("Caminho inexistente.\n");
    } else{
        printf("Custo do caminho: %d\n",distancia[vertFinal]);
    }

    int caminho[totalVert];
    int contador = 0;
    for (i = vertFinal; i != -1; i = anterior[i]) {
            caminho[contador++] = i;
        }

    printf("Caminho:");
        for (i = contador - 1; i >= 0; i--) {
            printf(" %c", caminho[i] + 'A');  // Convertendo o índice de volta para caractere
            if (i > 0) printf(" ->");
        }
        printf("\n");
}

int main(){
    FILE *fonte = fopen("topologia.txt", "r");
    if(fonte == NULL){
        printf("erro ao abrir arquivo...\n");
        exit (1);
    }

    grafo *mainGrafo = criaGrafo(3);

    char linha[50];
    char vertOrigem, vertFinal;
    int OrigemId, FinalId;
    int peso;
    while(fgets(linha,sizeof(linha),fonte)){
        sscanf(linha,"%c;%c;%d",&vertOrigem,&vertFinal,&peso);
        OrigemId = mapeiaVertice(vertOrigem);
        FinalId = mapeiaVertice(vertFinal);
        ligarAresta(mainGrafo, OrigemId, FinalId, peso);
    }

    printGrafo(mainGrafo);

    char CaminhoIni,CaminhoFim;
    int IdVertIni, IdVertFinal;
    printf("\nSelecione dois vertices para calcular o menor caminho.\n");
    printf("Vertice inicial: ");
    scanf("%c",&CaminhoIni);
    printf("\nVertice final: ");
    scanf(" %c",&CaminhoFim);

    IdVertIni = mapeiaVertice(CaminhoIni);
    IdVertFinal = mapeiaVertice(CaminhoFim);

    Dijkstra(mainGrafo,IdVertIni,IdVertFinal);

    fclose(fonte);
    return 0;
}