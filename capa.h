#define MAX_QUEUES 6
#define QUEUE_SIZE 10

typedef struct {
    int items[QUEUE_SIZE];
    int front;
    int rear;
} Queue;

Queue queues[MAX_QUEUES];

void initializeQueues() {
    for (int i = 0; i < MAX_QUEUES; i++) {
        queues[i].front = -1;
        queues[i].rear = -1;
    }
}

void Bordas(int ci, int li, int cf, int lf, int corTexto) {
    int i;
    textcolor(corTexto);
    gotoxy(ci, li); printf("%c", 201);
    gotoxy(cf, li); printf("%c", 187);
    gotoxy(ci, lf); printf("%c", 200);
    gotoxy(cf, lf); printf("%c", 188);
    
    for(i = ci + 1; i < cf; i++) {
        gotoxy(i, li); printf("%c", 205);
        gotoxy(i, lf); printf("%c", 205);
    }
    for(i = li + 1; i < lf; i++) {
        gotoxy(ci, i); printf("%c", 186);
        gotoxy(cf, i); printf("%c", 186);
    }
}

void drawBorder(int x, int y, int width, int height) {
    int i;
    textcolor(WHITE);
    
    gotoxy(x, y);
    printf("%c", 218);
    for (i = 1; i < width - 1; i++)
        printf("%c", 196);
    printf("%c", 191);
    
    for (i = 1; i < height - 1; i++) {
        gotoxy(x, y + i);
        printf("%c", 179);
        gotoxy(x + width - 1, y + i);
        printf("%c", 179);
    }

    gotoxy(x, y + height - 1);
    printf("%c", 192);
    for (i = 1; i < width - 1; i++)
        printf("%c", 196);
    printf("%c", 217);
}

void drawQueue(int x, int y, int queueNumber, const char* title, void* queue) {
    int i;
    textcolor(WHITE);

    gotoxy(x, y - 1);
    cprintf("%s", title);

    for (i = 0; i < QUEUE_SIZE; i++) {
        gotoxy(x, y + i);
        cprintf("|    |");
    }
    gotoxy(x, y + QUEUE_SIZE);
    cprintf("------");
    
    // Draw queue items (this part needs to be adapted based on your queue structure)
    // This is just a placeholder, you'll need to implement this based on your actual queue structure
    /*
    TpFilaPronto* current = (TpFilaPronto*)queue;
    for (i = 0; i < QUEUE_SIZE && current != NULL; i++) {
        gotoxy(x + 1, y + i);
        cprintf("%3d", current->PCB.pid);
        current = current->prox;
    }
    */
}

void drawRunningProcess(int x, int y, TpProcesso* running, int geral, int ut) {
    textcolor(YELLOW);
    gotoxy(x, y);
    printf("Processo sendo executado:");
    gotoxy(x, y + 1);
    printf("---------------");
    gotoxy(x, y + 2);
    printf("          ");
    gotoxy(x, y + 2);
    printf("PID: %d", running->pid);
    gotoxy(x, y + 3);
    printf("Prioridade: %d", running->prior);
    gotoxy(x, y + 4);
    printf("                    ");
    gotoxy(x, y + 4);
    printf("Tempo Restante: %d", running->tRestante);
    gotoxy(x, y + 5);
    printf("                    ");
    gotoxy(x, y + 5);
    printf("Tempo Geral: %d", geral);
    gotoxy(x, y + 6);
    printf("                       ");
    gotoxy(x, y + 6);
    printf("Fatia de Tempo: %d", ut);
}

void drawBlockedQueue(int x, int y, const char* title, TpFilaEspera* queue) {
    int i;
    int queueWidth = 10;
    int queueHeight = QUEUE_SIZE + 2;
    
    drawBorder(x, y, queueWidth, queueHeight);

    gotoxy(x + (queueWidth - strlen(title)) / 2, y - 1);
    printf("%s", title);

    TpFilaEspera* current = queue;
    for (i = 0; i < QUEUE_SIZE; i++) {
        gotoxy(x + 1, y + queueHeight - 2 - i);
        if (current != NULL) {
            printf("%3d", current->PCB.pid);
            current = current->prox;
        } else {
            printf("   ");
        }
    }
}
void ExibeVetores(int x, int y, int qtd){
	int i,j;
	gotoxy(x,y);
	printf("%c",201);
	gotoxy(x+1,y);
	printf("%c",205);
	gotoxy(x+2,y);
	printf("%c",187);
	for (i=1;i<qtd*2;i++){
		j=qtd-1-(i-1)/2;
		gotoxy(x,y+i);
		printf("%c",186);
		gotoxy(x+1,y+i);
		textcolor(j+1);
		printf("%d",j);
		textcolor(15);
		gotoxy(x+2,y+i);
		printf("%c",186);
		i++;
		gotoxy(x,y+i);
		printf("%c",204);
		gotoxy(x+1,y+i);
		printf("%c",205);
		gotoxy(x+2,y+i);
		printf("%c",185);
	}
	i--;
	gotoxy(x,y+i);
	printf("%c",200);
	gotoxy(x+1,y+i);
	printf("%c",205);
	gotoxy(x+2,y+i);
	printf("%c",188);
}

void ExibeFilasEspera(TpFilaEspera *f[TFE],int x, int y, int qtd){
	ExibeVetores(x,y,qtd);
	ExibirEspera(f,x+3,y+1);
}

void ExibeFilasPronto(TpFilaPronto *f[TFP],int x, int y, int qtd){
	ExibeVetores(x,y,qtd);
	ExibirPronto(f,x+3,y+1);
}

void displayQueues(TpFilaPronto* FilaP[], TpFilaEspera* FilaE[], TpProcesso* running, int geral, int ut) {
    int i, startX = 2, startY = 5, j;
    char title[20];
    
    clrscr();
    
    Bordas(1,1,120,40,MAGENTA);
    textcolor(WHITE);
    gotoxy(48, 2);
    printf("Process Queue Visualization");

	ExibeFilasEspera(FilaE, 5, 3, 4);
	ExibeFilasPronto(FilaP, 30, 3, 10);
	
//    for (i = 0; i < 10; i++) {
//        sprintf(title, "Ready %d", i);
//        drawQueue(startX + i * 15, startY, i, title, FilaP[i]);
//    }
//    
//    for (i = 0; i < 4; i++) {
//        sprintf(title, "Blocked %d", i);
//        drawQueue(startX + i * 15, startY + 15, i, title, FilaE[i]);
//    }

    drawRunningProcess(84, 10, running,geral,ut);
}


void PintarTela(int corDeFundo){
	int i, j;
	for(i=2;i<120;i++){
		for(j=2; j<30;j++){
			gotoxy(i,j);
			textbackground(LIGHTBLUE);
			printf(" ");
		}
	}
	
}

void LimpaTela(){
	int i, j;
	textbackground(0);
	for(i=2;i<140;i++)
		for(j=2;j<40;j++){
			gotoxy(i,j);
			printf(" ");
		}
}


void piscarTextoENTRAR(char *texto, int x, int y) {
    int visivel = 1;

    while (!kbhit()) {  
        gotoxy(x, y);   
        if (visivel) {
            textbackground(7);  
            textcolor(4);       
            printf("%s", texto);
        } else {
            textbackground(7);  
            printf("      ");   
        }
        visivel = !visivel;  
        Sleep(500);          
    }
}

void piscarTextoRELATORIO(char *texto, int x, int y) {
    int visivel = 1;

    while (!kbhit()) {  
        gotoxy(x, y);   
        if (visivel) {
            textbackground(7);  
            textcolor(4);       
            printf("%s", texto);
        } else {
            textbackground(7);  
            printf("          ");   
        }
        visivel = !visivel;  
        Sleep(500);          
    }
}

void drawHeader() {
    Bordas(20, 1, 100, 3, YELLOW);
    gotoxy(45, 2);
    textcolor(YELLOW);
    printf("Trabalho Ciclo de Vida de Processos");
}

void drawDeveloperInfo() {
    int startY = 25;
    char *devs[] = {"Desenvolvido por:", "Caua Pereira Domingues", "Joao Manuel Oliveira Pereira", "Joao Paulo Saccomani de Freitas Bortolloce"};
    int numLines = 4;
    int screenWidth = 120; //tamanho da tela de 120 carac

    textcolor(WHITE);
    for (int i = 0; i < numLines; i++) {
        int lineLength = strlen(devs[i]);
        int startX = (screenWidth - lineLength) / 2;
        gotoxy(startX, startY + i);
        printf("%s", devs[i]);
    }
}

void LimpaFila(int i, int j){
	int aux;
	
	textbackground(0);
	gotoxy(i,j);
	for(aux=15;aux>0;aux--)
		printf(" ");
}

void Capa(){
	
	int i=30, j=7, selectedOption = 0, key;
	char string[100];
	FILE *ptr =fopen("NomeTrab.txt", "r+");
	initializeQueues();
	Bordas(1,1,120,30,MAGENTA); //Coordenadas para capa
	PintarTela(4);
	drawDeveloperInfo();
	textcolor(7);
	fgets(string, 100, ptr);
	while(!feof(ptr)){
		gotoxy(i,j);
		printf("%s", string);
		j++;
		fgets(string, 100, ptr);
	}
	piscarTextoENTRAR("ENTRAR",57,23);
	LimpaTela();
	getche();
	fclose(ptr);
	gotoxy(35,30);
	textbackground(0);
}
