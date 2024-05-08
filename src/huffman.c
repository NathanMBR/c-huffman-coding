#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ASCII_SIZE 128
#define INPUT_SIZE 256

typedef struct FreqNode {
  char character;
  int quantity;
  struct FreqNode *left;
  struct FreqNode *right;
  struct FreqNode *next;
} FreqNode;

int* get_frequency_table(char string[]) {
  size_t string_length = strlen(string);
  if (string_length == 0)
    return NULL;

  int *frequency_table = (int *) calloc(ASCII_SIZE, sizeof(int));
  if (frequency_table == NULL)
    return NULL;

  for (int i = 0; i < ASCII_SIZE; i++)
    frequency_table[i] = 0;

  for (int i = 0; i < string_length; i++) {
    char character = string[i];
    frequency_table[character]++;
  }

  return frequency_table;
}

void print_frequency_table(int *table) {
  if (table == NULL)
    return;

  printf("Character code - Quantity:\n");

  for (int i = 0; i < ASCII_SIZE; i++)
    if (table[i] != 0)
      printf("%3d - %3d\n", i, table[i]);
}

FreqNode *get_frequency_ll(int *table) {
  if (table == NULL)
    return NULL;

  FreqNode *freq_root = NULL;

  for (int i = 0; i < ASCII_SIZE; i++) {
    int lowestIndex = -1;

    for (int j = 0; j < ASCII_SIZE; j++) {
      if (table[j] > 0)
        if (lowestIndex < 0 || table[j] < table[lowestIndex])
          lowestIndex = j;
    }

    if (lowestIndex < 0)
      break;

    FreqNode *freq_node = (FreqNode *) calloc(1, sizeof(FreqNode));
    if (freq_node == NULL)
      return NULL;

    freq_node->character = lowestIndex;
    freq_node->quantity = table[lowestIndex];
    freq_node->left = NULL;
    freq_node->right = NULL;
    freq_node->next = NULL;

    table[lowestIndex] = 0;

    if (freq_root == NULL) {
      freq_root = freq_node;
      continue;
    }

    FreqNode *aux = freq_root;
    while (aux->next != NULL)
      aux = aux->next;

    aux->next = freq_node;
  }

  return freq_root;
}

void print_frequency_list(FreqNode *list) {
  if (list == NULL)
    return;

  printf("Character code - Quantity:\n");

  while (list != NULL) {
    printf("%3d - %3d\n", list->character, list->quantity);
    list = list->next;
  }
}

FreqNode *get_frequency_bt(FreqNode *list) {
  if (list == NULL)
    return NULL;

  if (list->next == NULL)
    return list;

  FreqNode *tree = NULL;

  FreqNode *aux = list;
  while (aux->next != NULL) {
    tree = (FreqNode *) calloc(1, sizeof(FreqNode));
    if (tree == NULL)
      return NULL;

    tree->character = '\0';
    tree->quantity = aux->quantity + aux->next->quantity;

    tree->left = aux;
    tree->right = aux->next;

    FreqNode *prev_aux_sort = NULL;
    FreqNode *aux_sort = aux;

    while (aux_sort != NULL) {
      if (aux_sort->quantity > tree->quantity)
        break;

      prev_aux_sort = aux_sort;
      aux_sort = aux_sort->next;
    }

    if (prev_aux_sort != NULL)
      prev_aux_sort->next = tree;

    tree->next = aux_sort;

    aux = aux->next->next;
  }

  return tree;
}

void rcsv_print_frequency_bt(FreqNode *tree) {
  if (tree == NULL)
    return;

  rcsv_print_frequency_bt(tree->left);

  printf("%3d - %3d\n", tree->character, tree->quantity);

  rcsv_print_frequency_bt(tree->right);
}

void print_frequency_bt(FreqNode *tree) {
  if (tree == NULL)
    return;

  printf("Character value - Quantity:\n");
  rcsv_print_frequency_bt(tree);
}

int get_height_frequency_bt(FreqNode *tree) {
  if (tree == NULL)
    return -1;

  int left_height = get_height_frequency_bt(tree->left);
  int right_height = get_height_frequency_bt(tree->right);

  return left_height > right_height
    ? left_height + 1
    : right_height + 1;
}

void encode_frequency_bt(
  FreqNode *tree,
  char **dict,
  char string[],
  int depth
) {
  if (tree == NULL)
    return;

  if (tree->left == NULL && tree->right == NULL) {
    strcpy(dict[tree->character], string);
    return;
  }

  if (tree->left != NULL) {
    string[depth + 1] = '0';
    string[depth + 2] = '\0';

    encode_frequency_bt(
      tree->left,
      dict,
      string,
      depth + 1
    );
  }

  if (tree->right != NULL) {
    string[depth + 1] = '1';
    string[depth + 2] = '\0';

    encode_frequency_bt(
      tree->right,
      dict,
      string,
      depth + 1
    );
  }
}

char **get_frequency_dict(FreqNode *tree) {
  if (tree == NULL)
    return NULL;

  int tree_height = get_height_frequency_bt(tree);

  char **frequency_dict = (char **) calloc(ASCII_SIZE, sizeof(char *));
  if (frequency_dict == NULL)
    return NULL;

  int i;
  for (i = 0; i < ASCII_SIZE; i++) {
    frequency_dict[i] = (char *) calloc(tree_height + 1, sizeof(char));
    if (frequency_dict[i] == NULL)
      break;
  }

  if (i < ASCII_SIZE) {
    for (int j = 0; j < i; j++)
      free(frequency_dict[j]);

    free(frequency_dict);
    return NULL;
  }

  char *string = (char *) calloc(tree_height + 1, sizeof(char));
  if (string == NULL)
    return NULL;

  string[0] = '\0';

  encode_frequency_bt(
    tree,
    frequency_dict,
    string,
    -1
  );

  return frequency_dict;
}

void print_frequency_dict(char **dict) {
  if (dict == NULL)
    return;

  printf("Character code - Encode:\n");

  for (int i = 0; i < ASCII_SIZE; i++)
    if (dict[i][0] != '\0')
      printf("%3d - %s\n", i, dict[i]);
}

char *read_input(char message[]) {
  if (message != NULL)
    printf("%s", message);

  char *input = (char *) calloc(INPUT_SIZE + 1, sizeof(char));
  if (input == NULL)
    return NULL;

  fgets(input, INPUT_SIZE, stdin);

  for (int i = 0; i < INPUT_SIZE; i++)
    if (input[i] == '\n') {
      input[i] = '\0';
      break;
    }

  return input;
}

char *read_text_file(char path[]) {
  if (path == NULL)
    return NULL;

  FILE *file = fopen(path, "r");
  if (file == NULL)
    return NULL;

  fseek(file, 0, SEEK_END);
  long text_size = ftell(file);

  rewind(file);

  char *text = (char *) calloc(text_size + 1, sizeof(char));
  if (text == NULL) {
    fclose(file);
    return NULL;
  }

  fread(text, sizeof(char), text_size, file);
  text[text_size] = '\0';

  fclose(file);
  return text;
}

bool save_encoded_file(char **frequency_dict, char text[], char path[]) {
  if (frequency_dict == NULL)
    return false;

  if (text == NULL)
    return false;

  if (path == NULL)
    return false;

  FILE *file = fopen(path, "w");
  if (file == NULL)
    return false;

  for (size_t i = 0; i < strlen(text); i++) {
    char current_char = text[i];
    fputs(frequency_dict[current_char], file);
  }

  fclose(file);
  return true;
}

bool save_decoded_file(FreqNode *tree, char encoded_path[], char decoded_path[]) {
  if (tree == NULL)
    return false;

  if (encoded_path == NULL)
    return false;

  if (decoded_path == NULL)
    return false;

  FILE *encoded_file = fopen(encoded_path, "r");
  if (encoded_file == NULL)
    return false;

  FILE *decoded_file = fopen(decoded_path, "w");
  if (decoded_file == NULL) {
    fclose(encoded_file);
    return false;
  }

  FreqNode *aux = tree;
  while (!feof(encoded_file)) {
    char direction = fgetc(encoded_file);

    if (direction == '0')
      aux = aux->left;

    else if (direction == '1')
      aux = aux->right;

    if (aux->left == NULL && aux->right == NULL) {
      fputc(aux->character, decoded_file);
      aux = tree;
    }
  }

  fclose(encoded_file);
  fclose(decoded_file);

  return true;
}

int main() {
  printf("--- READING TEXT FILE ---\n");
  char *text_file_path = read_input("Insert the path to the file to read: ");
  if (text_file_path == NULL) {
    printf("Path reading error\n");
    return 1;
  }

  char *text = read_text_file(text_file_path);
  if (text == NULL) {
    printf("Text file reading error\n");
    return 1;
  }

  printf("\n");

  int *frequency_table = get_frequency_table(text);
  if (frequency_table == NULL) {
    printf("Frequency table creation error\n");
    return 1;
  }

  printf("--- PRINTING FREQUENCY TABLE ---\n");
  print_frequency_table(frequency_table);
  printf("\n");

  FreqNode *frequency_list = get_frequency_ll(frequency_table);
  if (frequency_list == NULL) {
    printf("Frequency list creation error\n");
    return 1;
  }

  printf("--- PRINTING FREQUENCY LIST ---\n");
  print_frequency_list(frequency_list);
  printf("\n");

  FreqNode *frequency_bt = get_frequency_bt(frequency_list);
  if (frequency_bt == NULL) {
    printf("Frequency tree creation error\n");
    return 1;
  }

  printf("--- PRINTING FREQUENCY TREE ---\n");
  print_frequency_bt(frequency_bt);
  printf("Tree height: %d\n", get_height_frequency_bt(frequency_bt));
  printf("\n");

  char **frequency_dict = get_frequency_dict(frequency_bt);
  if (frequency_dict == NULL) {
    printf("Frequency dictionary creation error\n");
    return 1;
  }

  printf("--- PRINTING FREQUENCY DICTIONARY ---\n");
  print_frequency_dict(frequency_dict);
  printf("\n");

  printf("--- SAVING FREQUENCY DICTIONARY ---\n");
  char *encoded_path = read_input("Insert the path with filename to save the encoded text: ");
  if (encoded_path == NULL) {
    printf("Encoded path reading error\n");
    return 1;
  }

  bool success_saving_encoded = save_encoded_file(frequency_dict, text, encoded_path);
  if (!success_saving_encoded) {
    printf("Frequency dictionary saving error\n");
    return 1;
  }

  printf("Successfully saved frequency dictionary!\n");
  printf("\n");

  printf("--- SAVING DECODED TEXT ---\n");
  char *decoded_path = read_input("Insert the path with filename to save the decoded text: ");
  if (decoded_path == NULL) {
    printf("Decoded path reading error\n");
    return 1;
  }

  bool success_saving_decoded = save_decoded_file(frequency_bt, encoded_path, decoded_path);
  if (!success_saving_decoded) {
    printf("Decoded text saving error\n");
    return 1;
  }

  printf("Successfully saved decoded text!\n");

  return 0;
}
