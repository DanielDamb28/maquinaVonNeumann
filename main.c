#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char memoria[154];
unsigned short int mar = 0,ibr = 0,imm = 0;
unsigned short int pc=0;
unsigned int mbr = 0;
unsigned char ir=0;
unsigned char equal = 0, lower = 0, greater = 0;
unsigned char flagLR = 0;
unsigned short int aluA = 0, aluB = 0, aluT = 0 ;
char erro[30];
int flagHlt = 0;


void imprimeEmBinario2(unsigned int num){
    for(int i = sizeof(num)*8 - 1; i >= 0; i--){
        printf("%d", (num>>i) & 1);
    }
}

void imprimeEmBinario(unsigned short int num){
    for(int i = sizeof(num)*8 - 1; i >= 0; i--){
        printf("%d", (num>>i) & 1);
    }
}

void limpaEspacosEmBrancoAEsquerdaDaString(char *string){
    int j, k = 0;
    int len = strlen(string);
    for(k = 0; k < len && isspace(string[0]); k++){
        j = 0;
        while(j <= len){
            string[j] = string[j + 1];
            j++;
        }
    }
}

void separaPosicaoDaMemoriaDoValorDeEntrada(char *valorDeEntrada, char *posicaoDaMemoria){
    for(int i = 0; i < strlen(valorDeEntrada); i++){
        if(valorDeEntrada[i] != ';')
            posicaoDaMemoria[i] = valorDeEntrada[i];
        else
            break;
    }
    limpaEspacosEmBrancoAEsquerdaDaString(posicaoDaMemoria);
}

int converteHexaParaInt(char* valorEmHexa){
    short int posicaoEmDecimal = strtol(valorEmHexa, NULL, 16);
    return posicaoEmDecimal;
}

int retornaPosicaoDaProximaInstrucao(char* valorDeEntrada, int posicaoInicialAnterior, char divisor){
    char *pos = strchr(valorDeEntrada + posicaoInicialAnterior, divisor);
    int inicioProximaInstrucao = (pos-valorDeEntrada) + 1;
    return inicioProximaInstrucao;
}

char separaDataOrInstructionDoValorDeEntrada(char* valorDeEntrada, int inicioProximaInstrucao){
    char dataOrInstruction;
    char stringTemp[10] = "\0\0\0\0\0\0\0\0\0\0";
    int j = 0;
    for(int i = inicioProximaInstrucao; i < strlen(valorDeEntrada); i++){
        if(valorDeEntrada[i] != ';')
            stringTemp[j++] = valorDeEntrada[i];
        else
            break;
    }
    limpaEspacosEmBrancoAEsquerdaDaString(stringTemp);
    dataOrInstruction = stringTemp[0];
    return dataOrInstruction;
}

void preencheTodasAsPosicoesDoVetorComVazio(char *vetor, int tamanho){
    for(int i = 0; i < tamanho; i++){
        vetor[i] = '\0';
    }
}

void separaPrimeiraInstrucaoDoValorDeEntrada(char *valorDeEntrada, char *primeiraInstrucao, int inicioProximaInstrucao){
    int contador = 0;
    for(int i = inicioProximaInstrucao; i < strlen(valorDeEntrada); i++){
        if(valorDeEntrada[i] != '/'){
            primeiraInstrucao[contador++] = valorDeEntrada[i];
        }else{
            primeiraInstrucao[contador++] = valorDeEntrada[i];
            break;
        }
    }
    limpaEspacosEmBrancoAEsquerdaDaString(primeiraInstrucao);
}

void separaOperacaoDaInstrucao(char *instrucao, char *sOp, int inicioEnderecoMemoria){
    int k = 0;
    for(int i = 0; i < inicioEnderecoMemoria; i++){
        if(instrucao[i] != ' '){
            sOp[k++] = instrucao [i];
        } else {
            break;
        }
    }
}

void separaEnderecoDaInstrucao(char *primeiraInstrucao, char *endereco, int inicioEnderecoMemoria){
    int j = 0;
    for(int i = inicioEnderecoMemoria; i < strlen(primeiraInstrucao); i++){
        if(primeiraInstrucao[i] != '/'){
            endereco[j++] = primeiraInstrucao[i];
        } else {
            break;
        }
    }

}

void separaSegundaInstrucaoDoValorDeEntrada(char *valorDeEntrada, char *segundaInstrucao, int inicioSegundaInstrucao){
    int contador = 0;
    for(int i = inicioSegundaInstrucao; i < strlen(valorDeEntrada); i++){
            segundaInstrucao[contador++] = valorDeEntrada[i];
    }
    limpaEspacosEmBrancoAEsquerdaDaString(segundaInstrucao);
}

int separaValorASerGuardado(char *valorDeEntrada, int inicioProximaInstrucao){
    int contador = 0;
    char valorASerGuardadoEmHexa[5] = "\0\0\0\0\0";
    for(int i = inicioProximaInstrucao; i < strlen(valorDeEntrada); i++){
            valorASerGuardadoEmHexa[contador++] = valorDeEntrada[i];
    }
    limpaEspacosEmBrancoAEsquerdaDaString(valorASerGuardadoEmHexa);
    int a = converteHexaParaInt(valorASerGuardadoEmHexa);
    return a;
}

void guardaInstrucaoNaMemoria(char *op, int enderecoDeMemoria, int posicaoNaMemoria, char *memoria){
    char conjuntoDeIntrucoes[32][6];
    strcpy(conjuntoDeIntrucoes[0], "hlt");strcpy(conjuntoDeIntrucoes[1], "nop");strcpy(conjuntoDeIntrucoes[2], "add");strcpy(conjuntoDeIntrucoes[3], "sub");
    strcpy(conjuntoDeIntrucoes[4], "mul");strcpy(conjuntoDeIntrucoes[5], "div");strcpy(conjuntoDeIntrucoes[6], "cmp");strcpy(conjuntoDeIntrucoes[7], "xchg");
    strcpy(conjuntoDeIntrucoes[8], "nad");strcpy(conjuntoDeIntrucoes[9], "or");strcpy(conjuntoDeIntrucoes[10], "xor");strcpy(conjuntoDeIntrucoes[11], "not");
    strcpy(conjuntoDeIntrucoes[12], "je");strcpy(conjuntoDeIntrucoes[13], "jne");strcpy(conjuntoDeIntrucoes[14], "jl");strcpy(conjuntoDeIntrucoes[15], "jle");
    strcpy(conjuntoDeIntrucoes[16], "jg");strcpy(conjuntoDeIntrucoes[17], "jge");strcpy(conjuntoDeIntrucoes[18], "jmp");strcpy(conjuntoDeIntrucoes[19], "lda");
    strcpy(conjuntoDeIntrucoes[20], "ldb");strcpy(conjuntoDeIntrucoes[21], "sta");strcpy(conjuntoDeIntrucoes[22], "stb");strcpy(conjuntoDeIntrucoes[23], "ldrb");
    strcpy(conjuntoDeIntrucoes[24], "movial");strcpy(conjuntoDeIntrucoes[25], "moviah");strcpy(conjuntoDeIntrucoes[26], "addia");strcpy(conjuntoDeIntrucoes[27], "subia");
    strcpy(conjuntoDeIntrucoes[28], "mulia");strcpy(conjuntoDeIntrucoes[29], "divia");strcpy(conjuntoDeIntrucoes[30], "lsh");strcpy(conjuntoDeIntrucoes[31], "rsh");

    unsigned int dado = 0;
    unsigned int parte1 = 0;
    unsigned int parte2 = 0;
    unsigned short int opInt = 0;
    for(int i = 0; i < 32; i++){
        if(op == ' ')
            break;
        if(!strcmp(op, conjuntoDeIntrucoes[i])){
            opInt = i;
            break;
        }
    }
    dado = opInt;
    dado = dado << 11;
    dado = dado | enderecoDeMemoria;
    parte1 = dado >> 8;
    parte2 = dado << 8;
    parte2 = parte2 >> 8;
    memoria[posicaoNaMemoria] = parte1;
    memoria[posicaoNaMemoria + 1] = parte2;





}

void executaLeituraDosDadosEGuardaNaMemoria(char *memoria, char *palavra){
    char valorDeEntrada[30] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

    char valorDaPosicaoDaMemoriaEmHexa[3] = "\0\0\0";
    int posicaoNaMemoria = 0;

    int inicioProximaInstrucao = 0;

    char dataOrInstruction;

    for(int i = 0; i < strlen(palavra);i++){
        valorDeEntrada[i] = palavra[i];
    }

    separaPosicaoDaMemoriaDoValorDeEntrada(&valorDeEntrada, valorDaPosicaoDaMemoriaEmHexa);
    posicaoNaMemoria = converteHexaParaInt(valorDaPosicaoDaMemoriaEmHexa);


    inicioProximaInstrucao = retornaPosicaoDaProximaInstrucao(valorDeEntrada, 0, ';');

    dataOrInstruction = separaDataOrInstructionDoValorDeEntrada(valorDeEntrada, inicioProximaInstrucao);


    inicioProximaInstrucao = retornaPosicaoDaProximaInstrucao(valorDeEntrada, inicioProximaInstrucao, ';');


    if(dataOrInstruction == 'i'){

        char primeiraInstrucao[15];
        char pEnderecoDeMemoriaEmHexa[4] = "\0\0\0\0";
        int pEnderecoDeMemoria = 0;
        char pOp[7] = "\0\0\0\0\0\0\0";

        char segundaInstrucao[15] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
        char sEnderecoDeMemoriaEmHexa[4] = "\0\0\0\0";
        int sEnderecoDeMemoria = 0;
        char sOp[7] = "\0\0\0\0\0\0\0";

        int inicioProximoComando = retornaPosicaoDaProximaInstrucao(valorDeEntrada, inicioProximaInstrucao, '/');
        preencheTodasAsPosicoesDoVetorComVazio(primeiraInstrucao, 15);
        separaPrimeiraInstrucaoDoValorDeEntrada(valorDeEntrada, primeiraInstrucao, inicioProximaInstrucao);
        if(strchr(primeiraInstrucao, ' ')){
            int inicioEnderecoMemoria = retornaPosicaoDaProximaInstrucao(primeiraInstrucao, 0, ' ');
            separaOperacaoDaInstrucao(primeiraInstrucao, pOp, inicioEnderecoMemoria);
            separaEnderecoDaInstrucao(primeiraInstrucao, pEnderecoDeMemoriaEmHexa, inicioEnderecoMemoria);
            pEnderecoDeMemoria = converteHexaParaInt(pEnderecoDeMemoriaEmHexa);
        }
        else{
            int inicioEnderecoMemoria = retornaPosicaoDaProximaInstrucao(primeiraInstrucao, 0, '/');
            separaOperacaoDaInstrucao(primeiraInstrucao, pOp, inicioEnderecoMemoria-1);
            pEnderecoDeMemoria = 0;
        }
        guardaInstrucaoNaMemoria(pOp, pEnderecoDeMemoria, posicaoNaMemoria, memoria);

        separaSegundaInstrucaoDoValorDeEntrada(valorDeEntrada, segundaInstrucao, inicioProximoComando);
        if(strchr(segundaInstrucao, ' ')){
            int inicioEnderecoMemoria = retornaPosicaoDaProximaInstrucao(segundaInstrucao, 0, ' ');
            separaOperacaoDaInstrucao(segundaInstrucao, sOp, inicioEnderecoMemoria);
            separaEnderecoDaInstrucao(segundaInstrucao, sEnderecoDeMemoriaEmHexa, inicioEnderecoMemoria);
            sEnderecoDeMemoria = converteHexaParaInt(sEnderecoDeMemoriaEmHexa);
        }else{
            int inicioEnderecoMemoria = strlen(segundaInstrucao);
            separaOperacaoDaInstrucao(segundaInstrucao, sOp, inicioEnderecoMemoria-1);
            sEnderecoDeMemoria = 0;
        }
        guardaInstrucaoNaMemoria(sOp, sEnderecoDeMemoria, posicaoNaMemoria + 2, memoria);

    }
    else if(dataOrInstruction == 'd'){

        unsigned short int valorASerGuardado = 0;
        valorASerGuardado = separaValorASerGuardado(valorDeEntrada, inicioProximaInstrucao);
        guardaInstrucaoNaMemoria(' ', valorASerGuardado, posicaoNaMemoria, memoria);
    }
}

void mostraRegistradores(){
     printf("\n \n \t\t Registradores :  \n\n");
     printf("  A:      %.04x               B:    %.04x               T:      %.04x   \n",aluA,aluB,aluT);
     printf("  MBR:    %.08x           IR:   %.04x               MAR:    %.04x   \n",mbr,ir,mar);
     printf("  IBR:    %.08x           PC:   %.04x               IMM:    %.04x   \n",ibr,pc,imm);
     printf("  E:      %.02x                 L:    %.02x                 G:      %.02x   \n",equal,lower,greater);
     printf("  lr:     %.02x         \n",flagLR);
}

void passaInstrucaoDaMemoriaParaOMbr(){
    unsigned int hexmbr;
    hexmbr = (((memoria[mar]<<24)| (memoria[mar+1]<<16) )|(memoria[mar+2]<<8))|(memoria[mar+3]);
    flagLR = 1;
    mbr = hexmbr;
}

unsigned short int passaDadoDaMemomiaParaOMbr(){
    unsigned short int hexmbr = (memoria[mar] << 8)|(memoria[mar + 1]);
    return hexmbr;
}

void passaDadoDoMbrParaAMemoria(){
    memoria[mar] = (mbr >> 8);
    memoria[mar + 1] = mbr;
}

void mostraMemoria(){

    printf("     ");

    for(int i = 0;i < 16; i++){
        printf(" %02x ", i);
    }

    for(int i = 0; i < 154;i++){

        if(i % 16 == 0){
            printf("\n");
            printf("    ---------------------------------------------------------------- ");
            printf("\n");
            printf(" %02x -", i);
        }
        printf("|%02x|",memoria[i]);
        if(i % 16 == 15){
            printf(" - %02x", i - 15);
        }
        if(i == 153){
            printf("\n");
            printf("    ---------------------------------------------------------------- ");
            printf("\n");
        }
    }

    printf("     ");

    for(int i = 145;i < 161; i++){
        printf(" %02x ", i - 145);
    }
}

void divideMbrEmIrMarIbr(){
    ir = mbr>>27;
    if(ir >= 24){
        imm = (mbr>>16)<<5;
        imm =  imm >>5;
    }else{
        mar = (mbr>>16)<<5;
        mar =  mar >>5;
    }
    mar = (mbr>>16)<<5;
    mar =  mar >>5;
    ibr = mbr;

}

void identificaOp(){
    switch(ir){
    case 0:
        //fim do programa
        strcpy(erro, "Fim do programa!");
        flagHlt = 1;
        break;

    case 1:
        //nop

        if(flagLR==1){
            pc=pc+4;
        }
        if(flagLR==0){
            flagLR=1;
        }
        break;
    case 2:
        //add
        aluA = aluA + aluB;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 3:
        //sub
        aluA = aluA - aluB;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 4:
        //mul
        aluA = aluA * aluB;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 5:
        //div
        aluA = aluA / aluB;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 6:
        //cmp
        //Se A = B, então E = 1; senão E = 0
        if(aluA == aluB){
            equal = 1;
        }else{
            equal = 0;
        }
        //Se A < B, então L = 1; senão L = 0;
        if(aluA < aluB){
            lower =1;
        }else{
            lower = 0;
        }
        //Se A > B, então G = 1; senão G = 0.
        if(aluA >  aluB){
            greater = 1;
        }else{
            greater = 0;
        }
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 7:
        //xchg
        //1) 𝑇 ← 𝐴;
       // 2) 𝐴 ← 𝐵;
       // 3) 𝐵 ← t.
       aluT = aluA;
       aluA = aluB;
       aluB = aluT;
       if(flagLR == 0)
            pc = pc +4;
        break;
    case 8:
        //and
        aluA = aluA&aluB;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 9:
        //or
        aluA = aluA|aluB;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 10:
        //xor
        aluA = aluA ^ aluB;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 11:
        //not
        aluA = !aluA;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 12:
        //je
        //muda o registrador PC para o endereço de memória X caso E = 1
        if(equal == 1){
            pc = mar;
        }else{
            pc=pc+4;
        }
        break;
    case 13:
        //jne
        //muda o registrador PC para o endereço de memória X caso E = 0.
        if(equal == 0){
            pc = mar;
        }else{
            pc= pc+4;
        }
        break;
    case 14:
        //jl
        //muda o registrador PC para o endereço de memória X caso L = 1
        if(lower == 1){
            pc = mar;
        }else{
            pc=pc+4;
        }
        break;
    case 15:
        //jle
        //muda o registrador PC para o endereço de memória X caso E = 1 ou L = 1.
        if(equal == 1| lower == 1){
            pc = mar;
        }else{
            pc =pc+4;
        }
        break;
    case 16:
        //jg
        if(greater == 1){
            pc = mar;
        }else{
            pc = pc +4;
        }
        break;
    case 17:
        //jge
        if(greater == 1 || equal == 1){
            pc = mar;
        } else {
            pc = pc + 4;
        }
        break;
    case 18:
        pc = mar;
        break;
    case 19:
        //lda
        mbr = passaDadoDaMemomiaParaOMbr();
        aluA = mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 20:
        //ldb
        mbr = passaDadoDaMemomiaParaOMbr();
        aluB = mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 21:
        //sta
        mbr = aluA;
        passaDadoDoMbrParaAMemoria();
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 22:
        //stb
        mbr = aluB;
        passaDadoDoMbrParaAMemoria();
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 23:
        //ldrb
        mbr = aluB;
        mar = mbr;
        passaDadoDaMemomiaParaOMbr();
        aluA = mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 24:
        //movial imm
        aluA = 0;
        mbr = imm;
        aluA = imm << 8;
        aluA = aluA >> 8;
        if(flagLR == 0)
            pc = pc +4;
        break;

    case 25:
        //moviah
        mbr = imm << 8;
        aluA = aluA << 8;
        aluA = aluA >> 8;
        aluA = aluA|imm;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 26:
        //addia
        mbr = imm;
        aluA = aluA + mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 27:
        //subia
        mbr = imm;
        aluA = aluA - mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 28:
        //mulia
        mbr = imm;
        aluA = aluA * mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 29:
        //divia
        mbr = imm;
        aluA = aluA/mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 30:
        //lsh
        mbr = imm;
        aluA = aluA << mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    case 31:
        //rsh
        mbr = imm;
        aluA = aluA >> mbr;
        if(flagLR == 0)
            pc = pc +4;
        break;
    }
}

void mostraMemoriaEResgistradores(){
    mostraMemoria();
    mostraRegistradores();
}

void executaInstrucao(){
    if(flagLR == 0){
        mar = pc;
        passaInstrucaoDaMemoriaParaOMbr();
        divideMbrEmIrMarIbr();
        flagLR = 1;
        identificaOp();
    }else{
        ir = (ibr >> 11);
        if(ir >= 24){
            imm = (ibr << 5);
            imm = imm >> 5;
        }else{
            mar = (ibr << 5);
            mar = mar >> 5;
        }
        flagLR = 0;
        identificaOp();

    }
}

int main(){


    for(int i = 0; i < 154; i++){
        if(memoria[i] =! 47){
            memoria[i] = 47;
        }
    }

    FILE *arquivo;
    char texto[30];

    arquivo = fopen("entrada.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int i = 0;
    while (fgets(texto, 30, arquivo) != NULL) {
        executaLeituraDosDadosEGuardaNaMemoria(memoria, texto);
        i++;
    }

    fclose(arquivo);

    preencheTodasAsPosicoesDoVetorComVazio(erro, 30);
    int comando = 1;

    while(comando =! 0){
        mostraMemoriaEResgistradores();

        printf("%x\n", memoria[mar]);
        printf("\n\n1- Executar clock de instrucao\n");
        printf("0- Finalizar programa\n");
        printf("%s\n", erro);
        printf("Comando: ");
        scanf("%i", &comando);
        if(comando == 1){
            executaInstrucao();
            if(ir != 0)
                preencheTodasAsPosicoesDoVetorComVazio(erro, 30);
            system("cls");
        }
        else if(comando == 0){
            break;
        }
        else {
            system("cls");
            strcpy(erro, "Comando Invalido");
        }
        if(flagHlt == 1){
            break;
        }
    }
    mostraMemoriaEResgistradores();
    printf("\n\n\nPrograma Finalizado\n\n\n");

    return 0;
}





