#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <sys/types.h>
#include <unistd.h>

#include "TAD_FilaPronto.h"
#include "TAD_FilaEspera.h"
//#include "Interface.h"

#define TFP 10
#define TFE 4

// fun��o para criar filhos --> FORK(FilaP[TFP],TpProcesso pai, pid_t &pids)
// Fun��o para colocar pai na fila de espera --> ProntoToWait(FilaE[TFE],TpProcesso pai,int tempoBloqueado);



TpProcesso FORK(TpFilaPronto *prontos[TFP], TpProcesso &pai, pid_t &pids)
{
	TpProcesso novo;
	pai.cpid = pids;
	novo.pid = pids++;
	novo.ppid = pai.pid;
	novo.estado = 'p';
	novo.tBloqueado = 0;
	novo.tRestante = pai.tRestante;
	novo.tTotal = 0;
	novo.cpid = 0;
	novo.prior = pai.prior;
	pai.qtdFilhos++;
	prontos[novo.prior] = EnqueuePronto(prontos[novo.prior], novo);
}

int sorteia()
{
	return rand() % 100;
}

TpProcesso InicializarProcesso()
{
	TpProcesso proc;
	proc.pid = 0;
	proc.ppid = 0;
	proc.estado = 'p';
	proc.qtdFilhos = 0;
	proc.tBloqueado = 0;
	proc.tRestante = 0;
	proc.tTotal = 0;
	proc.cpid = 0;
	proc.prior = 0;
	proc.tBloqueadoFilho = 0;
	return proc;
}

void InicializarFilasDeEspera(TpFilaEspera *f[TFP])
{
	int i;
	for (i = 0; i < TFE; i++)
		f[i] = NULL;
}

void InicializarFilasDePronto(TpFilaPronto *f[TFP])
{
	int i;
	for (i = 0; i < TFP; i++)
		f[i] = NULL;
}

char RecebeProcessos(TpFilaPronto *FilaP[TFP], pid_t &pids)
{
	system("cls");
	TpProcesso processo;
	char op;
	processo = InicializarProcesso();
	printf("Digite o tempo de cpu desse processo [1 a 50]: ");
	scanf("%d", &processo.tRestante);
	while (processo.tRestante <= 0 || processo.tRestante > 50)
	{
		printf("Tempo digitado eh invalido, digite o tempo de cpu desse processo [1 a 50]: ");
		scanf("%d", &processo.tRestante);
	}
	processo.pid = pids;
	pids++;
	printf("Digite a prioridade deste processo [0-9]: ");
	scanf("%d", &processo.prior);
	processo.prior = processo.prior % 10; // PARA TER CERTEZA QUE O VALOR DA VARIAVEL SERA ENTRE 0 E 9
	FilaP[processo.prior] = EnqueuePronto(FilaP[processo.prior], processo);
	printf("Deseja criar outro processo no momento? S-Sim / N-Nao: ");
	fflush(stdin);
	op = toupper(getch());
	while (op != 'S' && op != 'N')
	{
		fflush(stdin);
		op = toupper(getch());
	}
	if (op == 'S')
		return 1;
	return 0;
}

int BuscaMaiorPrioridade(TpFilaPronto *FilaP[TFP])
{
	int i;
	for (i = 9; i >= 0 && FilaP[i] == NULL; i--);
	return i;
}

void DecrementaTempoBloqueado(TpFilaEspera *FilaE[TFE])
{
	int i;
	TpFilaEspera *aux;
	for (i = 1; i < TFE; i++)
	{
		aux = FilaE[i];
		while (aux != NULL)
		{
			aux->PCB.tBloqueado--;
			aux = aux->prox;
		}
	}
}

void WaitToProntoGeral(TpFilaEspera *FilaE[TFE], TpFilaPronto *FilaP[TFP])
{
	int i;
	TpFilaEspera *aux;
	TpProcesso processo;
	for (i = 1; i < TFE; i++)
	{
		aux = FilaE[i];
		if (aux != NULL && aux->PCB.tBloqueado == 0)
		{
			FilaE[i] = dequeueEspera(FilaE[i], processo);
			FilaP[processo.prior] = EnqueuePronto(FilaP[processo.prior], processo);
		}
	}
}

void incrementaTempoTotal(TpFilaPronto *prontos[TFP], TpFilaEspera *espera[TFE])
{
	int i;
	TpFilaPronto *auxP;
	TpFilaEspera *auxE;
	for (i = 0; i < TFP; i++)
	{
		auxP = prontos[i];
		if(auxP!=NULL){
			auxP->PCB.tTotal++;
			auxP=auxP->prox;
			while (auxP != prontos[i])
			{
				auxP->PCB.tTotal++;
				auxP = auxP->prox;
			}
		}	
	}
	for (i = 1; i < TFE; i++)
	{
		auxE = espera[i];
		while (auxE != NULL)
		{
			auxE->PCB.tTotal++;
			auxE=auxE->prox;
		}
	}
	auxE = espera[0];
	while (auxE != NULL)
	{
		auxE->PCB.tTotal++;
		auxE->PCB.tBloqueadoFilho++;
		auxE=auxE->prox;
	}
}

void menuzinho (TpFilaPronto* prontos[TFP],char &flag,pid_t &pids){
	char op;
	TpProcesso novo;
	system("cls");
	do{
		printf("#### MENU ####\n");
		printf("Digite o que deseja:\n");
		printf("Digite C para criar novos processos");
		printf("Digite ESC para inicializar o fim do programa");
		fflush(stdin);
		op=getch();
		switch (op)
		{
		case 'C':
			RecebeProcessos(prontos,pids);
			break;
			
		case 27:
			flag=0;
			printf("Os processos nao criarao mais filhos a partir de agora\n");
			break;
		}
	}while(op!=27);
}

char ExisteProcesso(TpFilaPronto *FilaP[TFP], TpFilaEspera *FilaE[TFE]){
	int i;
	for(i=0;i<TFP;i++)
		if(FilaP[i]!=NULL)
			return 1;
	for(i=0;i<TFE;i++)
		if(FilaE[i]!=NULL)
			return 1;
	return 0;
}

void WaitToProntoPai(TpFilaPronto *FilaP[TFP],TpFilaEspera *FilaE[TFE],TpProcesso filho){
	TpFilaEspera *ant,*aux=FilaE[filho.prior];
	TpProcesso pai;
	ant=aux;
	while(aux!=NULL && aux->PCB.pid!=filho.ppid){
		ant=aux;
		aux=aux->prox;
	}
	if(aux!=NULL){
		if( FilaE[filho.prior]==ant){
			FilaE[filho.prior]=dequeueEspera(FilaE[filho.prior],pai);
			FilaP[pai.prior]=EnqueuePronto(FilaP[pai.prior],pai);
		}
		else{
			ant->prox=aux->prox;
			FilaP[filho.prior]=EnqueuePronto(FilaP[filho.prior],aux->PCB);
			delete aux;
		}
	}
}

void Execucao(TpFilaPronto *FilaP[TFP], TpFilaEspera *FilaE[TFE], pid_t &pids)
{
	char continua = 1, flag = 1;
	int ut, maiorPrior = BuscaMaiorPrioridade(FilaP), qtdFinalizados = 0;
	TpProcesso run;
	FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
	run.estado = 'x';
	while (continua)
	{
		ut = 0;
		while (!kbhit() && continua)
		{
			if (run.tRestante == 0)
			{
				qtdFinalizados++;
				WaitToProntoPai(FilaP,FilaE,run);
				if (ExisteProcesso(FilaP,FilaE))
				{
					maiorPrior = BuscaMaiorPrioridade(FilaP);
					if(maiorPrior<0){
						while (maiorPrior < 0)
						{
							DecrementaTempoBloqueado(FilaE);
							WaitToProntoGeral(FilaE, FilaP);
							incrementaTempoTotal(FilaP,FilaE);
							maiorPrior = BuscaMaiorPrioridade(FilaP);
						}
					}
					//Fun��o que retira o pai e coloca na fila de pronto, SE ELE TIVER PAI;
					FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
					ut = 0;
				}
				else{
					continua = 0; // se for -1 quer dizer que todas as filas est�o vazias, portanto pode finalizar a execucao
					flag=0;
				}
					}
			if (ut == 10)
			{
				maiorPrior = BuscaMaiorPrioridade(FilaP);
				if (run.prior <= maiorPrior)
				{ // Verifica se haver� mudan�a de contexto
					FilaP[run.prior] = EnqueuePronto(FilaP[run.prior], run);
					FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
				}
				ut = 0;
			}
			else
			{ // No momento da troca de contexto o tempo de wait dos outros processos � diminuido?
				if (flag && sorteia() < 3)
				{
					FORK(FilaP, run, pids);
					run.tBloqueado = 0;
					enqueueEspera(FilaE[0], run);
					maiorPrior = BuscaMaiorPrioridade(FilaP);
					FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
					ut = 0;
				}
				else
				{
					if (sorteia() < 3)
					{
						switch (sorteia() % 3 + 1)
						{
						case 1: // Fila de mouse
							run.tBloqueado = 8;
							FilaE[1]=enqueueEspera(FilaE[1], run);
							break;
						case 2: // Fila de Teclado
							run.tBloqueado = 15;
							FilaE[2]=enqueueEspera(FilaE[2], run);
							break;
						case 3: // Fila de HD
							run.tBloqueado = 25;
							FilaE[3]=enqueueEspera(FilaE[3], run);
							break;
						}
						maiorPrior = BuscaMaiorPrioridade(FilaP);
						while (maiorPrior < 0)
						{
							DecrementaTempoBloqueado(FilaE);
							WaitToProntoGeral(FilaE, FilaP);
							incrementaTempoTotal(FilaP,FilaE);
							maiorPrior = BuscaMaiorPrioridade(FilaP);
						}
						FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
						ut = 0;
					}
					// FUNCIONAMENTO DE DECREMENTO E INCREMENTO DE TODOS OS TEMPOS.
				}
			}
			incrementaTempoTotal(FilaP,FilaE);
			DecrementaTempoBloqueado(FilaE);
			WaitToProntoGeral(FilaE, FilaP);
			run.tRestante--;
			run.tTotal++;
			ut++;
			Sleep(500);
		}
		if(flag)
			menuzinho(FilaP,flag,pids);
	}
}

int main()
{
	TpFilaPronto *FilaP[TFP];
	TpFilaEspera *FilaE[TFE];
	InicializarFilasDePronto(FilaP);
	InicializarFilasDeEspera(FilaE);
	pid_t pids = 100;
	printf("Pressione qualquer tecla para criar primeiro processo!!!");
	fflush(stdin);
	getch();
	while (RecebeProcessos(FilaP, pids));
	Execucao(FilaP, FilaE, pids);
}
