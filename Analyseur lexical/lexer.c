#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "./header.h"
/*
Compte rendu du TP1
-------------------------------------------------------------------------------
Fait par : BOUIFADENE Obaydah
groupe : GL1
-------------------------------------------------------------------------------
    Pour executer ce code:
        >> make
        >> ./tp1 nom_du_fichier
    ou bien : 
        >> gcc -g *.c -o tp1 -lm
        >> ./tp1 nom_du_fichier

*/
void traiterErreur(char *msg)
{
    printf("erreur :: \nligne : %d --- %s\n", ligne, msg);
    exit(1); //exit with error code 1
}

char *lireIdentificateur()
{
    char *valeur = calloc(1, sizeof(char));
    valeur[0] = '\0';
    while (isalnum(lexer->caractere))
    {
        char *str = charToString();
        valeur = realloc(valeur, (strlen(valeur) + strlen(str) + 1) * sizeof(char));
        strcat(valeur, str);
        lireCharSuiv();
    }
    fillMemo();
    return valeur;
}
char *lireMot()
{
    lireCharSuiv();
    char *valeur = calloc(1, sizeof(char));
    valeur[0] = '\0';

    while (lexer->caractere != '"')
    {
        char *str = charToString();
        valeur = realloc(valeur, (strlen(valeur) + strlen(str) + 1) * sizeof(char));
        strcat(valeur, str);
        lireCharSuiv();
    }
    lireCharSuiv();
    return valeur;
}
int lireNombre()
{
    char *valeur = calloc(2, sizeof(char));
    while (isdigit(lexer->caractere))
    {
        strcat(valeur, charToString());
        lireCharSuiv();
    }
    fillMemo();
    return atoi(valeur);
}

void lireCharSuiv()
{
    if (memo)
    {
        lexer->caractere = memo;
        memo = 0;
        return;
    }
    lexer->caractere = fgetc(fichier);
    if (lexer->caractere == '\n')
    {
        ligne++;
    }
}
void handleComment()
{
    if (lexer->caractere == '{')
    {
        lireCharSuiv();
        if (lexer->caractere == '*')
        {
            do
            {
                lireCharSuiv();
            } while (lexer->caractere != '*' && lexer->caractere != EOF);
            lireCharSuiv();
        }
        else
        {
            traiterErreur("erreur dans le commentaire : { sans * ");
        }

        if (lexer->caractere != '}')
        {
            traiterErreur("erreur dans le commentaire : oublie de fermetture de commentaire");
        }
        lireCharSuiv();
    }
}
void passerSeperateurs()
{
    lireCharSuiv();
    handleComment();
    while (lexer->caractere == ' ' || lexer->caractere == '\t' || lexer->caractere == '\n')
    {
        lireCharSuiv();
        if (lexer->caractere == '{')
            handleComment();
    }
}
char *numberToString(int number)
{
    int length = (number == 0) ? 1 : (log10(number) + 1);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", number);
    return str;
}
char *charToString()
{
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->caractere;
    str[1] = '\0';
    return str;
}
void fillMemo()
{
    memo = lexer->caractere;
}
Token *tokenSuiv()
{

    passerSeperateurs();
    if (lexer->caractere == EOF)
    {
        return token_init(EOF_TOKEN, "EOF");
    }
    if (isalpha(lexer->caractere))
    {
        return genererToken();
    }
    else if (isdigit(lexer->caractere))
    {
        return token_init(INTLIT_TOKEN, numberToString(lireNombre()));
    }
    else
    {
        switch (lexer->caractere)
        {
        case '+':
            return token_init(PLUS_TOKEN, charToString());
        case '-':
            return token_init(MINUS_TOKEN, charToString());
        case '*':
            return token_init(STAR_TOKEN, charToString());
        case '/':
            return token_init(SLASH_TOKEN, charToString());

        case '.':
            return token_init(PT_TOKEN, charToString());
        case ',':
            return token_init(VIR_TOKEN, charToString());
        case '(':
            return token_init(PO_TOKEN, charToString());
        case ')':
            return token_init(PF_TOKEN, charToString());
        case ';':
            return token_init(PV_TOKEN, charToString());
        default:
            if (lexer->caractere == '"')
            {
                return token_init(STRING_TOKEN, lireMot());
            }
            if (lexer->caractere == ':')
            {
                char *valeur = calloc(1, sizeof(char));
                valeur[0] = lexer->caractere;
                lireCharSuiv();
                // case :=
                if (lexer->caractere == '=')
                {
                    char *str = charToString();
                    valeur = realloc(valeur, (strlen(valeur) + strlen(str) + 1) * sizeof(char));
                    strcat(valeur, str);
                    return token_init(AFF_TOKEN, valeur);
                }
                // free(valeur);
                fillMemo();
            }
            //case <
            else if (lexer->caractere == '<')
            {
                char *valeur = calloc(1, sizeof(char));
                valeur[0] = lexer->caractere;
                lireCharSuiv();
                if (lexer->caractere == '=')
                {
                    char *str = charToString();
                    valeur = realloc(valeur, (strlen(valeur) + strlen(str) + 1) * sizeof(char));
                    strcat(valeur, str);
                    return token_init(INFEG_TOKEN, charToString());
                }
                if (lexer->caractere == '>')
                {
                    char *str = charToString();
                    valeur = realloc(valeur, (strlen(valeur) + strlen(str) + 1) * sizeof(char));
                    strcat(valeur, str);
                    return token_init(DIFF_TOKEN, charToString());
                }
                // free(valeur);
                fillMemo();
                return token_init(INF_TOKEN, charToString());
            }
            else if (lexer->caractere == '>')
            {
                char *valeur = calloc(1, sizeof(char));
                valeur[0] = lexer->caractere;
                lireCharSuiv();
                if (lexer->caractere == '=')
                {
                    char *str = charToString();
                    valeur = realloc(valeur, (strlen(valeur) + strlen(str) + 1) * sizeof(char));
                    strcat(valeur, str);
                    return token_init(SUPEG_TOKEN, charToString());
                }
                // free(valeur);
                fillMemo();
                return token_init(SUP_TOKEN, charToString());
            }
        }
        return token_init(ERREUR_TOKEN, charToString());
    }
}

Token *genererToken()
{
    char *mot = lireIdentificateur();
    //check if is keyword
    int length = sizeof(motsCles) / sizeof(motsCles[0]);
    for (int i = 0; i < length; i++)
    {
        if (!strcmp(mot, motsCles[i]))
        {
            return token_init(i, mot);
        }
    }
    //else its an identifier
    return token_init(ID_TOKEN, mot);
}
Token *token_init(int type, char *valeur)
{
    Token *token = calloc(1, sizeof(Token));
    token->tokenType = type;
    token->valeur = valeur;
    return token;
}

void initializerLexer()
{
    lexer = calloc(1, sizeof(Lexer));
}
void verifierExigences(int argc, char *argv[])
{
    if (argc != 2)
    {
        traiterErreur("fichier manquant");
    }
    if ((fichier = fopen(argv[1], "r")) == NULL)
    {
        traiterErreur("probleme dans l'ouverture du fichier");
    }
}
const char *getTokenName(TokenType type)
{
    switch (type)
    {
    case PROGRAM_TOKEN:
        return "PROGRAM_TOKEN";
    case CONST_TOKEN:
        return "CONST_TOKEN";
    case VAR_TOKEN:
        return "VAR_TOKEN";
    case BEGIN_TOKEN:
        return "BEGIN_TOKEN";
    case END_TOKEN:
        return "END_TOKEN";
    case IF_TOKEN:
        return "IF_TOKEN";
    case THEN_TOKEN:
        return "THEN_TOKEN";
    case WHILE_TOKEN:
        return "WHILE_TOKEN";
    case DO_TOKEN:
        return "DO_TOKEN";
    case READ_TOKEN:
        return "READ_TOKEN";
    case WRITE_TOKEN:
        return "WRITE_TOKEN";
    case STRING_TOKEN:
        return "STRING_TOKEN";
    case ID_TOKEN:
        return "ID_TOKEN";
    case PV_TOKEN:
        return "PV_TOKEN";
    case PT_TOKEN:
        return "PT_TOKEN";
    case VIR_TOKEN:
        return "VIR_TOKEN";
    case AFF_TOKEN:
        return "AFF_TOKEN";
    case INF_TOKEN:
        return "INF_TOKEN";
    case INFEG_TOKEN:
        return "INFEG_TOKEN";
    case SUP_TOKEN:
        return "SUP_TOKEN";
    case SUPEG_TOKEN:
        return "SUPEG_TOKEN";
    case DIFF_TOKEN:
        return "DIFF_TOKEN";
    case PO_TOKEN:
        return "PO_TOKEN";
    case PF_TOKEN:
        return "PF_TOKEN";
    case PLUS_TOKEN:
        return "PLUS_TOKEN";
    case MINUS_TOKEN:
        return "MINUS_TOKEN";
    case STAR_TOKEN:
        return "STAR_TOKEN";
    case SLASH_TOKEN:
        return "SLASH_TOKEN";
    case INTLIT_TOKEN:
        return "INTLIT_TOKEN";
    case EOF_TOKEN:
        return "EOF_TOKEN";
    case ERREUR_TOKEN:
        return "ERREUR_TOKEN";
    }
}

int main(int argc, char *argv[])
{
    verifierExigences(argc, argv);
    initializerLexer();
    while (!feof(fichier))
    {
        Token *token = tokenSuiv();
        printf("%s <=> %s\n", getTokenName(token->tokenType), token->valeur);
    }
    return 0;
}