# 2gis-tsk

![build and run tests](https://github.com/ck33122/2gis-tsk/workflows/build%20and%20run%20tests/badge.svg?branch=master)

тестовое задание для 2gis

## Условие

Напишите (консольную) программу, принимающую на вход имя файла и набор параметров. В зависимости от параметров программа должна работать в трёх режимах:

1. `task -f Test.tst -m words -v mother` печатает количество слов «mother» в файле «Test.tst»
2. `task -f Test.tst -m checksum` печатает 32-битную чексумму, рассчитанную по алгоритму `checksum = word1 + word2 + ... + wordN` (где `word1`..`wordN` – 32-хбитные слова, представляющие содержимое файла)
3. `task -h` печатает информацию о программе и описание параметров.
