#include <bits/stdc++.h>

using namespace std;

#define tk_read 1
#define tk_write 2
#define tk_if 3
#define tk_then 4
#define tk_else 5
#define tk_do 6
#define tk_for 7
#define tk_to 8
#define tk_begin 9
#define tk_end 10
#define tk_integer 11
#define tk_real 12
#define tk_const 13
#define tk_var 14
#define tk_soma 15
#define tk_sub 16
#define tk_vezes 17
#define tk_div 18
#define tk_compara 19
#define tk_atribui 20
#define tk_diferente 21
#define tk_menor 22
#define tk_maior 23
#define tk_menorigual 24
#define tk_maiorigual 25
#define tk_abrepar 26
#define tk_fechapar 27
#define tk_virgula 28
#define tk_ponto 29
#define tk_pontoevirgula 30
#define tk_doispontos 31
#define tk_variavel 32
#define tk_numero 33
#define tk_abrechave 34
#define tk_fechachave 35
#define tk_numfloat 36
#define tk_EOF 37


void reconhece();
void ERRO();
void PROG();
void CONSTANTE();
void LCONST();
void LC2();
void NFLOAT();
void DCONST();
void DC2();
void DIGIT();
void NUM();
void N2();
void VAR();
void LIDENTS();
void LIS2();
void DEFLI();
void DEFLI2();
void LIDENT();
void LI2();
void BLOCO();
void COMANS();
void COMAN();
void ELSEO();
void OPR();
void IDENT();
void PALAVRA();
void CARACTER();
void LARG();
void LARG2();
void FATOR();
void EXP();
void E2();
void TERMO();
void T2();
void EXPR();

typedef struct tab{
    string id;
    char varcon;
    string tipo;
    int bloco;
    //int situacao;
}Tabela;

FILE *cod = fopen("codigo.txt","rt");
int token;
string lexema;
int linhas = 1;
int cont = 0;
int bloco = 0;
Tabela tab;
vector<Tabela> estado;
vector<string> variaveis;
string vertipo;
int tipon;

int eSimbolo(char c)
{
	if (c == '.' || c == '(' || c == ')' || c == ',' || c == ';' ||
		c == ':' || c == '+' || c == '-' || c == '*' || c == '/' ||
		c == '=' || c == '<' || c == '>')
		return 1;
    else
        return 0;
}
int simbolo(char lex)
{
        lexema+=lex;
        char aux;
        if(lex == '+')          return tk_soma;
        else if(lex == '-')     return tk_sub;
        else if(lex == '*')     return tk_vezes;
        else if(lex == '/')     return tk_div;
        else if(lex == '=')     return tk_compara;
        else if(lex == ';')     return tk_pontoevirgula;
        else if(lex == ':'){
            aux = fgetc(cod);
            if(aux == '='){
                lexema+=aux;
                lexema+='\0';
                return tk_atribui;
            }
            else if(aux != EOF){
                    lexema+='\0';
                    fseek(cod,-1,SEEK_CUR);
                    return tk_doispontos;
            }
        }
        else if(lex== '<'){
            aux=fgetc(cod);
            lexema+=aux;
            lexema+='\0';
            if( aux == '>' )    return tk_diferente;
            else if(aux == '=') return tk_menorigual;
            else{
                if(aux!=EOF)    fseek(cod,-1,SEEK_CUR);
                return tk_menor;
                }
        }
        else if(lex == '>'){
            aux = fgetc(cod);
            lexema+=aux;
            lexema+='\0';
            if(aux== '=')       return tk_maiorigual;
            else{
                if(aux!=EOF)    fseek(cod,-1,SEEK_CUR);
                return tk_maior;
            }
        }
        else if(lex== '(')      return tk_abrepar;
        else if(lex== ')')      return tk_fechapar;
        else if(lex == '{')     return tk_abrechave;
        else if(lex == '}')     return tk_fechachave;
        else if(lex == '.')     return tk_ponto;
        else if(lex == ',')     return tk_virgula;
}
int palavra(string lex)
{
    int i;
    string palavrasreservadas[]={"read","write","if","then","else","do","for","to","begin","end","integer","real","const","var"};
    for(i=0; i<14 ;i++)
        if(strcmp(lex.c_str(),palavrasreservadas[i].c_str())==0)
            return i+1;
    return tk_variavel;
}
void NextToken(int *token, string *lex)
{
    char c;
    *lex="";
    while(c=fgetc(cod)){
        if(c==EOF){
            *token = tk_EOF;
            return;
        }
        if(c=='{'){
            while(c != '}')
                c=fgetc(cod);
            c=fgetc(cod);
        }
        if(c == '\n' || c == 32 || c == '\t'){
            if(c=='\n')
                linhas++;
            continue;
        }
        else if(eSimbolo(c)){
            *token = simbolo(c);
            return;
        }
        else if(isalpha(c)){
            *lex+=c;
            while(c=fgetc(cod)){
                if(isalpha(c)||isdigit(c)){
                    *lex+=c;
                }
                else{
                    *lex+='\0';
                    break;
                }
            }
            fseek(cod,-1,SEEK_CUR);
            *token = palavra(*lex);
            return;
        }
        else if(isdigit(c)){
            int cont = 1;
            *lex+=c;
            while(c=fgetc(cod)){
                if(isdigit(c)){
                    *lex+=c;
                }
                else if(c == '.'){
                    cont--;
                    if(cont < 0){
                        fseek(cod,-1,SEEK_CUR);
                        *lex+='\0';
                        break;
                    }
                    *lex+=c;
                }
                else{
                    fseek(cod,-1,SEEK_CUR);
                    *lex+='\0';
                    break;
                }
            }
            if(cont == 0)   *token = tk_numfloat;
            else            *token = tk_numero;
            return;
        }
        else if (c == EOF)  *token = tk_EOF;
        else
            cout<< "Erro lexico : " << c << " nao reconhecido na linha " << linhas << endl;
    }
}
void printar()
{
    do{
        NextToken(&token,&lexema);
        cout << token << " " << lexema << endl;
    }while(token!=tk_EOF);
}
void verificaEscopo()
{
    for(int i=0;i<estado.size();i++)
        if(estado[i].id==lexema && estado[i].bloco<=bloco)
            return;
    cout << "Erro de escopo - variavel "<< lexema << " nao declarada na linha " << linhas << endl;
}
string retornaTipo()             /// continuar aqui
{
    for(int i=0;i<estado.size();i++)
        if(estado[i].id==lexema)
            return estado[i].tipo;
    //cout << "erro de tipo" << endl;
}
//void verificaTipo(string tipo, int n)


void verificaVarCon()
{
    for(int i=0;i<estado.size();i++)
            if(estado[i].id==lexema && estado[i].varcon=='v')
                return;
        cout << "Erro de tipo - constante "<< lexema << " nao pode ser lida/escrita na linha " << linhas << endl;
}
int reconhece (int tk)
{
    //cout << "atual -> "<< token << "| esperado -> "<< tk <<endl;
    if(token!=tk){

        ERRO();
        return 1;
    }
    NextToken(&token,&lexema);
    return 0;
}
void ERRO()
{
    cout << "Erro Sintatico - Simbolo "<< lexema<<" nao esperado na linha "<<linhas<<endl ;
    //NextToken(&token,&lexema);
}
void PROG()
{
    if(token == tk_const || token == tk_var || token == tk_begin)
    {
        CONSTANTE();
        VAR();
        BLOCO();
        reconhece(tk_ponto);
        reconhece(tk_EOF);
    }
    else
        ERRO();
}
void CONSTANTE()
{
    int erro=0;
    if(token==tk_const){
        erro=reconhece(tk_const);
        if(!erro)
            LCONST();
    }
    else if(token!=tk_var && token != tk_begin){
        ERRO();
        erro=1;
    }
    if(erro == 1)
        while(token!=tk_var && token != tk_begin)
            NextToken(&token,&lexema);
}
void LCONST()
{
    if(token == tk_variavel){
        DCONST();
        LC2();
    }
    else
        ERRO();
}
void LC2()
{
    int erro = 0;
    if(token == tk_variavel)
        LCONST();
    else if(token!= tk_var && token!= tk_begin){
        ERRO();
        erro = 1;                                       ///poderia colocar o while (token != ... aqui
    }
    if (erro ==1)
        while (token!= tk_var && token!= tk_begin)
            NextToken(&token,&lexema);
}
void DCONST()                           /// codigo erro escopo/tipo aqui
{
    if(token == tk_variavel){
        tab.id=lexema;                  /// adiciono o identificador da constante na instancia da struct
        reconhece(tk_variavel);
        tab.varcon = 'c';               /// marco com o 'c' de constante
        tab.bloco=bloco;                /// e adiciono bloco valido
        reconhece(tk_compara);
        DC2();
    }
    else
        ERRO();
}
void DC2()                              /// aqui tbm
{
    if(token == tk_numero)
    {
        tab.tipo="int";                 /// adiciono o tipo (float ou int )na instancia da structs
        estado.push_back(tab);          /// e adiciono a struct da constante na lista de estados
        //tab.situacao=1;
        reconhece(tk_numero);
        reconhece(tk_pontoevirgula);
    }
    else if (token == tk_numfloat)
    {
        tab.tipo="float";
        estado.push_back(tab);
        reconhece(tk_numfloat);
        reconhece(tk_pontoevirgula);
    }
    else
        ERRO();
}
void VAR()                              /// aqui tbm
{
    int erro=0;
    if(token == tk_var){
        tab.varcon='v';                 /// marco o identificador com o 'v' de variavel
        erro=reconhece(tk_var);
        if(!erro){
            LIDENTS();
            erro=reconhece(tk_pontoevirgula);
        }
    }
    else if(token == tk_begin || token==tk_read || token == tk_write || token == tk_if ||
            token == tk_for || token == tk_end || token == tk_variavel)
            {}
    else{
        ERRO ();
    }
    if (erro)
        while(token != tk_begin && token!=tk_read && token != tk_write && token != tk_if &&
                token != tk_for && token != tk_end && token != tk_variavel)
            NextToken(&token,&lexema);

}
void LIDENTS()
{
    if(token == tk_variavel){
        DEFLI();
        LIS2();
    }
    else
        ERRO();

}
void LIS2()
{
    int erro = 0;
    //cout << token << endl;
    if(token == tk_virgula)
    {
        erro = reconhece(tk_virgula);
        if(!erro)
            LIDENTS();
    }
    else if(token!= tk_pontoevirgula){
        ERRO();
        erro = 1;
    }
    if(erro == 1)
        while (token != tk_pontoevirgula)
            NextToken(&token,&lexema);
}
void DEFLI()
{

    if(token == tk_variavel){
        LIDENT();
        reconhece(tk_doispontos);
        DEFLI2();
    }
    else
        ERRO();
}
void DEFLI2()                           /// e aqui ...
{
    tab.bloco=bloco;                                    /// adiciono o bloco atual na instancia da struct
    if(token == tk_integer){
        tab.tipo="int";
        for(int i=0 ; i<variaveis.size() ; i++){        /// percorro a lista com as variaveis (abaixo) e vou adicionando
            tab.id=variaveis[i];                        /// todas na lista de estados como do tipo inteiro
            estado.push_back(tab);
        }
        variaveis.clear();                              /// e depois limpo a lista de variaveis
        reconhece(tk_integer);
    }
    else if(token == tk_real){
        tab.tipo="float";                               /// faço o mesmo de cima, so que com tipo float
        for(int i=0 ; i<variaveis.size() ; i++){
            tab.id=variaveis[i];
            estado.push_back(tab);
        }
        variaveis.clear();
        reconhece(tk_real);
    }
    else
        ERRO();
}
void LIDENT()                           /// e aqui tbm '-'
{
    if(token == tk_variavel){
        variaveis.push_back(lexema);        ///adiciono as variaveis em outra lista
        reconhece(tk_variavel);
        LI2();
    }
    else
        ERRO();
}
void LI2()
{
    int erro = 0;
    //cout << token << endl;
    if(token == tk_virgula){
        erro = reconhece(tk_virgula);
        if(!erro)
            LIDENT();
    }
    else if(token != tk_doispontos){
        ERRO();
        erro = 1;
    }
    if(erro == 1)
        while(token != tk_doispontos)
            NextToken(&token,&lexema);
}
void BLOCO()                            /// e ainda tem aqui tbm -.-'
{
    if(token == tk_begin){
        reconhece(tk_begin);
        bloco++;
        VAR();
        COMANS();
        reconhece(tk_end);
        //vector<Tabela>::iterator it=estado.begin();
        for(int i=0 ; i<estado.size(); i++)             ///retiro da lista as variaveis com o bloco que vai fechar
            if(estado[i].bloco == bloco)
                estado.erase(estado.begin()+i);
                //estado.pop_back();
        bloco--;
    }
    else
        ERRO();
}
void COMANS()
{
    int erro = 0;
    if (token==tk_read || token == tk_write || token == tk_if ||token == tk_for || token == tk_variavel){
        COMAN();
        erro = reconhece(tk_pontoevirgula);
        if(!erro)
            COMANS();
    }
    else if(token != tk_end){
        ERRO();
        erro = 1;
    }
    if(erro == 1)
        while(token != tk_end)
            NextToken(&token,&lexema);
}
void COMAN()
{
    if (token == tk_read)
    {
        reconhece(tk_read);
        reconhece(tk_abrepar);
        LARG();
        reconhece(tk_fechapar);
    }
    else if (token == tk_write){
        reconhece(tk_write);
        reconhece(tk_abrepar);
        LARG();
        reconhece(tk_fechapar);
    }
    else if (token == tk_if){
        reconhece(tk_if);
        EXPR();
        reconhece(tk_then);                      //0 erro aqui
        BLOCO();
        ELSEO();
    }
    else if (token == tk_variavel){
        vertipo = retornaTipo();
        verificaEscopo();
        reconhece(tk_variavel);
        tipon = 1;
        reconhece(tk_atribui);
        EXP();
    }
    else if (token == tk_for){
        reconhece(tk_for);
        verificaEscopo();
        reconhece(tk_variavel);
        reconhece(tk_compara);
        EXP();
        reconhece(tk_to);
        EXP();
        reconhece(tk_do);
        BLOCO();
    }
    else
        ERRO();

}
void ELSEO()
{
    int erro = 0;
    if(token == tk_else)
    {
        erro = reconhece(tk_else);
        if(!erro)
            BLOCO();
    }
    else if(token != tk_pontoevirgula){
        ERRO();
        erro = 1;
    }
    if (erro = 1)
        while (token != tk_pontoevirgula)
            NextToken(&token,&lexema);
}
void OPR()
{
    if(token == tk_compara)
        reconhece(tk_compara);
    else if(token == tk_diferente)
        reconhece(tk_diferente);
    else if(token == tk_menor)
        reconhece(tk_menor);
    else if(token == tk_menorigual)
        reconhece(tk_menorigual);
    else if(token == tk_maior)
        reconhece(tk_maior);
    else if(token == tk_maiorigual)
        reconhece(tk_maiorigual);
    else
        ERRO();
}
void LARG()
{
    if(token == tk_variavel)
    {
        verificaEscopo();
        reconhece(tk_variavel);
        LARG2();
    }
    else
        ERRO();
}
void LARG2()
{
    int erro = 0;
     if(token == tk_virgula)
     {
         erro = reconhece(tk_virgula);
         if (!erro)
            LARG();
     }
     else if(token != tk_fechapar){
        erro = 1;
        ERRO();
     }
     if (erro == 1)
        while (token != tk_fechapar)
            NextToken(&token,&lexema);
}
void FATOR()
{
     if(token == tk_abrepar)
     {
        reconhece(tk_abrepar);
        EXP();
        reconhece(tk_fechapar);
     }
     else  if(token == tk_variavel){
        verificaEscopo();
        reconhece(tk_variavel);
     }
     else  if(token == tk_numero)
        reconhece(tk_numero);
     else  if(token == tk_numfloat)
        reconhece(tk_numfloat);
     else
        ERRO();
}
void EXP()
{
     if(token == tk_variavel || token == tk_numfloat || token == tk_numero || token == tk_abrepar)
     {
         //if(token == tk_variavel)
           // verificaTipo()
         TERMO();
         E2();
     }
     else
        ERRO();
}
void E2()
{
    int erro = 0;
     if(token == tk_soma)
     {
         erro = reconhece(tk_soma);
         if(!erro){
            TERMO();
            E2();
         }
     }
     else if(token == tk_sub)
     {
         erro = reconhece(tk_sub);
         if(!erro){
            TERMO();
            E2();
         }
     }
     else if(token == tk_to || token == tk_do || token == tk_fechapar || token == tk_then ||
            token == tk_compara || tk_diferente == token || token == tk_menor || token == tk_pontoevirgula ||
            token == tk_maior || token == tk_menorigual || token == tk_maiorigual ) {}
    else {
        ERRO();
        erro = 1;
    }
    if(erro ==1)
        while (token != tk_to && token != tk_do && token != tk_fechapar &&
               token != tk_compara && token != tk_diferente && token != tk_menor && token != tk_menorigual &&
               token != tk_maior && token != tk_maiorigual && token != tk_pontoevirgula && token != tk_then)
               NextToken(&token,&lexema);
}
void TERMO()
{
    if(token == tk_variavel || token == tk_numfloat || token == tk_numero || token == tk_abrepar)
    {
        FATOR();
        T2();
    }
    else
        ERRO();
}
void T2()
{
    int erro = 0;
    if(token == tk_vezes)
     {
         erro = reconhece(tk_vezes);
         if(!erro){
            FATOR();
            T2();
         }
     }
     else if(token == tk_div)
     {
         erro = reconhece(tk_div);
         if(!erro){
            FATOR();
            T2();
         }
     }
     else if(token == tk_to || token == tk_do || token == tk_fechapar || token == tk_then || token == tk_soma ||
            token == tk_compara || tk_diferente == token || token == tk_menor || token == tk_pontoevirgula || token == tk_sub ||
            token == tk_maior || token == tk_menorigual || token == tk_maiorigual ) {}
    else{
        erro = 1;
        ERRO();
    }
    if(erro == 1)
        while (token != tk_to && token != tk_do && token != tk_fechapar && token != tk_soma && token != tk_sub &&
               token != tk_compara && token != tk_diferente && token != tk_menor && token != tk_menorigual &&
               token != tk_maior && token != tk_maiorigual && token != tk_pontoevirgula && token != tk_then)
            NextToken(&token,&lexema);
}
void EXPR()
{
    if(token == tk_variavel || token == tk_numfloat || token == tk_numero || token == tk_abrepar)
    {
        EXP();
        OPR();
        EXP();
    }
    else
        ERRO();
}
int main()
{
    NextToken(&token,&lexema);
    PROG();
    //printar();
        cout <<endl<< "Codigo Terminado!";
    fclose(cod);
    return 0;
}
