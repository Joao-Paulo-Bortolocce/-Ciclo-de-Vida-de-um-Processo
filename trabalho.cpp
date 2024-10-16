#include <stdio.h>
#include <conio2.h>
#include <stdlib.h>
#include<time.h>
#include <ctype.h>
#include <windows.h>
#include <sys/types.h>
#include <unistd.h>

#include "TAD_FilaPronto.h"
#include "TAD_FilaEspera.h"
#include "TAD_FilaTerminado.h"
#include "Interface.h"
#include "capa.h"

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
	novo.tBloqueadoTotal=0;
	novo.estado = 'p';
	novo.tBloqueado = 0;
	novo.tCPU = novo.tRestante = pai.tRestante;
	novo.tTotal = 0;
	novo.cpid = 0;
	novo.bloq=0;
	novo.tBloqueadoFilho=0;
	novo.qtdFilhos=0;
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
	proc.tBloqueadoTotal=0;
	proc.tRestante = 0;
	proc.tCPU=0;
	proc.tTotal = 0;
	proc.cpid = 0;
	proc.prior = 0;
	proc.tBloqueadoFilho = 0;
	proc.bloq=0;
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
	//quadrado(1,1,80,25,14);
	//quadrado(1,20,80,25,14);
	Bordas(1,1,120,30,MAGENTA);
	textcolor(WHITE);
	gotoxy(48,2);
	printf("MENU DE CRIACAO DE PROCESSOS");
	gotoxy(5,6);
	printf("Digite o tempo de cpu desse processo [1 a 50]: ");
	scanf("%d", &processo.tRestante);
	processo.tCPU = processo.tRestante;
	while (processo.tRestante <= 0 || processo.tRestante > 50)
	{
		gotoxy(5,6);
		printf("Tempo digitado invalido, digite o tempo de cpu desse processo [1 a 50]: ");
		scanf("%d", &processo.tRestante);
	}
	processo.pid = pids;
	processo.bloq=0;
	pids++;
	gotoxy(5,7);
	printf("Digite a prioridade deste processo [0-9]: ");
	scanf("%d", &processo.prior);
	processo.prior = processo.prior % 10; // PARA TER CERTEZA QUE O VALOR DA VARIAVEL SERA ENTRE 0 E 9
	FilaP[processo.prior] = EnqueuePronto(FilaP[processo.prior], processo);
	gotoxy(5,8);
	printf("Deseja criar outro processo no momento? S-Sim / N-Nao: ");
	fflush(stdin);
	op = toupper(getch());
	while (op != 'S' && op != 'N')
	{
		fflush(stdin);
		op = toupper(getch());
	}
	system("cls");
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

void menuzinho (TpFilaPronto* prontos[TFP],char &flag,pid_t &pids,int &velocidade){
	char op;
	TpProcesso novo;
	system("cls");
	getch();
	//quadrado(1,1,80,25,14);
	//quadrado(1,20,80,25,14);
	Bordas(1,1,120,30,MAGENTA);
	textcolor(WHITE);
	do{
		gotoxy(48,2);
		printf("#### MENU ####\n");
		gotoxy(5,6);
		printf("Digite o que deseja:\n");
		gotoxy(5,7);
		printf("Digite C para criar novos processos\n");
		gotoxy(5,8);
		printf("Digite ESC para inicializar o fim do programa\n");
		gotoxy(5,9);
		printf("Digite Seta Cima para aumentar a velocidade de execucao\n");
		gotoxy(5,10);
		printf("Digite Seta Baixo para diminuir a velocidade de execucao\n");
		gotoxy(5,11);
		printf("Digite BackSpace para retornar a execucao\n");
		gotoxy(75,4);
		printf("VELOCIDADE: %d",velocidade);
		fflush(stdin);
		op=toupper(getch());
		if(op==-32)
		op=toupper(getch());
		switch (op)
		{
		case 'C':
			RecebeProcessos(prontos,pids);
			break;
		case 72:
			if(velocidade<10)
			{
				velocidade++;
				gotoxy(75,4);
				printf("               ");
				gotoxy(75,4);
				printf("VELOCIDADE: %d",velocidade);
				//reexibir
			}
			else
			{
				gotoxy(2,21);
				printf("                                                        ");
				gotoxy(2,21);
				printf("Velocidade Maxima Atingida");
			}
			break;
		case 80:
			if(velocidade>1)
			{
				velocidade--;
				gotoxy(75,4);
				printf("               ");
				gotoxy(75,4);
				printf("VELOCIDADE: %d",velocidade);
				//reexibir
			}else
			{
				gotoxy(2,21);
				printf("                                                        ");
				gotoxy(2,21);
				printf("Velocidade Minima Atingida");
			}
			break;
			
		case 27:
			flag=0;
			gotoxy(2,21);
			printf("                                                        ");
			gotoxy(2,21);
			printf("Os processos nao criarao mais filhos a partir de agora\n");
			
			break;
		case 8:
			gotoxy(2,21);
			printf("                                                        ");
			gotoxy(2,21);
			printf("Retornando...\n");
			Sleep(1000);
		
			break;
		default:
			gotoxy(2,21);
			printf("                                                        ");
			gotoxy(2,21);
			printf("Comando nao reconhecido");
			break;
		}
	}while(op!=8);
	system("cls");
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
	TpFilaEspera *ant,*aux=FilaE[0];
	TpProcesso pai;
	ant=aux;
	while(aux!=NULL && aux->PCB.pid!=filho.ppid){
		ant=aux;
		aux=aux->prox;
	}
	if(aux!=NULL){
		if( aux==ant){
			FilaE[0]=dequeueEspera(FilaE[0],pai);
			FilaP[pai.prior]=EnqueuePronto(FilaP[pai.prior],pai);
		}
		else{
			ant->prox=aux->prox;
			FilaP[filho.prior]=EnqueuePronto(FilaP[filho.prior],aux->PCB);
			delete aux;
		}
	}
}

void AdicionaTempoBloquado(TpFilaEspera *FilaE[TFE]){
	 int i;
	 TpFilaEspera *aux;
	 for(i=0;i<TFE;i++){
	  	aux=FilaE[i];
	  	while(aux!=NULL){
	   		aux->PCB.tBloqueadoTotal++;
	   		aux=aux->prox;
	  	  }
	 }
}

TpFilaTerminado *  Execucao(TpFilaPronto *FilaP[TFP], TpFilaEspera *FilaE[TFE], TpFilaTerminado *FilaT,pid_t &pids,int &qtdFinalizados)
{
	char continua = 1, flag = 1;
	int ut, maiorPrior = BuscaMaiorPrioridade(FilaP),velocidade = 5,tempoGeral=0;
	TpProcesso run;
	FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
	run.estado = 'x';
	ut = 0;
	displayQueues(FilaP, FilaE, &run,tempoGeral,ut);
	while (continua)
	{
		
		fflush(stdin);
		while (!kbhit() && continua)
		{
			if (run.tRestante == 0)
			{
				qtdFinalizados++;
				FilaT = enqueueTerminado(FilaT,run);
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
					ExibeFilasEspera(FilaE,5,3,4);
					ExibeFilasPronto(FilaP,30,3,10);
					drawRunningProcess(84, 10, &run,tempoGeral,ut);
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
					ExibeFilasEspera(FilaE,5,3,4);
					ExibeFilasPronto(FilaP,30,3,10);
					drawRunningProcess(84, 10, &run,tempoGeral,ut);
				}
				ut = 0;
			}
			else
			{ // No momento da troca de contexto o tempo de wait dos outros processos � diminuido?
				if (flag && sorteia() < 8)
				{
					FORK(FilaP, run, pids);
					run.bloq=1;
					run.tBloqueado = 0;
					FilaE[0]=enqueueEspera(FilaE[0], run);
					maiorPrior = BuscaMaiorPrioridade(FilaP);
					FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
					ut = 0;
					ExibeFilasEspera(FilaE,5,3,4);
					ExibeFilasPronto(FilaP,30,3,10);
					drawRunningProcess(84, 10, &run,tempoGeral,ut);
				}
				else
				{
					if (sorteia() < 8)
					{
						run.bloq=1;
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
						ExibeFilasEspera(FilaE,5,3,4);
						ExibeFilasPronto(FilaP,30,3,10);
						drawRunningProcess(84, 10, &run,tempoGeral,ut);
					}
					// FUNCIONAMENTO DE DECREMENTO E INCREMENTO DE TODOS OS TEMPOS.
				}
			}
			if(continua){
				incrementaTempoTotal(FilaP,FilaE);
				DecrementaTempoBloqueado(FilaE);
				AdicionaTempoBloquado(FilaE);
				WaitToProntoGeral(FilaE, FilaP);
				maiorPrior = BuscaMaiorPrioridade(FilaP);
				if(maiorPrior>run.prior){
					FilaP[run.prior] = EnqueuePronto(FilaP[run.prior], run);
					FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
					ut=0;
				}
				run.tRestante--;
				run.tTotal++;
				ut++;
				ExibeFilasEspera(FilaE,5,3,4);
				ExibeFilasPronto(FilaP,30,3,10);
				drawRunningProcess(84, 10, &run,tempoGeral,ut);
//				//displayQueues(FilaP, FilaE, &run);
				tempoGeral++;
				Sleep(1100-velocidade*100);
			}
		}
		if(flag)
		{
			fflush(stdin);
			menuzinho(FilaP,flag,pids,velocidade);
			maiorPrior = BuscaMaiorPrioridade(FilaP);
			if(maiorPrior>run.prior){
				FilaP[run.prior] = EnqueuePronto(FilaP[run.prior], run);
				FilaP[maiorPrior] = DequeuePronto(FilaP[maiorPrior], run);
				ut=0;
			}	
			displayQueues(FilaP, FilaE, &run,tempoGeral,ut);
		}	
	}
	piscarTextoRELATORIO("RELATORIOS", 57,23);
	getch();
	LimpaTela();
	return FilaT;
}

void contaBloq(TpFilaTerminado *t,int &val,double &tot){
	double valor=0;
	while(t!=NULL){
		if(t->PCB.bloq)
		val++;
		valor+=t->PCB.tBloqueadoTotal;
		t=t->prox;
	}
	
	if(valor==0)
	{
		tot=0;
	}else
	{
		tot=valor/val;
	}
	

}

void qntPronto(TpFilaTerminado *t, int &val){
	while(t!=NULL){
		if(!t->PCB.bloq && t->PCB.tTotal<t->PCB.tCPU){
			val++;
		}
		t=t->prox;
	}
}

void ExibeRelatorios(int qtdFinalizados, TpFilaTerminado * FilaT){
	int bloq=0,prontos=0,i=11;
	TpProcesso p;
	double medioBloq=0;
	//quadrado(1,1,80,25,14);
	Bordas(1,1,120,50,MAGENTA);
	textcolor(WHITE);
	gotoxy(48,2);
	printf("RELATORIOS FINAIS");
	gotoxy(5,6);
	printf("Quantidade de processos finalizados : %d", qtdFinalizados);
	contaBloq(FilaT,bloq,medioBloq);
	gotoxy(5,7);
	printf("Quantidade de processos bloqueados : %d", bloq);
	gotoxy(5,8);
	printf("Media de tempo bloqueado : %.2lf", medioBloq);
	qntPronto(FilaT,prontos);
	gotoxy(5,9);
	printf("Processos entre Execucao e Pronto : %d", prontos);
	gotoxy(5,10);
	printf("Relatorios individuais : ");
	while(!isEmptyTerminado(FilaT)){
		FilaT = dequeueTerminado(FilaT,p);
		gotoxy(5,i++); 
		printf("PID : %d Tempo Total de Execucao: %d",p.pid,p.tTotal);
		gotoxy(5,i++);
		printf("QTD de Filhos: %d , Tempo Bloqueado Pelos Filhos: %d", p.qtdFilhos,p.tBloqueadoFilho);
	}
}

int main()
{
	int i=0;
	srand(time(NULL));
	TpFilaPronto *FilaP[TFP];
	TpFilaEspera *FilaE[TFE];
	TpFilaTerminado *FilaT;
	InicializarFilasDePronto(FilaP);
	InicializarFilasDeEspera(FilaE);
	FilaT = initTerminado();
	pid_t pids = 100;
	//inicioTOP();
	fflush(stdin);
	getch();
	Capa();
	while (RecebeProcessos(FilaP, pids));
	system("cls");
	FilaT = Execucao(FilaP,FilaE,FilaT,pids,i);
	fflush(stdin);
	ExibeRelatorios(i,FilaT);
	gotoxy(1,55);
}
