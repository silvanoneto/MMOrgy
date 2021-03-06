#include "grafo.h"
#include "search.h"
#include "cluster.h"

void encadeia_cidades (struct Cidade **cidade_fim, struct Cidade *cidade_novo){
	if((*cidade_fim)){
		(*cidade_fim)->prox = cidade_novo;
	}
	(*cidade_fim) = cidade_novo;
	(*cidade_fim)->prox = NULL;
}

struct Cidade *mapear_grafo (struct No *grafo, int index_Inicio){
	struct Cidade *head_cidade = (struct Cidade*)malloc(sizeof(struct Cidade));
	struct Cidade *cidade_inicio = (struct Cidade*)malloc(sizeof(struct Cidade));
	struct Cidade *cidade_fim = (struct Cidade*)malloc(sizeof(struct Cidade));
	struct NoQueue *fila_inicio = (struct NoQueue*)malloc(sizeof(struct NoQueue));
	struct NoQueue *fila_fim = (struct NoQueue*)malloc(sizeof(struct NoQueue));
	struct NoQueue *fila_novo = (struct NoQueue*)malloc(sizeof(struct NoQueue)); 
	
	cidade_inicio = cidade_fim = head_cidade;
	
	reset_visitas(&grafo);
	
	fila_novo->no = list_get(&grafo, index_Inicio);
	enqueue(&fila_fim, fila_novo);
	fila_inicio = fila_fim;
	
	strcpy(head_cidade->nome, grafo->perfil.cidade);
	head_cidade->perfil = NULL;
    head_cidade->prox = NULL;
	
	while(verif_visitas(&grafo)==1){
		struct NoQueue *fila_temp = (struct NoQueue*)malloc(sizeof(struct NoQueue));
		
		//Insere n�s correspondentes as arestas e remove os n�s ja verificados da lista de prioridades
		for(fila_temp = fila_fim; fila_inicio!=fila_temp->fila_prox; fila_inicio = fila_inicio->fila_prox){
			struct Amigos *temp_amigos = NULL;
		
			for(temp_amigos = fila_inicio->no->perfil.amigos; temp_amigos != NULL; temp_amigos = temp_amigos->prox_amigo){
				struct No *temp = list_get(&grafo, temp_amigos->indice);
				fila_novo = (struct NoQueue*)malloc(sizeof(struct NoQueue));
				
				if(temp->visitado==0){
					fila_novo->no = temp;
					enqueue(&fila_fim, fila_novo);
				}
			
			}
		
		}
		
		
		for(fila_temp = fila_inicio; fila_temp!=NULL; fila_temp = fila_temp->fila_prox){
			struct Cidade *cidade_temp = (struct Cidade*)malloc(sizeof(struct Cidade));
			int verif = 0;
			
			for (cidade_temp = cidade_inicio; cidade_temp!=NULL; cidade_temp = cidade_temp->prox){
				
				if(strcmp(fila_temp->no->perfil.cidade, cidade_temp->nome)==0){
					verif = 1;
					break;
				}
			}
			
			if (!verif){
				struct Cidade *novo = (struct Cidade*)malloc(sizeof(struct Cidade));
				strcpy(novo->nome, fila_temp->no->perfil.cidade);
                novo->perfil = NULL;
				encadeia_cidades(&cidade_fim, novo);
			}
			
		}
	
	}
	
	return head_cidade;

}

void insere_pessoasCidade(struct No **head_perfis, struct Cidade **head_cidades, int *quantidade_perfis){
	int i = 0;
	struct Cidade *headCluster = (struct Cidade *) malloc(sizeof(struct Cidade));
	struct Cidade *primeiro = (*head_cidades);
	struct No *firstNo = (*head_perfis);
	struct Perfil *ultimoPerfilClusterInserido = NULL;
	
	headCluster = (*head_cidades);
       
    while(primeiro){
		
    	primeiro->contador = 0;

		for(i = 0; i < (*quantidade_perfis); ++i){
			if(strcmp(firstNo->perfil.cidade, primeiro->nome) != 0){
				firstNo = firstNo->no_prox;
			} else {
				if(primeiro->perfil == NULL){
					primeiro->perfil = &firstNo->perfil;
					ultimoPerfilClusterInserido = primeiro->perfil;	
				} else {
					ultimoPerfilClusterInserido->prox_perfilCidade = &firstNo->perfil;
					ultimoPerfilClusterInserido = &firstNo->perfil; 
				}
                ultimoPerfilClusterInserido->prox_perfilCidade = NULL;
				firstNo = firstNo->no_prox;
				primeiro->contador += 1;
			}
        }
        firstNo = (*head_perfis);
        primeiro = primeiro->prox;       
    }
}

void insere_pessoasInteresses(struct No **head_perfis, struct Cluster_interesses **head_interesses){
    int i = 0;
	//struct Cluster_interesses *headCluster = (struct Cluster_interesses *) malloc(sizeof(struct Cluster_Interesses));
	struct Cluster_interesses *primeiro = (*head_interesses);
	struct No *firstNo = (*head_perfis);
    struct Interesses *temp, *aux;
    struct lista_perfisInteresses *ultimoLista = NULL, *primeiroLista = NULL;
    
	while(primeiro){
		primeiro->counter = 0;
        while(firstNo){
            temp = firstNo->perfil.interesses;    
            while(temp){
     
                    struct lista_perfisInteresses *nova = (struct lista_perfisInteresses *) malloc(sizeof(struct lista_perfisInteresses));
                    
                    if(strcmp(temp->nome, primeiro->interesse) != 0){ 
                        temp = temp->prox_interesse;
                 
                    } else {
                        if(!primeiroLista){
                            nova->proxi = NULL;
                            nova->perfil = &firstNo->perfil;
                            primeiroLista = nova;
                            ultimoLista = primeiroLista;
                            primeiro->counter += 1;
                            break;
                        } else {
                            nova->proxi = NULL;
                            nova->perfil = &firstNo->perfil;
                            ultimoLista->proxi = nova;
                            ultimoLista = nova; 
                            primeiro->counter += 1;
                            break;
                        }
                       // temp = temp->prox_interesse;

                    }

            }    
            firstNo = firstNo->no_prox;
        }
        
        
        firstNo = (*head_perfis);
        primeiro->lista = primeiroLista;
        primeiroLista = NULL;
        ultimoLista = NULL;
        primeiro = primeiro->prox;       
    }
}

void encadeia_interesses (struct Cluster_interesses ** head_interesses, struct Cluster_interesses ** fim, struct Cluster_interesses * novo){
    
    if (!(*fim)){
    	(*head_interesses) = novo;
    	(*fim) = (*head_interesses);
    }else{
    	(*fim)->prox = novo;
    	(*fim) = novo;
    }
        (*fim)->prox = NULL;
}

struct Cluster_interesses * mapear_interesses (struct No * grafo, int index_inicio){
    
    struct Cluster_interesses * head_interesses = (struct Cluster_interesses * ) malloc (sizeof (struct Cluster_interesses));
    struct Cluster_interesses * interesses_inicio = (struct Cluster_interesses * ) malloc (sizeof (struct Cluster_interesses));
    struct Cluster_interesses * interesses_fim = (struct Cluster_interesses * ) malloc (sizeof (struct Cluster_interesses));
    struct NoQueue *fila_inicio = (struct NoQueue*)malloc(sizeof(struct NoQueue));
	struct NoQueue *fila_fim = (struct NoQueue*)malloc(sizeof(struct NoQueue));
	struct NoQueue *fila_novo = (struct NoQueue*)malloc(sizeof(struct NoQueue)); 
    struct Cluster_interesses * novo = NULL;
    struct Interesses * temps = NULL;
    
    interesses_inicio = interesses_fim = head_interesses = NULL;
    
    reset_visitas (&grafo);
    
    fila_novo->no = list_get (&grafo,index_inicio);
    //fila_novo->no = grafo;
    
    enqueue (&fila_fim, fila_novo); //coloca o no visitado na fila e marca ele
    
    fila_inicio = fila_fim;
    
    for (temps = fila_inicio->no->perfil.interesses;temps != NULL; temps = temps->prox_interesse){
        novo = (struct Cluster_interesses*)malloc(sizeof(struct Cluster_interesses));
		strcpy(novo->interesse, temps->nome);
        encadeia_interesses(&head_interesses,&interesses_fim,novo);    
    }

    interesses_inicio = head_interesses;
    	
    while(verif_visitas(&grafo)==1){
		struct NoQueue *fila_temp = (struct NoQueue*)malloc(sizeof(struct NoQueue));
		
		//Insere n�s correspondentes as arestas e remove os n�s j� verificados da lista de prioridades
		for(fila_temp = fila_fim; fila_inicio!=fila_temp->fila_prox; fila_inicio = fila_inicio->fila_prox){
			struct Amigos *temp_amigos = NULL;
		
			for(temp_amigos = fila_inicio->no->perfil.amigos; temp_amigos != NULL; temp_amigos = temp_amigos->prox_amigo){
				struct No *temp = list_get(&grafo, temp_amigos->indice);
				fila_novo = (struct NoQueue*)malloc(sizeof(struct NoQueue));
				
				if(temp->visitado==0){
					fila_novo->no = temp;
					enqueue(&fila_fim, fila_novo);
				}
			}
		}
        
		for(fila_temp = fila_inicio; fila_temp!=NULL; fila_temp = fila_temp->fila_prox){
            
			struct Cluster_interesses *interesses_temp = (struct Cluster_interesses*)malloc(sizeof(struct Cluster_interesses));
			int verif = 0;
			struct Interesses * temp_interesses = NULL;  

			//andar com os interesses de cada perfil
			for (temp_interesses = fila_temp->no->perfil.interesses;temp_interesses != NULL; temp_interesses = temp_interesses->prox_interesse){
				struct Interesses *tempTeste = NULL;
				//anda com a lista encadeada de interesses

				tempTeste = temp_interesses;

				for (interesses_temp = interesses_inicio; interesses_temp!=NULL; interesses_temp = interesses_temp->prox){

					if(strcmp(tempTeste->nome,interesses_temp->interesse)==0){
						verif = 1;
						tempTeste = tempTeste->prox_interesse;
						break;
					}else{
        				verif = 0;
        				continue;  
				  	}
				}

				if(verif == 0){
					novo = (struct Cluster_interesses*)malloc(sizeof(struct Cluster_interesses));
					strcpy(novo->interesse, tempTeste->nome);
        			encadeia_interesses(&head_interesses,&interesses_fim,novo);
				}

			}
        }

	}

	return head_interesses;
}

/*Aqui � mostrado em % a quantidade de pessoas de cada cluster de interesses para cada cidade e ao mesmo tempo
separado os grupos com mais pessoas para cada cidade e depois notificando as pessoas da cidade que n�o fazem parte
do grupo*/
void porcentagem_cidade (struct Cidade **head_cidades, struct Cluster_interesses **head_interesses, FILE *arquivo){
	struct lista_perfisInteresses *perfis_interesses = NULL;
	struct Cluster_interesses *temp_interesses = NULL;
	struct Cidade *temp_cidade = NULL;
    struct Cidade *head_cidade_temp = *head_cidades;
    struct Cluster_interesses *GMOVI_TEMP = NULL;
    struct NoListaInteresses *GMOVI = (struct NoListaInteresses*) malloc(sizeof(struct NoListaInteresses)); //GMOVI = GRUPO MOVIMENTADO
    
    while(head_cidade_temp){
    	
		fprintf(arquivo, "\n%d pessoas mora(m) em %s\n", head_cidade_temp->contador, head_cidade_temp->nome);
		head_cidade_temp = head_cidade_temp->prox;
		
	}
	
	head_cidade_temp = *head_cidades;
	
	//aqui ele verifica cidade por cidade
	while(head_cidade_temp){
		struct NoListaInteresses *inicio = NULL;
    	struct NoListaInteresses *ultimo = NULL;
    	struct NoListaInteresses *temp_inicio = NULL;
		int quant_perfis = head_cidade_temp->contador;
		float porcentagem = 0.0f;
		GMOVI->lista = *head_interesses;
		
		fprintf(arquivo, "\n\nDados referentes a cidade de %s:\n\n", head_cidade_temp->nome);
		//aqui contamos quantas pessoas curtem certa coisa de uma certa cidade
		for(temp_interesses = (*head_interesses); temp_interesses != NULL; temp_interesses = temp_interesses->prox){
				
			int perfis = 0;
			
			//loop respons�vel por incrementar o valor da quantidade de pessoas por interesses de cada cidade
			for(perfis_interesses = temp_interesses->lista; perfis_interesses != NULL; perfis_interesses = perfis_interesses->proxi){
			
				if(strcmp(perfis_interesses->perfil->cidade, head_cidade_temp->nome) == 0) perfis+=1;
				
			}
			
			//convertendo quantidade de pessoas da cidade por quantidade de pessoas do grupo na cidade para %
			porcentagem = ((float)perfis/quant_perfis)*100.0;
			
			//verificando se porcentagem � maior que 0 e depois comparando qual interesse tem a maior porcentagem para poder jogar na lista
			if(porcentagem > 0){
					
            	temp_interesses->percent = porcentagem;
				fprintf(arquivo, "De todas as pessoas dessa cidade %.2f%% curtem %s\n", temp_interesses->percent, temp_interesses->interesse);
				
				if(temp_interesses->percent >= GMOVI->lista->percent){
					GMOVI = (struct NoListaInteresses *) malloc(sizeof(struct NoListaInteresses));
					GMOVI->lista = temp_interesses;
					inserirFilaDeInteresses(&ultimo, &inicio, GMOVI);
				}
				
	   		}
	   	
        }
		//imprimir(inicio);

        temp_inicio = inicio;

		while(temp_inicio != NULL){

			fprintf(arquivo, "\nGrupo(s) mais movimentado(s) -> %s", temp_inicio->lista->interesse);
			temp_inicio = temp_inicio->prox;
		
		}
		fprintf(arquivo, "\n\n");

		head_cidade_temp = head_cidade_temp->prox;
	}
}

void popular(struct No *head,FILE *newsfeed){
	struct No *temp=(struct No*)malloc(sizeof(struct No));
	struct Amigos *friends=(struct Amigos*)malloc(sizeof(struct Amigos));
	int i=0,j=0;
	
	for(temp=head;temp!=NULL;temp=temp->no_prox){
		i++;
	}
	
	for(temp=head;temp!=NULL;temp=temp->no_prox){
		for(friends=temp->perfil.amigos;friends!=NULL;friends=friends->prox_amigo){
			j++;
		}
		if(j>=(i/2)){
			fprintf(newsfeed,"%s e popular na rede\n",temp->perfil.nome);
		}
	}
}

void popular_grupo(struct Cidade *head,FILE *newsfeed){
	struct Cidade *temp=(struct Cidade*)malloc(sizeof(struct Cidade));
	struct Perfil *tempo=(struct Perfil*)malloc(sizeof(struct Perfil));
	struct Amigos *colegas=(struct Amigos*)malloc(sizeof(struct Amigos));
	int i;
	
	for(temp=head;temp!=NULL;temp=temp->prox){
		for(tempo=temp->perfil;tempo!=NULL;tempo=tempo->prox_perfilCidade){
			for(colegas=temp->perfil->amigos;colegas!=NULL;colegas=colegas->prox_amigo){
			i++;
			}
			if(i>temp->contador){
				fprintf(newsfeed,"%s e popular no grupo de %s \n",tempo->nome,temp->nome);
			}
			i=0;
		}
	}
}

void popular_interesses(struct  Cluster_interesses *head,FILE *newsfeed){
	struct Cluster_interesses *temp=(struct Cluster_interesses*)malloc(sizeof(struct Cluster_interesses));
	struct lista_perfisInteresses *tempo=(struct lista_perfisInteresses*)malloc(sizeof(struct lista_perfisInteresses));
	struct Perfil *colegas=(struct Perfil*)malloc(sizeof(struct Perfil));
	int i=0;
	
	for(temp=head;temp!=NULL;temp=temp->prox){
		for(tempo=temp->lista;tempo!=NULL;tempo=tempo->proxi){
			for(colegas=tempo->perfil;colegas!=NULL;colegas=colegas->prox_perfilCidade){
			i++;
			}
			if(i>temp->counter){
				fprintf(newsfeed,"%s e popular no grupo de interesses %s \n",tempo->perfil->nome,temp->interesse);
			}
			i=0;
		}
	}
}

void inserirFilaDeInteresses(struct NoListaInteresses **ultimo, struct NoListaInteresses **inicio, struct NoListaInteresses *novo){
	if((*ultimo) != NULL){
		novo->prox = (*ultimo)->prox;
		(*ultimo)->prox = novo;
		(*ultimo) = novo;
		return;
	}
	novo->prox = NULL;
	(*ultimo) = novo;
	(*inicio) = novo;
}

void removerDaFilaDeInteresses(struct NoListaInteresses **inicio){
	(*inicio) = (*inicio)->prox;
}

/*void imprimir(struct NoListaInteresses *Lista){
	
	while(Lista != NULL){
		
		printf("\nGrupo(s) mais movimentado(s) -> %s", Lista->lista->interesse);
		Lista = Lista->prox;
		
	}
	printf("\n\n");
}*/
