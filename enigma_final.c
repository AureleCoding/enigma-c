#include <stdio.h>

// Définition des constantes
#define START_LETTER 'A'
#define ROTOR_ROW 3
#define ROTOR_COL 26

// Initialisation du tableau de connexions
int connexion[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

// Définition des rotors, inverse des rotors et du réflecteur
int rotor[3][26] = {0};
int invRotor[3][26] = {0};
char textRotor[3][26] = {
    {'E', 'K', 'M', 'F', 'L', 'G', 'D', 'Q', 'V', 'Z', 'N', 'T', 'O', 'W', 'Y', 'H', 'X', 'U', 'S', 'P', 'A', 'I', 'B', 'R', 'C', 'J'},
    {'A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W', 'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E'},
    {'B', 'D', 'F', 'H', 'J', 'L', 'C', 'P', 'R', 'T', 'X', 'V', 'Z', 'N', 'Y', 'E', 'I', 'W', 'G', 'A', 'K', 'M', 'U', 'S', 'Q', 'O'}};

int reflector[26] = {0};
char textReflector[26] = {'E', 'J', 'M', 'Z', 'A', 'L', 'Y', 'X', 'V', 'B', 'W', 'F', 'C', 'R', 'Q', 'U', 'O', 'N', 'T', 'S', 'P', 'I', 'K', 'H', 'G', 'D'};

// Position initial des rotors
int shift[3] = {0, 0, 0};

void Text2Value()
{
    // Conversion des textes des rotors
    for (int i = 0; i < ROTOR_ROW; i++)
    {
        for (int j = 0; j < ROTOR_COL; j++)
        {
            rotor[i][j] = textRotor[i][j] - START_LETTER;
        }
    }

    // Conversion du texte du réflecteur
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

void ConnectCables(int permu)
{
    char letter1, letter2;
    int temp;
    char permutations[6][2] = {};
    int permutationsRemaining = 1;

    for (int occu = 0; occu < permu; occu++)
    {
        do
        {
            printf("Permutation(s) restante(s) %d\n", permu - occu);

            printf("lettre 1 : ");
            scanf(" %c", &letter1);
            printf("lettre 2 : ");
            scanf(" %c", &letter2);

            // Vérification des lettres déjà permutées
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    if (permutations[i][j] == letter1)
                    {
                        printf("%c a déjà été permuté\n", letter1);
                        permutationsRemaining = 0;
                    }

                    if (permutations[i][j] == letter2)
                    {
                        printf("%c a déjà été permuté\n", letter2);
                        permutationsRemaining = 0;
                    }
                }
            }
        } while (permutationsRemaining == 0);

        permutations[occu][0] = letter1;
        permutations[occu][1] = letter2;

        // Permutation des connexions
        temp = connexion[letter1 - START_LETTER];
        connexion[letter1 - START_LETTER] = connexion[letter2 - START_LETTER];
        connexion[letter2 - START_LETTER] = temp;

        printf("%c a été permuté avec %c\n", letter1, letter2);
    }
}

int Encrypt(int letter)
{
    letter -= START_LETTER;

    // Passage à travers le tableau de connexions
    letter = connexion[letter];

    // Passage à travers les rotors
    for (int i = 0; i < 3; i++)
    {
        letter = (rotor[i][(letter - shift[i] + 26) % 26] + shift[i]) % 26;
    }

    // Passage à travers le reflecteur
    letter = reflector[letter];

    // Passage à travers les rotors inverses
    for (int i = 2; i >= 0; i--)
    {
        letter = (invRotor[i][(letter - shift[i] + 26) % 26] + shift[i]) % 26;
    }

    // Passage inverses à travers le tableau de connexions
    letter = connexion[letter];

    // Rotation des rotors
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

    return letter + START_LETTER;
}

int main()
{
    char textInput[200] = {};
    char textOutput[200] = {};
    int permu;
    char pos;

    printf("Veuillez saisir le texte à chiffrer : ");
    scanf("%s", textInput);

    printf("Veuillez donner les positions initiales des rotors.\n");
    for (int i = 0; i < 3; i++)
    {
        printf("Position initiale du rotor %d : ", i + 1);
        scanf(" %c", &pos);
        shift[i] = pos - START_LETTER;
    }

    printf("Veuillez saisir le nombre de permutations : ");
    scanf("%d", &permu);

    Text2Value();
    ConnectCables(permu);
    GenerateInvPerm();

    printf("Texte entré : %s\n", textInput);

    for (int i = 0; textInput[i] != '\0'; i++)
    {
        // Vérifie que le texte saisi est en majuscules
        if (textInput[i] < 'A' || textInput[i] > 'Z')
        {
            printf("Le texte saisi doit être en majuscules");
        }
        // Vérifie qu'il n'y a pas d'espaces dans le texte
        if (textInput[i] == ' ')
        {
            printf("Votre texte ne doit pas comporter d'espaces");
        }

        textOutput[i] = Encrypt(textInput[i]);
    }

    printf("Texte sorti : %s\n", textOutput);

    return 0;
}