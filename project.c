#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define NUM_OF_ALPHA 26
#define MAX_NUMBER_OF_NODES 1500000
#define MAX_LENGTH_OF_WORD 100
#define MAX_OCCURENCE_OF_SAME_WRONG_WORD 10000
#define MAX_NUMBER_OF_WRONG_WORDS 600000

int trie[MAX_NUMBER_OF_NODES][NUM_OF_ALPHA + 1];
int next = 0;

int i, j, ec, fg, ec2;
char fn[20], e, c;
FILE *fp1, *fp2, *fp;

typedef long long ll;

typedef struct WrongWord WrongWord;
struct WrongWord {
    char word[MAX_LENGTH_OF_WORD];
    ll pos;
};

WrongWord *wrong_word_list[MAX_NUMBER_OF_WRONG_WORDS];

ll current_pos = 0; // Declare current_pos only once

void Create();
void Append();
void Delete();
void Display();
void spell_checker();
void build_trie(char s[]);
int contains(char s[]);
int ll_cmp(const void *p1, const void *p2);
int wrong_word_cmp(const void *p1, const void *p2);
void spell_check();

int f_getc(FILE *fp) {
    int c = fgetc(fp);
    if (c != EOF)
        current_pos++;
    return c;
}

ll get_word(char s[], FILE *in) {
    ll c, begin_of_word, lim = MAX_LENGTH_OF_WORD;
    char *w = s;
    while (isspace(c = f_getc(in)) || isdigit(c)) {
        ;
    }
    if (c != EOF) {
        *w++ = tolower(c);
        begin_of_word = current_pos;
    }
    if (!isalpha(c)) {
        *w = '\0';
        return begin_of_word;
    }
    for (; --lim > 0; ++w) {
        if (!isalpha(c = f_getc(in))) {
            break;
        }
        *w = tolower(c);
    }
    *w = '\0';
    return begin_of_word;
}

void build_trie(char s[]) {
    ll i, t = 1;
    for (i = 0; s[i] != '\0'; ++i) {
        if (isupper(s[i])) {
            s[i] = tolower(s[i]);
        }
        int pos = s[i] - 'a';
        if (trie[t][pos] == 0) {
            trie[t][pos] = ++next;
        }
        t = trie[t][pos];
    }
}

int contains(char s[]) {
    ll i, t = 1;
    for (i = 0; s[i] != '\0'; ++i) {
        int pos = s[i] - 'a';
        if (trie[t][pos] == 0) {
            return 0;
        }
        t = trie[t][pos];
    }
    return 1;
}

int ll_cmp(const void *p1, const void *p2) {
    return *((const ll **) p1) - *((const ll **) p2);
}

int wrong_word_cmp(const void *p1, const void *p2) {
    return strcmp((*(const WrongWord **) p1)->word, (*(const WrongWord **) p2)->word);
}

void spell_check() {
    FILE *dict = fopen("Dictionary.txt", "r");
    if (!dict) {
        fprintf(stderr, "dictionary.txt cannot be opened!\n");
        return;
    }

    char word[MAX_LENGTH_OF_WORD];
    while (fgets(word, sizeof word, dict)) {
        word[strcspn(word, "\r\n")] = 0;
        build_trie(word);
    }
    fclose(dict);

    FILE *in = fopen("article.txt", "r");
    if (!in) {
        fprintf(stderr, "article.txt cannot be opened\n");
        return;
    }

    char str[MAX_LENGTH_OF_WORD];
    ll begin_of_word, wrong_word_count = 0;
    while ((begin_of_word = get_word(str, in)) != EOF) {
        if (!contains(str)) {
            WrongWord *wwp = malloc(sizeof(WrongWord));
            if (!wwp) {
                fprintf(stderr, "Memory error!\n");
                return;
            }
            strcpy(wwp->word, str);
            wwp->pos = begin_of_word;
            wrong_word_list[wrong_word_count++] = wwp;
        }
    }

    qsort(wrong_word_list, wrong_word_count, sizeof wrong_word_list[0], wrong_word_cmp);

    wrong_word_list[wrong_word_count] = malloc(sizeof wrong_word_list[0]);
    strcpy(wrong_word_list[wrong_word_count++]->word, "");

    FILE *out = fopen("misspelling.txt", "w");
    if (!out) {
        fprintf(stderr, "misspelling.txt cannot be opened!\n");
        return;
    }

    char last_word[MAX_LENGTH_OF_WORD] = "";
    ll i, j, pos[MAX_OCCURENCE_OF_SAME_WRONG_WORD], count = 0;
    for (i = 0; i < wrong_word_count; ++i) {
        if (strcmp(last_word, wrong_word_list[i]->word)) {
            if (*last_word) {
                fprintf(out, "%s ", last_word);
                qsort(pos, count, sizeof pos[0], ll_cmp);
                for (j = 0; j < count; ++j) {
                    fprintf(out, "%lld%c", pos[j], j == count - 1 ? '\n' : ' ');
                }
            }
            count = 0;
            strcpy(last_word, wrong_word_list[i]->word);
            pos[count++] = wrong_word_list[i]->pos;
        } else {
            pos[count++] = wrong_word_list[i]->pos;
        }
    }
    fclose(out);
}

void Create() {
    fp1 = fopen("article.txt", "w");
    printf("\n\tEnter the text and press '.' to save\n\n\t");
    while (1) {
        c = getchar();
        fputc(c, fp1);
        if (c == '.') {
            fclose(fp1);
            printf("\n\tEnter then new filename: ");
            scanf("%s", fn);
            fp1 = fopen("article.txt", "r");
            fp2 = fopen(fn, "w");
            while (!feof(fp1)) {
                c = getc(fp1);
                putc(c, fp2);
            }
            fclose(fp2);
            break;
        }
    }
}

void Display() {
    printf("\n\tEnter the file name: ");
    scanf("%s", fn);
    fp1 = fopen(fn, "r");
    if (fp1 == NULL) {
        printf("\n\tFile not found!");
        goto end1;
    }
    while (!feof(fp1)) {
        c = getc(fp1);
        printf("%c", c);
    }
end1:
    fclose(fp1);
    printf("\n\n\tPress any key to continue...");
    getch();
}

void Delete() {
    printf("\n\tEnter the file name: ");
    scanf("%s", fn);
    fp1 = fopen(fn, "r");
    if (fp1 == NULL) {
        printf("\n\tFile not found!");
        goto end2;
    }
    fclose(fp1);
    if (remove(fn) == 0) {
        printf("\n\n\tFile has been deleted successfully!");
        goto end2;
    } else {
        printf("\n\tError!\n");
    }
end2:
    printf("\n\n\tPress any key to continue...");
    getch();
}

void Append() {
    printf("\n\tEnter the file name: ");
    scanf("%s", fn);
    fp1 = fopen(fn, "r");
    if (fp1 == NULL) {
        printf("\n\tFile not found!");
        goto end3;
    }
    while (!feof(fp1)) {
        c = getc(fp1);
        printf("%c", c);
    }
    fclose(fp1);
    printf("\n\tType the text and press 'Ctrl+S' to append.\n");
    fp1 = fopen(fn, "a");
    while (1) {
        c = getch();
        if (c == 19)
            goto end3;
        if (c == 13) {
            c = '\n';
            printf("\n\t");
            fputc(c, fp1);
        } else {
            printf("%c", c);
            fputc(c, fp1);
        }
    }
end3:
    fclose(fp1);
    getch();
}

void spell_checker() {
    spell_check();
}

int main() {
    do {
        printf("\n\t***** TEXT EDITOR *****");
        printf("\n\tMENU:\n\t-----\n");
        printf("\n\t1.CREATE\n\t2.DISPLAY\n\t3.APPEND\n\t4.DELETE\n\t5.SPELL_CHECKER\n\t6.EXIT\n");
        printf("\n\tEnter your choice: ");
        scanf("%d", &ec);
        switch (ec) {
            case 1:
                Create();
                break;
            case 2:
                Display();
                break;
            case 3:
                Append();
                break;
            case 4:
                Delete();
                break;
            case 5:
                spell_checker();
                break;
            case 6:
                exit(0);
        }
    } while (1);
    return 0;
}
