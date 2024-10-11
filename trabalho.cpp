#include<stdio.h>
#include <conio.h>
#include<ctype.h>
#include <sys/types.h>
#include <unistd.h>

#include"TAD_FilaPronto.h"
#include"TAD_FilaEspera.h"

#define TFP 10
#define TFE 4

//função para criar filhos --> FORK(FilaP[TFP],TpProcesso pai, *pids)
//Função para colocar pai na fila de espera --> PaiToWait(FilaE[TFE],TpProcesso pai);


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

void Execucao(TpFilaPronto *FilaP[TFP],	TpFilaEspera *FilaE[TFE],pid_t &pids){
	char flag=1;
	int ut,maiorPrior=BuscaMaiorPrioridade(FilaP),qtdFinalizados=0;;
	TpProcesso run;
	FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
	run.estado='x';
	while(flag){
		ut=0;
		while(!kbhit()){
			if(run.tRestante==0){
				qtdFinalizados++;
				maiorPrior=BuscaMaiorPrioridade(FilaP);
				if(maiorPrior>0){
					FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
					ut=0;
				}
				else
					flag=0; // se for -1 quer dizer que todas as filas estão vazias, portanto pode finalizar a execucao
			}
			if(ut==10){
				maiorPrior=BuscaMaiorPrioridade(FilaP);
				if(run.prior <= maiorPrior){ //Verifica se haverá mudança de contexto
					FilaP[run.prior]=EnqueuePronto(FilaP[run.prior],run);
					FilaP[maiorPrior]=DequeuePronto(FilaP[maiorPrior],run);
				}
				ut=0;
			}
			//else{} No momento da troca de contexto o tempo de wait dos outros processos é diminuido?
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
