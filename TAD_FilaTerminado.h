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
	TpFilaTerminado *novo = novoTerminado(p), *aux=f,*ant=f;
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
	p=f->PCB;
	f=f->prox;
	
	delete aux;
	return f;
}

char isEmptyTerminado (TpFilaTerminado *f){
	return f==NULL;
}

void exibeTerminado(TpFilaTerminado *f){
	TpProcesso p;
	while(f!=NULL){
		p=f->PCB;
		printf("PID : %d\n",p.pid);
		f = f->prox;
	}
}
