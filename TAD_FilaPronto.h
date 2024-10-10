struct TpProcesso{
	int qtdFilhos, tRestante, tBloqueado, tTotal,prior;
	pid_t pid,ppid,cpid;
	char estado; //P=pronto, E=espera, X=executando
};

struct TpFilaPronto{
	TpFilaPronto *prox;
	TpProcesso PCB;
};

TpFilaPronto* InitPronto(){
	return NULL;
}

TpFilaPronto* novoProcessoPronto(TpProcesso processo){
	TpFilaPronto *caixa = new TpFilaPronto;
	caixa->PCB=processo;
	caixa->prox=NULL;
	return caixa;
}

TpFilaPronto *EnqueuePronto(TpFilaPronto *f, TpProcesso processo){
	TpFilaPronto *nova=novoProcessoPronto(processo),*aux;
	if(f==NULL)
		f=nova;	
	else{
		aux=f;
		while(aux->prox!=f){
			aux=aux->prox;
		}
		aux->prox=nova;
	}
	nova->prox=f;	
	return f;
}

TpFilaPronto* DequeuePronto(TpFilaPronto *f, TpProcesso &processo){
	TpFilaPronto *aux=f;
	if(f->prox==f){
		processo=f->PCB;
		delete f;
		return NULL;
	}
	else{
		while(aux->prox!=f){
			aux=aux->prox;
		}
		aux->prox=f->prox;
		aux=f;
		f=aux->prox;
		processo=aux->PCB;
		delete aux;
		return f;
	}
}

char isEmptyPronto(TpFilaPronto *f){
	return f==NULL;
}
