struct TpFilaTerminado{
	TpFilaTerminado *prox;
	TpProcesso PCB;
};

TpFilaTerminado * initTerminado(){
	return NULL;
}

TpFilaTerminado * novoTerminado(TpProcesso p){
	TpFilaTerminado *caixa = new TpFilaTerminado;
	caixa->PCB = p;
	caixa->prox = NULL;
	return caixa;
}

TpFilaTerminado * enqueueTerminado(TpFilaTerminado * f, TpProcesso p){
	TpFilaTerminado *novo = novoTerminado(p), *aux=f,*ant;
	if(f==NULL){
		f=novo;
	}
	else
	{
		while(aux!=NULL)
		{
			ant=aux;
			aux=aux->prox;
		}
		
		ant->prox=novo;
	}
	return f;
}

TpFilaTerminado * dequeueTerminado(TpFilaTerminado *f,TpProcesso &p){
	TpFilaTerminado *aux = f;
	f=f->prox;
	p=f->PCB;
	delete aux;
	return f;
}

char isEmptyTerminado (TpFilaTerminado *f){
	return f==NULL;
}

