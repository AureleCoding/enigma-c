#include <stdio.h>

#define START_LETTER 'A'
#define ROTOR_ROW 3
#define ROTOR_COL 26

int connexion[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

int rotor[3][26] = {0};
int invRotor[3][26] = {0};
char textRotor[3][26] = {
    {'E', 'K', 'M', 'F', 'L', 'G', 'D', 'Q', 'V', 'Z', 'N', 'T', 'O', 'W', 'Y', 'H', 'X', 'U', 'S', 'P', 'A', 'I', 'B', 'R', 'C', 'J'},
    {'A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W', 'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E'},
    {'B', 'D', 'F', 'H', 'J', 'L', 'C', 'P', 'R', 'T', 'X', 'V', 'Z', 'N', 'Y', 'E', 'I', 'W', 'G', 'A', 'K', 'M', 'U', 'S', 'Q', 'O'}};

int reflector[26] = {0};
char textReflector[26] = {'E', 'J', 'M', 'Z', 'A', 'L', 'Y', 'X', 'V', 'B', 'W', 'F', 'C', 'R', 'Q', 'U', 'O', 'N', 'T', 'S', 'P', 'I', 'K', 'H', 'G', 'D'};

int shift[3] = {0};

void ConnectCables(int permu)
{
    char letter1, letter2;
    int temp;
    char permutation[6] = {};

    for (int occu = 0; occu < permu; occu++)
    {
        do
        {
            printf("permutation restante %d\n", permu - occu);
            printf("lettre 1 : ");
            scanf(" %c", &letter1);

            for (int i = 0; permutation[i] != '\0'; i++)
            {
                if (permutation[i] == letter1)
                {
                    printf("%c a deja été permuté\n", letter1);
                    letter1 = 0;
                }
            }
        } while (letter1 == 0);

        printf("lettre 2 : ");
        scanf(" %c", &letter2);

        permutation[occu] = letter1;

        temp = *(connexion + (letter1 - START_LETTER));
        connexion[letter1 - START_LETTER] = *(connexion + (letter2 - START_LETTER));
        connexion[letter2 - START_LETTER] = temp;

        printf("%c a été permuté avec %c\n", letter1, letter2);
    }
}

void Text2Value()
{
    for (int i = 0; i < ROTOR_ROW; i++)
    {
        for (int j = 0; j < ROTOR_COL; j++)
        {
            rotor[i][j] = textRotor[i][j] - START_LETTER;
        }
    }

    for (int i = 0; i < ROTOR_COL; i++)
    {
        reflector[i] = textReflector[i] - START_LETTER;
    }
}

void GenerateInvPerm()
{
    int value;
    for (int i = 0; i < ROTOR_ROW; i++)
    {
        for (int j = 0; j < ROTOR_COL; j++)
        {
            value = rotor[i][j];
            invRotor[i][value] = j;
        }
    }
}

int Encrypt(int letter)
{
    letter = letter - START_LETTER;
    letter = connexion[letter];

    for (int i = 0; i < 3; i++)
    {
        letter = (rotor[i][(letter - shift[i] + 26) % 26] + shift[i]) % 26;
    }

    letter = reflector[letter];

    for (int i = 2; i >= 0; i--)
    {
        letter = (invRotor[i][(letter - shift[i] + 26) % 26] + shift[i]) % 26;
    }

    shift[0]++;
    if (shift[0] == 26)
    {
        shift[0] = 0;
        shift[1]++;
        if (shift[1] == 26)
        {
            shift[1] = 0;
            shift[2]++;
            if (shift[2] == 26)
            {
                shift[2] = 0;
            }
        }
    }

    letter = connexion[letter];

    return letter + START_LETTER;
}

int main(void)
{
    char textInput[200] = {};
    char textOutput[200] = {};
    int permu;

    printf("Veuillez saisir le texte à chiffrer : ");
    scanf("%s", textInput);

    printf("Veuillez donner les positions initiales des rotors.\n");
    printf("Position initiale du rotor I : ");
    scanf(" %c", &shift[0] - START_LETTER);

    printf("Position initiale du rotor II : ");
    scanf(" %c", &shift[1] - START_LETTER);

    printf("Position initiale du rotor III : ");
    scanf(" %c", &shift[2] - START_LETTER);

    printf("Veuillez saisir le nombre de permutations : ");
    scanf("%d", &permu);

    Text2Value();
    ConnectCables(permu);
    GenerateInvPerm();

    printf("Texte entré : %s\n", textInput);

    for (int i = 0; textInput[i] != '\0'; i++)
    {
        textOutput[i] = Encrypt(textInput[i]);
    }

    printf("Texte sortie : %s\n", textOutput);

    return 0;
}