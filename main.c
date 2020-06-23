#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "main.h"
#include "helper_functions.h"


int main()
{
	 twoD_t meta_data0 = {
                           .r = 32,
                           .c = 32,
                           .channel = 1,
                           .data = buffer0,
                           .bias = NULL
                       };

	 twoD_t meta_data1 = {
                           .r = 14,
                           .c = 14,
                           .channel = 6,
                           .data = buffer1,
                           .bias = NULL
                       };

	 twoD_t meta_data2 = {
                           .r = 5,
                           .c = 5,
                           .channel = 16,
                           .data = buffer0,
                           .bias = NULL
                       };

	 twoD_t meta_data3 = {
                           .r = 120,
                           .c = 1,
                           .channel = 1,
                           .data = buffer1,
                           .bias = NULL
                       };

	 twoD_t meta_data4 = {
                           .r = 84,
                           .c = 1,
                           .channel = 1,
                           .data = buffer0,
                           .bias = NULL
                       };

	 twoD_t meta_data5 = {
                           .r = 10,
                           .c = 1,
                           .channel = 1,
                           .data = buffer1,
                           .bias = NULL
                       };


	 memcpy(buffer0, test, sizeof(test));

	 printf("---Network starts---\n");
	 conv2D(&meta_data0, &w_0_weight_2d, &meta_data1, &reLU, 2, 2, 2, 0);
	 conv2D(&meta_data1, &w_3_weight_2d, &meta_data2, &reLU, 2, 2, 2, 0);
	 dot(&meta_data2, &w_7_weight_2d, &meta_data3, &reLU);
	 dot(&meta_data3, &w_9_weight_2d, &meta_data4, &reLU);
	 dot(&meta_data4, &w_11_weight_2d, &meta_data5, NULL);

	 print_twoD(&meta_data5, 0);
	 printf("PREDICTION: %d\n", get_class(&meta_data5));
	 return 0;
}