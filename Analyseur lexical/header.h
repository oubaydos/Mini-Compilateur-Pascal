FILE *fichier;
int ligne = 0;
char memo = 0;
typedef struct
{
    char caractere;
} Lexer;
typedef enum
{
    PROGRAM_TOKEN,
    CONST_TOKEN,
    VAR_TOKEN,
    BEGIN_TOKEN,
    END_TOKEN,
    IF_TOKEN,
    THEN_TOKEN,
    WHILE_TOKEN,
    DO_TOKEN,
    READ_TOKEN,
    WRITE_TOKEN,
    STRING_TOKEN,
    ID_TOKEN,
    PV_TOKEN,
    PT_TOKEN,
    VIR_TOKEN,
    AFF_TOKEN,
    INF_TOKEN,
    INFEG_TOKEN,
    SUP_TOKEN,
    SUPEG_TOKEN,
    DIFF_TOKEN,
    PO_TOKEN,
    PF_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    STAR_TOKEN,
    SLASH_TOKEN,
    INTLIT_TOKEN,
    EOF_TOKEN,
    ERREUR_TOKEN
} TokenType;
char *motsCles[] = {
    "program",
    "const",
    "var",
    "begin",
    "end",
    "if",
    "then",
    "while",
    "do",
    "read",
    "write",
};
typedef struct
{
    TokenType tokenType;
    char *valeur;
} Token;

Lexer *lexer;

void traiterErreur(char *msg);
char *lireIdentificateur();
char *lireMot();
int lireNombre();
void lireCharSuiv();
void passerSeperateurs();
char *numberToString(int number);
char *charToString();
void fillMemo();
Token *tokenSuiv();
Token *genererToken();
Token *token_init(int type, char *valeur);
void initializerLexer();
void verifierExigences(int argc, char *argv[]);
const char *getTokenName(TokenType type);