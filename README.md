<h2 style="font-style:italic"><span style="font-size:22px"><strong>PDP-11</strong></span></h2>

<hr />
<p><span style="font-size:16px">Эмулятор pdp11 на си</span></p>
<h2 style="font-style:italic"><strong>Запуск тестов</strong></h2>

<hr />

<table border="1" cellpadding="1" cellspacing="1" dir="ltr" style="width:500px">

    gcc pdp11.c pdp11.h do_functions.c -o pdp11
    ./pdp11 -t путь/до/теста/расширения pdp.o

Например:    ./pdp11 -t pdp11_tests/01_sum/01_sum.pdp.o.
</table>

<p>&nbsp;</p>