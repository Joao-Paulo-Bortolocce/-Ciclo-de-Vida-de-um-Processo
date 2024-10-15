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

void enqueue(int queueNum, int value) {
    if (queues[queueNum].rear == QUEUE_SIZE - 1) {
        // Queue is full
        return;
    }
    if (queues[queueNum].front == -1) {
        queues[queueNum].front = 0;
    }
    queues[queueNum].rear++;
    queues[queueNum].items[queues[queueNum].rear] = value;
}

int dequeue(int queueNum) {
    if (queues[queueNum].front == -1) {
        // Queue is empty
        return -1;
    }
    int item = queues[queueNum].items[queues[queueNum].front];
    queues[queueNum].front++;
    if (queues[queueNum].front > queues[queueNum].rear) {
        queues[queueNum].front = queues[queueNum].rear = -1;
    }
    return item;
}

void drawQueue(int x, int y, int queueNumber) {
    int i;
    textcolor(WHITE);
    
    gotoxy(x, y);
    printf("%c", 218);
    for (i = 1; i <= QUEUE_SIZE; i++) {
        printf("%c%c%c", 196, 196, 196);
        if (i < QUEUE_SIZE) printf("%c", 194);
    }
    printf("%c", 191);
    
    gotoxy(x, y + 1);
    for (i = 0; i <= QUEUE_SIZE; i++) {
        printf("%c   ", 179);
    }
    
    gotoxy(x, y + 2);
    printf("%c", 192);
    for (i = 1; i <= QUEUE_SIZE; i++) {
        printf("%c%c%c", 196, 196, 196);
        if (i < QUEUE_SIZE) printf("%c", 193);
    }
    printf("%c", 217);
    
    gotoxy(x, y + 3);
    printf("Queue %d", queueNumber + 1);
}

void displayQueues() {
    for (int i = 0; i < MAX_QUEUES; i++) {
        drawQueue(5, 5 + i * 4, i);
        
        if (queues[i].front != -1) {
            for (int j = queues[i].front; j <= queues[i].rear; j++) {
                gotoxy(6 + j * 4, 6 + i * 4);
                printf("%2d", queues[i].items[j]);
            }
        }
    }
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
	for(i=2;i<120;i++)
		for(j=2;j<30;j++){
			gotoxy(i,j);
			printf(" ");
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

void piscarTexto(const char *texto, int x, int y) {
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

void drawMenu(int selectedOption) {
    int startY = 7;
    char *options[] = {"1 - Cria Filho", "2 - Aumenta Velocidade", "3 - Diminui Velocidade", "BACKSPACE - SAIR"};
    
    Bordas(70, 5, 110, 11, CYAN);
    
    for (int i = 0; i < 4; i++) {
        gotoxy(72, startY + i);
        if (i == selectedOption) {
            textcolor(RED);
            textbackground(0);
            printf("> %s", options[i]);
        } else {
            textcolor(CYAN);
            textbackground(RED);
            printf("  %s", options[i]);
        }
    }
    
    textcolor(WHITE);
    textbackground(BLACK);
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

void Capa(){
	
	int i=30, j=7, selectedOption = 0, key;
	char string[100];
	FILE *ptr =fopen("NomeTrab.txt", "r+");

	initializeQueues();
	
    enqueue(0, 5); enqueue(0, 10); enqueue(0, 15);
    enqueue(1, 20); enqueue(1, 25);
    enqueue(2, 30); enqueue(2, 35); enqueue(2, 40);	
	
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
	
	piscarTexto("ENTRAR",57,23);
	
	
	/*gotoxy(60,23);printf("Desenvolvido por:");
	gotoxy(61,23);printf("Caua Pereira Domingues");
	gotoxy(62,23);printf("Joao Manuel Oliveira Pereira");
	gotoxy(63,23);printf("Joao Paulo Saccomani de Freitas Bortolloce");*/
	
	LimpaTela();
	//drawHeader();
    //displayQueues();
	
	getche();
	
	
	fclose(ptr);
	gotoxy(35,30);
	textbackground(0);
}
