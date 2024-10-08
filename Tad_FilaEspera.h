struct TpFilaEspera{
	TpProcesso PCB;
	TpFilaEspera * prox;
};

TpFilaEspera* initEspera(){
	return NULL;
}

TpFilaEspera* novoProcessoEspera(TpProcesso p){
	TpFilaEspera *caixa = new TpFilaEspera;
	caixa->PCB=p;
	caixa->prox=NULL;
	return caixa;
}

TpFilaEspera* enqueueEspera (TpFilaEspera *f,TpProcesso p)
{
	TpFilaEspera *nova = novoProcessoEspera(p),*aux;
	if(f==NULL){
		f=nova;
	}
	else
	{
		aux=f;
		while(aux->prox!=NULL)
			aux=aux->prox;
			
		aux->prox=nova;
	}
	return f;
}
TpFilaEspera * dequeueEspera (TpFilaEspera *f,TpProcesso &p){
	TpFilaEspera *aux;
	aux=f;
	p=f->PCB;
	if(f->prox==NULL){
		delete aux;
		return NULL;
	}
	else
	{
		f=f->prox;
		delete aux;
		return f;	
	}
}

char isEmptyEspera (TpFilaEspera *f)
{
	return f == NULL;
}

TpProcesso topoEspera (TpFilaEspera * f){
	if(f!=NULL)
	{
		TpProcesso p = f->PCB;
		return p;
	}
}

void exibeEspera (TpFilaEspera *f){
	TpProcesso p;
	while(!isEmptyEspera(f)){
		f = dequeueEspera(f,p);
		printf("%u\n",p.pid);
	}
}
