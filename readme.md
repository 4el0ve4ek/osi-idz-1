# Аксенов Даниил | БПИ218 | ИДЗ-1 | Вариант 20
Условие задачи:
Разработать программу, вычисляющую число вхождений различных цифр в заданной ASCII-строке.

#### Схема решения:
```
-----------------------------------
|Процесс 1                        |
|1. create pipe №1                |
|2. fork() (создаем процесс 2)    |
|---------------------------------|
|3. input.txt --> <runtime memory>|
|4. pipe№1 <-- <runtime memory>   |
|(загружаем в pipe№1              |
|           входные данные)       |
-----------------------------------
           \
            \
            -\------------------------------------
            |Процесс 2                           |
            |1. fork() (создаем процесс 3)       |
            |------------------------------------|
            |1. pipe№1 --> <runtime memory>.     |
            |2. Выполняем задачу подсчета        | 
            |                    уникальных цифр |
            |3. pipe№2 <-- answer                |
            |(Загружаем в pipe№2 найденный ответ)|
            ------------\-------------------------
                         \
                          \
                        ---\----------------------------------
                        |Процесс 3                           |
                        |1. pipe№2 --> <runtime memory>      |
                        |2. output.txt <-- <runtime memory>  |
                        |(сохраняем ответ в выходной файл)   |
                        --------------------------------------

```


#### Команда для запуска:
```
gcc main.c -o res && ./res <имя входного файла> <имя выходного файла>
```

#### Ограничения:
  
   Размер входного файле не должен превышать 25000 байт

#### Тесты:
```
gcc main.c -o res
./res input_1.txt output_1.txt
./res input_2.txt output_2.txt
./res input_3.txt output_3.txt
./res input_4.txt output_4.txt
./res input_5.txt output_5.txt
./res input_6.txt output_6.txt
```
