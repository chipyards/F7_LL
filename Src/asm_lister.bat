arm-atollic-eabi-gcc ..\Src\float_test.c -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -std=gnu11 -O0 -Wall -S -o ..\Doc\float_testO0.txt
arm-atollic-eabi-gcc ..\Src\float_test.c -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -std=gnu11 -O1 -Wall -S -o ..\Doc\float_testO1.txt
arm-atollic-eabi-gcc ..\Src\float_test.c -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -std=gnu11 -O2 -Wall -S -o ..\Doc\float_testO2.txt
arm-atollic-eabi-gcc ..\Src\float_test.c -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -std=gnu11 -O3 -Wall -S -o ..\Doc\float_testO3.txt
arm-atollic-eabi-gcc ..\Src\float_test.c -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -std=gnu11 -Os -Wall -S -o ..\Doc\float_testOs.txt
arm-atollic-eabi-gcc ..\Src\float_test.c -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -std=gnu11 -Og -Wall -S -o ..\Doc\float_testOg.txt
arm-atollic-eabi-gcc ..\Src\float_test.c -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -std=gnu11 -Ofast -Wall -S -o ..\Doc\float_testOfast.txt
