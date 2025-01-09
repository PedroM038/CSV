#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_ROWS 1000
#define MAX_COLS 50

typedef struct {
    char **data;
    int rows;
    int cols;
} CSV;

// Function prototypes
CSV* read_csv(const char *filename);
void display_data(CSV *csv, int start_row, int end_row);
void filter_data(CSV *csv);
void sort_data(CSV *csv);
void select_data(CSV *csv);
void save_data(CSV *csv, const char *filename);
void free_csv(CSV *csv);

int main() {
    char filename[256];
    CSV *csv = NULL;
    int choice;

    printf("=== CSV File Handler ===\n");
    printf("Enter CSV filename: ");
    scanf("%s", filename);

    csv = read_csv(filename);
    if (!csv) {
        printf("Error reading CSV file!\n");
        return 1;
    }

    while (1) {
        printf("\n1. Display data\n");
        printf("2. Filter data\n");
        printf("3. Sort data\n");
        printf("4. Select data\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_data(csv, 0, csv->rows);
                break;
            case 2:
                filter_data(csv);
                break;
            case 3:
                sort_data(csv);
                break;
            case 4:
                select_data(csv);
                break;
            case 5:
                free_csv(csv);
                return 0;
            default:
                printf("Invalid option!\n");
        }
    }

    return 0;
}

CSV* read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    CSV *csv = malloc(sizeof(CSV));
    csv->data = malloc(MAX_ROWS * sizeof(char*));
    csv->rows = 0;
    csv->cols = 0;

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file) && csv->rows < MAX_ROWS) {
        csv->data[csv->rows] = strdup(line);
        csv->rows++;
    }

    // Count columns from first row
    char *token = strtok(strdup(csv->data[0]), ",");
    while (token) {
        csv->cols++;
        token = strtok(NULL, ",");
    }

    fclose(file);
    return csv;
}

void display_data(CSV *csv, int start_row, int end_row) {
    if (start_row < 0) start_row = 0;
    if (end_row > csv->rows) end_row = csv->rows;

    for (int i = start_row; i < end_row; i++) {
        printf("%s", csv->data[i]);
    }
}

void free_csv(CSV *csv) {
    if (!csv) return;
    for (int i = 0; i < csv->rows; i++) {
        free(csv->data[i]);
    }
    free(csv->data);
    free(csv);
}

// TODO: Implement filter_data, sort_data, select_data, and save_data functions
void filter_data(CSV *csv) {
    char column_name[MAX_LINE];
    char value[MAX_LINE];
    int column_index = -1;

    printf("Enter column name to filter: ");
    scanf("%s", column_name);
    printf("Enter value to filter: ");
    scanf("%s", value);

    // Find the column index
    char *token = strtok(strdup(csv->data[0]), ",");
    for (int i = 0; token != NULL; i++) {
        if (strcmp(token, column_name) == 0) {
            column_index = i;
            break;
        }
        token = strtok(NULL, ",");
    }

    if (column_index == -1) {
        printf("Column not found!\n");
        return;
    }

    // Display filtered rows
    for (int i = 1; i < csv->rows; i++) {
        char *row = strdup(csv->data[i]);
        token = strtok(row, ",");
        for (int j = 0; j < column_index; j++) {
            token = strtok(NULL, ",");
        }
        if (token && strcmp(token, value) == 0) {
            printf("%s", csv->data[i]);
        }
        free(row);
    }
}

void sort_data(CSV *csv) {
    int column_index = -1;
    char column_name[MAX_LINE];

    printf("Enter column name to sort by: ");
    scanf("%s", column_name);

    // Find the column index
    char *token = strtok(strdup(csv->data[0]), ",");
    for (int i = 0; token != NULL; i++) {
        if (strcmp(token, column_name) == 0) {
            column_index = i;
            break;
        }
        token = strtok(NULL, ",");
    }

    if (column_index == -1) {
        printf("Column not found!\n");
        return;
    }

    // Sort rows based on the column
    for (int i = 1; i < csv->rows - 1; i++) {
        for (int j = i + 1; j < csv->rows; j++) {
            char *row_i = strdup(csv->data[i]);
            char *row_j = strdup(csv->data[j]);
            char *value_i = strtok(row_i, ",");
            char *value_j = strtok(row_j, ",");
            for (int k = 0; k < column_index; k++) {
                value_i = strtok(NULL, ",");
                value_j = strtok(NULL, ",");
            }
            if (value_i && value_j && strcmp(value_i, value_j) > 0) {
                char *temp = csv->data[i];
                csv->data[i] = csv->data[j];
                csv->data[j] = temp;
            }
            free(row_i);
            free(row_j);
        }
    }
}

void select_data(CSV *csv) {
    int start_row, end_row;

    printf("Enter start row: ");
    scanf("%d", &start_row);
    printf("Enter end row: ");
    scanf("%d", &end_row);

    display_data(csv, start_row, end_row);
}

void save_data(CSV *csv, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < csv->rows; i++) {
        fprintf(file, "%s", csv->data[i]);
    }

    fclose(file);
    printf("Data saved to %s\n", filename);
}