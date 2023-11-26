Implemented Mustache commands
=============================

## Implemented standard commands

* ``{{ var }}`` - Variable substitution (escaped)
* ``{{{ var }}}`` - Variable substitution (unescaped)
* ``{{# var }} {{\ var }}`` - Sections (repeated blocks)
* ``{{^ var }} {{\ var }}`` - Inverted Sections
* ``{{> partial }}`` - Partials
* ``{{! comment }}`` - Comments

## Not implemented commands

* Lambdas
* Set Delimiters

## Non-standard commands

``{{< partial }}`` - Templates

Like partials but takes partial to open from a variable.

``{{= var }} {{\ var }}`` - Empty test

Like sections but *do not change context*.
It's used to check if a variable is empty.

``{{0 var }} {{\ var }}`` - Existance tests

It's used to check if a variable exists.
Compared to ``{{= }}`` and ``{{# }}`` it checks the *keys* of the json
objects instead of the *values*. If the key exists it renders the section.

``{{ array[index] }}`` - Array element selection

Prints an element of the array.
I works also for sections. Eg: you can evaluate an array element using:
``{{# array[index] }}``, ``{{^ array[index] }}``, ``{{= array[index] }}``
or ``{{0 array[index] }}``.

## See also

* https://mustache.github.io/ for original language reference.
