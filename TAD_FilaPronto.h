#define TFP 10

struct TpProcesso
{
	int qtdFilhos, tRestante, tCPU,tBloqueado, tTotal, prior, tBloqueadoFilho;
	pid_t pid, ppid, cpid, tBloqueadoTotal;
	char estado,bloq; // P=pronto, E=espera, X=executando
};

struct TpFilaPronto
{
	TpFilaPronto *prox;
	TpProcesso PCB;
};

TpFilaPronto *InitPronto()
{
	return NULL;
}

void ExibirPronto(TpFilaPronto *f[TFP], int x, int y){
	int i;
	TpFilaPronto *aux;
	for(i=TFP-1;i>=0;i--,y+=2){
		aux= f[i];
		gotoxy(x,y);
		if(aux!=NULL){
			textcolor(i+1);
			printf("%c  ",1);
			aux=aux->prox;
		}	
		while(aux!=f[i]){
			printf("%c  ",1);
			aux = aux ->prox;
		}
	}
}

TpFilaPronto *novoProcessoPronto(TpProcesso processo)
{
	TpFilaPronto *caixa = new TpFilaPronto;
	caixa->PCB = processo;
	caixa->prox = NULL;
	return caixa;
}

TpFilaPronto *EnqueuePronto(TpFilaPronto *f, TpProcesso processo)
{
	TpFilaPronto *nova = novoProcessoPronto(processo), *aux;
	if (f == NULL)
		f = nova;
	else
	{
		aux = f;
		while (aux->prox != f)
		{
			aux = aux->prox;
		}
		aux->prox = nova;
	}
	nova->prox = f;
	return f;
}

TpFilaPronto *DequeuePronto(TpFilaPronto *f, TpProcesso &processo)
{
	TpFilaPronto *aux = f;
	if (f->prox == f)
	{
		processo = f->PCB;
		delete f;
		return NULL;
	}
	else
	{
		while (aux->prox != f)
		{
			aux = aux->prox;
		}
		aux->prox = f->prox;
		aux = f;
		f = aux->prox;
		processo = aux->PCB;
		delete aux;
		return f;
	}
}

char isEmptyPronto(TpFilaPronto *f)
{
	return f == NULL;
}
