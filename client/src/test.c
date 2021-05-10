// #include <stdio.h>
// #include <stdlib.h>

// int main(int argc, char const *argv[])
// {
//     int step = 5;
//     int multiplier = 1;
//     int* arr = malloc(sizeof(int) * step);
    
//     for (int i = 0; i < 25; i++)
//     {
//         if (i % step == 0)
//             arr = realloc(arr, sizeof(int) * step * multiplier++);

//         arr[i] = i;
//     }
    
//     for (int i = 0; i < 25; i++)
//     {
//         printf("%i ", arr[i]);
//     }
    

//     return 0;
// }
