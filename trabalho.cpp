#include<stdio.h>
#include <conio.h>
#include<stdlib.h>
#include<ctype.h>
#include <sys/types.h>
#include <unistd.h>

#include"TAD_FilaPronto.h"
#include"TAD_FilaEspera.h"

#define TFP 10
#define TFE 4

//função para criar filhos --> FORK(FilaP[TFP],TpProcesso pai, pid_t &pids)
//Função para colocar pai na fila de espera --> ProntoToWait(FilaE[TFE],TpProcesso pai,int tempoBloqueado);

int sorteia(){
	return rand()%100;
}

TpProcesso InicializarProcesso(){
	TpProcesso proc;
	proc.pid=0;
	proc.ppid=0;
	proc.estado='p';
	proc.qtdFilhos=0;
	proc.tBloqueado=0;
	proc.tRestante=0;
	proc.tTotal=0;
	proc.cpid=0;
	proc.prior=0;
	return proc;
}

void InicializarFilasDeEspera(TpFilaEspera *f[TFP]){
	int i;
	for(i=0;i<TFE;i++)
		f[i]=NULL;
}

void InicializarFilasDePronto(TpFilaPronto *f[TFP]){
	int i;
	for(i=0;i<TFP;i++)
		f[i]=NULL;
}

char RecebePrimeirosProcessos(TpFilaPronto *FilaP[TFP],pid_t &pids){
	system("cls");
	TpProcesso processo;
	char op;
	processo=InicializarProcesso();
	printf("Digite o tempo de cpu desse processo [1 a 50]: ");
	scanf("%d",&processo.tRestante);
	while(processo.tRestante <=0 || processo.tRestante>50){
		printf("Tempo digitado eh invalido, digite o tempo de cpu desse processo [1 a 50]: ");
		scanf("%d",&processo.tRestante);
	}
	processo.pid=pids;
	pids++;
	printf("Digite a prioridade deste processo [0-9]: ");
	scanf("%d",&processo.prior);
	processo.prior = processo.prior %10;// PARA TER CERTEZA QUE O VALOR DA VARIAVEL SERA ENTRE 0 E 9
	FilaP[processo.prior]=EnqueuePronto(FilaP[processo.prior],processo);
	printf("Deseja criar outro processo no momento? S-Sim / N-Nao: ");
	fflush(stdin);
	op=toupper(getch());
	while(op!='S' && op!='N'){
		fflush(stdin);
		op=toupper(getch());
	}
	if(op=='S')
		return 1;
	return 0;
}

int BuscaMaiorPrioridade(TpFilaPronto *FilaP[TFP]){
	int i;
	for(i=9;i>=0 && FilaP[i]==NULL;i--);
	return i;
}

void DecrementaTempoBloqueado(TpFilaEspera *FilaE[TFE]){
	int i;
	TpFilaEspera *aux;
	for(i=1;i<TFE;i++){
		aux=FilaE[i];
		while(aux!=NULL){
			aux->PCB.tBloqueado--;
			aux=aux->prox;
		}
	}
}

void WaitToProntoGeral(TpFilaEspera *FilaE[TFE],TpFilaPronto *FilaP[TFP]){
	int i;
	TpFilaEspera *aux;
	TpProcesso processo;
	for(i=1;i<TFE;i++){
		aux=FilaE[i];
		if(aux!=NULL && aux->PCB.tBloqueado==0){
			FilaE[i]=dequeueEspera(FilaE[i],processo);
			FilaP[processo.prior]=EnqueuePronto(FilaP[processo.prior],processo);
		}
	}
}

void Execucao(TpFilaPronto *FilaP[TFP],	TpFilaEspera *FilaE[TFE],pid_t &pids){
	char continua=1,flag=1;
	int ut,maiorPrior=BuscaMaiorPrioridade(FilaP),qtdFinalizados=0;;
	TpProcesso run;
	FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
	run.estado='x';
	while(continua){
		ut=0;
		while(!kbhit()){
			if(run.tRestante==0){
				qtdFinalizados++;
				maiorPrior=BuscaMaiorPrioridade(FilaP);
				if(maiorPrior>0){
					//WaitToProntoPai(FilaP[run.prior],FilaE[0],run.ppid);//Função que retira o pai e coloca na fila de pronto, SE ELE TIVER PAI;
					FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
					ut=0;
				}
				else
					continua=0; // se for -1 quer dizer que todas as filas estão vazias, portanto pode finalizar a execucao
			}
			if(ut==10){
				maiorPrior=BuscaMaiorPrioridade(FilaP);
				if(run.prior <= maiorPrior){ //Verifica se haverá mudança de contexto
					FilaP[run.prior]=EnqueuePronto(FilaP[run.prior],run);
					FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
				}
				ut=0;
			}
			else{//No momento da troca de contexto o tempo de wait dos outros processos é diminuido?
				if(flag && sorteia()<3){
					FORK(FilaP,run, pids);
					run.tBloqueado=0;
					enqueueEspera(FilaE[0],run);
					maiorPrior=BuscaMaiorPrioridade(FilaP);
					FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
					ut=0;
				}
				else{
					if(sorteia()<3){
						switch(sorteia()%3+1){
							case 1:// Fila de mouse
								run.tBloqueado=8;
								enqueueEspera(FilaE[1],run);
								break;
							case 2:// Fila de Teclado
								run.tBloqueado=15;
								enqueueEspera(FilaE[2],run);
								break;
							case 3:// Fila de HD
								run.tBloqueado=25;
								enqueueEspera(FilaE[3],run);
								break;
						}
						maiorPrior=BuscaMaiorPrioridade(FilaP);
						while(maiorPrior<0){
							DecrementaTempoBloqueado(FilaE);
							WaitToProntoGeral(FilaE,FilaP);
							maiorPrior=BuscaMaiorPrioridade(FilaP);
						}
						FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
						ut=0;
					}
					//FUNCIONAMENTO DE DECREMENTO E INCREMENTO DE TODOS OS TEMPOS.
						
				}
				
			} 
		}
	}
}

int main(){
	TpFilaPronto *FilaP[TFP];
	TpFilaEspera *FilaE[TFE];
	InicializarFilasDePronto(FilaP);
	InicializarFilasDeEspera(FilaE);
	pid_t pids=100;
	printf("Pressione qualquer tecla para criar primeiro processo!!!");
	fflush(stdin);
	getch();
	while(RecebePrimeirosProcessos(FilaP,pids));
	Execucao(FilaP,FilaE,pids);
}
