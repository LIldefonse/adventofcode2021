#include <stdlib.h>
#include <stdio.h>

#define LINE_LENGTH 80

int main(int argc, char** argv) {
    FILE* input = fopen("input", "r");
    char line[LINE_LENGTH];
    int prev_sum = 0;
    int value;
    int prev_value = 0;
    int prev_prev_value = 0;
    int sum;
    int count = 0;
    while(fgets(line, LINE_LENGTH, input)){
        value = atoi(line);
        if (prev_prev_value && prev_value) {
            sum = prev_prev_value + prev_value + value;
        
            if (prev_sum && prev_sum < sum) {
                printf("Increasing : ");
                count ++;
            }
            printf("%d -> %d\n", prev_sum, sum);
        }
        
        prev_prev_value = prev_value;
        prev_value = value;
        prev_sum = sum;
    }

    printf("%d depth measurement increased\n", count);
    fclose(input);
    return EXIT_SUCCESS;
}